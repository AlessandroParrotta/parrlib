#pragma once

#include <vector>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "..\Vector2f.h"
#include "..\Shader.h"

#include "..\debug.h"

class DrawAccumulator {
	struct Attribute {
		GLuint id;
		int idx;
		int type;

		int maxData = 0;
		int curData = 0;
		float *data;

		void addToGPU() {
			for (int i = 0; i < maxData; i++) {
				data[i] = 0.0f;
			}

			glGenBuffers(1, &id);
			glBindBuffer(GL_ARRAY_BUFFER, id);

				glBufferData(GL_ARRAY_BUFFER, maxData*sizeof(GLfloat), data, GL_DYNAMIC_DRAW);
				glVertexAttribPointer(idx, type, GL_FLOAT, false, 0, NULL);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void updateGPUBuffer() {
			glBindBuffer(GL_ARRAY_BUFFER, id);

				glBufferSubData(GL_ARRAY_BUFFER, 0, curData * sizeof(GLfloat), data);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void updateTotalBuffer() {
			glBindBuffer(GL_ARRAY_BUFFER, id);

				glBufferData(GL_ARRAY_BUFFER, curData * sizeof(GLfloat), data, GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		Attribute() {

		}

		Attribute(int idx, int type, int maxData, float *data) {
			this->idx = idx;
			this->type = type;
			this->maxData = maxData*type;
			this->data = data;
		}

		Attribute(int idx, int type, int maxData) : Attribute(idx, type, maxData, new float[maxData*type]) {
		}

		void addData(float elem[], int offset) {
			for (int i = 0; i < type; i++) {
				data[curData + i] = elem[offset + i];
				//std::cout << elem[offset + i] << "\n";
			}

			curData += type;

			//updateGPUBuffer();
		}

		void dispose() {
			glDeleteBuffers(1, &id);
			delete[] data;
		}
	};

private:
	GLuint vao, ibo;
	int *indices;
	int maxElements = 0;
	int curElement = 0;
	int drawStyle = GL_TRIANGLES;
	std::vector<Attribute> attributes;

public:

	DrawAccumulator();
	DrawAccumulator(int drawStyle, int maxElements);

	~DrawAccumulator();

	void addAttribute(int type);

	void accumulate(float values[]);
	void accumulate(std::vector<float> v);

	void draw();

	void dispose();
};

