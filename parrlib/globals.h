#pragma once

#include <string>
#include <unordered_map>

#include "textrenderer.h"
#include "shader.h"

namespace prb {
	class Sprite;
	class Mesh3D;

	namespace globals {
		bool findTxr(std::string const& name);
		void setTxr(std::string const& name, std::vector<std::string> const& fonts, int const& fontSize);
		void setTxr(std::string const& name, int const& fontSize);
		TextRenderer& getTxr(std::string const& name);

		bool findSprite(std::string const& name);
		void setSprite(std::string const& name, Sprite const& s);
		Sprite& getSprite(std::string const& name);

		bool findShader(std::string const& name);
		void setShader(std::string const& name, Shader const& sh);
		Shader& getShader(std::string const& name, std::vector<std::pair<std::string, std::string>> attributes);

		//bool findVAO(std::string const& name);
		//void setVAO(std::string const& name, VAO const& vao);
		//VAO& getVAO(std::string const& name);

		//bool findMesh(std::string const& name);
		//void setMesh(std::string const& name, Mesh3D const& m);
		//Mesh3D& getMesh(std::string const& name);
	}

#ifndef PARRLIB_NAMESPACE_SHORTEN
	namespace glb = globals;
#endif

}
