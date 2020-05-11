#pragma once

#include <vector>
#include <unordered_map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "vector2f.h"
#include "vector3f.h"
#include "vector4f.h"
#include "utils2d/axisalignedboundingbox2d.h"
#include "constants.h"

#include "otherutil.h"

#include "matrix3f.h"

class FrameBufferObject;

namespace util {

	float toRadians(float degrees);
	float toDegrees(float radians);

	extern int circleDetail;
	extern Vector2f camPos;


	extern std::vector<mat3> mStack; //matrix stack
	void pushMatrix();
	void pushMatrix(const mat3 &modifier);
	void popMatrix();
	mat3 getTopMatrix();
	mat3 getTopMatrixAspectInv();
	mat3 getTopMatrixInvAspect();
	mat3 getTopMatrixOrthoInverted();

	mat3 getAspectOrthoX();
	mat3 getAspectOrthoY();
	mat3 getMinAspectOrtho();
	mat3 getMaxAspectOrtho();
	mat3 getAspectOrtho();

	mat3 fromNdc();
	mat3 toNdc();

	mat3 fromNdcAspect();
	mat3 toNdcAspect();
	
	mat3 fromNdcAspectInv();
	mat3 toNdcAspectInv();

	aabb2 getResbbNDCAspect();

	void translate(vec2 v);
	void rotate(float angle);
	void scale(vec2 v);
	void ortho(float left, float right, float bottom, float top);
	void lookAt(vec2 eye, vec2 pos);
	void multMatrix(const mat3& m);
	void resetMatrix();

	mat3 getQuadrantMatrix(int idx, int nPerRow);

	void drawPoly(std::vector<vec2> const& vs, const int& style);
	void drawPoly(std::vector<vec2> const& vs);


	void drawTexture(GLuint texID, float x, float y, float sizeX, float sizeY);
	void drawTexture(GLuint texID, float x, float y, float sizeX, float sizeY, float cordMultX, float cordMultY);
	void drawTexture(const GLuint& texID, const Vector2f& quadPos, const Vector2f& quadSize);
	void drawTexture(GLuint texID, Vector2f quadPos, Vector2f quadSize, Vector2f coordMult);
	void drawTexture(GLuint const& tex, mat3 const& transform);
	void drawTexture(GLuint const& tex, aabb2 const& coord, mat3 const& transform);

	void drawTextureRegion(GLuint tex, vec2 pos, vec2 size, vec2 s, vec2 t);

	void drawSubTex(GLuint const& tex, vec2 const& txmin, vec2 const& txmax, mat3 const& transform);
	void drawSubTex(GLuint const& tex, aabb2 const& coord, aabb2 const& tx, mat3 const& transform);

	void drawQuadTexCoord(float x, float y, float sizeX, float sizeY);
	void drawQuadTexCoord(Vector2f quadPos, Vector2f quadSize);


	void drawQuad(Vector2f quadPos, Vector2f quadSize, float rotation, Vector2f pivot, const int &style = GL_TRIANGLE_FAN);
	void drawQuad(Vector2f quadPos, Vector2f quadSize, Vector2f lookat, const int &style = GL_TRIANGLE_FAN);
	void drawQuad(Vector2f quadPos, Vector2f quadSize, float rotation, const int &style = GL_TRIANGLE_FAN);


	void drawQuad(float x, float y, float sizeX, float sizeY, const int &style = GL_TRIANGLE_FAN);
	void drawQuad(Vector2f quadPos, Vector2f quadSize, const int &style = GL_TRIANGLE_FAN);
	void drawQuad(vec2 const& pos, vec2 const& size, mat3 const& transform, int const& style = GL_TRIANGLE_FAN);
	void drawQuad(aabb2 const& bb, mat3 const& transform, const int &style = GL_TRIANGLE_FAN);
	void drawQuad(aabb2 const& bb, const int &style = GL_TRIANGLE_FAN);


	void drawCircle(float x, float y, float radius, const int &style = GL_TRIANGLE_FAN);
	void drawCircle(Vector2f cPos, float radius, const int &style = GL_TRIANGLE_FAN);

	
	void drawLine(Vector2f p1, Vector2f p2);


	extern outl::uniss text;
	void drawText(std::string const& font, int const& fontSize, mat3 const& transform);
	void drawText(int const& fontSize, mat3 const& transform);
	void drawText(int const& fontSize, vec2 const& pos);
	void drawText(mat3 const& transform);
	void drawText(vec2 const& pos);

	aabb2 getTextBound(std::string const& font, int const& fontSize, mat3 const& transform);
	aabb2 getTextBound(mat3 const& transform);

	//void setTextOutline(unsigned int txo);
	//unsigned int getTextOutline();

	//void setTextColor(vec4 color);
	//vec4 getTextColor();
	//void setTextOutlineColor(vec4 color);
	//vec4 getTextOutlineColor();


	void drawSprite(std::string const& filename, std::string const& identifier, std::string const& animation, mat3 const& transform);
	void drawSprite(std::string const& filename, std::string const& animation, mat3 const& transform);


	void drawSprite(std::string const& filename, mat3 const& transform);


	void setColor3f(Vector3f col);
	void setColor4f(Vector4f col);
	
	void setColor(Vector3f col);
	void setColor(Vector4f col);

	void setColor(std::string const& str);
	void setColor(const char* str);

	vec4 getColor();

	void setClearColor(Vector4f col);

	bool blendEnabled();
	void blendEnable(int blendFuncSrc, int blendFuncDst);
	void blendEnable();

	void blendDisable();

	void textureEnable(int texture);
	void textureDisable();

	void textureEnable(int texture);
	void textureDisable();


	void texEnable();
	void texDisable();

	void enable3D();
	void disable3D();

	void tex3DEnable();
	void tex3DDisable();

	//enable 3d, texture and additive blending
	void enable3DTexABlend();
	void disable3DTexABlend();


	void stencilEnable();
	void stencilEnableWritingMask();
	void stencilDisableWriting();
	void stencilDisableWritingInv();
	void stencilDisable();
	void stencilClear();

	void stencilMask(GLuint mask);
	void stencilSetOp(GLenum sfail, GLenum dsfail, GLenum pass);
	void stencilSetOp(GLenum fail, GLenum pass);

	void stencilSetFunc(GLenum func, GLint value, GLuint mask);
	void stencilSetFunc(GLenum func, GLint value);

	void stencilSetAlwaysPass(GLint value);

	void stencilSetMode(GLenum passif, GLint value, GLuint mask);
	void stencilSetModeEnableBlendAlpha(GLenum passif, GLint value, GLuint mask);
	void stencilSetModeDisableBlend(GLenum passif, GLint value, GLuint mask);


	GLint getActiveTexture();
	void setActiveTexture(GLuint texture);

	void bindTexture(GLuint target, GLuint texture);
	void bindTexture(GLuint texture);

	void bindFramebuffer(GLuint fboid);					
	void bindFramebuffer(FrameBufferObject const& fbo); 

	void unbindFramebuffer(); //binds the framebuffer that was bound before the last bindFramebuffer call

	void unbindFramebufferReset(); //unbinds the framebuffer without rebinding the previous one

	GLuint getCurrentFramebuffer(); //returns the id of the current framebuffer


	bool getVecInQuad(Vector2f v, Vector2f quadPos, Vector2f quadSize);
	bool getMouseInQuad(Vector2f quadPos, Vector2f quadSize);


	struct GLScissor {
		Vector2f pos, size;

		GLScissor() {

		}

		GLScissor(Vector2f pos, Vector2f size) {
			this->pos = pos;
			this->size = size;
		}
	};
	extern std::vector<GLScissor> scissors;
	void pushGLScissor(Vector2f pos, Vector2f size);
	void popGLScissor();


	bool checkForGLErrors();

	std::string printGLError(GLenum err);

	std::string fboAttachmentToString(GLuint attachment);

	std::string textureFormatToString(GLenum format);

	std::string glCodeToString(GLenum code);

	std::string glIntToString(GLint code);


	float randomInterval(float low, float high);

	float randomInterval(float range);

	
	float frand();


	int approxEquals(float f, float val, float low, float high);
	
	int approxEquals(float f, float val, float range);


	int clamp(int i, int imin, int imax);

	int pmod(int i, int n);

	int writeToBMP(std::string fileName, std::vector<std::vector<Vector3f>> pixels);
}

#ifndef PARRLIB_NAMESPACE_SHORTEN
namespace utl = util;
#endif
