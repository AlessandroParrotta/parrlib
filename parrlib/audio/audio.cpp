#include "audio.h"

namespace prb {
	namespace audio {
		void init() {
			if (!alutInit(NULL, NULL)) { std::cout << "error initing alut!\n"; std::terminate(); }
		}

		void destroy() {
			if (!alutExit()) { std::cout << "error exiting alut!\n"; checkALErrors(); }
		}

		bool checkALErrors() {
			bool error = false;

			ALCenum err;
			while ((err = alGetError()) != ALC_NO_ERROR) {
				std::cout << "AL error: ";
				switch (err) {
				case AL_INVALID_NAME: std::cout << "AL_INVALID_NAME"; break;
				case AL_INVALID_ENUM: std::cout << "AL_INVALID_ENUM"; break;
				case AL_INVALID_VALUE: std::cout << "AL_INVALID_VALUE"; break;
				case AL_INVALID_OPERATION: std::cout << "AL_INVALID_OPERATION"; break;
				case AL_OUT_OF_MEMORY: std::cout << "AL_OUT_OF_MEMORY"; break;
				default: std::cout << "unknown AL error";
				}
				std::cout << "\n";

				error = true;
			}

			ALenum alutErr;
			while ((alutErr = alutGetError()) != ALUT_ERROR_NO_ERROR) {

				const char* str = alutGetErrorString(alutErr);

				std::cout << "alut error: ";
				switch (alutErr) {
				case ALUT_ERROR_OUT_OF_MEMORY: std::cout << "ALUT_ERROR_OUT_OF_MEMORY"; break;
				case ALUT_ERROR_INVALID_ENUM: std::cout << "ALUT_ERROR_INVALID_ENUM"; break;
				case ALUT_ERROR_INVALID_VALUE: std::cout << "ALUT_ERROR_INVALID_VALUE"; break;
				case ALUT_ERROR_INVALID_OPERATION: std::cout << "ALUT_ERROR_INVALID_OPERATION"; break;
				case ALUT_ERROR_NO_CURRENT_CONTEXT: std::cout << "ALUT_ERROR_NO_CURRENT_CONTEXT"; break;
				case ALUT_ERROR_AL_ERROR_ON_ENTRY: std::cout << "ALUT_ERROR_AL_ERROR_ON_ENTRY"; break;
				case ALUT_ERROR_ALC_ERROR_ON_ENTRY: std::cout << "ALUT_ERROR_ALC_ERROR_ON_ENTRY"; break;
				case ALUT_ERROR_OPEN_DEVICE: std::cout << "ALUT_ERROR_OPEN_DEVICE"; break;
				case ALUT_ERROR_CLOSE_DEVICE: std::cout << "ALUT_ERROR_CLOSE_DEVICE"; break;
				case ALUT_ERROR_CREATE_CONTEXT: std::cout << "ALUT_ERROR_CREATE_CONTEXT"; break;
				case ALUT_ERROR_MAKE_CONTEXT_CURRENT: std::cout << "ALUT_ERROR_MAKE_CONTEXT_CURRENT"; break;
				case ALUT_ERROR_DESTROY_CONTEXT: std::cout << "ALUT_ERROR_DESTROY_CONTEXT"; break;
				case ALUT_ERROR_GEN_BUFFERS: std::cout << "ALUT_ERROR_GEN_BUFFERS"; break;
				case ALUT_ERROR_BUFFER_DATA: std::cout << "ALUT_ERROR_BUFFER_DATA"; break;
				case ALUT_ERROR_IO_ERROR: std::cout << "ALUT_ERROR_IO_ERROR"; break;
				case ALUT_ERROR_UNSUPPORTED_FILE_TYPE: std::cout << "ALUT_ERROR_UNSUPPORTED_FILE_TYPE"; break;
				case ALUT_ERROR_UNSUPPORTED_FILE_SUBTYPE: std::cout << "ALUT_ERROR_UNSUPPORTED_FILE_SUBTYPE"; break;
				case ALUT_ERROR_CORRUPT_OR_TRUNCATED_DATA: std::cout << "ALUT_ERROR_CORRUPT_OR_TRUNCATED_DATA"; break;
				default: std::cout << "unknown alut error";
				}

				if (str) {
					std::cout << ": " << str;
				}

				std::cout << "\n";

				error = true;
			}

			return error;
		}

		void listAudioDevices(const ALCchar* devices) {
			const ALCchar* device = devices, * next = devices + 1;
			size_t len = 0;

			fprintf(stdout, "Devices list:\n");
			fprintf(stdout, "----------\n");
			while (device && *device != '\0' && next && *next != '\0') {
				fprintf(stdout, "%s\n", device);
				len = strlen(device);
				device += (len + 1);
				next += (len + 2);
			}
			fprintf(stdout, "----------\n");
		}
		//listAudioDevices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));
	}
}
