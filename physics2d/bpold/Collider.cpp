#include "Collider.h"



Collider::Collider(Vector2f &pos, float &rot, std::vector<Vector2f> &verts) : pos(pos), rot(rot), verts(verts){
	//verts = Utils2D::makeClockwise(verts);
	forceUpdateAxes();
	forceUpdateWVerts();
}


Collider::~Collider()
{
}

void Collider::forceUpdateAxes() {
	if (verts.size() != axes.size()) {
		axes = std::vector<Vector2f>(verts.size());
	}

	for (int i = 0; i < verts.size(); i++) {
		axes[i] = verts[i]- verts[(i + 1) % verts.size()];
		axes[i] = Vector2f(-axes[i].y, axes[i].x).normalized();
	}
}

void Collider::updateAxes() {
	if (oldRot != rot) {
		forceUpdateAxes();
		forceUpdateWVerts();

		oldRot = rot;
	}
 }

void Collider::forceUpdateWVerts() {
	if (wVerts.size() != verts.size()) {
		wVerts = std::vector<Vector2f>(verts.size());
	}

	for (int i = 0; i < verts.size(); i++) {
		wVerts[i] = verts[i] + pos;
	}
}

void Collider::updateWVerts() {
	if (oldPos != pos) {
		forceUpdateWVerts();

		oldPos = pos;
	}
}

void Collider::update() {
	updateAxes();
	updateWVerts();
}

std::shared_ptr<Collision> Collider::checkCollision(Collider &other) {
	/*if (Utils2D::polyIntersPolyCheck(verts, other.verts, pos - other.pos)) {
		return std::make_shared<Collision>(this, other);
	}*/

	//Vector2f relPos = other.pos - pos;

	float overlap = FLT_MAX;
	Vector2f smallestAxis = FLT_MAX;

	for (int i = 0; i < axes.size(); i++) {
#ifdef COLLIDER_DEBUG
		vec2  center = (pos + other.pos) / 2.0f;
		float dist = 0.4f;// (other.pos - pos).magnitude();

		vec2 axis = axes[i];
		Utils2D::Line l(center -axis.perp() * dist - axis*2.0f, center -axis.perp()  * dist + axis*2.0f);
		
		Debug::drawLine(Consts::project(l.p1), Consts::project(l.p2), Vector4f(1.0f,0.0f,0.0f,1.0f));
		
		Debug::drawText(axes[i].toString(), Consts::project(wVerts[i]), 0.5f*Consts::zoom);
#endif

		Projection p1(other, axes[i]);
		Projection p2(*this, axes[i]);

#ifdef COLLIDER_DEBUG
		Utils2D::Line lp1(center -axis.perp()  * (dist+0.02f) + axis*p1.min, center -axis.perp()  * (dist + 0.02f) + axis*p1.max);
		Utils2D::Line lp2(center -axis.perp()  * (dist + 0.04f) + axis*p2.min, center -axis.perp()  * (dist + 0.04f) + axis*p2.max);

		Debug::drawLine(Consts::project(lp1.p1), Consts::project(lp1.p2), Vector4f(0.0f, 1.0f, 0.0f, 1.0f));
		Debug::drawText("min", Consts::project(lp1.p1),  0.25f);
		Debug::drawText("max", Consts::project(lp1.p2), 0.25f);

		Debug::drawLine(Consts::project(lp2.p1), Consts::project(lp2.p2), Vector4f(0.0f, 0.0f, 1.0f, 1.0f));
		Debug::drawText("min", Consts::project(lp2.p1), 0.25f);
		Debug::drawText("max", Consts::project(lp2.p2), 0.25f);
#endif
		if (p1.overlaps(p2)) {
			float ov = p1.getOverlap(p2);
#ifdef COLLIDER_DEBUG
			Debug::drawText(std::to_string(ov), Consts::project(lp1.p1+0.05f), 0.25f);
#endif
			if (ov != 0.0f && abs(ov) < abs(overlap)) {
				overlap = ov;
				smallestAxis = axes[i];
			}
		}
		else {
			return nullptr;
		}
	}

	for (int i = 0; i < other.axes.size(); i++) {
#ifdef COLLIDER_DEBUG
		vec2  center = (pos + other.pos) / 2.0f;
		float dist = 0.8f;// (other.pos - pos).magnitude();

		vec2 axis = other.axes[i];
		Utils2D::Line l(center - axis.perp() * dist - axis * 2.0f, center - axis.perp()  * dist + axis * 2.0f);

		Debug::drawLine(Consts::project(l.p1), Consts::project(l.p2), Vector4f(1.0f, 1.0f, 1.0f, 1.0f));

		Debug::drawText(other.axes[i].toString(), Consts::project(other.wVerts[i]), 0.5f*Consts::zoom);
#endif
		Projection p1 = Projection(other, other.axes[i]);
		Projection p2 = Projection(*this, other.axes[i]);

#ifdef COLLIDER_DEBUG
		Utils2D::Line lp1(center - axis.perp()  * (dist + 0.02f) + axis * p1.min, center - axis.perp()  * (dist + 0.02f) + axis * p1.max);
		Utils2D::Line lp2(center - axis.perp()  * (dist + 0.04f) + axis * p2.min, center - axis.perp()  * (dist + 0.04f) + axis * p2.max);

		Debug::drawLine(Consts::project(lp1.p1), Consts::project(lp1.p2), Vector4f(0.0f, 1.0f, 0.0f, 1.0f));
		Debug::drawText("min", Consts::project(lp1.p1), 0.25f);
		Debug::drawText("max", Consts::project(lp1.p2), 0.25f);

		Debug::drawLine(Consts::project(lp2.p1), Consts::project(lp2.p2), Vector4f(0.0f, 0.0f, 1.0f, 1.0f));
		Debug::drawText("min", Consts::project(lp2.p1), 0.25f);
		Debug::drawText("max", Consts::project(lp2.p2), 0.25f);
#endif

		if (p1.overlaps(p2)) {
			float ov = p1.getOverlap(p2);

#ifdef COLLIDER_DEBUG
			Debug::drawText(std::to_string(ov), Consts::project(lp1.p1+0.05f),  0.25f);
#endif
			if (ov != 0.0f && abs(ov) < abs(overlap)) {
				overlap = ov;
				smallestAxis = other.axes[i];
			}
		}
		else {
			return nullptr;
		}
	}

	//Debug::rtlog << "overlap " << overlap << "\n";

	if (overlap == FLT_MAX || overlap == 0.0f) return nullptr;

#ifdef COLLIDER_DEBUG
	Debug::rtlog << "collides!  " << smallestAxis << " " << overlap << "\n";
#endif
	collided = true;
	other.collided = true;
	return std::make_shared<Collision>(*this, other, smallestAxis, overlap);
}