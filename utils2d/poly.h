#pragma once

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <vector>
#include <algorithm>

#include <Parrlib\Debug.h>
#include <Parrlib\Vector2f.h>
#include <Parrlib\Vector4f.h>

#include "Utils2D.h"

#include "GameObject.h"

#include "Consts.h"
#include "AxisAlignedBoundingBox2D.h"

class Poly : public GameObject{
protected:
	std::vector<Vector2f> verts;
	Vector4f color = vec4(1.0f).colMul(0.8f,0.8f,0.8f);

public:
	std::vector<Vector2f> rVerts;
	aabb2 boundingBox;
	aabb2 rBoundingBox; //rotated (using rVerts)
	aabb2 wBoundingBox; //world bounding box

	bool isTrash = false;

	Vector2f vel=Vector2f();
	float angMom = 0.0f;
	float mass = 1.0f;

	float area=0.0f, perimeter=0.0f;
	
	std::vector<std::vector<Vector2f>> drawMeshes;

	void Poly::calcDrawColors();
	std::vector<Vector4f> drawColors;

	void setColor(Vector4f color);
	Vector4f getColor();

	Poly();
	Poly(Vector2f pos, float rot);
	Poly(Vector2f pos, float rot, std::vector<Vector2f> verts, float perimeter, float area);
	Poly(Vector2f pos, float rot, std::vector<Vector2f> verts);

	~Poly();

	virtual void setVerts(std::vector<Vector2f> verts);
	virtual void setVerts(std::vector<Vector2f> verts, std::vector<std::vector<Vector2f>> drawMeshes);
	virtual std::vector<Vector2f> getVerts();

	virtual void calcPerimeter();
	virtual void calcArea();

	virtual bool lineIntersCheck(utils2::Line l);
	virtual std::vector<float> lineInters(utils2::Line l);
	

	virtual std::vector<utils2::LinePolyInters> lineIntersID(utils2::Line l);

	virtual std::vector<std::shared_ptr<Poly>> breakPoly(Vector2f point);
	virtual std::vector<std::shared_ptr<Poly>> breakPoly(Poly &destructor);
	virtual std::vector<std::shared_ptr<Poly>> slicePoly(utils2::Line l);

	virtual void addForce(Vector2f force);
	virtual void addTorque(float torque);

	virtual void forceUpdateVerts();
	virtual void updateVerts();

	virtual void setPos(const Vector2f &pos);
	virtual void setRot(const float &rot);

	virtual void update();

	virtual void triangulateMesh();
	virtual void draw(Vector2f camPos, float zoom);
};

typedef Poly poly;
