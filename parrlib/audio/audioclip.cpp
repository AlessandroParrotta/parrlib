#include "audioclip.h"
namespace prb {
	AudioClip::AudioClip() {

	}

	AudioClip::AudioClip(ALuint buffer) {
		id = buffer;
	}

	AudioClip::AudioClip(const char* fileName) {
		id = alutCreateBufferFromFile(fileName);
		if (id == 0) std::cout << "could not load clip from file " << fileName << "\n";
	}
	AudioClip::AudioClip(std::string const& fileName) : AudioClip(fileName.c_str()) {}

	void AudioClip::dispose() {
		if (id) alDeleteBuffers(1, &id);
	}
}
