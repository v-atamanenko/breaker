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

// Structure commune à tous les monstres.
#define MST_COMMON_DATA_SZ  64
struct SMstCommon
{
	u8	nUsed;			// 0 = slot vide, 1 = slot occupé.
	u8	nMstNo;			// No du monstre.

	void (*pFctInit) (struct SMstCommon *pMst);	// Fct d'init du monstre.
	s32 (*pFctMain) (struct SMstCommon *pMst);		// Fct principale du monstre.

	s32	nPosX, nPosY;
	s32	nSpd;
	u8	nAngle;
	s32	nAnm;			// Anim.
	u8	nPhase;

	u8	pData[MST_COMMON_DATA_SZ];	// On fera pointer les structures spécifiques ici.

};

extern	u32	gnMstPrio;		// Pour priorité de l'affichage.
#define	MSTPRIO_AND	31

// Prototypes.
void MstInitEngine(void);
void MstManage(void);
s32 MstAdd(u32 nMstNo, s32 nPosX, s32 nPosY);

u32 MstCheckRectangle(s32 nXMin, s32 nXMax, s32 nYMin, s32 nYMax);


