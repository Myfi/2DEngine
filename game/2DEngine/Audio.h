#ifndef _AUDIO_H
#define _AUDIO_H

#include <xaudio2.h>
#include "wave.h"

class Audio
{
	private:
		Wave buffer;
		IXAudio2* g_engine;
		IXAudio2SourceVoice* g_source;
		IXAudio2MasteringVoice* g_master;
	public:

	Audio(); //Constructor

	virtual void release(); //Release

	virtual void playSound(char c[], double vol); //Play the sound
};

#endif

