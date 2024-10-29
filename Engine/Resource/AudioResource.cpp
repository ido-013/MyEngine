#include "AudioResource.h"

AudioResource::~AudioResource()
{
	UnloadData();
}

void AudioResource::LoadData(const std::string& _filename)
{
	//audio = AEAudioLoadMusic(name.c_str());
	//data = &audio;
}

void AudioResource::ReloadData(const std::string& _filename)
{
	UnloadData();
	LoadData(_filename);
}

void AudioResource::UnloadData()
{
}
