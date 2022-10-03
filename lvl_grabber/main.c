/* ---------- ---------- ---------- ---------- ---------- ---------- ----------
Breaker - Level grabber.
Code: Copyright (C) 2009 Clement Corde, c1702@yahoo.com

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
---------- ---------- ---------- ---------- ---------- ---------- ---------- */

#include <stdlib.h>
#include <unistd.h>
#include "SDL.h"
#include "ctypes.h"

#define	BONUS_MAX	10

// Lecture BMP et création de "levels.h"
void CreateLevels(void)
{
	SDL_Surface	*pBmp;
	u8	*pPic;
	u32	nScanX, nScanY;
	u32	nLevNo;
	u32	i, j;
	s8	nVal;

	// Load Picture.
	pBmp = SDL_LoadBMP("levels.bmp");
	if (pBmp == NULL) {
		fprintf(stderr, "Couldn't load picture: %s\n", SDL_GetError());
		exit(1);
	}

	// Parcours de l'image.
	pPic = (u8 *)pBmp->pixels;
	nLevNo = 1;
	for (nScanY = 0; nScanY < pBmp->h; nScanY += 20)
	{
		for (nScanX = 0; nScanX < pBmp->w; nScanX += 16)
		{
			if (*(pPic + (nScanY * pBmp->w) + nScanX) != 0)
			{
				//printf("Level %d at (%d, %d)\n", (int)nLevNo, (int)nScanX, (int)nScanY);

				// Briques.
				printf("s8 gLev%d[TABLE_Width * TABLE_Height] = {\n", (int)nLevNo);
				for (j = 0; j < 17; j++)
				{
					for (i = 0; i < 15; i++)
					{
						nVal = -1;
						if (*(pPic + ((nScanY + j) * pBmp->w) + nScanX + i) <= 15)
						{
							nVal = *(pPic + ((nScanY + j) * pBmp->w) + nScanX + i) - 1;
						}
						else
						if (*(pPic + ((nScanY + j) * pBmp->w) + nScanX + i) <= 31)
						{
							nVal = *(pPic + ((nScanY + j) * pBmp->w) + nScanX + i) - 6;
						}
						printf("%d, ", (int)nVal);
					}
					printf("\n");
				}
				printf("};\n\n");

				// Bonuses.
				printf("u8 gBon%d[32] = {\n", (int)nLevNo);
				for (j = 18; j < 20; j++)
				{
					for (i = 0; i < 16; i++)
					{
						nVal = 0;
						if (*(pPic + ((nScanY + j) * pBmp->w) + nScanX + i) >= 128 &&
							*(pPic + ((nScanY + j) * pBmp->w) + nScanX + i) <= 128 + BONUS_MAX)
						{
							nVal = *(pPic + ((nScanY + j) * pBmp->w) + nScanX + i) - 128;
						}
						printf("%d, ", (int)nVal);
					}
					printf("\n");
				}
				printf("};\n\n");

				nLevNo++;
			}
		}
	}

	// Table des niveaux + table des bonuses.
	printf("#define	LEVEL_Max\t%d\n", (int)nLevNo-1);
	printf("s8 *gpLevels[] = {\n");
	for (i = 1; i < nLevNo; i++)
	{
		printf("gLev%d, ", (int)i);
		if (((i - 1) & 7) == 0) printf("\n");
	}
	printf("};\n\n");

	printf("u8 *gpBonuses[] = {\n");
	for (i = 1; i < nLevNo; i++)
	{
		printf("gBon%d, ", (int)i);
		if (((i - 1) & 7) == 0) printf("\n");
	}
	printf("};\n\n");


	SDL_FreeSurface(pBmp);
}


int main(int argc, char **argv)
{
	// initialize SDL video
	if (SDL_Init( SDL_INIT_VIDEO ) < 0)
	{
		printf( "Unable to init SDL: %s\n", SDL_GetError() );
		return 1;
	}

	// make sure SDL cleans up before exit
	atexit(SDL_Quit);

	// create a new window
	SDL_Surface *screen = SDL_SetVideoMode(640, 480, 16, SDL_SWSURFACE);
	if (!screen)
	{
		printf("Unable to set 640x480 video: %s\n", SDL_GetError());
		return 1;
	}

	// load an image
	SDL_Surface *bmp = SDL_LoadBMP("cb.bmp");
	if (!bmp)
	{
		printf("Unable to load bitmap: %s\n", SDL_GetError());
		return 1;
	}

	// centre the bitmap on screen
	SDL_Rect dstrect;
	dstrect.x = (screen->w - bmp->w) / 2;
	dstrect.y = (screen->h - bmp->h) / 2;


CreateLevels();


	// program main loop
	u32 done = 0;
	while (!done)
	{
		// message processing loop
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
		    // check for messages
		    switch (event.type)
		    {
		        // exit if the window is closed
		    case SDL_QUIT:
		        done = 1;
		        break;

		        // check for keypresses
		    case SDL_KEYDOWN:
		        {
		            // exit if ESCAPE is pressed
		            if (event.key.keysym.sym == SDLK_ESCAPE)
		                done = 1;
		            break;
		        }
		    } // end switch
		} // end of message processing

		// DRAWING STARTS HERE

		// clear screen
		SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));

		// draw bitmap
		SDL_BlitSurface(bmp, 0, screen, &dstrect);

		// DRAWING ENDS HERE

SDL_Delay(10);

		// finally, update the screen :)
		SDL_Flip(screen);
	} // end main loop

	// free loaded bitmap
	SDL_FreeSurface(bmp);

	// all is well ;)
	printf("Exited cleanly\n");
	return 0;
}

