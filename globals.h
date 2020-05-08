#pragma once

#include <string>
#include <unordered_map>

#include "textrenderer.h"

class Sprite;

namespace globals {
	void setTxr(std::string const& name, std::vector<std::string> const& fonts, int const& fontSize);
	void setTxr(std::string const& name, std::string const& font, int const& fontSize);
	void setTxr(std::string const& name, int const& fontSize);
	TextRenderer& getTxr(std::string const& name);

	void setSprite(std::string const& name, Sprite const& s);
	Sprite& getSprite(std::string const& name);

	void setShader(std::string const& name, Shader const& sh);
	Shader& getShader(std::string const& name);

	bool findVAO(std::string const& name);
	void setVAO(std::string const& name, VAO const& vao);
	VAO& getVAO(std::string const& name);
}

#ifndef PARRLIB_NAMESPACE_SHORTEN
namespace glb = globals;
#endif
