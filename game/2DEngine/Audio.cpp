#include "audio.h"

Audio::Audio()
{
	//must call this for COM
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	//create the engine
	if (FAILED(XAudio2Create(&g_engine)))
	{
		CoUninitialize();
		return;
	}

	//create the mastering voice
	if (FAILED(g_engine->CreateMasteringVoice(&g_master)))
	{
		g_engine->Release();
		CoUninitialize();
		return;
	}

	//helper class to load wave files; trust me, this makes it MUCH easier
	Wave buffer;
}

void Audio::playSound(char c[])
{
	//load a wave file
	if (!buffer.load(c))
	{
		g_engine->Release();
		CoUninitialize();
		return;
	}

	//create the source voice, based on loaded wave format
	if (FAILED(g_engine->CreateSourceVoice(&g_source, buffer.wf())))
	{
		g_engine->Release();
		CoUninitialize();
		return;
	}

	//start consuming audio in the source voice
	g_source->Start();

	//play the sound
	g_source->SubmitSourceBuffer(buffer.xaBuffer());

}

void Audio::release()
{
	//release the engine, NOT the voices!
	g_engine->Release();

	//again, for COM
	CoUninitialize();

	return;
}



