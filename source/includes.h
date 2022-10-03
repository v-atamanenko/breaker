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

// Includes.
#include <stdlib.h>
#include <unistd.h>
#include "SDL.h"
#include <math.h>
#include <time.h>
#include <assert.h>

#include "ctypes.h"

#include "render.h"
#include "preca.h"
#include "transit.h"
#include "breaker.h"
#include "frame.h"
#include "sprites.h"
#include "animspr.h"
#include "anims.h"
#include "dust.h"
#include "fire.h"
#include "mst.h"
#include "monsters.h"
#include "font.h"
#include "menu.h"
#include "sfx.h"

// Define.
#define SCR_Width	320
#define SCR_Height	240

#define	SHADOW_OfsX	4
#define	SHADOW_OfsY	4

#define	GFX_NbBkg	5
#define	MENU_NbBkg	2

#define	MOUSE_BtnLeft	1	// Masques binaires.
#define	MOUSE_BtnRight	2

#define	SPR_Palette_Idx	128	// 0 à x : Palette du décor / x à 256 : Palette des sprites.
#define	CLR_BLACK_IDX	7	// Index du noir dans la palette.

// Types de variables.
struct SGene
{
	SDL_Surface *pScreen;	// Ptr sur le buffer écran.
	SDL_Surface *pLevel;	// Ptr sur l'image de fond d'un level.
	SDL_Surface *pLev[GFX_NbBkg];	// Les images de fond.
	SDL_Surface *pBackground;		// Ptr sur l'images de fond des menus.
	SDL_Rect	*pBkgRect;			// Ptr sur le rect pour déplacer le blit.
	SDL_Rect	sBkgRect;			// Rect pour déplacer le blit.
	SDL_Surface *pBkg[MENU_NbBkg];	// Les images de fond.
	SDL_Surface	*pTitleEffect;		// Image 512x256 pour effet sur main menu.

	u8	*pKeys;		// Buffer clavier.

	SDL_Color	pColors[256];	// Palette générale, à réinitialiser au changement de mode.
	SDL_Color	pSprColors[256 - SPR_Palette_Idx];	// Palette des sprites.

	SDL_Color	pCurPal[256];	// Palette lors des fades, pour le Scale 2x en 32 BPP.

	s16	pSinCos[256 + 64];	// Table contenant les sin et cos * 256, sur 256 angles.
	s16	*pSin;			// Ptrs sur les tables.
	s16	*pCos;

	s32	nMousePosX, nMousePosY;	// Position de la souris.
	u8	nMouseButtons;			// Boutons de la souris

	s32	nFadeVal;		// 0 = Noir / 256 = Couleurs finales.
	u32	nPalUpdate;		// 0 = Pas de changement de palette, 1 = Changement.

	u32	nInactiveCnt;		// Compteur d'inactivité pour les menus.

};

// Structure d'échange entre les différents modules.
struct SExg
{
	u32	nExitCode;	// Pour sortie du jeu. Tjs à 0, sauf pour sortie.
	u32	nLevel;		// Level atteint au game over.
	u32	nScore;		// Score au game over.


};

// Variables générales.
extern struct SGene gVar;
extern struct SExg gExg;


