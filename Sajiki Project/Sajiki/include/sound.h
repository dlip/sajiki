#ifndef SOUND_H
#define SOUND_H

#include "singleton.h"

#include "fmod.hpp"
#include "fmod_errors.h"

namespace Sajiki
{
	typedef struct
	{
		std::string filename;
		FMOD::Sound *sound;
	}SoundInfo;

	/**
		@class Sound
		@brief FMOD sound system.
	 */
	class Sound: public Singleton<Sound>
	{
		friend Singleton;
	public:
		void Init();
		void AddSound(std::string soundname, bool loop=false, bool stream=false);
		void PlaySound(std::string soundname);
		FMOD::Sound* GetSound(std::string soundname);
		void CacheSounds();
		void StopSound(std::string soundname);
		void SetGain(std::string soundname, float fGain);
		void PlayMusic(int no);
		bool bSoundEnabled;
		void LoadOptions();
		void SaveOptions();
		void Update();
	private:

		FMOD::System     *system;
		FMOD::Channel *musicChannel;
		std::vector<SoundInfo*> vecSounds;
		std::vector<SoundInfo*>::iterator vecSoundsIter;
		std::string strCurrentSong;

	protected:
		Sound(){};
		~Sound();
	};

	#define ASound Sound::Instance()
}
#endif