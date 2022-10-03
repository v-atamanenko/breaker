/* ---------- ---------- ---------- ---------- ---------- ---------- ----------
Breaker - Arkanoid like game.
Code: Copyright (C) 2009 Clement Corde, c1702@yahoo.com
All graphics and sounds are the property of their respective owners,
Taito for the most of them.

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

// Arkanoid Like. v.2.16
// Code: 17o2!!
// Contact: Clement CORDE, c1702@yahoo.com

// Credits for the material used here:
//
// Some of the graphics were ripped from the ST version.
// Backgrounds taken from the StrategyWiki online arcade screenshots (http://strategywiki.org/wiki/Arkanoid/Walkthrough).
// This means some of the graphics are probably the property of Taito.
// As there is no profit intended, I hope I won't get sewed.
//
// Font found on Daniel Guldkrans's website (http://www.algonet.se/~guld1/freefont.htm), and slightly retouched by me.
//
// All additional graphics by me.
//


#include "includes.h"


// Variables générales.
struct SGene gVar;
struct SExg gExg;

//=====================================

#define	EVENT_EmergencyExit		1		// Comment to remove emergency exit (Esc).

// Gestionnaire d'évènements.
int EventHandler(u32 nInGame)
{
	SDL_Event event;
	static u32	nLastPhase;

	gVar.nInactiveCnt++;				// Called one time every frame.

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
#if defined(EVENT_EmergencyExit)
			if (gVar.pKeys[SDLK_ESCAPE]) return (1);	// Emergency exit.
#endif

			// Toggle fullscreen/windowed.
			if (gVar.pKeys[SDLK_F10])
			{
				gRender.nFullscreenMode ^= 1;
				Render_SetVideoMode();
				// Replace la souris à l'endroit du joueur.
				if (nInGame) SDL_WarpMouse(gBreak.nPlayerPosX, gBreak.nPlayerPosY);		// => Génère un event.
			}
			// Toggle render mode.
			if (gVar.pKeys[SDLK_F9])
			{
				// *** In case F9 is pressed while the game is paused ***
				if (gRender.nRenderMode == 0)
				{
					SDL_SetPalette(gRender.pScreenBuf2, SDL_LOGPAL, gVar.pColors, 0, 256);		// Sets the palette BEFORE the blit, otherwise the blit tries to remap!
					SDL_BlitSurface(gVar.pScreen, NULL, gRender.pScreenBuf2, NULL);
					gVar.nPalUpdate = 1;
				}

				if (++gRender.nRenderMode >= e_RenderMode_MAX) gRender.nRenderMode = 0;
				Render_SetVideoMode();

				// *** In case F9 is pressed while the game is paused ***
				if (gRender.nRenderMode == 0)
				{
					SDL_BlitSurface(gRender.pScreenBuf2, NULL, gVar.pScreen, NULL);
				}
			}

			// Gestion de la pause.
			if (nInGame == 1 && gVar.pKeys[SDLK_p])
			{
				if (gBreak.nPhase == e_Game_Pause)
				{
					gBreak.nPhase = nLastPhase;			// On sort de la pause.
					SDL_ShowCursor(SDL_DISABLE);		// Cache le pointeur de la souris.
					// Replace la souris à l'endroit du joueur.
					SDL_WarpMouse(gBreak.nPlayerPosX, gBreak.nPlayerPosY);		// => Génère un event.
					Sfx_ChannelsRestore();				// Restaure les sons en cours.
				}
				else
				{
					// On passe en pause.
					nLastPhase = gBreak.nPhase;
					gBreak.nPhase = e_Game_Pause;

					Sfx_ChannelsSave();					// Sauvegarde + arrêt des sons en cours.

					// Affichage du texte de pause.
					char	*pStrPause = "PAUSE";
					u32 i = Font_Print(0, 10, pStrPause, FONT_NoDisp);	// Pour centrage.
					Font_Print((SCR_Width / 2) - (i / 2), 123, pStrPause, 0);
					SprDisplayAll();

					SDL_Surface	*pScr = Render_GetRealVideoSurfPtr();
					SDL_WarpMouse(pScr->w / 2, pScr->h / 2);	// Centrage du pointeur au milieu de l'écran.
					SDL_ShowCursor(SDL_ENABLE);		// Affiche le pointeur de la souris.
				}
			}

			gVar.nInactiveCnt = 0;
			break;

		case SDL_KEYUP:
			break;

		case SDL_MOUSEMOTION:
			gVar.nMousePosX = event.motion.x;
			gVar.nMousePosY = event.motion.y;

			gVar.nInactiveCnt = 0;
			break;

		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
				gVar.nMouseButtons |= MOUSE_BtnLeft;
				break;

			default:
				break;
			}

			gVar.nInactiveCnt = 0;
			break;

		case SDL_QUIT:		// Fermeture de la fenêtre.
			exit(0);
			break;
		}
	}
	return (0);
}

//=====================================

// Création de la palette :
// On recopie dans la palette générale la partie de palette correspondant au décor + la palette des sprites.
void SetPalette(SDL_Surface *pBkg, SDL_Color *pSprPal, u32 nSprPalIdx)
{
	u32	i;
	SDL_Color	*pSrcPal = pBkg->format->palette->colors;

	// Couleurs du décor.
	for (i = 0; i < nSprPalIdx; i++)
	{
		gVar.pColors[i] = pSrcPal[i];
	}

	// Couleurs des sprites.
	for (; i < 256; i++)
	{
		gVar.pColors[i] = pSprPal[i - SPR_Palette_Idx];
	}

	// Palette logique.
	SDL_SetPalette(gVar.pScreen, SDL_LOGPAL, gVar.pColors, 0, 256);		// Sur gVar.pScreen car c'est la surface de rendu.

}

//=====================================

// Le Menu (générique).
u32 Menu(void (*pFctInit)(void), u32 (*pFctMain)(void))
{
	u32	nMenuVal = MENU_Null;

	SDL_FillRect(gVar.pScreen, NULL, CLR_BLACK_IDX);	// Clear screen.
	RenderFlip(0);
	Fade(0);

	gVar.pBackground = gVar.pBkg[0];	// Décor par défaut.
	gVar.pBkgRect = NULL;				// Par défaut, NULL (toute la surface).

	(*pFctInit)();						// Menu, Init fct.
	// Sets up palette.
	SetPalette(gVar.pBackground, gVar.pSprColors, SPR_Palette_Idx);

	// Main loop.
	gVar.nInactiveCnt = 0;				// RAZ compteur menu.
	FrameInit();
	while (nMenuVal == MENU_Null)
	{
		// Gestion des évenements.
		gVar.nMouseButtons = 0;			// Raz mouse buttons.
#if defined(EVENT_EmergencyExit)
		if (EventHandler(0) != 0) { nMenuVal = MENU_Quit; break; }
#else
		EventHandler(0);
#endif

		SDL_BlitSurface(gVar.pBackground, gVar.pBkgRect, gVar.pScreen, NULL);	// Recopie le décor.
		nMenuVal = (*pFctMain)();		// Menu, Main fct.
		SprDisplayAll();

		RenderFlip(1);					// Wait for frame, Flip.
		Fade(gVar.nFadeVal);
	}

	return (nMenuVal);
}

//=====================================

// La boucle de jeu.
void Game(void)
{


	// Cinématique d'intro.
//todo:...


	SDL_FillRect(gVar.pScreen, NULL, CLR_BLACK_IDX);	// Clear screen.
	RenderFlip(0);
	Fade(0);

	// Init.
	ExgBrkInit();
	// Sets up palette (Même palette pour tous les niveaux).
	SetPalette(gVar.pLev[0], gVar.pSprColors, SPR_Palette_Idx);
	//>> (Mettre le fader ?)
	gVar.nFadeVal = 256;
	Fade(gVar.nFadeVal);				// Remet la palette physique.
	//<<

	// Main loop.
	gVar.nInactiveCnt = 0;				// RAZ compteur menu.
	FrameInit();
	while (gExg.nExitCode == 0)
	{
		// Gestion des évenements.
		gVar.nMouseButtons = 0;			// Raz mouse buttons.
#if defined(EVENT_EmergencyExit)
		if (EventHandler(1) != 0) break;
#else
		EventHandler(1);
#endif

		if (gBreak.nPhase != e_Game_Pause)
		{
			SDL_BlitSurface(gVar.pLevel, NULL, gVar.pScreen, NULL);		// Copie de l'image de fond.
			Breaker();
			SprDisplayAll();
			Transit_Manage();
		}
		RenderFlip(1);	// Wait for frame, Flip.
		//Fade(gVar.nFadeVal);
	}
	SDL_ShowCursor(SDL_DISABLE);		// Cache le pointeur de la souris, au cas ou on quitte pendant la pause.


	// Si jeu terminé, cinématique de fin.
//	if (gExg.nExitCode == e_Game_AllClear) {}
//todo:...


	// High score ?
	if (gExg.nExitCode == e_Game_GameOver || gExg.nExitCode == e_Game_AllClear)
	{
		if (Scr_CheckHighSc(gExg.nScore) >= 0)
		{
			// Saisie du nom.
			Menu(MenuGetName_Init, MenuGetName_Main);
			// Affichage de la table des high scores.
			Menu(MenuHighScores_Init, MenuHighScores_Main);
		}
	}

}

//=====================================

// Point d'entrée.
int main(int argc, char *argv[])
{
	u32	nLoop;
	u32	nMenuVal;
	u32	i;

	assert(MstCheckStructSizes() == 0);		// Debug : Vérifie la taille des structures spécifiques des monstres.

	// SDL Init.
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}
	// atexit : Quand on quittera (exit, return...), SDL_Quit() sera appelée.
	atexit(SDL_Quit);

	// Video mode init.
	Render_InitVideo();
	SDL_WM_SetCaption("Arkanoid by 17o2!!", NULL);	// Nom de la fenêtre.
	gRender.nRenderMode = e_RenderMode_TV2x;//e_RenderMode_Scale2x;
	Render_SetVideoMode();

	gVar.pKeys = SDL_GetKeyState(NULL);		// Kb ptr init.

	// Sine and Cosine tables precalculation.
	PrecaSinCos();

	// Load sprites.
	SprInitEngine();
	SprLoadBMP("gfx/bricks.bmp", gVar.pSprColors, SPR_Palette_Idx);
	SprLoadBMP("gfx/font_small.bmp", NULL, 0);

	// Load levels background pictures.
	char	*pBkgLevFilenames[GFX_NbBkg] = { "gfx/lev1.bmp", "gfx/lev2.bmp", "gfx/lev3.bmp", "gfx/lev4.bmp", "gfx/levdoh.bmp" };
	for (i = 0; i < GFX_NbBkg; i++)
	{
		if ((gVar.pLev[i] = SDL_LoadBMP(pBkgLevFilenames[i])) == NULL) {
			fprintf(stderr, "Couldn't load picture '%s' : %s\n", pBkgLevFilenames[i], SDL_GetError());
			exit(1);
		}
	}
	gVar.pLevel = gVar.pLev[0];

	// Load menus backgound pictures.
	char	*pBkgMenFilenames[MENU_NbBkg] = { "gfx/bkg1.bmp", "gfx/bkg2.bmp" };
	for (i = 0; i < MENU_NbBkg; i++)
	{
		if ((gVar.pBkg[i] = SDL_LoadBMP(pBkgMenFilenames[i])) == NULL) {
			fprintf(stderr, "Couldn't load picture '%s' : %s\n", pBkgMenFilenames[i], SDL_GetError());
			exit(1);
		}
	}
	gVar.pBackground = gVar.pBkg[0];

	// Load title effect picture.
	char	*pTitleEffectFilename = "gfx/arka_bw.bmp";
	if ((gVar.pTitleEffect = SDL_LoadBMP(pTitleEffectFilename)) == NULL) {
		fprintf(stderr, "Couldn't load picture '%s' : %s\n", pTitleEffectFilename, SDL_GetError());
		exit(1);
	}
	// Pre-add for title effect. (I'm too lazy to restart Photoshop to change the color number! ;) ).
	{
		u32	i, j;
		for (j = 0; j < gVar.pTitleEffect->h; j++)
			for (i = 0; i < gVar.pTitleEffect->w; i++)
				if ( *((u8 *)gVar.pTitleEffect->pixels + (j * gVar.pTitleEffect->pitch) + i) )
					*((u8 *)gVar.pTitleEffect->pixels + (j * gVar.pTitleEffect->pitch) + i) = 6;
	}

	// Sound init.
	Sfx_SoundInit();
	Sfx_LoadWavFiles();
	Sfx_SoundOn();						// Start playback.

	//
	MenuInit();
	// Load high-score table.
	Scr_Load();

	SDL_ShowCursor(SDL_DISABLE);		// Hide mouse cursor.

	// Infinite loop.
	nMenuVal = MENU_Main;
	nLoop = 1;
	while (nLoop)
	{
		switch (nMenuVal)
		{
		case MENU_Main:			// Main menu.
			nMenuVal = Menu(MenuMain_Init, MenuMain_Main);
			break;

		case MENU_Game:			// Game.
			Game();
			nMenuVal = MENU_Main;
			break;

		case MENU_HallOfFame:	// High scores.
			MenuHighScores_RankClear();
			Menu(MenuHighScores_Init, MenuHighScores_Main);
			nMenuVal = MENU_Main;
			break;

		case MENU_PowerUps:		// Power-Ups.
			Menu(MenuPowerUps_Init, MenuPowerUps_Main);
			nMenuVal = MENU_Main;
			break;

		case MENU_Quit:			// Leaving.
			nLoop = 0;
			break;
		}

	}

	SDL_ShowCursor(SDL_ENABLE);			// Show mouse cursor.

	Sfx_SoundOff();						// Stop playback.
	Sfx_FreeWavFiles();					// Libère les ressources des fx.
	SprRelease();						// Libère les ressources des sprites.

	// Free the allocated surfaces.
	for (i = 0; i < GFX_NbBkg; i++)
	{
		SDL_FreeSurface(gVar.pLev[i]);
	}
	for (i = 0; i < MENU_NbBkg; i++)
	{
		SDL_FreeSurface(gVar.pBkg[i]);
	}
	SDL_FreeSurface(gVar.pTitleEffect);

	// Libère les ressources de rendu.
	RenderRelease();

	return (0);
}



