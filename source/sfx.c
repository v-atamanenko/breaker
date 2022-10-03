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


struct SSfxGene
{
	u8	nInit;		// Son initialis� (1) ou pas (0).
	SDL_AudioSpec	sAudioSpec;
    SDL_AudioCVT	pCvt[e_Sfx_LAST];

};
struct SSfxGene	gSfx;


#define SFX_MAX_SOUNDS	2
struct SSample
{
	u8	*pData;
	u32	nDPos;
	u32	nDLen;
	u8	nPrio;	// Priorit� du son en cours.
} gpSounds[SFX_MAX_SOUNDS];

// Mixer, appel� par SDL.
void Sfx_MixAudio(void *unused, u8 *stream, int len)
{
    u32	i;
    u32	amount;

    for (i = 0; i < SFX_MAX_SOUNDS; i++)
    {
        amount = (gpSounds[i].nDLen - gpSounds[i].nDPos);
        if (amount > (u32)len)
        {
            amount = len;
        }
        SDL_MixAudio(stream, &gpSounds[i].pData[gpSounds[i].nDPos], amount, SDL_MIX_MAXVOLUME);
        gpSounds[i].nDPos += amount;
    }
}

// Nettoyage des canaux.
void Sfx_ClearChannels(void)
{
	u32	i;

    for (i = 0; i < SFX_MAX_SOUNDS; i++)
    {
		gpSounds[i].nDPos = 0;
		gpSounds[i].nDLen = 0;
	}

}


struct SSample	gpSoundsBkp[SFX_MAX_SOUNDS];	// Pour stop/reprise du son pendant la pause.
//m	s32	gnMusicNoBkp;	// Bkp no de musique.

// Sauvegarde des datas + clear channels.
void Sfx_ChannelsSave(void)
{
	u32	i;

	if (!gSfx.nInit) return;

//m	gnMusicNoBkp = gSfx.nMusicNo;		// Sauvegarde du no de musique.
//m	gSfx.nMusicNo = e_YmMusic_NoMusic;

	for (i = 0; i < SFX_MAX_SOUNDS; i++)
		memcpy(&gpSoundsBkp[i], &gpSounds[i], sizeof(struct SSample));

	Sfx_ClearChannels();
}

// Remise en place des datas.
void Sfx_ChannelsRestore(void)
{
	u32	i;

	if (!gSfx.nInit) return;

//m	gSfx.nMusicNo = gnMusicNoBkp;		// Resume music.

	for (i = 0; i < SFX_MAX_SOUNDS; i++)
		memcpy(&gpSounds[i], &gpSoundsBkp[i], sizeof(struct SSample));
}


// Sound, initialisation. A appeler 1 fois.
void Sfx_SoundInit(void)
{
	gSfx.nInit = 0;

	// Set 16-bit stereo audio at 22Khz.
	gSfx.sAudioSpec.freq = 22050;
	gSfx.sAudioSpec.format = AUDIO_S16;
	gSfx.sAudioSpec.channels = 2;
	gSfx.sAudioSpec.samples = 512;        // A good value for games.
	gSfx.sAudioSpec.callback = Sfx_MixAudio;
	gSfx.sAudioSpec.userdata = NULL;

	// Open the audio device and start playing sound!
	if (SDL_OpenAudio(&gSfx.sAudioSpec, NULL) < 0)
	{
		//fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
		//exit(1);
		printf("Unable to open audio: %s\n", SDL_GetError());
		printf("Sound disabled.\n");
		return;
	}

	gSfx.nInit = 1;		// Ok.

	Sfx_ClearChannels();	// Nettoyage des structures.

}

// Sound on.
void Sfx_SoundOn(void)
{
	if (!gSfx.nInit) return;
	SDL_PauseAudio(0);

}

// Sound off.
void Sfx_SoundOff(void)
{
	if (!gSfx.nInit) return;
	SDL_CloseAudio();

}


// Chargement de tous les fichiers WAV.
void Sfx_LoadWavFiles(void)
{
	u32	i;

    SDL_AudioSpec sWave;
    u8	*pData;
    Uint32	nDLen;

	char	*pSfxFilenames[e_Sfx_LAST] = {
		"sfx/_pill_bonus.wav", "sfx/_pill_malus.wav", "sfx/_shot.wav", "sfx/_door_through.wav",
		"sfx/_menu_click.wav", "sfx/_brick_bounce.wav", "sfx/_ball_bounce.wav", "sfx/_explosion1.wav",
		"sfx/_explosion2.wav", "sfx/_brick_dissolve.wav", "sfx/_extra_life.wav", "sfx/_bat_ping.wav",
		"sfx/_bat_magnet.wav",
	};

	if (!gSfx.nInit) return;

	for (i = 0; i < e_Sfx_LAST; i++)
	{
		// Load the sound file and convert it to 16-bit stereo at 22kHz
		if (SDL_LoadWAV(pSfxFilenames[i], &sWave, &pData, &nDLen) == NULL)
		{
			fprintf(stderr, "Couldn't load %s: %s\n", pSfxFilenames[i], SDL_GetError());
			return;
		}
		SDL_BuildAudioCVT(&gSfx.pCvt[i], sWave.format, sWave.channels, sWave.freq,
			gSfx.sAudioSpec.format, gSfx.sAudioSpec.channels, gSfx.sAudioSpec.freq);

		gSfx.pCvt[i].buf = (u8*)malloc(nDLen * gSfx.pCvt[i].len_mult);
		memcpy(gSfx.pCvt[i].buf, pData, nDLen);
		gSfx.pCvt[i].len = nDLen;
		SDL_ConvertAudio(&gSfx.pCvt[i]);
		SDL_FreeWAV(pData);

	}

}

// Lib�re les ressources occup�es par les fichiers WAV.
void Sfx_FreeWavFiles(void)
{
	u32	i;

	if (!gSfx.nInit) return;

	for (i = 0; i < e_Sfx_LAST; i++)
	{
		free(gSfx.pCvt[i].buf);
	}

}


// Joue un son.
// Le minimum :
// On commence par chercher un canal vide.
// Si il n'y en a pas, on note celui qui � la priorit� la plus faible.
// Si plusieurs ont la m�me priorit�, on note celui qui est le plus proche de la fin.
// Enfin, si la prio du son � jouer est ok, on le joue dans le canal not�.
void Sfx_PlaySfx(u32 nSfxNo, u32 nSfxPrio)
{
	u32	index;

	u8	nPrioMinVal = 255;
	u32	nPrioMinPos = 0;
	u32	nPrioMinDiff = (u32)-1;

	if (nSfxNo >= e_Sfx_LAST) return;	// S�curit�.

    // Look for an empty (or finished) sound slot.
    for (index = 0; index < SFX_MAX_SOUNDS; index++)
    {
        if (gpSounds[index].nDPos == gpSounds[index].nDLen)
        {
            break;
        }
        //
        if (gpSounds[index].nPrio < nPrioMinVal)
        {
			nPrioMinVal = gpSounds[index].nPrio;
			nPrioMinPos = index;
        	nPrioMinDiff = gpSounds[index].nDLen - gpSounds[index].nDPos;
		}
		else if (gpSounds[index].nPrio == nPrioMinVal)
		{
			if (gpSounds[index].nDLen - gpSounds[index].nDPos < nPrioMinDiff)
			{
				//nPrioMinVal = sounds[index].nPrio;
				nPrioMinPos = index;
				nPrioMinDiff = gpSounds[index].nDLen - gpSounds[index].nDPos;
			}
		}

    }

	// On a trouv� un emplacement libre ?
    if (index == SFX_MAX_SOUNDS)
    {
    	// Non, la prio demand�e est > ou == � la prio mini en cours ?
		if (nSfxPrio < nPrioMinVal) return;
		index = nPrioMinPos;
    }

    // Put the sound data in the slot (it starts playing immediately).
    SDL_LockAudio();
    gpSounds[index].pData = gSfx.pCvt[nSfxNo].buf;
    gpSounds[index].nDLen = gSfx.pCvt[nSfxNo].len_cvt;
    gpSounds[index].nDPos = 0;
    gpSounds[index].nPrio = (u8)nSfxPrio;
    SDL_UnlockAudio();

}


