#pragma once

#include <iostream>
#include <string>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

namespace prb {
	class AudioClip {
	public:
		ALuint id = 0; //buffer id

		AudioClip();
		AudioClip(ALuint buffer);
		AudioClip(const char* fileName);
		AudioClip(std::string const& fileName);

		void dispose();
	};

	typedef AudioClip audioc;
}
