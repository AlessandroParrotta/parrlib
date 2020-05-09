#pragma once

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <vector>

#include "vector2f.h"
#include "vector3f.h"
#include "vector4f.h"

class VertexBufferObject {
public:
	GLuint id = 0;

	int dataType = 2;  //2 = vec2, 3 = vec3 ....
	int drawType = GL_DYNAMIC_DRAW;

	int vertPointer = 0;

	GLsizei size; //size in number of floats, not bytes

	void bind();
	void unbind();

	void setup(int listPos);
	void setupForVAO(int index);

	void draw();

	void dispose();

	void modifyValues(std::vector<float> const& v, int offset);
	void modifyValues(std::vector<float> const& v);

	void setData(std::vector<float> const& data, GLsizei const& size);
	void setData(std::vector<float> const& data);

	//T type must provide a const operator[] overload and a const int length() function (akin to a 'vec'(2,3,4,n...) class)
	template<typename T>
	void setData(std::vector<T> const& data) {
		if (data.size() == 0) return;
		int length = data[0].length();

		std::vector<float> adata;
		adata.reserve(data.size() * length);
		for (int i = 0; i < data.size(); i++) {
			for (int j = 0; j < length; j++) {
				//std::cout << "pushed " << data[i][j] << "\n";
				adata.push_back(data[i][j]);
			}
		}

		setData(adata);
	}

	//void setData(std::vector<vec2> const& data);
	//void setData(std::vector<vec3> const& data);
	//void setData(std::vector<vec4> const& data);

	bool null();

	VertexBufferObject();
	VertexBufferObject(std::vector<float> const& data, int dataType, int drawType);

	VertexBufferObject(std::vector<float> const& data, int dataType);

	~VertexBufferObject();
};

typedef VertexBufferObject VBO;
