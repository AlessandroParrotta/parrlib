#include "DrawAccumulator.h"



DrawAccumulator::DrawAccumulator()
{
}

DrawAccumulator::DrawAccumulator(int drawStyle, int maxElements){
	this->drawStyle = drawStyle;
	this->maxElements = maxElements;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

		indices = new int[maxElements];
		for (int i = 0; i < maxElements; i++) {
			indices[i] = 0;
		}

		//glGenBuffers(1, &ibo);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

			//glBufferData(GL_ELEMENT_ARRAY_BUFFER,  maxElements * sizeof(int), indices, GL_DYNAMIC_DRAW);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

DrawAccumulator::~DrawAccumulator()
{
}

void DrawAccumulator::addAttribute(int type) {
	attributes.push_back(Attribute(attributes.size(), type, maxElements));

	glBindVertexArray(vao);

		attributes[attributes.size()-1].addToGPU();

	glBindVertexArray(0);

	//std::cout << "added to gpu!\n";
}

void DrawAccumulator::accumulate(float values[]) {
	if (curElement < maxElements) {
		//glBindVertexArray(vao);

			int iter = 0;
			for (int i = 0; i < attributes.size(); i++) {
				attributes[i].addData(values, iter);
				//std::cout << "iter " << iter << "\n";
				iter += attributes[i].type;
			}

		//glBindVertexArray(0);

		//indices[curElement] = curElement;
		//std::cout << curElement << " " << attributes[0].curData << " " << attributes[1].curData << "\n";
		//std::cout << indices[curElement] << "\n";
 		curElement++;
	}
	else {
		if (maxElements > 0) {
			draw();
			accumulate(values);
		}
	}
}

void DrawAccumulator::accumulate(std::vector<float> v) {
	accumulate(&v[0]);
}

void DrawAccumulator::draw() {
	if (curElement > 0) {
		glBindVertexArray(vao);

		for (int i = 0; i < attributes.size(); i++) {
			glEnableVertexAttribArray(i);
		}

		for (int i = 0; i < attributes.size(); i++) {
			attributes[i].updateTotalBuffer();
		}

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

			//glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxElements * sizeof(int), indices, GL_DYNAMIC_DRAW);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glDrawArrays(drawStyle, 0, curElement);
		//glDrawElements(drawStyle, curElement, GL_UNSIGNED_INT, 0);

		for (int i = attributes.size() - 1; i >= 0; i--) {
			glDisableVertexAttribArray(i);
		}

		glBindVertexArray(0);

		for (int i = 0; i < attributes.size(); i++) {
			attributes[i].curData = 0;
		}

		curElement = 0;
	}
}

void DrawAccumulator::dispose() {
	for (int i = 0; i < attributes.size(); i++) {
		attributes[i].dispose();
	}
	delete[] indices;
	glDeleteBuffers(1, &ibo);
	glDeleteBuffers(1, &vao);
}
