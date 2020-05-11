#include <parrlib/globals.h>

#include <parrlib/sprite.h>

#include <parrlib/utils3d/mesh3d.h>

namespace globals {
	std::unordered_map<std::string, TextRenderer> txrs;

	bool findTxr(std::string const& name) { return txrs.find(name) != txrs.end(); }
	void setTxr(std::string const& name, std::vector<std::string> const& fonts, int const& fontSize) { txrs[name] = { fonts, fontSize }; }
	void setTxr(std::string const& name, std::string const& font, int const& fontSize) { setTxr(name, std::vector<std::string>{ font }, fontSize); }
	void setTxr(std::string const& name, int const& fontSize) { setTxr(name, "assets/fonts/segoeui.ttf", fontSize); }
	TextRenderer& getTxr(std::string const& name){
		if (txrs.find(name) == txrs.end()) setTxr(name, 24);
		return txrs[name];
	}

	std::unordered_map<std::string, Sprite> sprites;
	bool findSprite(std::string const& name) { return sprites.find(name) != sprites.end(); }
	void setSprite(std::string const& name, Sprite const& s) { sprites[name] = s; }
	Sprite& getSprite(std::string const& name) {
		if (sprites.find(name) == sprites.end()) sprites[name] = name.c_str();
		return sprites[name];
	}

	std::unordered_map<std::string, Shader> shaders;
	bool findShader(std::string const& name) { return shaders.find(name) != shaders.end(); }
	void setShader(std::string const& name, Shader const& sh) { shaders[name] = sh; }
	Shader& getShader(std::string const& name) {
		if (shaders.find(name) == shaders.end()) shaders[name] = { (name + ".vert"), (name + ".frag") };
		return shaders[name];
	}

	std::unordered_map<std::string, VAO> vaos;
	bool findVAO(std::string const& name) { return vaos.find(name) != vaos.end(); }
	void setVAO(std::string const& name, VAO const& vao) { vaos[name] = vao; }
	VAO& getVAO(std::string const& name) {
		if (vaos.find(name) == vaos.end()) vaos[name] = {};
		return vaos[name];
	}


	std::unordered_map<std::string, Mesh3D> meshes;
	bool findMesh(std::string const& name) { return meshes.find(name) != meshes.end(); }
	void setMesh(std::string const& name, Mesh3D const& m) { meshes[name] = m; }
	Mesh3D& getMesh(std::string const& name) { 
		if (!findMesh(name)) meshes[name] = { name.c_str() };
		return meshes[name]; 
	}
}
