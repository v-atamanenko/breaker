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

#include "includes.h"

#define	TRANSIT_WIDTH	16
#define	TRANSIT_SLOTS	((SCR_Width/TRANSIT_WIDTH) + (SCR_Height/TRANSIT_WIDTH))
struct STransit
{
	u32	nOn;
	u32	nFinished;					// 0 = pas fini / 1 = fini. (Il faut continuer à tracer la transition fermante une fois finie !).
	u8	pnDelay[TRANSIT_SLOTS];		// Delai avant lancement.
	u8	pnInc[TRANSIT_SLOTS];		// Incréments de 0 à 8.

};
struct STransit	gTransit;

//=====================================

// Initialisation de la transition 'in' (noir > qqchose).
void TransitIn_Init(void)
{
	u32	i;

	gTransit.nOn = 1;
	gTransit.nFinished = 0;

	for (i = 0; i < TRANSIT_SLOTS; i++)
	{
		gTransit.pnDelay[i] = i;
		gTransit.pnInc[i] = 0;
	}
}

// Gestion de la transition.
void TransitIn_Manage(void)
{
	u32	i;
	u32	nSlotsEnded = 0;

	// Incrémentation des indices.
	for (i = 0; i < TRANSIT_SLOTS; i++)
	{
		if (gTransit.pnDelay[i])
			gTransit.pnDelay[i]--;
		else
		{
			if (gTransit.pnInc[i] < TRANSIT_WIDTH)
				gTransit.pnInc[i]++;
			else
				nSlotsEnded++;
		}
	}

	// Tracé.
	SDL_LockSurface(gVar.pScreen);

	u8	*pScr = (u8 *)gVar.pScreen->pixels;
	u8	*pScr2;
	s32	nScrPitch = gVar.pScreen->pitch;
	u32	j, k, m, n;
	s32	nOffset = 0;
	u32	nOffs2;

	for (i = 0; i < TRANSIT_SLOTS; i++)
	{
		pScr2 = pScr;
		nOffs2 = nOffset;
		nOffset += TRANSIT_WIDTH;

		for (j = 0; j < SCR_Height; j++)
		{
			if (!((s32)nOffs2 + gTransit.pnInc[i] >= SCR_Width || (s32)(nOffs2 + TRANSIT_WIDTH) <= 0))	// On dégage les segments complètement dehors.
			{

				if (nOffs2 + gTransit.pnInc[i] < SCR_Width && nOffs2 + TRANSIT_WIDTH < SCR_Width)		// unsigned!
				{
					// Cas sans problèmes.
					m = TRANSIT_WIDTH - gTransit.pnInc[i];
					n = m & 3;	// Bytes restants.
					m >>= 2;	// Quads.
					for (k = gTransit.pnInc[i]; m; m--, k += sizeof(u32))
						*(u32 *)(pScr2 + nOffs2 + k) = ((CLR_BLACK_IDX << 24) | (CLR_BLACK_IDX << 16) | (CLR_BLACK_IDX << 8) | CLR_BLACK_IDX);
//						*(u32 *)(pScr2 + nOffs2 + k) = ((8 << 24) | (8 << 16) | (8 << 8) | 8);
					for (; n; n--, k += sizeof(u8))
						*(pScr2 + nOffs2 + k) = CLR_BLACK_IDX;
//						*(pScr2 + nOffs2 + k) = 9;
				}
				else
				{
					// Cas avec problèmes.
					for (k = gTransit.pnInc[i]; k < TRANSIT_WIDTH; k++)
					{
						if (nOffs2 + k < SCR_Width)		// unsigned!
							*(pScr2 + nOffs2 + k) = CLR_BLACK_IDX;
//							*(pScr2 + nOffs2 + k) = 0;
					}
				}

			}

/*
// 0 optim :
			for (k = gTransit.pnInc[i]; k < TRANSIT_WIDTH; k++)
			{
				if (nOffs2 + k < SCR_Width)		// unsigned!
					*(pScr2 + nOffs2 + k) = CLR_BLACK_IDX;
			}
*/

			nOffs2--;
			pScr2 += nScrPitch;
		}

	}

	SDL_UnlockSurface(gVar.pScreen);

	// Terminé ?
	if (nSlotsEnded == TRANSIT_SLOTS)
	{
		gTransit.nOn = 0;
		gTransit.nFinished = 1;
	}
}

//=====================================

// Initialisation de la transition 'out' (qqchose > noir).
void TransitOut_Init(void)
{
	u32	i;

	gTransit.nOn = 2;
	gTransit.nFinished = 0;

	for (i = 0; i < TRANSIT_SLOTS; i++)
	{
		gTransit.pnDelay[i] = i;
		gTransit.pnInc[i] = 0;
	}
}

// Gestion de la transition.
void TransitOut_Manage(void)
{
	u32	i;
	u32	nSlotsEnded = 0;

	// Incrémentation des indices.
	for (i = 0; i < TRANSIT_SLOTS; i++)
	{
		if (gTransit.pnDelay[i])
			gTransit.pnDelay[i]--;
		else
		{
			if (gTransit.pnInc[i] < TRANSIT_WIDTH)
				gTransit.pnInc[i]++;
			else
				nSlotsEnded++;
		}
	}

	// Tracé.
	SDL_LockSurface(gVar.pScreen);

	u8	*pScr = (u8 *)gVar.pScreen->pixels;
	u8	*pScr2;
	s32	nScrPitch = gVar.pScreen->pitch;
	u32	j, k, m, n;
	s32	nOffset = 0;
	u32	nOffs2;

	for (i = 0; i < TRANSIT_SLOTS && gTransit.pnDelay[i] == 0; i++)
	{
		pScr2 = pScr;
		nOffs2 = nOffset;
		nOffset += TRANSIT_WIDTH;

		for (j = 0; j < SCR_Height; j++)
		{
			if (!((s32)nOffs2 >= SCR_Width || (s32)(nOffs2 + gTransit.pnInc[i]) <= 0))	// On dégage les segments complètement dehors.
			{

				if (nOffs2 < SCR_Width && nOffs2 + gTransit.pnInc[i] < SCR_Width)		// unsigned!
				{
					// Cas sans problèmes.
					m = gTransit.pnInc[i];
					n = m & 3;	// Bytes restants.
					m >>= 2;	// Quads.
					for (k = 0; m; m--, k += sizeof(u32))
						*(u32 *)(pScr2 + nOffs2 + k) = ((CLR_BLACK_IDX << 24) | (CLR_BLACK_IDX << 16) | (CLR_BLACK_IDX << 8) | CLR_BLACK_IDX);
//						*(u32 *)(pScr2 + nOffs2 + k) = ((8 << 24) | (8 << 16) | (8 << 8) | 8);
					for (; n; n--, k += sizeof(u8))
						*(pScr2 + nOffs2 + k) = CLR_BLACK_IDX;
//						*(pScr2 + nOffs2 + k) = 9;
				}
				else
				{
					// Cas avec problèmes.
					for (k = 0; k < gTransit.pnInc[i]; k++)
					{
						if (nOffs2 + k < SCR_Width)		// unsigned!
							*(pScr2 + nOffs2 + k) = CLR_BLACK_IDX;
//							*(pScr2 + nOffs2 + k) = 0;
					}
				}

			}

/*
// 0 optim :
			for (k = 0; k < gTransit.pnInc[i]; k++)
			{
				if (nOffs2 + k < SCR_Width)		// unsigned!
					*(pScr2 + nOffs2 + k) = CLR_BLACK_IDX;
			}
*/

			nOffs2--;
			pScr2 += nScrPitch;
		}

	}

	SDL_UnlockSurface(gVar.pScreen);

	// Terminé ?
	if (nSlotsEnded == TRANSIT_SLOTS) gTransit.nFinished = 2;

}

// Returns 1 if transit is finished, 0 otherwise.
u32 TransitOut_IsFinished(void)
{
	return (gTransit.nFinished == 2 ? 1 : 0);
}

//=====================================

// Returns 1 if transit IN is active, 2 if transit OUT is active, and 0 otherwise.
u32 Transit_IsActive(void)
{
	return (gTransit.nOn);
}

// Gestion de la transition.
void Transit_Manage(void)
{
	if (gTransit.nOn == 0) return;
	if (gTransit.nOn == 1)
		TransitIn_Manage();
	else
		TransitOut_Manage();
}

