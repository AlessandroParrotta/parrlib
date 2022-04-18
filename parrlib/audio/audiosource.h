#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include <parrlibcore/vector3f.h>

#include "AudioClip.h"

namespace prb {
	class AudioSource {
		vec3 vpos = 0.f;
		vec3 vvel = 0.f;
		vec3 vdir = 0.f;

		vec3 voriat = 0.f;	//orientation at
		vec3 voriup = 0.f;	//orientation up

		float vpitch = 0.f;
		float vgain = 0.f;

		bool vplaying = false;
		bool vpaused = false;
		bool vstopped = false;

	public:
		ALuint id = 0;

		AudioClip clip;

		void pos(vec3 pos);
		vec3 pos() const;

		void vel(vec3 vel);
		vec3 vel() const;

		void dir(vec3 dir);
		vec3 dir() const;

		void ori(vec3 oriat, vec3 oriup);

		void oriat(vec3 oriat);
		vec3 oriat() const;

		void oriup(vec3 oriup);
		vec3 oriup() const;

		void pitch(float pitch);
		float pitch() const;

		void gain(float gain);
		float gain() const;

		void init();

		AudioSource();
		AudioSource(ALuint id);

		AudioSource(ALuint buffer, vec3 const& pos, vec3 const& vel, vec3 const& dir, vec3 const& oriat, vec3 const& oriup);
		AudioSource(ALuint buffer, vec3 const& pos);

		AudioSource(vec3 const& pos, vec3 const& vel, vec3 const& dir, vec3 const& oriat, vec3 const& oriup);
		AudioSource(vec3 const& pos);

		AudioSource(AudioClip const& ac);
		AudioSource(AudioClip const& ac, vec3 const& pos);

		AudioSource(const char* ac);
		AudioSource(const char* ac, vec3 const& pos);

		void setClip(AudioClip const& ac);
		void setBuffer(ALuint buffer);		//alias for setClip(AudioClip(buffer))

		float getTime() const;

		void play();
		void play(AudioClip const& ac);

		void pause();
		void stop();

		bool playing();
		bool playing() const;
		bool paused() const;
		bool stopped() const;

		void dispose();
	};

	typedef AudioSource audios;
}
