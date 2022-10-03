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

enum
{
	e_RenderMode_Normal = 0,
	e_RenderMode_Scale2x,
	e_RenderMode_TV2x,
	//
	e_RenderMode_MAX
};

// Paramètres de rendu.
struct SRender
{
	// Note : Je laisse le pointeur pScreen dans gVar.
	// On fait dans tous les cas le rendu "normal" dans pScreen. Si on a un post-effect à faire, on redirige pScreen dans un buffer secondaire, qu'on recopiera avec l'effet voulu dans l'écran réel une fois le tracé du jeu fini.

	SDL_Surface *pScreen2x;		// En modes 2x, ptr sur la surface écran réelle.
	SDL_Surface *pScreenBuf2;	// Buffer de rendu pour le jeu en modes 2x (à la place de la surface écran réelle).

	u8	nRenderMode;			// Mode en cours : normal / 2x.
	u8	nFullscreenMode;		// Fullscreen ou pas.

};
extern struct SRender	gRender;


void Render_InitVideo(void);
void Render_SetVideoMode(void);
void RenderFlip(u32 nSync);
void RenderRelease(void);
SDL_Surface * Render_GetRealVideoSurfPtr(void);

