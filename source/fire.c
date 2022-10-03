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

// Gestion des tirs.

#include "includes.h"

// D�finition des tirs.
struct SFireRecord
{
	u64	*pAnm;
	s32	nSpeed;
	u8	nAngle;
	u8	nPlyr;		// 1 = Tir du joueur, 0 = Tir d'un monstre;
	u64	*pAnmDust;	// Anim de disparition. NULL si pas utilis�e.

};
struct SFireRecord	gpFireTable[] =
{
	{ gAnm_PlyrShot, 0x400, 64, 1, NULL },		// Joueur: Tir de la mitrailleuse.
	{ gAnm_DohMissile, 0x200, 64+128, 0, gAnm_DohMissileDisp },	// Doh: Tir.

//todo: rajouter un dust g�n�rique pour le tir ?
};


struct SFire
{
	u8	nUsed;			// 0 = slot vide, 1 = slot occup�.

	s32	nAnm;			// Anim.
	s32	nPosX, nPosY;	// 8b de virgule fixe.
	s32	nSpeed;
	u8	nAngle;
	u8	nPlyr;
	u64	*pAnmDust;		// Anim de disparition. NULL si pas utilis�e.

};

#define	FIRE_MAX_SLOTS	64
struct SFire	gpFireSlots[FIRE_MAX_SLOTS];
u32	gnFireLastUsed;


// RAZ moteur.
void FireInitEngine(void)
{
	u32	i;

	// RAZ de tous les slots.
	for(i = 0; i < FIRE_MAX_SLOTS; i++)
	{
		gpFireSlots[i].nUsed = 0;
	}
	gnFireLastUsed = 0;

}

// Cherche un slot libre.
// Out : N� d'un slot libre. -1 si erreur.
s32 FireGetSlot(void)
{
	u32	i;

	for (i = gnFireLastUsed; i < FIRE_MAX_SLOTS; i++)
	{
		if (gpFireSlots[i].nUsed == 0)
		{
			gnFireLastUsed = i + 1;		// La recherche commencera au suivant.
			return (i);
		}
	}
	return (-1);
}

// Lib�re un slot.
void FireReleaseSlot(u32 nSlotNo)
{
	// Lib�re l'anim.
	AnmReleaseSlot(gpFireSlots[nSlotNo].nAnm);
	// Pour acc�l�rer la recherche des slots libres.
	if (nSlotNo < gnFireLastUsed)
	{
		gnFireLastUsed = nSlotNo;
	}
	gpFireSlots[nSlotNo].nUsed = 0;

}

// Init d'un tir.
// In : sAngle = -1 => On prend l'angle par d�faut. Sinon val [0;255] => Angle.
// Out : N� du slot. -1 si erreur.
s32 FireAdd(u32 nShot, s32 nPosX, s32 nPosY, s32 nAngle)
{
	s32	nSlotNo;

	if ((nSlotNo = FireGetSlot()) == -1) return (-1);
	if ((gpFireSlots[nSlotNo].nAnm = AnmSet(gpFireTable[nShot].pAnm, -1)) == -1) return (-1);

	gpFireSlots[nSlotNo].nUsed = 1;
	gpFireSlots[nSlotNo].nPosX = nPosX << 8;
	gpFireSlots[nSlotNo].nPosY = nPosY << 8;
	gpFireSlots[nSlotNo].nSpeed = gpFireTable[nShot].nSpeed;
	gpFireSlots[nSlotNo].nAngle = (nAngle == -1 ? gpFireTable[nShot].nAngle : (nAngle & 0xFF));
	gpFireSlots[nSlotNo].nPlyr = gpFireTable[nShot].nPlyr;
	gpFireSlots[nSlotNo].pAnmDust = gpFireTable[nShot].pAnmDust;

	return (nSlotNo);
}


// Gestion des tirs.
void FireManage(void)
{
	u32	i;

	for (i = 0; i < FIRE_MAX_SLOTS; i++)
	{
		if (gpFireSlots[i].nUsed)
		{
			s32	nSpr;

			nSpr = AnmGetImage(gpFireSlots[i].nAnm);
			if (nSpr == -1)
			{
				// L'anim est finie. On kille le tir.
				FireReleaseSlot(i);
			}
			else
			{
				// D�placement du tir.
				gpFireSlots[i].nPosX += (gVar.pCos[gpFireSlots[i].nAngle] * gpFireSlots[i].nSpeed) >> 8;
				gpFireSlots[i].nPosY += (gVar.pSin[gpFireSlots[i].nAngle] * gpFireSlots[i].nSpeed) >> 8;
				// Clip ? (simplifi�, le joueur tire vers le haut, le boss vers le bas).
				if (gpFireSlots[i].nPosY >> 8 <= WALL_YMin || gpFireSlots[i].nPosY >> 8 >= SCR_Height + 5)
				{
					FireReleaseSlot(i);
					// eventuellement, dust.
					goto _Skip;
				}


				// Sp�cifique au casse-brique, on teste les collisions briques-tir ici.
				if (gpFireSlots[i].nPlyr)
				{
					// Collision avec une brique ?
					// Avancement < hauteur d'une brique => On teste en (x,y) directement.
					s32	nBx, nBy;
				//todo: comme pour CollBricks(), on peut faire deux tables pour �viter toutes ces divisions. (??? Dans CollBricks, le test n'a pas �t� concluant...).
					nBx = ((gpFireSlots[i].nPosX >> 8) - WALL_XMin) / BRICK_Width;
					nBy = ((gpFireSlots[i].nPosY >> 8) - WALL_YMin) / BRICK_Height;
					if (BrickHit(nBx, nBy, 0) != (u32)-1)
					{
						FireReleaseSlot(i);
						// ... eventuellement, dust. A placer en bas de la brique.
						goto _Skip;
					}
				}


				// Collision avec un monstre ? => Les monstres viendront tester les tirs.

				// Affichage du tir.
				SprDisplay(nSpr, gpFireSlots[i].nPosX >> 8, gpFireSlots[i].nPosY >> 8, e_Prio_Tirs);
_Skip:
				i = i;
			}
		}
	}

}


//=============================================================================
// Teste si un monstre se prend un tir.
// Out: 0 = Pas de choc / 1 = Hit. (Eventuellement, renvoyer le nb de pts de d�gats...).
u32 MstCheckFire(u32 nSpr, s32 nPosX, s32 nPosY)//, u32 pDustAnm = NULL)	// Pour dust particulier en fct du monstre. Si NULL, utiliser dust g�n�rique.
{
	u32	i;

	s32	nXMin1, nXMax1, nYMin1, nYMax1;
	struct SSprite *pSpr1 = SprGetDesc(nSpr);

	nXMin1 = nPosX - pSpr1->nPtRefX;
	nXMax1 = nXMin1 + pSpr1->nLg;
	nYMin1 = nPosY - pSpr1->nPtRefY;
	nYMax1 = nYMin1 + pSpr1->nHt;

	nXMin1 <<= 8; nXMax1 <<= 8;		// D�calage, pour optimiser les comparaisons.
	nYMin1 <<= 8; nYMax1 <<= 8;		// Ca �vitera de faire un d�calage pour chaque tir, et pour chaque monstre.

	for (i = 0; i < FIRE_MAX_SLOTS; i++)
	{
		if (gpFireSlots[i].nUsed)
		{
			// Le test suivant est comment� pour le casse briques :
			// Ne sert � rien parce qu'il n'y a que le joueur qui tire, sauf au niveau du boss, dans lequel le joueur ne peut pas tirer.
			//if (gpFireSlots[i].nPlyr)
			//{
				// On teste juste un point. Si on veut les rectangles, appeler SprCheckColBox().
				if (gpFireSlots[i].nPosX >= nXMin1 && gpFireSlots[i].nPosX <= nXMax1 &&
					gpFireSlots[i].nPosY >= nYMin1 && gpFireSlots[i].nPosY <= nYMax1)
				{
					/* Dust ok, mais pas utilis�. (Quand le joueur se prend un tir au niveau du boss, �a ne se voit tellement pas qu'autant le laisser comment�).
					// Dust de disparition.
					if (gpFireSlots[i].pAnmDust != NULL)
					{
						DustSet(gpFireSlots[i].pAnmDust, gpFireSlots[i].nPosX >> 8, gpFireSlots[i].nPosY >> 8);
					}
					*/
					FireReleaseSlot(i);
					return (1);
					// Pour d'autres types de jeu, continuer la boucle pour que l'ennemi puisse se prendre plusieurs balles en 1 fois et/ou arr�ter les tirs.
				}
			//}

		}
	}

	return (0);

}


//=============================================================================
// Supprime les tirs du boss et les remplace par un dust.
void FireRemoveDohShoots(void)
{
	u32	i;

	for (i = 0; i < FIRE_MAX_SLOTS; i++)
	{
		if (gpFireSlots[i].nUsed)
		{
			// Dust de disparition.
			if (gpFireSlots[i].pAnmDust != NULL)
			{
				DustSet(gpFireSlots[i].pAnmDust, gpFireSlots[i].nPosX >> 8, gpFireSlots[i].nPosY >> 8);
			}
			FireReleaseSlot(i);
		}
	}

}


