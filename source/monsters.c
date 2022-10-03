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

//=============================================================================
void MstInit_Pill(struct SMstCommon *pMst);
s32 MstMain_Pill(struct SMstCommon *pMst);
void MstInit_Generateur(struct SMstCommon *pMst);
s32 MstMain_Generateur(struct SMstCommon *pMst);
void MstInit_Mst1(struct SMstCommon *pMst);
s32 MstMain_Mst1(struct SMstCommon *pMst);
void MstInit_DoorR(struct SMstCommon *pMst);
s32 MstMain_DoorR(struct SMstCommon *pMst);
void MstInit_Doh(struct SMstCommon *pMst);
s32 MstMain_Doh(struct SMstCommon *pMst);

struct SMstTb gpMstTb[] =
{
	{ MstInit_Pill, MstMain_Pill, gAnm_Itm1, 100 },		// Pill: Aimant.
	{ MstInit_Pill, MstMain_Pill, gAnm_Itm2, 100 },		// Pill: Mitrailleuse.
	{ MstInit_Pill, MstMain_Pill, gAnm_Itm3, 100 },		// Pill: Balle traversante.
	{ MstInit_Pill, MstMain_Pill, gAnm_Itm4, 100 },		// Pill: Balle bigger.
	{ MstInit_Pill, MstMain_Pill, gAnm_Itm5, 100 },		// Pill: Balle x3
	{ MstInit_Pill, MstMain_Pill, gAnm_Itm6, 100 },		// Pill: Raquette bigger.
	{ MstInit_Pill, MstMain_Pill, gAnm_Itm7, 0 },		// Pill: Raquette smaller.
	{ MstInit_Pill, MstMain_Pill, gAnm_Itm8, 100 },		// Pill: 1Up.
	{ MstInit_Pill, MstMain_Pill, gAnm_Itm9, 100 },		// Pill: Porte à droite.
	{ MstInit_Pill, MstMain_Pill, gAnm_Itm10, 100 },	// Pill: Speed Up.
	{ MstInit_Pill, MstMain_Pill, gAnm_Itm11, 100 },	// Pill: Speed Down.
	{ MstInit_Generateur, MstMain_Generateur, gAnm_MstDoorWait, 0 },
	{ MstInit_Mst1, MstMain_Mst1, gAnm_Mst1, 100 },		// Monstres basiques des niveaux.
	{ MstInit_DoorR, MstMain_DoorR, gAnm_MstDoorRight, 0 },	// Porte à droite.
	{ MstInit_Doh, MstMain_Doh, gAnm_MstDohAppears, 10000 },	// Doh.

};

//=============================================================================
// Variables générales spécifiques.
struct SMstMisc
{
	u32	nNbMstLev;		// Pour compter le nombre de monstres présents. (3 max).
	// Pas dans la struct du générateur, parce que les autres monstres accèdent aussi à la variable.

	u32	nPillNb;		// Nb de pillules présente en même temps.

	u8	nMstDoorR:1;	// Flag pour déclencher l'ouverture de la porte.
	// Déclenchement quand on attrape la pillule.

};
struct SMstMisc	gMstMisc;

//=============================================================================
// Monstre pillule (bonus).

// Renvoie le nombre de pillules présentes à l'écran.
u32 MstPill_GetNb(void)
{
	return (gMstMisc.nPillNb);
}

// Init pour pillules.
void MstInit_Pill(struct SMstCommon *pMst)
{
	gMstMisc.nPillNb++;
}

// Routine commune à toutes les pillules.
s32 MstMain_Pill(struct SMstCommon *pMst)
{
	s32	nSpr;

	// Déplacement.
	pMst->nPosY += 0x100;
	// Sortie de l'écran ?
	if (pMst->nPosY >> 8 > SCR_Height + 8)
	{
		// Tuage de l'ennemi.
		gMstMisc.nPillNb--;
		return (-1);
	}

	// Contact avec le joueur ?
	nSpr = AnmGetImage(pMst->nAnm);
	if (AnmGetKey(gBreak.nPlayerAnmNo) != e_AnmKey_PlyrDeath)
	if (SprCheckColBox(AnmGetLastImage(gBreak.nPlayerAnmNo), gBreak.nPlayerPosX, gBreak.nPlayerPosY,
		nSpr, pMst->nPosX >> 8, pMst->nPosY >> 8))
	{
		u32	nSfx = e_Sfx_PillBonus;		// Fx par défaut, bonus.

		// Bonus.
		switch (pMst->nMstNo)
		{
		case e_Mst_Pill_Aimant:
			BreakerBonusSetAimant();
			break;

		case e_Mst_Pill_Mitrailleuse:
			BreakerBonusSetMitrailleuse();
			break;

		case e_Mst_Pill_BallTraversante:
			BreakerBonusBallTraversante();
			break;

		case e_Mst_Pill_BallBigger:
			BreakerBonusBallBigger();
			break;

		case e_Mst_Pill_BallX3:
			BreakerBonusBallX3();
			break;

		case e_Mst_Pill_RaqRallonge:
			BreakerBonusRaquetteSize(1);
			break;

		case e_Mst_Pill_RaqReduit:
			nSfx = e_Sfx_PillMalus;
			BreakerBonusRaquetteSize(-1);
			break;

		case e_Mst_Pill_1Up:
			nSfx = (u32)-1;	// No sound, sfx lancé par le bonus.
			BreakerBonus1Up();
			break;

		case e_Mst_Pill_DoorR:
			MstDoorROpen();
			break;

		case e_Mst_Pill_SpeedUp:
			nSfx = e_Sfx_PillMalus;
			BreakerBonusSpeedUp(1);
			break;

		case e_Mst_Pill_SpeedDown:
			BreakerBonusSpeedUp(-1);
			break;
		}

		// Score.
		gBreak.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;
		// Sfx.
		Sfx_PlaySfx(nSfx, e_SfxPrio_30);

		// (dust, éventuellement).

		// Tuage de l'ennemi.
		gMstMisc.nPillNb--;
		return (-1);
	}

	// Affichage du bonus.
	SprDisplay(nSpr | SPR_Flag_Shadow, pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Briques + gnMstPrio);//e_Prio_Briques + 1);
	return (0);
}

//=============================================================================
// Monstre qui fait apparaitre les monstres.

// Phases.
enum
{
	e_MstGenerateur_PhaseWait = 0,
	e_MstGenerateur_PhaseOuverture,
	e_MstGenerateur_PhaseSortie,
	e_MstGenerateur_PhaseFermeture,
};

#define	MSTLEV_Max	3

// Structure spécifique.
struct SMstGenerateur
{
	u16	nCnt;
	u8	nSortieNo;
};

void MstInit_Generateur(struct SMstCommon *pMst)
{
	struct SMstGenerateur	*pSpe = (struct SMstGenerateur *)pMst->pData;

	pMst->nPhase = e_MstGenerateur_PhaseWait;
	pSpe->nCnt = 60;
	gMstMisc.nNbMstLev = 0;		// Pour compter le nombre de monstres présents. (3 max).

	gMstMisc.nPillNb = 0;		// Nb de pillules présente en même temps. Un peu cochon, mais le générateur est initialisé dans chaque tableau.

}

s32 MstMain_Generateur(struct SMstCommon *pMst)
{
	struct SMstGenerateur	*pSpe = (struct SMstGenerateur *)pMst->pData;
	u16	nSortiesPosXY[] = { 64,12,  159,12,  253,12,  159,12 };	// Offsets des sorties.

	switch (pMst->nPhase)
	{
	case e_MstGenerateur_PhaseWait:
		if (gBreak.nRemainingBricks == 0) return (0);	// On ne génère plus de monstres une fois un niveau terminé. (Pour éviter le scoring).

		if (--pSpe->nCnt != 0) return (0);
		// Nb de monstres max atteint ?
		if (gMstMisc.nNbMstLev >= MSTLEV_Max)
		{
			pSpe->nCnt = 60;	// On rééssaye dans une seconde.
			return (0);
		}
		// Passage à la phase suivante.
		pMst->nPhase = e_MstGenerateur_PhaseOuverture;
		AnmSet(gAnm_MstDoorOpen, pMst->nAnm);	// Anim réservée à la création du monstre, pas de pb d'allocation.
		// Choix de la sortie.
		pSpe->nSortieNo = rand() & 3;
		pMst->nPosX = nSortiesPosXY[pSpe->nSortieNo * 2];
		pMst->nPosY = nSortiesPosXY[(pSpe->nSortieNo * 2) + 1];
		break;

	case e_MstGenerateur_PhaseOuverture:
		if (AnmGetKey(pMst->nAnm) == 1)
		{
			pMst->nPhase = e_MstGenerateur_PhaseSortie;
			// Génération du monstre.
			if (MstAdd(e_Mst_Mst1, pMst->nPosX, pMst->nPosY - 8) != -1)
			{
				gMstMisc.nNbMstLev++;		// Pour compter le nombre de monstres présents.
			}
		}
		break;

	case e_MstGenerateur_PhaseSortie:
		if (AnmGetKey(pMst->nAnm) == 2)
		{
			pMst->nPhase = e_MstGenerateur_PhaseFermeture;
		}
		// Cache en plus, pour masquer le monstre.
		SprDisplay(e_Spr_SortieMstCache, pMst->nPosX, pMst->nPosY, e_Prio_Monstres + MSTPRIO_AND + 1);
		break;

	case e_MstGenerateur_PhaseFermeture:
		// Anim terminée ?
		if (AnmGetKey(pMst->nAnm) == e_AnmKey_Null)
		{
			pMst->nPhase = e_MstGenerateur_PhaseWait;
			pSpe->nCnt = 60;
		}
		break;
	}

	// Affichage.
	SprDisplay(AnmGetImage(pMst->nAnm), pMst->nPosX, pMst->nPosY, e_Prio_Monstres - 1);

	return (0);
}

//=============================================================================
// Monstre 1.

// Phases.
enum
{
	e_Mst1_PhaseWait = 0,
	e_Mst1_PhaseArrivee,
	e_Mst1_PhaseMove,
	e_Mst1_PhaseCircle,

};

// Structure spécifique.
struct SMstMst1
{
	u8	nCnt;	// Nb de frames avant changement de direction.

};

// Sous routine pour déplacement du monstre.
u32 Mst1Move(struct SMstCommon *pMst)
{
	s32	nDestX, nDestY;
	s32	nDestX2, nDestY2;
	s32	nBx, nBy;

	// Position de destination.
	nDestX = pMst->nPosX + gVar.pCos[pMst->nAngle];	// * spd
	nDestY = pMst->nPosY + gVar.pSin[pMst->nAngle];	// * spd
	if (pMst->nPhase == e_Mst1_PhaseCircle) nDestY += 0x10;	// Cercles, on descend petit à petit.
	nDestX2 = nDestX + (8 * gVar.pCos[pMst->nAngle]);	// Pour tester plus loin que le pt de ref du sprite.
	nDestY2 = nDestY + (8 * gVar.pSin[pMst->nAngle]);
	// Dans le mur ?
	if (nDestX2 <= WALL_XMin << 8 || nDestX2 >= WALL_XMax << 8 || nDestY2 <= WALL_YMin << 8)
	{
		return (1);
	}
	// Dans les briques ? (Si pas trop bas !)
	nBx = ((nDestX2 >> 8) - WALL_XMin) / BRICK_Width;
	nBy = ((nDestY2 >> 8) - WALL_YMin) / BRICK_Height;
	if (nBy < TABLE_Height && gBreak.pLevel[(nBy * TABLE_Width) + nBx].nPres)
	{
		return (1);
	}
	// Déplacement sur coord finales.
	pMst->nPosX = nDestX;
	pMst->nPosY = nDestY;
	return (0);

}

// Init.
void MstInit_Mst1(struct SMstCommon *pMst)
{
	u64	*pAnm[] = { gAnm_Mst1, gAnm_Mst2, gAnm_Mst3, gAnm_Mst4 };

	pMst->nPhase = e_Mst1_PhaseWait;
	pMst->nAnm = AnmSet(pAnm[gBreak.nLevel & 3], pMst->nAnm);

}

// Main.
s32 MstMain_Mst1(struct SMstCommon *pMst)
{
	struct SMstMst1	*pSpe = (struct SMstMst1 *)pMst->pData;
	s32	nSpr;
	struct SBall	*pBall;
	u32	i, n;

	switch (pMst->nPhase)
	{
	case e_Mst1_PhaseWait:
		pMst->nPhase = e_Mst1_PhaseArrivee;
		return (0);
		break;

	case e_Mst1_PhaseArrivee:		// Descente de la porte.
		pMst->nPosY += 0x100;
		if (pMst->nPosY >> 8 > WALL_YMin + BRICK_Height + (BRICK_Height / 2))
		{
			pSpe->nCnt = 16;
			pMst->nAngle = 192;
			pMst->nPhase = e_Mst1_PhaseMove;
		}
		break;

	case e_Mst1_PhaseMove:			// Déplacement normal.
		// Changement de direction ?
		if (--pSpe->nCnt == 0)
		{
			// On passe en cercles ?
			if (pMst->nPosY > (WALL_YMin + 64) << 8)
			{
				pMst->nPhase = e_Mst1_PhaseCircle;
			}
			else
			{
				pMst->nAngle = (rand() & 3) << 6;
				pSpe->nCnt = rand() | 16;			// 16 : Au minimum, 16 frames avant le chgt de dir.
			}
		}
		// Déplacement.
		if (Mst1Move(pMst))
		{
			// Pb, on change de dir.
			pMst->nAngle = (rand() & 3) << 6;
		}
		break;

	case e_Mst1_PhaseCircle:		// Cercles.
		pMst->nAngle += 2;
		// Déplacement.
		if (Mst1Move(pMst))
		{
			// Pb, on revient en lignes.
			pMst->nAngle = (rand() & 3) << 6;
			pMst->nPhase = e_Mst1_PhaseMove;
			pSpe->nCnt = 128;
		}
		break;
	}

	// Sortie de l'écran ?
	if (pMst->nPosY >= (SCR_Height + 16) << 8)
	{
		// Tuage de l'ennemi.
		gMstMisc.nNbMstLev--;		// Pour compter le nombre de monstres présents.
		return (-1);
	}

	nSpr = AnmGetImage(pMst->nAnm);

	// Le monstre se prend un tir ?
	if (MstCheckFire(nSpr, pMst->nPosX >> 8, pMst->nPosY >> 8))
	{
		// Score.
		gBreak.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;
		// Le monstre disparaît.
		DustSet(gAnm_MstExplo1, pMst->nPosX >> 8, pMst->nPosY >> 8);
		// Tuage de l'ennemi.
		gMstMisc.nNbMstLev--;		// Pour compter le nombre de monstres présents.
		return (-1);
	}

	// Contact avec le joueur ?
	if (AnmGetKey(gBreak.nPlayerAnmNo) != e_AnmKey_PlyrDeath)
	if (SprCheckColBox(AnmGetLastImage(gBreak.nPlayerAnmNo), gBreak.nPlayerPosX, gBreak.nPlayerPosY,
		nSpr, pMst->nPosX >> 8, pMst->nPosY >> 8))
	{
		// Score.
//		if (gBreak.nPhase != e_Game_LevelSelection)
		if (gBreak.nPhase > e_Game_LevelSelectionTimeOut)
			gBreak.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;
		// Le monstre disparaît.
		DustSet(gAnm_MstExplo1, pMst->nPosX >> 8, pMst->nPosY >> 8);
		// Tuage de l'ennemi.
		gMstMisc.nNbMstLev--;		// Pour compter le nombre de monstres présents.
		return (-1);
	}

	// Contact avec la balle ?
	for (i = 0, n = 0; i < BALL_MAX_NB && n < gBreak.nBallsNb; i++)
	{
		pBall = &gBreak.pBalls[i];
		if (pBall->nUsed)
		{
			if (SprCheckColBox(pBall->nSpr, pBall->nPosX >> 8, pBall->nPosY >> 8,
				nSpr, pMst->nPosX >> 8, pMst->nPosY >> 8))
			{
				// Score.
//				if (gBreak.nPhase != e_Game_LevelSelection)
				if (gBreak.nPhase > e_Game_LevelSelectionTimeOut)
					gBreak.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;
				// Le monstre disparaît.
				DustSet(gAnm_MstExplo1, pMst->nPosX >> 8, pMst->nPosY >> 8);
				// La balle change de direction, sauf si elle est aimantée.
				if ((pBall->nFlags & BALL_Flg_Aimantee) == 0)
				{
					pBall->nAngle = (u8)((rand() & 15) << 4) + 8;	// Pour éviter des angles foireux (0, 128).
				}
				// Tuage de l'ennemi.
				gMstMisc.nNbMstLev--;		// Pour compter le nombre de monstres présents.
				return (-1);
			}
			n++;
		}
	}

	// Affichage.
	SprDisplay(nSpr | SPR_Flag_Shadow, pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Monstres + gnMstPrio);

	return (0);
}

//=============================================================================
// Monstre Porte à droite.

// Phases.
enum
{
	e_MstDoorR_PhaseClosed = 0,
	e_MstDoorR_PhaseOpened,
	e_MstDoorR_PhaseSuckingIn,

};

// Déclenche l'ouverture de la porte.
void MstDoorROpen(void)
{
	gMstMisc.nMstDoorR = 1;		// On tente de déclencher l'ouverture.

}

// Init.
void MstInit_DoorR(struct SMstCommon *pMst)
{
	pMst->nPhase = e_MstDoorR_PhaseClosed;
	gMstMisc.nMstDoorR = 0;		// RAZ interrupteur.

}

// Main.
s32 MstMain_DoorR(struct SMstCommon *pMst)
{
	s32	nSpr;

	nSpr = AnmGetImage(pMst->nAnm);

	switch (pMst->nPhase)
	{
	case e_MstDoorR_PhaseClosed:
		// Interrupteur ?
		if (gMstMisc.nMstDoorR)
		{
			pMst->nPhase = e_MstDoorR_PhaseOpened;
			gMstMisc.nMstDoorR = 0;		// RAZ interrupteur.
		}
		return (0);
		break;

	case e_MstDoorR_PhaseOpened:
		// Contact avec le joueur ?
		if (SprCheckColBox(AnmGetLastImage(gBreak.nPlayerAnmNo), gBreak.nPlayerPosX, gBreak.nPlayerPosY,
			nSpr, pMst->nPosX >> 8, pMst->nPosY >> 8))
		{
			// Le joueur sera aspiré.
			gBreak.nPlayerFlags |= PLAYER_Flg_DoorR;
			// Le joueur ne peut plus exploser.
			gBreak.nPlayerFlags |= PLAYER_Flg_NoKill;
			// Sfx.
			Sfx_PlaySfx(e_Sfx_DoorThrough, e_SfxPrio_40);
			// Phase monstre.
			pMst->nPhase = e_MstDoorR_PhaseSuckingIn;
		}
		break;

	case e_MstDoorR_PhaseSuckingIn:		// On aspire la raquette à l'intérieur (fait au niveau du joueur).

		break;

	}

	// Affichage.
	SprDisplay(nSpr, pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Monstres);
	// Cache pour le joueur si nécessaire.
	if (pMst->nPhase == e_MstDoorR_PhaseSuckingIn)
	{
		SprDisplay(nSpr + 1, pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Raquette + 5);
	}

	return (0);
}


//=============================================================================
// Doh !

#define	DOH_LifePts	20

// Phases.
enum
{
	e_MstDoh_Appear = 0,
	e_MstDoh_Idle,
	e_MstDoh_Shoot,
	e_MstDoh_Death1,	// Avec les explosions.
	e_MstDoh_Death2,	// Disparition.

};

// Structure spécifique.
struct SMstDoh
{
	u8	nLifePts;	// Points de vie.
	u8	nDeath1;	// Compteur pour pendant combien de temps on balance des explosions.

	u16	nCntIdle;	// Pause en idle.

	u16	nCntAttk;	// Pause entre les tirs.
	u16	nCntAttkInit;	// Durée de la pause entre les tirs pour reset.
	u16	nNbAttk;	// Nb de tirs.

	u8	nNoCol;		// Quand la balle touche, pour ne pas retoucher tant qu'il y a collision.

	u16	nLastBallPosX[3], nLastBallPosY[3];

};

#define	DOH_PauseIdle_Long	100
#define	DOH_PauseIdle_Avg	80
#define	DOH_PauseIdle_Short	60
#define	DOH_PauseShoot	16
// Initialise les timers des différentes phases.
void DohInitTimers(struct SMstDoh *pSpe)
{
	switch (pSpe->nLifePts >> 2)
	{
	case 0:			// Très excité. 1 pause courte et 3 tirs.
		pSpe->nCntIdle = DOH_PauseIdle_Short;
		pSpe->nNbAttk = 3;
		pSpe->nCntAttk = 1;
		break;

	case 1:			// Bien excité. 1 pause courte et 2 tirs.
		pSpe->nCntIdle = DOH_PauseIdle_Short;
		pSpe->nNbAttk = 2;
		pSpe->nCntAttk = 1;
		break;

	case 2:			// Un peu plus excité. 1 pause courte et 1 tir.
		pSpe->nCntIdle = DOH_PauseIdle_Short;
		pSpe->nNbAttk = 1;
		pSpe->nCntAttk = 1;
		break;

	case 3:			// Un peu excité. 1 pause moyenne et 1 tir.
		pSpe->nCntIdle = DOH_PauseIdle_Avg;
		pSpe->nNbAttk = 1;
		pSpe->nCntAttk = 1;
		break;

	default:		// Pas excité. 1 longue pause et 1 tir.
		pSpe->nCntIdle = DOH_PauseIdle_Long;
		pSpe->nNbAttk = 1;
		pSpe->nCntAttk = 1;		// Premier shoot : -1 et on tire.
		break;
	}
	pSpe->nCntAttkInit = DOH_PauseShoot;

}

u8	gpnDohAngles[SCR_Width];

// Init.
void MstInit_Doh(struct SMstCommon *pMst)
{
	struct SMstDoh	*pSpe = (struct SMstDoh *)pMst->pData;

	pMst->nPhase = e_MstDoh_Appear;
	pSpe->nLifePts = DOH_LifePts;
	pSpe->nNoCol = 0;

	DohInitTimers(pSpe);

	static u8	nPreca = 0;
	if (nPreca == 0)
	{
		// Précalcul de la table d'angles.
		s32	i;
		for (i = 0; i < SCR_Width; i++)
			gpnDohAngles[i] = (u8)(atan2((pMst->nPosY >> 8) - gBreak.nPlayerPosY, i - (pMst->nPosX >> 8)) * 128 / 3.1415927);
		nPreca = 1;
	}

}

// Test de collision au pixel (pas bien du tout).
// Note : Vu comme c'est pas bien, envoyer le plus petit sprite en 1, le plus gros en 2.
u32 SprCheckColPix(u32 nSpr1, s32 nPosX1, s32 nPosY1, u32 nSpr2, s32 nPosX2, s32 nPosY2)
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
		// Oui, on va tester au pixel.
		s32	ix, iy;

		for (iy = nYMin1; iy < nYMax1; iy++)
		{
			if (iy >= nYMin2 && iy < nYMax2)
			{
				for (ix = nXMin1; ix < nXMax1; ix++)
				{
					if (ix >= nXMin2 && ix < nXMax2)
					{
						if (pSpr1->pGfx[((iy - nYMin1) * pSpr1->nLg) + (ix - nXMin1)] &&
							pSpr2->pGfx[((iy - nYMin2) * pSpr2->nLg) + (ix - nXMin2)])
						{
							return (1);
						}
					}
				}
			}
		}

	}

	return (0);
}

// Affichage de la barre de vie du boss.
#define	DohBar_X	250
#define	DohBar_Y	9
void DisplayDohLifeBar(u32 nNbLifePts)
{
	u32	nLev;
	u32	nPosX;

	// La barre.
	SprDisplay(e_Spr_BossBar, DohBar_X, DohBar_Y, e_Prio_HUD);
	SprDisplay(e_Spr_BossBarTop, DohBar_X, DohBar_Y, e_Prio_HUD+2);

	// Les pts de vie.
	nLev = (32 * nNbLifePts) / DOH_LifePts;
	nPosX = DohBar_X;
	while (nLev >= 8)
	{
		SprDisplay(e_Spr_BossBarPts + 7, nPosX, DohBar_Y, e_Prio_HUD + 1);
		nPosX += 8;
		nLev -= 8;
	}
	if (nLev)
	{
		SprDisplay(e_Spr_BossBarPts + nLev - 1, nPosX, DohBar_Y, e_Prio_HUD + 1);
	}

}

// Main.
s32 MstMain_Doh(struct SMstCommon *pMst)
{
	struct SMstDoh	*pSpe = (struct SMstDoh *)pMst->pData;
	struct SBall *pBall;
	s32	nSpr;
	u32	rVal;


	// Affichage de la barre de vie du boss.
	DisplayDohLifeBar(pSpe->nLifePts);


	// En hit ? On affiche le spr et on sort.
//todo: Si on a une anim d'ouverture/fermeture, on peut mettre un compteur pour le hit au lieu
// d'une anim et faire continuer l'anim an mettant les sprites en blanc pendant la durée du compteur.
	if (AnmGetKey(pMst->nAnm) == e_AnmKey_MstDohHit)	// Sauf quand en 'hit'. En plus, ça laisse à la balle le temps de dégager.
	{
		nSpr = AnmGetImage(pMst->nAnm);
		SprDisplay(nSpr, pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Briques);
		// Last pos de la balle.
		pSpe->nLastBallPosX[2] = pSpe->nLastBallPosX[1];
		pSpe->nLastBallPosY[2] = pSpe->nLastBallPosY[1];
		pSpe->nLastBallPosX[1] = pSpe->nLastBallPosX[0];
		pSpe->nLastBallPosY[1] = pSpe->nLastBallPosY[0];
		pSpe->nLastBallPosX[0] = gBreak.pBalls[0].nPosX >> 8;
		pSpe->nLastBallPosY[0] = gBreak.pBalls[0].nPosY >> 8;
		return (0);
	}


	switch (pMst->nPhase)
	{
	case e_MstDoh_Appear:	// Apparition.
		// Pas pendant la selection du level (Même si on ne devrait pas laisser le choix jusque là...).
		if (gBreak.nPhase != e_Game_Normal) return (0);

		// Tant que clef != Null
		if (AnmGetKey(pMst->nAnm) != e_AnmKey_Null) break;
		// Si Null, c'est que l'apparition est terminée, on passe en Idle.
		pMst->nPhase = e_MstDoh_Idle;
		gBreak.nPlayerFlags &= ~PLAYER_Flg_BossWait;	// Signal pour le joueur.
		// En on ne breake pas ! (Même si ça ne serait pas grave).

	case e_MstDoh_Idle:		// Phase d'attente.
		AnmSetIfNew(gAnm_MstDohIdle, pMst->nAnm);

		// Pas de décompte pendant l'apparition ou la mort du joueur.
		if (AnmGetKey(gBreak.nPlayerAnmNo) != e_AnmKey_Null) break;

		// On passe en phase de tir ?
		if (--pSpe->nCntIdle == 0)
		{
			AnmSetIfNew(gAnm_MstDohMouthOpens, pMst->nAnm);
			pMst->nPhase = e_MstDoh_Shoot;
			DohInitTimers(pSpe);	// On change de vitesse si nécessaire.
		}
		break;

	case e_MstDoh_Shoot:	// Phase de tir.
		AnmSetIfNew(gAnm_MstDohShoot, pMst->nAnm);

		// Si ouverture de la bouche pas terminée, stop.
		if (AnmGetKey(pMst->nAnm) != e_AnmKey_Null) break;

		// Tir ?
		if (--pSpe->nCntAttk == 0)
		{
			// Tir.
//spd			s32 nAng = (s32)(atan2((pMst->nPosY >> 8) - gBreak.nPlayerPosY, gBreak.nPlayerPosX - (pMst->nPosX >> 8)) * 128 / 3.1415927);
			s32	nAng = gpnDohAngles[gBreak.nPlayerPosX];
			if (FireAdd(1, pMst->nPosX >> 8, pMst->nPosY >> 8, nAng) != -1)
			{
				Sfx_PlaySfx(e_Sfx_BatMagnet, e_SfxPrio_20);	// Sfx.
			}

			// On repasse en phase de repos ?
			if (--pSpe->nNbAttk == 0)
			{
				AnmSetIfNew(gAnm_MstDohMouthCloses, pMst->nAnm);
				DohInitTimers(pSpe);	// On change de vitesse si nécessaire.
				pMst->nPhase = e_MstDoh_Idle;
			}
			else
			{
				pSpe->nCntAttk = pSpe->nCntAttkInit;	// Reset du compteur d'attente entre les tirs.
			}
		}
		break;

	case e_MstDoh_Death1:	// Les explosions.
		AnmSetIfNew(gAnm_MstDohIdle, pMst->nAnm);
		if (--pSpe->nDeath1)
		{
			struct SSprite *pSpr = SprGetDesc(AnmGetLastImage(pMst->nAnm));
			s32	nPosX, nPosY;

			nPosX = (pMst->nPosX >> 8) - pSpr->nPtRefX + (rand() % pSpr->nLg);
			nPosY = (pMst->nPosY >> 8) - pSpr->nPtRefY + (rand() % pSpr->nHt);
			DustSet(gAnm_MstExplo1NoSound, nPosX, nPosY);	// Anim sans son, pour ne pas déclencher le sfx à chaque frame.
			// Son d'explosion.
			if ((pSpe->nDeath1 & 0x0F) == 0x0E)	// Toutes les 16 frames, dès la première (au premier passage, cpt vaut 255 - 1).
			{
				Sfx_PlaySfx(e_Sfx_Explosion1, e_SfxPrio_40);	// Sfx explosion.
			}
		}
		else
		{
			pMst->nPhase = e_MstDoh_Death2;
			// Score.
			gBreak.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;
		}
		break;

	case e_MstDoh_Death2:	// La disparition.
		AnmSetIfNew(gAnm_MstDohDisappears, pMst->nAnm);

		if ((u32)AnmGetLastImage(pMst->nAnm) == SPR_NoSprite)
			gBreak.nRemainingBricks = 0;	// Anim terminée, on quitte.
		else
			gVar.nInactiveCnt = 0;			// RAZ pour ne pas lancer le Vaus trop vite dans la porte en cas d'inactivité.

		break;

	}

	nSpr = AnmGetImage(pMst->nAnm);

	// Touché par la balle ?
	pBall = &gBreak.pBalls[0];	// 1 seule balle dans ce niveau.
	rVal = SprCheckColPix(pBall->nSpr, pBall->nPosX >> 8, pBall->nPosY >> 8, nSpr, pMst->nPosX >> 8, pMst->nPosY >> 8);
	if (rVal)
	{
		if (pSpe->nNoCol == 0)
		{

			// Rebond de la balle.
			struct SSprite *pSpr = SprGetDesc(nSpr);
			s32	nXMin, nXMax, nYMin, nYMax;

			nXMin = (pMst->nPosX >> 8) - pSpr->nPtRefX;
			nXMax = nXMin + pSpr->nLg;
			nYMin = (pMst->nPosY >> 8) - pSpr->nPtRefY;
			nYMax = nYMin + pSpr->nHt;

			if (pSpe->nLastBallPosY[2] >= nYMin && pSpe->nLastBallPosY[2] <= nYMax)
			{
				pBall->nAngle = 128 - pBall->nAngle;
			}
			else if (pSpe->nLastBallPosX[2] >= nXMin && pSpe->nLastBallPosX[2] <= nXMax)
			{
				pBall->nAngle = -pBall->nAngle;
			}
			else
			{
				// Coin.
				s32	dx, dy;
				dx = pSpe->nLastBallPosX[2] - (pMst->nPosX >> 8);	// Pour le sens.
				dy = pSpe->nLastBallPosY[2] - (pMst->nPosY >> 8);

				// J'ai honte, c'est une recopie du code de collision avec la brique.
				if (dx >= 0 && dy >= 0)			// Bas droite.
				{
					if ((s8)pBall->nAngle >= -32 && (s8)pBall->nAngle < 64+32)
					{
						pBall->nAngle -= 64;
					}
					else
					{
						pBall->nAngle += 64;
					}
				}
				else if (dx >= 0 && dy <= 0)	// Haut droite.
				{
					if (pBall->nAngle >= 32 && pBall->nAngle < 128+32)
					{
						pBall->nAngle -= 64;
					}
					else
					{
						pBall->nAngle += 64;
					}
				}
				else if (dx <= 0 && dy <= 0)	// Haut gauche.
				{
					if (pBall->nAngle >= 64+32 && pBall->nAngle < 192+32)
					{
						pBall->nAngle -= 64;
					}
					else
					{
						pBall->nAngle += 64;
					}
				}
				else if (dx <= 0 && dy >= 0)	// Bas gauche.
				{
					if ((s8)pBall->nAngle >= -64-32 && (s8)pBall->nAngle < 32)
					{
						pBall->nAngle -= 64;
					}
					else
					{
						pBall->nAngle += 64;
					}
				}

				// En y, on empêche les rebonds trop "horizontaux".
				if ((s8)pBall->nAngle > -16 && (s8)pBall->nAngle < 16)
				{
					pBall->nAngle = ((s8)pBall->nAngle >= 0 ? 16 : -16);
				}
				else if (pBall->nAngle > 128-16 && pBall->nAngle < 128+16)
				{
					pBall->nAngle = (pBall->nAngle >= 128 ? 128+16 : 128-16);
				}

			}


			// Il reste des points de vie ?
			if (pSpe->nLifePts)
			{
				if (--pSpe->nLifePts == 0)
				{
					// Death !
					pMst->nPhase = e_MstDoh_Death1;
					pSpe->nDeath1 = 255;
					// On fait disparaitre la balle avec un dust...
					BallsKill();
					// ... et le joueur ne peut plus exploser.
					gBreak.nPlayerFlags |= PLAYER_Flg_NoKill;
					// On fait aussi disparaitre les tirs.
					FireRemoveDohShoots();
				}
				else
				{
					// Anim de hit.
					AnmSetIfNew(gAnm_MstDohHit, pMst->nAnm);
				}
			}

			pSpe->nNoCol = 1;

		} // if (pSpe->nNoCol == 0)
	}
	else if (pSpe->nNoCol) pSpe->nNoCol = 0;


	// Affichage.
	SprDisplay(nSpr, pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Briques);

	// Last pos de la balle.
	pSpe->nLastBallPosX[2] = pSpe->nLastBallPosX[1];
	pSpe->nLastBallPosY[2] = pSpe->nLastBallPosY[1];
	pSpe->nLastBallPosX[1] = pSpe->nLastBallPosX[0];
	pSpe->nLastBallPosY[1] = pSpe->nLastBallPosY[0];
	pSpe->nLastBallPosX[0] = gBreak.pBalls[0].nPosX >> 8;
	pSpe->nLastBallPosY[0] = gBreak.pBalls[0].nPosY >> 8;
	return (0);

}

//=============================================================================

// Debug, vérification de la taille des structures.
u32 MstCheckStructSizes(void)
{
	assert(sizeof(struct SMstDoh) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMstMst1) < MST_COMMON_DATA_SZ);
	return (0);
}

