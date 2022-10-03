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

//
// Display routines: 1x / 2x / TV 2x
//

#include "includes.h"

#define	BPP32_2X	1		// Usually faster. Comment for 16 BPP output.

struct SRender	gRender;

#define	TV2X_MUL	200		// Darkening factor (200/256) for TV effect.
u32	gpPal[256];
u32	gpPalDark[256];

#if defined(BPP32_2X)
// Scaling 2x. 8 > 32 BPP.
void Render_Scale2x(SDL_Surface *pSDL_Src, SDL_Surface *pSDL_Dst)
{
	// Conversion de la palette courante.
	if (gVar.nPalUpdate)
	{
		u32	i;
		for (i = 0; i < 256; i++)
			gpPal[i] = SDL_MapRGB(pSDL_Dst->format, gVar.pCurPal[i].r, gVar.pCurPal[i].g, gVar.pCurPal[i].b);
	}

	// 2x.
	SDL_LockSurface(pSDL_Src);
	SDL_LockSurface(pSDL_Dst);

	u32	y, x;
	u8	*pSrc = pSDL_Src->pixels;
	u8	*pDst = pSDL_Dst->pixels;
	u32	nClr;

	u8	*pSrc2;
	u32	*pDst2a, *pDst2b;

	for (y = 0; y < SCR_Height; y++)
	{
		pSrc2 = pSrc;
		pDst2a = (u32 *)pDst;
		pDst2b = (u32 *)(pDst + pSDL_Dst->pitch);
		for (x = 0; x < SCR_Width; x++)
		{
			nClr = gpPal[*pSrc2++];
			*pDst2a++ = nClr;
			*pDst2a++ = nClr;
			*pDst2b++ = nClr;
			*pDst2b++ = nClr;
		}
		pSrc += pSDL_Src->pitch;
		pDst += pSDL_Dst->pitch * 2;	// x2 to skip 2 raster lines.
	}

	SDL_UnlockSurface(pSDL_Src);
	SDL_UnlockSurface(pSDL_Dst);
}

// TV 2x, 8 > 32 BPP.
void Render_TV2x(SDL_Surface *pSDL_Src, SDL_Surface *pSDL_Dst)
{
	// Conversion de la palette courante.
	if (gVar.nPalUpdate)
	{
		u32	i;
		for (i = 0; i < 256; i++)
		{
			gpPal[i] = SDL_MapRGB(pSDL_Dst->format, gVar.pCurPal[i].r, gVar.pCurPal[i].g, gVar.pCurPal[i].b);
			gpPalDark[i] = SDL_MapRGB(pSDL_Dst->format, (gVar.pCurPal[i].r * TV2X_MUL) / 256, (gVar.pCurPal[i].g * TV2X_MUL) / 256, (gVar.pCurPal[i].b * TV2X_MUL) / 256);
		}
	}

	// 2x.
	SDL_LockSurface(pSDL_Src);
	SDL_LockSurface(pSDL_Dst);

	u32	y, x;
	u8	*pSrc = pSDL_Src->pixels;
	u8	*pDst = pSDL_Dst->pixels;
	u32	nClr;

	u8	*pSrc2;
	u32	*pDst2a, *pDst2b;

	for (y = 0; y < SCR_Height; y++)
	{
		pSrc2 = pSrc;
		pDst2a = (u32 *)pDst;
		pDst2b = (u32 *)(pDst + pSDL_Dst->pitch);
		for (x = 0; x < SCR_Width; x++)
		{
			nClr = gpPal[*pSrc2];
			*pDst2a++ = nClr;
			*pDst2a++ = nClr;
			nClr = gpPalDark[*pSrc2++];
			*pDst2b++ = nClr;
			*pDst2b++ = nClr;
		}
		pSrc += pSDL_Src->pitch;
		pDst += pSDL_Dst->pitch * 2;	// x2 to skip 2 raster lines.
	}

	SDL_UnlockSurface(pSDL_Src);
	SDL_UnlockSurface(pSDL_Dst);
}
#else
// Scaling 2x. 8 BPP.
void Render_Scale2x(SDL_Surface *pSDL_Src, SDL_Surface *pSDL_Dst)
{
	SDL_LockSurface(pSDL_Src);
	SDL_LockSurface(pSDL_Dst);

	u32	y, x;
	u8	*pSrc = pSDL_Src->pixels;
	u8	*pDst = pSDL_Dst->pixels;
	u16	nClr;

	u8	*pSrc2;
	u16	*pDst2a, *pDst2b;

	for (y = 0; y < SCR_Height; y++)
	{
		pSrc2 = (u8 *)pSrc;
		pDst2a = (u16 *)pDst;
		pDst2b = (u16 *)(pDst + pSDL_Dst->pitch);
		for (x = 0; x < SCR_Width; x++)
		{
			nClr = *pSrc2++;
			nClr = nClr | (nClr << 8);
			*pDst2a++ = nClr;
			*pDst2b++ = nClr;
		}
		pSrc += pSDL_Src->pitch;
		pDst += pSDL_Dst->pitch * 2;	// x2 to skip 2 raster lines.
	}

	SDL_UnlockSurface(pSDL_Src);
	SDL_UnlockSurface(pSDL_Dst);
}

// TV 2x, 8 > 16 BPP.
void Render_TV2x(SDL_Surface *pSDL_Src, SDL_Surface *pSDL_Dst)
{
	// Conversion de la palette courante.
	if (gVar.nPalUpdate)
	{
		u32	i;
		for (i = 0; i < 256; i++)
		{
			gpPal[i] = SDL_MapRGB(pSDL_Dst->format, gVar.pCurPal[i].r, gVar.pCurPal[i].g, gVar.pCurPal[i].b);
			gpPalDark[i] = SDL_MapRGB(pSDL_Dst->format, (gVar.pCurPal[i].r * TV2X_MUL) / 256, (gVar.pCurPal[i].g * TV2X_MUL) / 256, (gVar.pCurPal[i].b * TV2X_MUL) / 256);
		}
	}

	// 2x.
	SDL_LockSurface(pSDL_Src);
	SDL_LockSurface(pSDL_Dst);

	u32	y, x;
	u8	*pSrc = pSDL_Src->pixels;
	u8	*pDst = pSDL_Dst->pixels;
	u32	nClr;

	u8	*pSrc2;
	u16	*pDst2a, *pDst2b;

	for (y = 0; y < SCR_Height; y++)
	{
		pSrc2 = pSrc;
		pDst2a = (u16 *)pDst;
		pDst2b = (u16 *)(pDst + pSDL_Dst->pitch);
		for (x = 0; x < SCR_Width; x++)
		{
			nClr = gpPal[*pSrc2];
			*pDst2a++ = nClr;
			*pDst2a++ = nClr;
			nClr = gpPalDark[*pSrc2++];
			*pDst2b++ = nClr;
			*pDst2b++ = nClr;
		}
		pSrc += pSDL_Src->pitch;
		pDst += pSDL_Dst->pitch * 2;	// x2 to skip 2 raster lines.
	}

	SDL_UnlockSurface(pSDL_Src);
	SDL_UnlockSurface(pSDL_Dst);
}
#endif

typedef void (*pRenderFct)(SDL_Surface *pSDL_Src, SDL_Surface *pSDL_Dst);
// Rendu + Flip.
void RenderFlip(u32 nSync)
{
	static pRenderFct	pFctTb[e_RenderMode_MAX] = { NULL, Render_Scale2x, Render_TV2x };

	if (pFctTb[gRender.nRenderMode] != NULL) pFctTb[gRender.nRenderMode](gVar.pScreen, gRender.pScreen2x);

	if (nSync) FrameWait();
	SDL_Flip(gRender.nRenderMode == e_RenderMode_Normal ? gVar.pScreen : gRender.pScreen2x);

	gVar.nPalUpdate = 0;	// Dans tous les cas, RAZ du flag.

}

// Set video mode.
SDL_Surface * VideoModeSet(u32 nScrWidth, u32 nScrHeight, u32 nBPP, u32 nSDL_Flags)
{
	SDL_Surface *pSurf;

	pSurf = SDL_SetVideoMode(nScrWidth, nScrHeight, nBPP, SDL_SWSURFACE | nSDL_Flags);
	if (pSurf == NULL)
	{
		fprintf(stderr, "VideoModeSet(): Couldn't set video mode: %s\n",SDL_GetError());
		//exit(1);
	}
	else
		SDL_FillRect(pSurf, NULL, (nBPP == 8 ? CLR_BLACK_IDX : 0));	// Sous Windows, SDL_SetVideoMode génère un event SDL_VIDEOEXPOSE. Comme on gère F9 dans les events, ça évite le flash rose (0 = rose / 7 = noir).
	return (pSurf);
}

// Met le mode video qui va bien.
void Render_SetVideoMode(void)
{
	switch (gRender.nRenderMode)
	{
	case e_RenderMode_TV2x:
		gVar.nPalUpdate = 1;	// Au chgt de mode, on force le recalcul de la palette.
#if defined(BPP32_2X)
		gRender.pScreen2x = VideoModeSet(SCR_Width * 2, SCR_Height * 2, 32, (gRender.nFullscreenMode ? SDL_FULLSCREEN : 0));
#else
		gRender.pScreen2x = VideoModeSet(SCR_Width * 2, SCR_Height * 2, 16, (gRender.nFullscreenMode ? SDL_FULLSCREEN : 0));
#endif
		gVar.pScreen = gRender.pScreenBuf2;
		if (gRender.pScreen2x != NULL)		// Ok.
		{
//			SDL_SetPalette(gRender.pScreen2x, SDL_PHYSPAL | SDL_LOGPAL, gVar.pColors, 0, 256);
			SDL_SetPalette(gRender.pScreenBuf2, SDL_LOGPAL, gVar.pColors, 0, 256);
			return;
 		}
		// Erreur => On repasse en mode Scale 2x.
		gRender.nRenderMode = e_RenderMode_Scale2x;
		// ... et pas de break.

	case e_RenderMode_Scale2x:
		gVar.nPalUpdate = 1;	// Au chgt de mode, on force le recalcul de la palette.
#if defined(BPP32_2X)
		gRender.pScreen2x = VideoModeSet(SCR_Width * 2, SCR_Height * 2, 32, (gRender.nFullscreenMode ? SDL_FULLSCREEN : 0));
#else
		gRender.pScreen2x = VideoModeSet(SCR_Width * 2, SCR_Height * 2, 8, (gRender.nFullscreenMode ? SDL_FULLSCREEN : 0));
#endif
		gVar.pScreen = gRender.pScreenBuf2;
//		if (gRender.pScreen2x != NULL) return;		// Ok.
		if (gRender.pScreen2x != NULL)		// Ok.
		{
#if !defined(BPP32_2X)
			SDL_SetPalette(gRender.pScreen2x, SDL_PHYSPAL | SDL_LOGPAL, gVar.pColors, 0, 256);
#endif
			SDL_SetPalette(gRender.pScreenBuf2, SDL_LOGPAL, gVar.pColors, 0, 256);
			return;
 		}
		// Erreur => On repasse en mode Normal et Windowed.
		gRender.nRenderMode = e_RenderMode_Normal;
		gRender.nFullscreenMode = 0;
		// ... et pas de break.

	case e_RenderMode_Normal:
	default:
		gVar.nPalUpdate = 1;	// Au chgt de mode, on force le recalcul de la palette.
		gVar.pScreen = VideoModeSet(SCR_Width, SCR_Height, 8, (gRender.nFullscreenMode ? SDL_FULLSCREEN : 0));
		gRender.pScreen2x = NULL;
		if (gVar.pScreen == NULL) exit(1);	// Message d'erreur dans VideoModeSet.
		SDL_SetPalette(gVar.pScreen, SDL_PHYSPAL | SDL_LOGPAL, gVar.pColors, 0, 256);
		break;
	}

}

// Renvoie un ptr sur la surface écran réelle (pour les palettes).
SDL_Surface * Render_GetRealVideoSurfPtr(void)
{
	return (gRender.nRenderMode == e_RenderMode_Normal ? gVar.pScreen : gRender.pScreen2x);
}

// Init de la vidéo.
void Render_InitVideo(void)
{
	gRender.nRenderMode = e_RenderMode_Normal;
	gRender.nFullscreenMode = 0;

	gRender.pScreen2x = NULL;		// En mode 2x, ptr sur la surface écran.
	gRender.pScreenBuf2 = NULL;		// Buffer de rendu pour le jeu en mode 2x (à la place de la surface écran réelle).

	// On initialise d'abord un écran en mode e_RenderMode_Normal. Important, car on fait un CreateRGBSurface à partir de cette surface.
	gVar.pScreen = VideoModeSet(SCR_Width, SCR_Height, 8, gRender.nFullscreenMode ? SDL_FULLSCREEN : 0);
	if (gVar.pScreen == NULL) exit(1);
	// On créé un buffer de la taille de l'écran.
	// => En mode 2x, on switche le ptr pScreen sur cette surface, les rendus du jeu se font donc dedans. Puis on fait le scale/filtre du buffer vers la vraie surface écran.
	gRender.pScreenBuf2 = SDL_CreateRGBSurface(SDL_SWSURFACE, SCR_Width, SCR_Height, 8, gVar.pScreen->format->Rmask, gVar.pScreen->format->Gmask, gVar.pScreen->format->Bmask, 0);
	if (gRender.pScreenBuf2 == NULL)
	{
		fprintf(stderr, "Render_InitVideo(): Unable to allocate SDL surface: %s\n", SDL_GetError());
		exit(1);
	}

}

// Libère les ressources du rendu. (1 fois !).
void RenderRelease(void)
{
	SDL_FreeSurface(gRender.pScreenBuf2);
}

