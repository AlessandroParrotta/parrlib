#include "audiosource.h"

namespace prb {
	void AudioSource::pos(vec3 pos) { vpos = pos; alSource3f(id, AL_POSITION, pos.x, pos.y, pos.z); }
	vec3 AudioSource::pos() const { return vpos; }

	void AudioSource::vel(vec3 vel) { vvel = vel; alSource3f(id, AL_POSITION, vel.x, vel.y, vel.z); }
	vec3 AudioSource::vel() const { return vvel; }

	void AudioSource::dir(vec3 dir) { vdir = dir; alSource3f(id, AL_DIRECTION, dir.x, dir.y, dir.z); }
	vec3 AudioSource::dir() const { return vdir; }

	void AudioSource::ori(vec3 oriat, vec3 oriup) {
		voriat = oriat; voriup = oriup;
		const ALfloat orif[] = { oriat.x, oriat.y, oriat.z, oriup.x, oriup.y, oriup.z };
		alSourcefv(id, AL_ORIENTATION, orif);
	}

	void AudioSource::oriat(vec3 oriat) { voriat = oriat; ori(oriat, voriup); }
	vec3 AudioSource::oriat() const { return voriat; }

	void AudioSource::oriup(vec3 oriup) { voriup = oriup; ori(voriat, oriup); }
	vec3 AudioSource::oriup() const { return voriup; }

	void AudioSource::pitch(float pitch) { vpitch = pitch; alSourcef(id, AL_PITCH, pitch); }
	float AudioSource::pitch() const { return vpitch; }

	void AudioSource::gain(float gain) { vgain = gain; alSourcef(id, AL_GAIN, gain); }
	float AudioSource::gain() const { return vgain; }

	void AudioSource::init() {
		alGenSources(1, &id);
	}

	AudioSource::AudioSource() {}
	AudioSource::AudioSource(ALuint id) {
		this->id = id;
	}

	AudioSource::AudioSource(const char* ac) : AudioSource(AudioClip(ac)) {}
	AudioSource::AudioSource(const char* ac, vec3 const& pos) : AudioSource(AudioClip(ac), pos) {}

	AudioSource::AudioSource(ALuint buffer, vec3 const& pos, vec3 const& vel, vec3 const& dir, vec3 const& oriat, vec3 const& oriup) {
		this->id = buffer;

		this->pos(pos);
		this->vel(vel);
		this->dir(dir);
		this->ori(oriat, oriup);
	}
	AudioSource::AudioSource(ALuint buffer, vec3 const& pos) : AudioSource(buffer, pos, 0.f, 0.f, 0.f, 0.f) {}

	AudioSource::AudioSource(vec3 const& pos, vec3 const& vel, vec3 const& dir, vec3 const& oriat, vec3 const& oriup) {
		init();

		this->pos(pos);
		this->vel(vel);
		this->dir(dir);
		this->ori(oriat, oriup);
	}
	AudioSource::AudioSource(vec3 const& pos) : AudioSource(pos, 0.f, 0.f, 0.f, 0.f) {}

	AudioSource::AudioSource(AudioClip const& ac) { init(); setClip(ac); }
	AudioSource::AudioSource(AudioClip const& ac, vec3 const& pos) : AudioSource(ac) { this->pos(pos); }

	void AudioSource::setClip(AudioClip const& ac) {
		this->clip = ac;
		alSourcei(id, AL_BUFFER, ac.id);
	}

	void AudioSource::setBuffer(ALuint buffer) {
		clip = AudioClip(buffer);
	}

	float AudioSource::getTime() const {
		float off = 0.f;
		alGetSourcef(id, AL_SEC_OFFSET, &off);
		return off;
	}

	void AudioSource::play() {
		alSourcePlay(id);
		vstopped = false;
		vpaused = false;
		vplaying = true;
	}
	void AudioSource::play(AudioClip const& ac) {
		setClip(ac); play();
	}

	void AudioSource::pause() {
		alSourcePause(id);
		vpaused = true;
		vplaying = false;
	}
	void AudioSource::stop() {
		alSourceStop(id);
		vstopped = true;
		vpaused = false;
		vplaying = false;
	}

	bool AudioSource::playing() { ALint state = 0; if (vplaying) { alGetSourcei(id, AL_SOURCE_STATE, &state); vplaying = (state == AL_PLAYING); } return vplaying; }
	bool AudioSource::playing() const { ALint state = 0; alGetSourcei(id, AL_SOURCE_STATE, &state); return state == AL_PLAYING; }
	bool AudioSource::paused() const { return vpaused; }
	bool AudioSource::stopped() const { return vstopped; }

	void AudioSource::dispose() {
		clip.dispose();
		if (id) alDeleteSources(1, &id);
	}
}
