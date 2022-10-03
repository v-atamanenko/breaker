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

// Enums.
enum
{
	MENU_Null = 0,
	MENU_Main,
	MENU_Game,
	MENU_HallOfFame,
	MENU_PowerUps,
	MENU_Quit,

};


// Prototypes.
void Fade(s32 nFadeVal);

void MenuInit(void);
void MenuMain_Init(void);
u32 MenuMain_Main(void);
void MenuHighScores_RankClear(void);
void MenuHighScores_Init(void);
u32 MenuHighScores_Main(void);
void MenuGetName_Init(void);
u32 MenuGetName_Main(void);
void MenuPowerUps_Init(void);
u32 MenuPowerUps_Main(void);

s32 Scr_CheckHighSc(u32 nScorePrm);
void Scr_Load(void);
void Scr_Save(void);



