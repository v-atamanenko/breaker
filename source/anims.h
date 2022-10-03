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

// Codes de contrôle.
#define	BIT31	(1 << 31)
enum
{
	e_Anm_Jump	= BIT31 | 1,	// Ptr + offset.
	e_Anm_Goto	= BIT31 | 2,	// Initialise une autre anim.
	e_Anm_End	= BIT31 | 3,	// Fin d'une anim. Renvoie SPR_NoSprite, place e_AnmFlag_End, ne libère pas le slot.
	e_Anm_Kill	= BIT31 | 4,	// Fin d'une anim. Renvoie -1 et libère le slot (ex: dust).
	e_Anm_Sfx	= BIT31 | 5,	// Joue un son. e_Anm_Sfx, No sfx, Prio sfx.

};

// Clefs d'anim.	16b = Priorité (à faire) | 16b = No.
#define	ANMPRIO(x)	(x << 16)
enum
{
	e_AnmKey_Null		= 0,
	e_AnmKey_PlyrAppear	= ANMPRIO(1) + 0,
	e_AnmKey_PlyrDeath	= ANMPRIO(2) + 0,

	e_AnmKey_MstDohMoutOpens = ANMPRIO(1) + 0,
	e_AnmKey_MstDohMoutCloses = ANMPRIO(1) + 1,
	e_AnmKey_MstDohAppears = ANMPRIO(1) + 2,
	e_AnmKey_MstDohDisappears = ANMPRIO(3) + 0,
	e_AnmKey_MstDohHit	= ANMPRIO(2) + 0,

};


// Définition des anims.

extern u64	gAnm_Raquette[];
extern u64	gAnm_RaqAppear[];
extern u64	gAnm_RaqAimant[];
extern u64	gAnm_RaqDeath0[];
extern u64	gAnm_RaqDeath1[];
extern u64	gAnm_RaqDeath2[];
extern u64	gAnm_RaqDeath3[];
extern u64	gAnm_RaqRallonge0[];
extern u64	gAnm_RaqReduit0[];
extern u64	gAnm_RaqRallonge1[];
extern u64	gAnm_RaqReduit1[];
extern u64	gAnm_RaqRallonge2[];
extern u64	gAnm_RaqReduit2[];
extern u64	gAnm_RaqMitGRepos[];
extern u64	gAnm_RaqMitDRepos[];
extern u64	gAnm_RaqMitGShoot[];
extern u64	gAnm_RaqMitDShoot[];
extern u64	gAnm_PlyrShot[];
extern u64	gAnm_RaqClignG[];
extern u64	gAnm_RaqClignD[];
extern u64	gAnm_BrickExplo[];
extern u64	gAnm_Brick2HitExplo[];
extern u64	gAnm_BrickCBExplo[];
extern u64	gAnm_Brick2Hit[];
extern u64	gAnm_BrickCBHit[];
extern u64	gAnm_BrickIndesHit[];
extern u64	gAnm_Itm1[];
extern u64	gAnm_Itm2[];
extern u64	gAnm_Itm3[];
extern u64	gAnm_Itm4[];
extern u64	gAnm_Itm5[];
extern u64	gAnm_Itm6[];
extern u64	gAnm_Itm7[];
extern u64	gAnm_Itm8[];
extern u64	gAnm_Itm9[];
extern u64	gAnm_Itm10[];
extern u64	gAnm_Itm11[];
extern u64	gAnm_MstDoorOpen[];
extern u64	gAnm_MstDoorOpened[];
extern u64	gAnm_MstDoorClose[];
extern u64	gAnm_MstDoorWait[];
extern u64	gAnm_Mst1[];
extern u64	gAnm_Mst2[];
extern u64	gAnm_Mst3[];
extern u64	gAnm_Mst4[];
extern u64	gAnm_MstExplo1[];
extern u64	gAnm_MstExplo1NoSound[];
extern u64	gAnm_DohMissileDisp[];
extern u64	gAnm_MstDoorRight[];
extern u64	gAnm_MstDohIdle[];
extern u64	gAnm_MstDohHit[];
extern u64	gAnm_MstDohShoot[];
extern u64	gAnm_MstDohAppears[];
extern u64	gAnm_MstDohDisappears[];
extern u64	gAnm_MstDohMouthOpens[];
extern u64	gAnm_MstDohMouthCloses[];
extern u64	gAnm_DohMissile[];



