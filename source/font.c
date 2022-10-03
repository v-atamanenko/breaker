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


// itoa.
void MyItoA(s32 nNb, char *pDst)
{
	char	cMin = ' ';
	char	*pPtr;
	u32	nTmp;

	// Cas des nombres négatifs.
	if (nNb < 0)
	{
		cMin = '-';
		nNb = -nNb;
	}

	pPtr = pDst + strlen(pDst) - 1;
	nTmp = nNb;
	do
	{
		*pPtr-- = (char)((nTmp % 10) + '0');
	} while (pPtr >= pDst && (nTmp /= 10) > 0);

	// Négatifs.
	if (cMin != ' ' && pPtr >= pDst) *pPtr = cMin;

}


// Affichage d'une phrase en sprites.
// Renvoie la largeur en pixels de la phrase.
u32 Font_Print(s32 nPosX, s32 nPosY, char *pStr, u32 nFlags)
{
	char	cChr;
	struct SSprite	*pSpr;
	s32	nPosXOrg = nPosX;

	while (*pStr)
	{
		cChr = *pStr++;
		cChr -= ' ';
		if (cChr != 0)
		{
			// Char normal.
			cChr--;
			pSpr = SprGetDesc(e_Spr_FontSmall + cChr);
			if ((nFlags & FONT_NoDisp) == 0) SprDisplay(e_Spr_FontSmall + cChr, nPosX, nPosY, e_Prio_HUD);
		}
		else
		{
			// Espace, on avance de la taille d'un 'I'.
			pSpr = SprGetDesc(e_Spr_FontSmall + 'I' - ' ');
		}
		nPosX += pSpr->nLg + 1;
	}

	return ((u32)(nPosX - nPosXOrg));

}



