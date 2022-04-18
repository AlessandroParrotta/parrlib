#pragma once

#include <parrlibcore/matrix3f.h>
#include <parrlibcore/matrix4f.h>

namespace prb {
	namespace impl {
		class Shader_base {
		public:
			//virtual ~Shader_base();
			//virtual void setUniform(std::string name, const void* value, size_t byteSize) = 0;

			virtual void setUniform(std::string const& name, int i) = 0;
			virtual void setUniform(std::string const& name, float f) = 0;
			virtual void setUniform(std::string const& name, bool b) = 0;

			virtual void setUniform(std::string const& name, vec2 const& v) = 0;
			virtual void setUniform(std::string const& name, vec3 const& v) = 0;
			virtual void setUniform(std::string const& name, vec4 const& v) = 0;

			virtual void setUniform(std::string const& name, mat3 const& mat) = 0;
			virtual void setUniform(std::string const& name, mat4 const& mat) = 0;

			virtual void use() const = 0;
			virtual void release() const = 0;
		};
	}
}
