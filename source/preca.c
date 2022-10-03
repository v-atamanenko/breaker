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
#define PI 3.1415927

// Précalcul des tables de sinus-cosinus.
// 256 angles, val *256 (=> varie de -256 à 256).
void PrecaSinCos(void)
{
	u32	i;

	for (i = 0; i < 256 + 64; i++)
	{
		gVar.pSinCos[i] = (s16) (cos(i * 2 * PI / 256) * 256);
		//printf("i = %d : %f\n", i, cos(i * 2 * PI / 256) * 256);
		//printf("i = %d : %d\n", i, gVar.pSinCos[i]);
	}
	gVar.pSin = gVar.pSinCos + 64;
	gVar.pCos = gVar.pSinCos;

	/*
	for (i = 0; i < 256; i++)
	{
		printf("i = %d : sin = %d - cos = %d\n", i, gVar.pSin[i], gVar.pCos[i]);
	}
	*/

}


