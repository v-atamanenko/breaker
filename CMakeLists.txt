cmake_minimum_required(VERSION 3.1)

SET(PLATFORM "DESKTOP" CACHE STRING "Target build platform. Options: VITA / DESKTOP")
OPTION(EXTRADBG "Enable extra debugging tools for retail Vitas: vita-parse-core and vita-companion" OFF)

if (PLATFORM STREQUAL "VITA")
    if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
        if(DEFINED ENV{VITASDK})
            set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
        else()
            message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
        endif()
    endif()

    if(EXTRADBG)
        set(PSVITAIP "192.168.1.21" CACHE STRING "PSVita IP for vita-companion integration")
        set(PARSECOREPATH "/Users/v/Repos/vitasdk/vita-parse-core/main.py" "vita-parse-core/main.py path for core dump parser integration")
    endif()
endif()

project(breaker C)

if (PLATFORM STREQUAL "VITA")
    # Add Vita helper macros to build Vita specific artifacts (self/vpk).
    include("${VITASDK}/share/vita.cmake" REQUIRED)

    set(VITA_APP_NAME "Breaker")
    set(VITA_TITLEID  "BREAKER00")
    set(VITA_VERSION  "01.00")

    set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1 -d ATTRIBUTE2=12")

    #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-optimize-sibling-calls -fdiagnostics-color=always -g -Wl,-q,--allow-multiple-definition -D_GNU_SOURCE -Wall -O3")

    set(PLATFORM_LIBS
            SDL2::SDL2-static
            ScePower_stub
            SceDisplay_stub
            SceCtrl_stub
            SceAudio_stub
            SceSysmodule_stub
            SceGxm_stub
            SceCommonDialog_stub
            SceAppMgr_stub
            SceTouch_stub
            SceHid_stub
            SceMotion_stub
            m
    )

    add_definitions(-DPLATFORM_VITA)
elseif(PLATFORM STREQUAL "DESKTOP")
    if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        list(APPEND CMAKE_PREFIX_PATH "/usr/local/Cellar/sdl/1.2.15_3")
    endif()

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL REQUIRED sdl)
    include_directories(${SDL_INCLUDE_DIRS})

    if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        set(PLATFORM_LIBS
                ${SDL_LINK_LIBRARIES}
                "-framework Cocoa"
                )
    else()
        set(PLATFORM_LIBS
                ${SDL_LINK_LIBRARIES}
                )
    endif()

    add_definitions(-DPLATFORM_DESKTOP)
endif()

include_directories(
        ${CMAKE_SOURCE_DIR}/source
)

add_executable(${CMAKE_PROJECT_NAME}
        source/main.c
        source/anims.c
        source/animspr.c
        source/breaker.c
        source/dust.c
        source/fire.c
        source/font.c
        source/frame.c
        source/menu.c
        source/monsters.c
        source/mst.c
        source/preca.c
        source/render.c
        source/sfx.c
        source/sprites.c
        source/transit.c
        )

target_link_libraries(${CMAKE_PROJECT_NAME}
        ${PLATFORM_LIBS}
        c
        )

if (PLATFORM STREQUAL "VITA")
    vita_create_self(eboot.bin ${CMAKE_PROJECT_NAME} UNSAFE)
    vita_create_vpk(${CMAKE_PROJECT_NAME}.vpk ${VITA_TITLEID} eboot.bin
            VERSION ${VITA_VERSION}
            NAME ${VITA_APP_NAME}
            FILE ${CMAKE_SOURCE_DIR}/assets/livearea/icon0.png sce_sys/icon0.png
            ${CMAKE_SOURCE_DIR}/assets/livearea/startup.png sce_sys/livearea/contents/startup.png
            ${CMAKE_SOURCE_DIR}/assets/livearea/bg.png sce_sys/livearea/contents/bg.png
            ${CMAKE_SOURCE_DIR}/assets/livearea/template.xml sce_sys/livearea/contents/template.xml
            ${CMAKE_SOURCE_DIR}/source/gfx gfx
            ${CMAKE_SOURCE_DIR}/source/sfx sfx
            )

    if (EXTRADBG)
        add_custom_target(send
                COMMAND echo destroy | nc ${PSVITAIP} 1338
                COMMAND curl -T eboot.bin ftp://${PSVITAIP}:1337/ux0:/app/${VITA_TITLEID}/
                COMMAND echo launch ${VITA_TITLEID} | nc ${PSVITAIP} 1338
                DEPENDS eboot.bin)

        add_custom_target(dump
                COMMAND ${CMAKE_SOURCE_DIR}/get_dump.sh ${PSVITAIP}
                COMMAND python3 ${PARSECOREPATH} coredump ${CMAKE_PROJECT_NAME})
    endif()
else()
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/source/gfx $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/gfx/)
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/source/sfx $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/sfx/)
endif()
