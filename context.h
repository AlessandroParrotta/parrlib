#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>

#include "debugmenu/debugmenu.h"

#include "Time.h"
#include "Input.h"
#include "FrameBufferObject.h"
#include "Shader.h"
#include "VertexArrayObject.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

class TextRendererOld;

namespace Context {

	extern double fps;

	extern vec2 wsize; //window size
	extern aabb2 sbb;
	extern int framerateCap;
	extern bool inApp; //whether the program is inside the app function

	void setFiltering(GLint filtering);

	extern GLint fboid; //currently bound fbo

	extern GLFWwindow* glfwwindow;

	extern const GLFWvidmode* vidmodes;
	extern int vidmodecount;

	void setscmode(int sc);
	int getscmode();

	/*
		Initializes the main OpenGL context and starts running the application
		funcs should be passed in this order (you can pass NULL or nullptr to skip functions you don't want to implement):
		funcs[0] = init	--run once at the start
		funcs[1] = update --run every frame
		funcs[2] = draw	--optional, run every frame after update()
		funcs[3] = destroy --run once when the application is closed (before GL destruction)
		funcs[4] = resize --run every frame while the app is being resized
		funcs[5] = startresize --run once when the app starts being resized
		funcs[6] = endresize --run once when the app stops being resized
	*/
	void setup(std::vector<std::function<void()>> const& tfuncs, std::string const& args);
	/*
		Initializes the main OpenGL context and starts running the application
		funcs should be passed in this order (you can pass NULL or nullptr to skip functions you don't want to implement):
		funcs[0] = init	--run once at the start
		funcs[1] = update --run every frame
		funcs[2] = draw	--optional, run every frame after update()
		funcs[3] = destroy --run once when the application is closed (before GL destruction)
		funcs[4] = resize --run every frame while the app is being resized
		funcs[5] = startresize --run once when the app starts being resized
		funcs[6] = endresize --run once when the app stops being resized
	*/
	void setup(std::vector<std::function<void()>> const& tfuncs);

	//returns the resolution of the native window
	vec2 wres();
	//returns the resolution of the rendering framebuffer
	vec2 res();

	void resize(vec2 const& size);

	void setVSync(bool vSync);
	bool getVSync();

	void setClearColor(vec4 col);
	vec4 getClearColor();

	void setShader(std::string const& name, Shader const& shader);
	Shader& getShader(std::string const& name);

	void setvao(std::string const& name, VAO const& vao);
	VAO& getvao(std::string const& name);
}

namespace prc = Context;
