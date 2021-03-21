#include "App.h"

#include "Audio.h"

#include "Defs.h"
#include "Log.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"

Audio::Audio() : Module()
{
	music = NULL;
	name.Create("audio");
}

Audio::~Audio()
{}

bool Audio::Awake(pugi::xml_node& config)
{
	LOG("Loading Audio Mixer");
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		return true;
	}

	// Load support for the OGG image formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if ((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		return true;
	}

	// Initialize SDL_mixer
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		return true;
	}
	else
	{
		Mix_VolumeMusic(config.child("music").attribute("volume").as_int(64));
		volumeFx = config.child("fx").attribute("volume").as_int(64);
		LOG("Set volume to: %d\n", Mix_VolumeMusic(-1));
	}

	return true;
}

bool Audio::CleanUp()
{
	if (!active)
	{
		return true;
	}

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if (music != NULL)
	{
		Mix_FreeMusic(music);
	}

	ListItem<Mix_Chunk*>* item;
	for (item = fx.start; item != NULL; item = item->next)
	{
		Mix_FreeChunk(item->data);
	}

	fx.Clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

bool Audio::Load(pugi::xml_node& save)
{
	LOG("Loading SDL rendering info");

	Mix_VolumeMusic(save.child("volume").attribute("value").as_int());

	return true;
}

bool Audio::Save(pugi::xml_node& save)
{
	LOG("Saving SDL rendering info");

	pugi::xml_node volume = save.append_child("volume");
	volume.append_attribute("value").set_value(Mix_VolumeMusic(-1));

	return true;
}

bool Audio::PlayMusic(const char* path, float fade_time)
{
	if (!active)
	{
		return false;
	}

	if (music != NULL)
	{
		if (fade_time > 0.0f)
		{
			Mix_FadeOutMusic(int(fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS(path);

	if (music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		return false;
	}
	else
	{
		if (fade_time > 0.0f)
		{
			if (Mix_FadeInMusic(music, -1, (int)(fade_time * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				return false;
			}
		}
		else
		{
			if (Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				return false;
			}
		}
	}

	LOG("Successfully playing %s", path);
	return true;
}

bool Audio::StopMusic()
{
	if (Mix_HaltMusic() == 0) return true;
}

uint Audio::LoadFx(const char* path)
{
	uint ret = 0;

	if (!active)
	{
		return 0;
	}

	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if (chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.Add(chunk);
		ret = fx.Count();
	}

	return ret;
}

bool Audio::UnloadFx(uint index)
{
	ListItem<Mix_Chunk*>* s = fx.At(index - 1);
	if (s != nullptr)
	{
		Mix_FreeChunk(s->data);
		return fx.Del(s);
	}

	return false;
}

bool Audio::PlayFx(uint id, int repeat)
{
	if (!active)
	{
		return false;
	}

	if (id > 0 && id <= fx.Count())
	{
		Mix_PlayChannel(-1, fx[id - 1], repeat);
	}

	return true;
}

bool Audio::SetFxVolume(uint index)
{
	ListItem<Mix_Chunk*>* s = fx.At(index - 1);
	if (s != nullptr)
	{
		Mix_VolumeChunk(s->data, volumeFx);
		return true;
	}

	return false;
}

int Audio::GetMusicVolume()
{
	return Mix_VolumeMusic(-1);
}

void Audio::SetMusicVolume(int volume)
{
	Mix_VolumeMusic(volume);
}

int Audio::GetFxVolume()
{
	return volumeFx;
}

void Audio::SetFxVolumeValue(int volume)
{
	volumeFx = volume;
	ListItem<Mix_Chunk*>* s = fx.start;
	while (s != nullptr)
	{
		Mix_VolumeChunk(s->data, volume);
		s = s->next;
	}
}