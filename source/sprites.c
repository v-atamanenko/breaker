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

// Gestion des sprites.

#include "includes.h"

//#define	DISPLAY_INFO	1	// Commenter pour supprimer la sortie txt.

// Pour capture des sprites.
#define	SPR_MAX_NB	2048
struct SSprite	pSpr[SPR_MAX_NB];
u32	gnSprNbSprites;		// Nb de sprites captur�s.


// Pour tri des sprites � chaque frame.
struct SSprStockage
{
	u32 nSprNo;
	s32 nPosX, nPosY;
	u32 nPrio;

};
#define	SPR_STO_MAX	512
struct SSprStockage	gpSprSto[SPR_STO_MAX];
struct SSprStockage	*gpSprSort[SPR_STO_MAX];	// Pour tri.
u32	gnSprSto;			// Nb de sprites stock�s pour affichage.


// Initialisation du moteur.
void SprInitEngine(void)
{
	gnSprNbSprites = 0;	// Nb de sprites captur�s.
	gnSprSto = 0;		// Nb de sprites stock�s pour affichage.

}

// Nettoyage.
void SprRelease(void)
{
	u32	i;

	for (i = 0; i < gnSprNbSprites; i++)
	{
		free(pSpr[i].pGfx);
	}
}

// R�cup�ration des sprites d'une planche.
// In: pSprPal == NULL, on ne sauvegarde pas la palette.
//     pSprPal != NULL, on sauvegarde la palette de nPalIdx � 256.
void SprLoadBMP(char *pFilename, SDL_Color *pSprPal, u32 nPalIdx)
{
	SDL_Surface	*pPlanche;
	u32	nNbSprPlanche = 0;

	// Lecture du BMP.
	pPlanche = SDL_LoadBMP(pFilename);
	if (pPlanche == NULL) {
		fprintf(stderr, "Couldn't load picture: %s\n", SDL_GetError());
		exit(1);
	}

	// Sauvegarde la palette ?
	if (pSprPal != NULL)
	{
		SDL_Color	*pSrcPal = pPlanche->format->palette->colors;
		u32	i;

		for (i = nPalIdx; i < 256; i++)
		{
			pSprPal[i - nPalIdx] = pSrcPal[i];
		}
	}

	// On parcourt la planche pour en extraire les sprites.
	u32	ix, iy;
	u8	*pPix = (u8 *)pPlanche->pixels;
#ifdef	DISPLAY_INFO
printf("w = %d / h = %d\n", pPlanche->w, pPlanche->h);
#endif
	for (iy = 0; iy < (u32)pPlanche->h; iy++)
	{
		for (ix = 0; ix < (u32)pPlanche->w; ix++)
		{
			// On tombe sur un sprite ?
			if (*(pPix + (iy * pPlanche->pitch) + ix) == 0)
			{
				// On a encore de la place ?
				if (gnSprNbSprites >= SPR_MAX_NB)
				{
					printf("Spr: No more sprites slots available.\n");
					SprRelease();
					exit(1);
					// Fain�antise, on peut faire un syst�me de realloc tous les x sprites.
				}
#ifdef	DISPLAY_INFO
printf("sprite at (%d, %d)\n", (int)ix, (int)iy);
#endif
				u32	LgExt, HtExt;
				u32	PtRefX, PtRefY;		// Pts de ref.
				u32	ii, ij, ik;

				// Recherche des largeurs ext�rieures (cadre de 1 pixel). + Pts de ref.
				PtRefX = 0;
				LgExt = 1;
				ii = ix + 1;
				while(*(pPix + (iy * pPlanche->pitch) + ii) == 0 || *(pPix + (iy * pPlanche->pitch) + ii + 1) == 0)
				{
					if (*(pPix + (iy * pPlanche->pitch) + ii) != 0) PtRefX = LgExt - 1;
					ii++;
					LgExt++;
				}

				PtRefY = 0;
				HtExt = 1;
				ii = iy + 1;
				while(*(pPix + (ii * pPlanche->pitch) + ix) == 0 || *(pPix + ((ii + 1) * pPlanche->pitch) + ix) == 0)
				{
					if (*(pPix + (ii * pPlanche->pitch) + ix) != 0) PtRefY = HtExt - 1;
					ii++;
					HtExt++;
				}
#ifdef	DISPLAY_INFO
printf("lg ext = %d / ht ext = %d / ref (%d, %d)\n", (int)LgExt, (int)HtExt, (int)PtRefX, (int)PtRefY);
#endif
				// Stockage des valeurs.
				pSpr[gnSprNbSprites].nPtRefX = PtRefX;
				pSpr[gnSprNbSprites].nPtRefY = PtRefY;
				pSpr[gnSprNbSprites].nLg = LgExt - 2;
				pSpr[gnSprNbSprites].nHt = HtExt - 2;
				// Avec un seul malloc (taille gfx + taille masque).
				pSpr[gnSprNbSprites].pGfx = (u8 *)malloc(pSpr[gnSprNbSprites].nLg * pSpr[gnSprNbSprites].nHt * 2);
				if (pSpr[gnSprNbSprites].pGfx == NULL)
				{
					printf("Spr: malloc failed.\n");
					SprRelease();
					exit(1);
				}
				pSpr[gnSprNbSprites].pMask = pSpr[gnSprNbSprites].pGfx + (pSpr[gnSprNbSprites].nLg * pSpr[gnSprNbSprites].nHt);

				// R�cup�ration du sprite + g�n�ration du masque.
				ik = 0;
				for (ij = 0; ij < HtExt - 2; ij++)
				{
					for (ii = 0; ii < LgExt - 2; ii++)
					{
						pSpr[gnSprNbSprites].pGfx[ik] = *(pPix + ((iy + ij + 1) * pPlanche->pitch) + (ix + ii + 1));
						pSpr[gnSprNbSprites].pMask[ik] = (pSpr[gnSprNbSprites].pGfx[ik] ? 0 : 255);
						ik++;
					}
				}

				// Effacement du sprite dans la planche originale.
				for (ij = 0; ij < HtExt; ij++)
				{
					for (ii = 0; ii < LgExt; ii++)
					{
						*(pPix + ((iy + ij) * pPlanche->pitch) + (ix + ii)) = 255;
					}
				}

				// Termin�.
				nNbSprPlanche++;
				gnSprNbSprites++;

			}

		}
	}

#ifdef	DISPLAY_INFO
printf("Total sprites in '%s': %d.\n", pFilename, (int)nNbSprPlanche);
printf("Total sprites: %d.\n", (int)gnSprNbSprites);
#endif

	// On lib�re la surface.
	SDL_FreeSurface(pPlanche);

}

// Renvoie un ptr sur un descripteur de sprite.
struct SSprite *SprGetDesc(u32 nSprNo)
{
    return (&pSpr[nSprNo]);
}


// Affichage d'un sprite. (+ gestion de l'ombre).
// Avec �cran lock�.
void SprDisplayLock(u32 nSprNo, s32 nPosX, s32 nPosY)
{
	s32	nXMin, nXMax, nYMin, nYMax;
	s32	nSprXMin, nSprXMax, nSprYMin, nSprYMax;
	s32	diff;
	u8	*pScr = (u8 *)gVar.pScreen->pixels;

	u32	nSprFlags = nSprNo;		// Pour conserver les flags.

	nSprNo &= ~SPR_Flag_Shadow;

	nXMin = nPosX - pSpr[nSprNo].nPtRefX;
	nXMax = nXMin + pSpr[nSprNo].nLg - 1;
	nYMin = nPosY - pSpr[nSprNo].nPtRefY;
	nYMax = nYMin + pSpr[nSprNo].nHt - 1;

	nSprXMin = 0;
	nSprXMax = pSpr[nSprNo].nLg - 1;
	nSprYMin = 0;
	nSprYMax = pSpr[nSprNo].nHt - 1;

	// Clips.
	if (nXMin < 0)
	{
		diff = 0 - nXMin;
		nSprXMin += diff;
	}
	if (nXMax > SCR_Width - 1)
	{
		diff = nXMax - (SCR_Width - 1);
		nSprXMax -= diff;
	}
	// Sprite compl�tement en dehors ?
	if (nSprXMin - nSprXMax > 0) return;
	//
	if (nYMin < 0)
	{
		diff = 0 - nYMin;
		nSprYMin += diff;
	}
	if (nYMax > SCR_Height - 1)
	{
		diff = nYMax - (SCR_Height - 1);
		nSprYMax -= diff;
	}
	// Sprite compl�tement en dehors ?
	if (nSprYMin - nSprYMax > 0) return;


	s32	ix, iy;
	u32	b4, b1, b4b, b1b;
	u8	*pMsk = pSpr[nSprNo].pMask;
	u8	*pGfx = pSpr[nSprNo].pGfx;
	s32	nScrPitch = gVar.pScreen->pitch;

	b1b = nSprXMax - nSprXMin + 1;
	b4b = b1b >> 2;		// Nb de quads.
	b1b &= 3;			// Nb d'octets restants ensuite.
	pScr += ((nYMin + nSprYMin) * nScrPitch) + nXMin;
	pMsk += (nSprYMin * pSpr[nSprNo].nLg);
	pGfx += (nSprYMin * pSpr[nSprNo].nLg);

	if (nSprFlags & SPR_Flag_Shadow)
	{
		// Affichage d'une ombre.
		u8	*pSrc = (u8 *)gVar.pLevel->pixels;			// Source = image du level.
		s32	nSrcPitch = gVar.pLevel->pitch;

		pSrc += ((nYMin + nSprYMin) * nSrcPitch) + nXMin;

		for (iy = nSprYMin; iy <= nSprYMax; iy++)
		{
			b4 = b4b;
			for (ix = nSprXMin; b4; b4--, ix += 4)
			{
				*(u32 *)(pScr + ix) &= *(u32 *)(pMsk + ix);	//spd++
				*(u32 *)(pScr + ix) |= ( *(u32 *)(pSrc + ix) & ~*(u32 *)(pMsk + ix) ) +
									(~*(u32 *)(pMsk + ix) & 0x06060606);
			}
			b1 = b1b;
			for (; b1; b1--, ix++)
			{
				*(pScr + ix) &= *(pMsk + ix);	//spd++
				*(pScr + ix) |= ( *(pSrc + ix) & ~*(pMsk + ix) ) + (~*(pMsk + ix) & 0x06);
			}
			pScr += nScrPitch;
			pSrc += nSrcPitch;
			pMsk += pSpr[nSprNo].nLg;
			pGfx += pSpr[nSprNo].nLg;
		}

	}
	else
	{
		// Affichage normal.

		for (iy = nSprYMin; iy <= nSprYMax; iy++)
		{
			b4 = b4b;
			for (ix = nSprXMin; b4; b4--, ix += 4)
			{
				*(u32 *)(pScr + ix) &= *(u32 *)(pMsk + ix);
				*(u32 *)(pScr + ix) |= *(u32 *)(pGfx + ix);
			}
			b1 = b1b;
			for (; b1; b1--, ix++)
			{
				*(pScr + ix) &= *(pMsk + ix);
				*(pScr + ix) |= *(pGfx + ix);
			}
			pScr += nScrPitch;
			pMsk += pSpr[nSprNo].nLg;
			pGfx += pSpr[nSprNo].nLg;
		}

	}

}

// Inscrit les sprites dans une liste.
void SprDisplay(u32 nSprNo, s32 nPosX, s32 nPosY, u32 nPrio)
{
//	if (gnSprSto >= SPR_STO_MAX) return;
	if (gnSprSto >= SPR_STO_MAX) { fprintf(stderr, "Sprites: Out of slots!\n"); return; }

	if (nSprNo == SPR_NoSprite) return;			// Peut servir pour des clignotements, par exemple.

	gpSprSto[gnSprSto].nSprNo = nSprNo;
	gpSprSto[gnSprSto].nPosX = nPosX;
	gpSprSto[gnSprSto].nPosY = nPosY;
	gpSprSto[gnSprSto].nPrio = nPrio;
	gpSprSort[gnSprSto] = &gpSprSto[gnSprSto];	// Pour tri.

	gnSprSto++;

}

// La comparaison du qsort.
int qscmp(const void *pEl1, const void *pEl2)
{
	return ((*(struct SSprStockage **)pEl1)->nPrio - (*(struct SSprStockage **)pEl2)->nPrio);
}

// Trie la liste des sprites et les affiche.
// A appeler une fois par frame.
void SprDisplayAll(void)
{
	u32	i;

	if (gnSprSto == 0) return;		// Rien � faire ?

	// Tri sur la priorit�.
	qsort(gpSprSort, gnSprSto, sizeof(struct SSprStockage *), qscmp);

	// Affichage.
	SDL_LockSurface(gVar.pScreen);
	// Premi�re passe pour les ombres (en dessous de tout).
	for (i = 0; i < gnSprSto; i++)
		if (gpSprSort[i]->nSprNo & SPR_Flag_Shadow)
		{
			SprDisplayLock(gpSprSort[i]->nSprNo, gpSprSort[i]->nPosX + SHADOW_OfsX, gpSprSort[i]->nPosY + SHADOW_OfsY);
			gpSprSort[i]->nSprNo &= ~SPR_Flag_Shadow;
		}
	// Sprites normaux.
	for (i = 0; i < gnSprSto; i++)
		SprDisplayLock(gpSprSort[i]->nSprNo, gpSprSort[i]->nPosX, gpSprSort[i]->nPosY);
	SDL_UnlockSurface(gVar.pScreen);

	// RAZ pour le prochain tour.
	gnSprSto = 0;

}


// Teste une collision entre 2 sprites.
// Out: 1 col, 0 pas col.
u32 SprCheckColBox(u32 nSpr1, s32 nPosX1, s32 nPosY1, u32 nSpr2, s32 nPosX2, s32 nPosY2)
{
	s32	nXMin1, nXMax1, nYMin1, nYMax1;
	s32	nXMin2, nXMax2, nYMin2, nYMax2;
	struct SSprite *pSpr1 = SprGetDesc(nSpr1);
	struct SSprite *pSpr2 = SprGetDesc(nSpr2);

	nXMin1 = nPosX1 - pSpr1->nPtRefX;
	nXMax1 = nXMin1 + pSpr1->nLg;
	nYMin1 = nPosY1 - pSpr1->nPtRefY;
	nYMax1 = nYMin1 + pSpr1->nHt;

	nXMin2 = nPosX2 - pSpr2->nPtRefX;
	nXMax2 = nXMin2 + pSpr2->nLg;
	nYMin2 = nPosY2 - pSpr2->nPtRefY;
	nYMax2 = nYMin2 + pSpr2->nHt;

	// Collisions entre les rectangles ?
	if (nXMax1 >= nXMin2 && nXMin1 <= nXMax2 && nYMax1 >= nYMin2 && nYMin1 <= nYMax2)
	{
		return (1);
	}

	return (0);
}


