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
// Petit moteur de monstres.
//

#include "includes.h"

// externs.
extern	struct SMstTb gpMstTb[];

#define	MST_MAX_SLOTS	32
struct SMstCommon	gpMstSlots[MST_MAX_SLOTS];
u32	gnMstLastUsed;

u32	gnMstPrio;		// Pour priorit� de l'affichage.


// RAZ moteur.
void MstInitEngine(void)
{
	u32	i;

	// RAZ de tous les slots.
	for(i = 0; i < MST_MAX_SLOTS; i++)
	{
		gpMstSlots[i].nUsed = 0;
	}
	gnMstLastUsed = 0;

}

// Cherche un slot libre.
// Out : N� d'un slot libre. -1 si erreur.
s32 MstGetSlot(void)
{
	u32	i;

	for (i = gnMstLastUsed; i < MST_MAX_SLOTS; i++)
	{
		if (gpMstSlots[i].nUsed == 0)
		{
			gnMstLastUsed = i + 1;		// La recherche commencera au suivant.
			return (i);
		}
	}
	return (-1);
}

// Lib�re un slot.
void MstReleaseSlot(u32 nSlotNo)
{
	// Lib�re l'anim.
	if (gpMstSlots[nSlotNo].nAnm != -1) AnmReleaseSlot(gpMstSlots[nSlotNo].nAnm);
	// Pour acc�l�rer la recherche des slots libres.
	if (nSlotNo < gnMstLastUsed)
	{
		gnMstLastUsed = nSlotNo;
	}
	gpMstSlots[nSlotNo].nUsed = 0;

}

// Ajoute un monstre dans la liste.
s32 MstAdd(u32 nMstNo, s32 nPosX, s32 nPosY)
{
	s32	nSlotNo;

	if ((nSlotNo = MstGetSlot()) == -1) return (-1);
	gpMstSlots[nSlotNo].nAnm = -1;
	if (gpMstTb[nMstNo].pAnm != NULL)	// Si NULL, on ne r�serve pas d'anim.
	{
		if ((gpMstSlots[nSlotNo].nAnm = AnmSet(gpMstTb[nMstNo].pAnm, -1)) == -1) return (-1);	// Pour r�server une anim.
	}

	gpMstSlots[nSlotNo].nUsed = 1;
	gpMstSlots[nSlotNo].nMstNo = nMstNo;

	gpMstSlots[nSlotNo].nPosX = nPosX << 8;
	gpMstSlots[nSlotNo].nPosY = nPosY << 8;

	gpMstSlots[nSlotNo].nSpd = 0;
	gpMstSlots[nSlotNo].nAngle = 0;
	gpMstSlots[nSlotNo].pFctInit = gpMstTb[nMstNo].pFctInit;
	gpMstSlots[nSlotNo].pFctMain = gpMstTb[nMstNo].pFctMain;
	// Appel de la fonction d'init du monstre.
	gpMstSlots[nSlotNo].pFctInit(&gpMstSlots[nSlotNo]);

	return (nSlotNo);
}


// Gestion des monstres.
void MstManage(void)
{
	u32	i;

	gnMstPrio = 0;
	for (i = 0; i < MST_MAX_SLOTS; i++)
	{
		if (gpMstSlots[i].nUsed)
		{
			if (gpMstSlots[i].pFctMain(&gpMstSlots[i]) == -1)
			{
				// Le monstre est mort, on lib�re le slot.
				MstReleaseSlot(i);
			}
			gnMstPrio = (gnMstPrio + 1) & MSTPRIO_AND;
		}
	}

}


// Teste si un monstre se trouve dans un rectangle (pour retour des briques qui reviennent).
u32 MstCheckRectangle(s32 nXMin, s32 nXMax, s32 nYMin, s32 nYMax)
{
	u32	i;

	for (i = 0; i < MST_MAX_SLOTS; i++)
	{
		if (gpMstSlots[i].nUsed)
		{
			if (gpMstSlots[i].nPosX >= nXMin && gpMstSlots[i].nPosX <= nXMax &&
				gpMstSlots[i].nPosY >= nYMin && gpMstSlots[i].nPosY <= nYMax) return (1);
		}
	}

	return (0);
}

