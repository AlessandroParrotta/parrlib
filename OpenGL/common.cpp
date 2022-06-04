#include "common.h"

#include <parrlibcore/stringutils.h>

namespace gl {
	namespace com {
		bool blendEnabled = false;
		void setBlend(bool enabled) { blendEnabled = enabled; (enabled ? glEnable : glDisable)(GL_BLEND); }

		bool texture2DEnabled = false;
		void setTexture2D(bool enabled) { blendEnabled = enabled; (enabled ? glEnable : glDisable)(GL_TEXTURE_2D); }

		//Returns the last Win32 error, in string format. Returns an empty string if there is no error.
		std::string GetLastErrorAsString(DWORD lastError)
		{
			//Get the error message, if any.
			DWORD errorMessageID = lastError;
			if (errorMessageID == 0)
				return std::string(); //No error message has been recorded

			LPSTR messageBuffer = nullptr;
			size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

			std::string message(messageBuffer, size);

			//Free the buffer.
			LocalFree(messageBuffer);

			return message;
		}
		bool getLastError(bool showErrorMessage) {
			DWORD lastErr = GetLastError();
			if (lastErr == 0) return false;

			std::string lastErrStr = GetLastErrorAsString(lastErr);

			if (lastErrStr.compare("") != 0) { if (showErrorMessage) { deb::tss << "Win32 Error (" << lastErr << "): " << stru::towstr(lastErrStr); deb::mbe(); } return true; }

			return false;
		}

		bool getLastError() {
			return getLastError(true);
		}

		//bool checkForGLFWErrors() {
		//	const char* str = NULL;
		//	if (glfwGetError(&str) != GLFW_NO_ERROR) { 
		//		if (str) {
		//			deb::debss << "GLFW Error: " << str; deb::msberr();
		//		}
		//		return true;
		//	}
		//	return false;
		//}

		bool checkForGLErrors() {
			GLenum err = GL_NO_ERROR;
			bool wasError = false;

			while ((err = glGetError()) != GL_NO_ERROR) {
				deb::tss << "GL error: " << stru::towstr(printGLError(err)) << "\n"; deb::mbe();
				wasError = true;
			}

			return wasError;
		}

		std::string printGLError(GLenum err) {
			std::string result = "";
			if (err != GL_NO_ERROR) {
				switch (err) {
				case GL_INVALID_ENUM: result += "GL_INVALID_ENUM"; break;
				case GL_INVALID_VALUE: result += "GL_INVALID_VALUE"; break;
				case GL_INVALID_OPERATION: result += "GL_INVALID_OPERATION"; break;
				case GL_STACK_OVERFLOW: result += "GL_STACK_OVERFLOW"; break;
				case GL_STACK_UNDERFLOW: result += "GL_STACK_UNDERFLOW"; break;
				case GL_OUT_OF_MEMORY: result += "GL_OUT_OF_MEMORY"; break;
				case GL_INVALID_FRAMEBUFFER_OPERATION: result += "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
				//case GL_CONTEXT_LOST: result += "GL_CONTEXT_LOST"; break;
					//case GL_TABLE_TOO_LARGE1: ARB image extension (deprecated)
				default:
					result += "unknown GL error\n";
				}
			}
			return result;
		}

		std::string fboAttachmentToString(GLuint attachment) {
			if (attachment >= GL_COLOR_ATTACHMENT0 && attachment <= GL_COLOR_ATTACHMENT31) return "GL_COLOR_ATTACHMENT" + std::to_string(attachment - GL_COLOR_ATTACHMENT0);
			switch (attachment) {
			case GL_DEPTH_ATTACHMENT: return "GL_DEPTH_ATTACHMENT";
			case GL_STENCIL_ATTACHMENT: return "GL_STENCIL_ATTACHMENT";
			case GL_DEPTH_STENCIL_ATTACHMENT: return "GL_DEPTH_STENCIL_ATTACHMENT";
			}
			return "?";
		}

		std::string textureFormatToString(GLenum format) {
			switch (format) {
			case GL_RGBA: return "GL_RGBA";
			case GL_RGB: return "GL_RGB";
			case GL_RG: return "GL_RG";
			case GL_RED: return "GL_RED";
			case GL_GREEN: return "GL_GREEN";
			case GL_BLUE: return "GL_BLUE";
			}
			return "unknown";
		}

		std::string glCodeToString(GLenum code) {
			switch (code) {
			case GL_RGBA:			return "GL_RGBA";
			case GL_RGB:			return "GL_RGB";
			case GL_RG:				return "GL_RG";
			case GL_RED:			return "GL_RED";
			case GL_GREEN:			return "GL_GREEN";
			case GL_BLUE:			return "GL_BLUE";
			case GL_ALPHA:			return "GL_ALPHA";

			case GL_UNSIGNED_BYTE:	return "GL_UNSIGNED_BYTE";
			case GL_FLOAT:			return "GL_FLOAT";
			case GL_INT:			return "GL_INT";
			case GL_UNSIGNED_INT:	return "GL_UNSIGNED_INT";

			case GL_DEPTH_ATTACHMENT:			return "GL_DEPTH_ATTACHMENT";
			case GL_STENCIL_ATTACHMENT:			return "GL_STENCIL_ATTACHMENT";
			case GL_DEPTH_STENCIL_ATTACHMENT:	return "GL_DEPTH_STENCIL_ATTACHMENT";

			case GL_NEAREST:		return "GL_NEAREST";
			case GL_LINEAR:			return "GL_LINEAR";

			case GL_CLAMP_TO_EDGE:			return "GL_CLAMP_TO_EDGE";
			case GL_MIRRORED_REPEAT:			return "GL_MIRRORED_REPEAT";
			case GL_REPEAT:			return "GL_REPEAT";
			case GL_CLAMP_TO_BORDER:			return "GL_CLAMP_TO_BORDER";

			case GL_DEPTH_COMPONENT:			return "GL_LINEAR";
			case GL_DEPTH_STENCIL:			return "GL_LINEAR";

			case GL_R8:	return "GL_R8";
			case GL_R8_SNORM:	return "GL_R8_SNORM";
			case GL_R16:	return "GL_R16";
			case GL_R16_SNORM:	return "GL_R16_SNORM";
			case GL_RG8:		return "GL_RG8";
			case GL_RG8_SNORM:	return "GL_RG8_SNORM";
			case GL_RG16:		return "GL_RG16";
			case GL_RG16_SNORM:	return "GL_RG16_SNORM";
			case GL_R3_G3_B2:return "GL_R3_G3_B2";
			case GL_RGB4:	return "GL_RGB4";
			case GL_RGB5:	return "GL_RGB5";
			case GL_RGB8:	return "GL_RGB8";
			case GL_RGB8_SNORM:	return "GL_RGB8_SNORM";
			case GL_RGB10:	return "GL_RGB10";
			case GL_RGB12:	return "GL_RGB12";
			case GL_RGB16_SNORM:	return "GL_RGB16_SNORM";
			case GL_RGBA2:	return "GL_RGBA2";
			case GL_RGBA4:	return "GL_RGBA4";
			case GL_RGB5_A1:	return "GL_RGB5_A1";
			case GL_RGBA8:	return "GL_RGBA8";
			case GL_RGBA8_SNORM:	return "GL_RGBA8_SNORM";
			case GL_RGB10_A2:	return "GL_RGB10_A2";
			case GL_RGB10_A2UI:	return "GL_RGB10_A2UI";
			case GL_RGBA12:	return "GL_RGBA12";
			case GL_RGBA16:	return "GL_RGBA16";
			case GL_SRGB8:	return "GL_SRGB8";
			case GL_SRGB8_ALPHA8: return "GL_SRGB8_ALPHA8";
			case GL_R16F:	return "GL_R16F";
			case GL_RG16F:	return "GL_RG16F";
			case GL_RGB16F:	return "GL_RGB16F";
			case GL_RGBA16F:	return "GL_RGBA16F";
			case GL_R32F:	return "GL_R32F";
			case GL_RG32F:	return "GL_RG32F";
			case GL_RGB32F:	return "GL_RGB32F";
			case GL_RGBA32F:	return "GL_RGBA32F";
			case GL_R11F_G11F_B10F:	return "GL_R11F_G11F_B10F";
			case GL_RGB9_E5:			return "GL_RGB9_E5";
			case GL_R8I:		return "GL_R8I";
			case GL_R8UI:	return "GL_R8UI";
			case GL_R16I:	return "GL_R16I";
			case GL_R16UI:	return "GL_R16UI";
			case GL_R32I:	return "GL_R32I";
			case GL_R32UI:	return "GL_R32UI";
			case GL_RG8I:	return "GL_RG8I";
			case GL_RG8UI:	return "GL_RG8UI";
			case GL_RG16I:	return "GL_RG16I";
			case GL_RG16UI:	return "GL_RG16UI";
			case GL_RG32I:	return "GL_RG32I";
			case GL_RG32UI:	return "GL_RG32UI";
			case GL_RGB8I:	return "GL_RGB8I";
			case GL_RGB8UI:	return "GL_RGB8UI";
			case GL_RGB16I:	return "GL_RGB16I";
			case GL_RGB16UI:	return "GL_RGB16UI";
			case GL_RGB32I:		return "GL_RGB32I";
			case GL_RGB32UI:	return "GL_RGB32UI";
			case GL_RGBA8I:		return "GL_RGBA8I";
			case GL_RGBA8UI:	return "GL_RGBA8UI";
			case GL_RGBA16I:	return "GL_RGBA16I";
			case GL_RGBA16UI:	return "GL_RGBA16UI";
			case GL_RGBA32I:	return "GL_RGBA32I";
			case GL_RGBA32UI:	return "GL_RGBA32UI";

			case GL_COMPRESSED_RED:	return "GL_COMPRESSED_RED";
			case GL_COMPRESSED_RG:	return "GL_COMPRESSED_RG";
			case GL_COMPRESSED_RGB:	return "GL_COMPRESSED_RGB";
			case GL_COMPRESSED_RGBA:	return "GL_COMPRESSED_RGBA";
			case GL_COMPRESSED_SRGB:	return "GL_COMPRESSED_SRGB";
			case GL_COMPRESSED_SRGB_ALPHA:	return "GL_COMPRESSED_SRGB_ALPHA";
			case GL_COMPRESSED_RED_RGTC1:	return "GL_COMPRESSED_RED_RGTC1";
			case GL_COMPRESSED_SIGNED_RED_RGTC1:	return "GL_COMPRESSED_SIGNED_RED_RGTC1";
			case GL_COMPRESSED_RG_RGTC2:			return "GL_COMPRESSED_RG_RGTC2";
			case GL_COMPRESSED_SIGNED_RG_RGTC2:		return "GL_COMPRESSED_SIGNED_RG_RGTC2";
				//case GL_COMPRESSED_RGBA_BPTC_UNORM:	return "GL_RGBA16UI";
				//case GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM:	return "GL_RGBA16UI";
				//case GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT:return "GL_RGBA16UI";
				//case GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT:return "GL_RGBA16UI";


			}
			return "unknown";
		}

		std::string glIntToString(GLint code) {
			switch (code) {
			case GL_RGBA: return "GL_RGBA";
			case GL_RGB: return "GL_RGB";
			case GL_RG: return "GL_RG";
			case GL_RED: return "GL_RED";
			case GL_GREEN: return "GL_GREEN";
			case GL_BLUE: return "GL_BLUE";
			}
			return "unknown";
		}
	}
}
