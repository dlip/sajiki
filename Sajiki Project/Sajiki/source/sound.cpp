#include "stdafx.h"
#include "sound.h"
#include "level.h"
namespace Sajiki
{



	void Sound::Init()
	{

		FMOD::System_Create(&system);
		system->init(32, FMOD_INIT_NORMAL, 0);
		LoadOptions();
		
		musicChannel=0;

		if (bSoundEnabled)
			CacheSounds();

	}


	void Sound::CacheSounds()
	{

		//cache sound files

		std::ifstream myFile ("./media/sounds/soundcache.txt", std::ios::in);
		std::string str;
		while (std::getline(myFile, str))
		{
			AddSound(str);
		}
		myFile.close();

		//cache music files
		for(int x=1; x<7; x++)
		{
			std::stringstream strMusic;
			strMusic << "music" << x << ".mp3";
			AddSound(strMusic.str(), true, true);
		}

	}

	Sound::~Sound()
	{
		system->close();

	}

	void Sound::PlayMusic(int no)
	{

		if(musicChannel)
		{
			musicChannel->stop();
			musicChannel=0;
		}

		if(no==0 || !bSoundEnabled)
			return;

		std::stringstream strMusic;
		strMusic << "music" << no << ".mp3";
		FMOD::Sound *music = GetSound(strMusic.str());

		system->playSound(FMOD_CHANNEL_FREE, music, false, &musicChannel);

	}

	void Sound::LoadOptions()
	{
		std::ifstream myFile ("options.txt", std::ios::in);
		
		std::string str;
		std::getline( myFile, str);
		if(str == "" || str == "1")
			bSoundEnabled=true;
		else
			bSoundEnabled=false;

		myFile.close();
	}

	void Sound::Update()
	{
		system->update();
	}

	void Sound::SaveOptions()
	{
		std::ofstream myFile ("options.txt", std::ios::out);
		myFile << bSoundEnabled;

		myFile.close();
	}

	void Sound::AddSound(std::string soundname, bool loop, bool stream)
	{

		if(GetSound(soundname))
			return;

		FMOD::Sound *sound=0;

		std::string filename = "./media/sounds/" + soundname;

		if(stream)
			system->createStream(filename.c_str(), FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound);
		else
			system->createSound(filename.c_str(), FMOD_HARDWARE, 0, &sound);

		SoundInfo *sInfo = new SoundInfo;
		sInfo->filename = soundname;
		sInfo->sound = sound;
		vecSounds.push_back(sInfo);
	}

	FMOD::Sound* Sound::GetSound(std::string soundname)
	{

		FMOD::Sound *sound=0;

		for(vecSoundsIter=vecSounds.begin(); vecSoundsIter!=vecSounds.end(); vecSoundsIter++)
		{
			if ((*vecSoundsIter)->filename==soundname)
				sound=(*vecSoundsIter)->sound;
		}

		return sound;
	}

	void Sound::PlaySound(std::string soundname)
	{

		if(!bSoundEnabled)
			return;
		FMOD::Channel *channel = 0;
		FMOD::Sound *sound=GetSound(soundname);



		if(sound!=0)
			system->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);

	}

	void Sound::SetGain(std::string soundname, float fGain)
	{
	/*	if (!bSoundEnabled)
			return;

		soundManager->getSound(soundname)->setGain(fGain);*/

	}

	void Sound::StopSound(std::string soundname)
	{
		if (!bSoundEnabled)
			return;
		//soundManager->getSound(soundname)->stop();
	}

}