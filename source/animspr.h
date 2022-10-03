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
void AnmInitEngine(void);
s32 AnmSet(u64 *pAnm, s32 nSlotNo);
s32 AnmSetIfNew(u64 *pAnm, s32 nSlotNo);
void AnmReleaseSlot(s32 nSlotNo);
s32 AnmGetImage(s32 nSlotNo);
s32	AnmGetLastImage(s32 nSlotNo);
u32 AnmGetKey(s32 nSlotNo);
u32 AnmCheckEnd(s32 nSlotNo);


