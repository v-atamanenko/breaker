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

// Prototypes.
void Sfx_SoundInit(void);
void Sfx_SoundOn(void);
void Sfx_SoundOff(void);
void Sfx_LoadWavFiles(void);
void Sfx_FreeWavFiles(void);
void Sfx_PlaySfx(u32 nSfxNo, u32 nSfxPrio);
void Sfx_ChannelsSave(void);
void Sfx_ChannelsRestore(void);


// Enums.
enum
{
	e_Sfx_PillBonus,
	e_Sfx_PillMalus,
	e_Sfx_Shot,
	e_Sfx_DoorThrough,
	e_Sfx_MenuClic,
	e_Sfx_BrickBounce,
	e_Sfx_BallBounce,
	e_Sfx_Explosion1,
	e_Sfx_Explosion2,
	e_Sfx_BrickDissolve,
	e_Sfx_ExtraLife,
	e_Sfx_BatPing,
	e_Sfx_BatMagnet,

	e_Sfx_LAST
};


enum
{
	e_SfxPrio_0 = 0,
	e_SfxPrio_10 = 10,
	e_SfxPrio_20 = 20,
	e_SfxPrio_30 = 30,
	e_SfxPrio_40 = 40,
	e_SfxPrio_Max = 254,
};
