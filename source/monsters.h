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

// Structures.
struct SMstTb
{
	void (*pFctInit) (struct SMstCommon *pMst);
	s32 (*pFctMain) (struct SMstCommon *pMst);
	u64	*pAnm;
	u16	nPoints;
};


// Prototypes.
void MstDoorROpen(void);
u32 MstPill_GetNb(void);
u32 MstCheckStructSizes(void);	// Debug.


// Liste des monstres.
enum
{
	e_Mst_Pill_0 = 0,		// Pour base du random.
	e_Mst_Pill_Aimant = e_Mst_Pill_0,
	e_Mst_Pill_Mitrailleuse,
	e_Mst_Pill_BallTraversante,
	e_Mst_Pill_BallBigger,
	e_Mst_Pill_BallX3,
	e_Mst_Pill_RaqRallonge,
	e_Mst_Pill_RaqReduit,
	e_Mst_Pill_1Up,
	e_Mst_Pill_DoorR,
	e_Mst_Pill_SpeedUp,
	e_Mst_Pill_SpeedDown,
	e_Mst_Generateur,		// Generateur d'ennemis.
	e_Mst_Mst1,				// Monstres des niveaux (x & 3).
	e_Mst_DoorR,			// Porte à droite.
	e_Mst_Doh,				// Doh !

};



