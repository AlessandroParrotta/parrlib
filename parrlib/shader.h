#pragma once

#include <unordered_map>
#include <iostream>

#include "Input.h"
#include "constants.h"
#include "Time.h"
#include "math/Matrix4f.h"
#include "math/Matrix3f.h"
#include "math/Matrix2f.h"

#include "math/Vector2f.h"
#include "math/Vector3f.h"

#include "math/OtherUtil.h"

#include "impl/impl.h"

namespace prb {
	class Shader {
	public:
		impl::Shader_base* ptr;

		Shader();
		Shader(std::string const& name, std::vector<std::pair<std::string, std::string>> attributes);
		Shader(std::string const& name);
		//Shader(std::string const& vert, std::string const& frag);
		~Shader();

		void setUniform(std::string const& name, int i);
		void setUniform(std::string const& name, float f);
		void setUniform(std::string const& name, bool b);

		void setUniform(std::string const& name, vec2 const& v);
		void setUniform(std::string const& name, vec3 const& v);
		void setUniform(std::string const& name, vec4 const& v);

		void setUniform(std::string const& name, mat3 const& mat);
		void setUniform(std::string name, mat4 const& mat);

		void use() const;
		void release() const;
	};

}
