#pragma once

#include <iostream>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include "audioclip.h"
#include "audiosource.h"


namespace audio {
	void init();

	void destroy();

	bool checkALErrors();
	void listAudioDevices(const ALCchar* devices);
}

