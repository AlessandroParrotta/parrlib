#pragma once

#include <string>

#include <glad/glad.h>

#include "../parrlib/debug.h"

namespace gl {
	using namespace prb;
	namespace com {
		extern bool blendEnabled; extern bool texture2DEnabled;
		void setBlend(bool blend);
		void setTexture2D(bool enabled);
		struct tempBlendDisable {
			bool blendWasEnabled = false;
			tempBlendDisable() { deb::mbi("blend disable"); blendWasEnabled = blendEnabled; if (blendEnabled) setBlend(false); }
			~tempBlendDisable() { deb::mbi("blend enable"); if (!blendWasEnabled) setBlend(true); }
		};


        //Returns the last Win32 error, in string format. Returns an empty string if there is no error.
		std::string GetLastErrorAsString(DWORD lastErr);
		bool getLastError(bool showErrorMessage);
        bool getLastError();

		//bool checkForGLFWErrors();

		bool checkForGLErrors();

		std::string printGLError(GLenum err);
		std::string fboAttachmentToString(GLuint attachment);
		std::string textureFormatToString(GLenum format);
		std::string glCodeToString(GLenum code);
		std::string glIntToString(GLint code);
	}
}