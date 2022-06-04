#include "vertexbuffer.h"

#include "statemachine.h"

namespace softst {

	VertexBuffer::VertexBuffer() {}
	VertexBuffer::VertexBuffer(void* data, size_t byteSize, std::vector<unsigned int> const& dataTypes, UINT offset) {
		this->dataTypes = dataTypes; 

		float* fdata = (float*)data;
		size_t fsize = byteSize / sizeof(float);
		this->data.resize(fsize);
		for (int i = 0; i < fsize; i++) { this->data[i] = fdata[i]; }
	}
	VertexBuffer::VertexBuffer(std::vector<float> const& data, std::vector<unsigned int> const& dataTypes, UINT offset) {
		this->data = data; 
		this->dataTypes = dataTypes;
	}
	VertexBuffer::VertexBuffer(size_t byteSize, std::vector<unsigned int> const& dataTypes, UINT offset) {
		this->data.resize(byteSize / sizeof(float));
		this->dataTypes = dataTypes;
	}

	void VertexBuffer::resize(size_t size) { //in bytes
		data.resize(size);
	} 
	void VertexBuffer::setData(std::vector<float> const& data, std::vector<unsigned int> const& dataTypes) {
		this->data = data;
		this->dataTypes = dataTypes;
	}

	void VertexBuffer::use() {
		statemachine::bindVertexBuffer(this);
	}
	void VertexBuffer::release() {
		statemachine::unbindVertexBuffer();
	}

	void VertexBuffer::draw() {
		const fbuf* fb = statemachine::currentFrameBuffer();
		const Shader* vSh = statemachine::currentVertexShader();
		const Shader* fSh = statemachine::currentFragmentShader();

		std::vector<float> vertResult1; // pre-allocated for vertex shader calculation
		std::vector<float> vertResult2; // pre-allocated for vertex shader calculation
		std::vector<float> vertResult3; // pre-allocated for vertex shader calculation
		aabb2 vertBB;

		std::vector<float> interpData; // pre-allocated for fragment shader calculation

		vec4 fragResult = 0.f; // pre-allocated for fragment shader calculation

		int vertLength = 0; for (int i = 0; i < dataTypes.size(); i++) vertLength += dataTypes[i];
		for (int i = 0; i < data.size(); i += vertLength * 3) {
			if (i + vertLength * 3 > data.size()) break;

			vSh->vertexShaderSolve(vertResult1, data, i, dataTypes); // result stored in vertResult
			vSh->vertexShaderSolve(vertResult2, data, i + vertLength, dataTypes); // result stored in vertResult
			vSh->vertexShaderSolve(vertResult3, data, i + vertLength * 2, dataTypes); // result stored in vertResult

			// possible standard: always put position first in vertResult (4-dimensional vector always) then all others in the order they're specified by the user
			vertBB = aabb2{ vec2{ vertResult1[0],  vertResult1[1] } };
			vertBB.rescale(vec2{ vertResult2[0],  vertResult2[1] });
			vertBB.rescale(vec2{ vertResult3[0],  vertResult3[1] });

			for (int y = vertBB.miny(); y < vertBB.maxy(); y++) {
				for (int x = vertBB.minx(); x < vertBB.maxx(); x++) {
					// TODO: do interpData...
					fSh->fragmentShaderSolve(fragResult, interpData);
				}
			}
		}
	}

	void VertexBuffer::dispose() {
		data.clear();
		dataTypes.clear();
	}

	extern "C" __declspec(dllexport) impl::VertexBuffer_base * createVertexBuffer_1() { return new VertexBuffer(); }
	extern "C" __declspec(dllexport) impl::VertexBuffer_base * createVertexBuffer_2(void* data, size_t byteSize, std::vector<unsigned int> const& dataTypes, UINT offset) { return new VertexBuffer(data, byteSize, dataTypes, offset); }
	extern "C" __declspec(dllexport) impl::VertexBuffer_base * createVertexBuffer_3(std::vector<float> const& data, std::vector<unsigned int> const& dataTypes, UINT offset) { return new VertexBuffer(data, dataTypes, offset); }
	extern "C" __declspec(dllexport) impl::VertexBuffer_base * createVertexBuffer_4(size_t byteSize, std::vector<unsigned int> const& dataTypes, UINT offset) { return new VertexBuffer(byteSize, dataTypes, offset); }
}
