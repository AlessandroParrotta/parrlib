#include <parrlib/utils2d/Poly.h>


Poly::Poly() : GameObject(){
	verts = std::vector<Vector2f>(0);
	rVerts = std::vector<Vector2f>(0);
}

Poly::Poly(Vector2f pos, float rot) : Poly() {
	this->pos = pos;
	this->rot = rot;
}

Poly::Poly(Vector2f pos, float rot, std::vector<Vector2f> verts, float perimeter, float area) : Poly(pos, rot) {
	setVerts(verts);
	this->perimeter = perimeter;
	this->area = area;
}

Poly::Poly(Vector2f pos, float rot, std::vector<Vector2f> verts) : Poly(pos, rot, verts, utils2::calcPerimeter(verts), utils2::calcArea(verts)) {}


Poly::~Poly(){

}

void Poly::calcDrawColors() {
	drawColors = std::vector<Vector4f>(0);
	if (consts::flatShading) {
		drawColors.reserve(drawMeshes.size());

		for (int i = 0; i < drawMeshes.size(); i++) {
			/*int maxYVert = 0;
			for (int j = 0; j < drawMeshes[i].size(); j++) {
				if (drawMeshes[i][j].y > drawMeshes[i][maxYVert].y) maxYVert = j;
			}

			float val = drawMeshes[i][maxYVert].y;*/

			float  avg = 0.0f;
			for (int j = 0; j < drawMeshes[i].size(); j++) {
				avg += drawMeshes[i][j].y;
			}
			avg /= (float)drawMeshes[i].size();

			float val = avg;
			float perc = (val -boundingBox.fmin().y)/(boundingBox.fmax().y-boundingBox.fmin().y);
			drawColors.push_back(vec4(1.f).colMul(color.xyz()+color.xyz()*(perc-0.5f)));
		}

		/*float colRange = 0.1f, colHalf = colRange / 2.0f;
		for (int i = 0; i < drawMeshes.size(); i++) {
			drawColors.push_back(vec4(1.0f).colMul(color.xyz() + (Util::frand()*colRange - colHalf)));
		}*/
	}
}

void Poly::setColor(Vector4f color) {
	if (this->color != color) {
		this->color = color;
		calcDrawColors();
	}
}

Vector4f Poly::getColor() {
	return color;
}

void Poly::setVerts(std::vector<Vector2f> verts) {
	this->verts = utils2::correctTolerancePoints(verts);
	forceUpdateVerts();
	if (consts::triangulateMeshes) {
		triangulateMesh();
	}
	boundingBox = utils2::calcBoundingBox(verts);
	calcDrawColors();
	calcArea();
	calcPerimeter();

	//col = std::make_shared<Collider>(pos, rot, rVerts);
}

void Poly::setVerts(std::vector<Vector2f> verts, std::vector<std::vector<Vector2f>> drawMeshes) {
	this->verts = utils2::correctTolerancePoints(verts);
	forceUpdateVerts();
	if (consts::triangulateMeshes) {
		this->drawMeshes = drawMeshes;
	}
	boundingBox = utils2::calcBoundingBox(verts);
	calcDrawColors();
	calcArea();
	calcPerimeter();

	//col = std::make_shared<Collider>(pos, rot, rVerts);
}

std::vector<Vector2f> Poly::getVerts() {
	return verts;
}

void Poly::calcPerimeter() {
	perimeter = utils2::calcPerimeter(verts);
}

void Poly::calcArea() {
	area = utils2::calcArea(verts);
	mass = (float)pow(area, 0.4f);
}

bool Poly::lineIntersCheck(utils2::Line l) {
	l.p1 = l.p1 - pos;
	l.p2 = l.p2 - pos;

	for (int i = 0; i < rVerts.size(); i++) {
		Vector2f point;
		if (utils2::lineInters(l, utils2::Line(rVerts[i], rVerts[(i + 1) % rVerts.size()]), point)) {
			return true;
		}
	}

	return false;
}

std::vector<float> Poly::lineInters(utils2::Line l) {
	l.p1 = l.p1 - pos;
	l.p2 = l.p2 - pos;

	std::vector<float> interss = std::vector<float>(0);

	for (int i = 0; i < rVerts.size(); i++) {
		Vector2f point;
		if (utils2::lineInters(l, utils2::Line(rVerts[i], rVerts[(i+1)% rVerts.size()]), point)) {
			interss.push_back((point-l.p1).magnitude());
		}
	}

	std::sort(interss.begin(), interss.end(), std::less<float>());

	return interss;
}

std::vector<std::shared_ptr<Poly>> Poly::breakPoly(Vector2f point) {
	Vector2f relPoint = point - pos;

	std::vector<utils2::BrokenPoly> brokenPs = utils2::breakPoly(relPoint, rVerts);

	if (brokenPs.size() <= 0) return {};

	std::vector<std::shared_ptr<Poly>> broken(brokenPs.size());
	for (int i = 0; i < brokenPs.size(); i++) {
		broken[i] = std::make_shared<Poly>(pos+brokenPs[i].offset, 0.0f, brokenPs[i].brokenPoly);
		broken[i]->setColor(color);
	}

	return broken;
}

/*
	-1: break the poly with breakPoly
	-2:	break the drawMeshes with breakPoly
	-3:	
*/
std::vector<std::shared_ptr<Poly>> Poly::breakPoly(Poly &destructor) {
	std::vector<Vector2f> offDest = destructor.rVerts;
	for (int i = 0; i < offDest.size(); i++) {
		offDest[i] -= pos - destructor.pos;
	}

	//for (int i = 0; i < offDest.size(); i++) {
	//	Debug::drawLine(Consts::project(pos+offDest[i]), Consts::project(pos+offDest[(i+1)%offDest.size()]), Vector4f(0.0f,0.0f,1.0f,1.0f));
	//}

	//-1:--------------------------------------

	//std::chrono::time_point<std::chrono::steady_clock> timerEarly2 = std::chrono::high_resolution_clock::now();
	
	std::vector<utils2::BrokenPoly> broken = utils2::breakPoly(offDest, rVerts);

	//std::chrono::time_point<std::chrono::steady_clock> timerNow2 = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<float, std::nano> timeDiff = timerNow2 - timerEarly2;
	//Debug::rtlog << "breakPoly time " << timeDiff.count() / 1000000.0f << "ms\n";

	//-2:--------------------------------------
	if (consts::triangulateMeshes) {
#ifdef OPTIMIZE_CONVEX
		std::chrono::time_point<std::chrono::steady_clock> timerEarly = chrono::high_resolution_clock::now();
		std::vector<std::vector<Vector2f>> drawMeshes = this->drawMeshes;
		for (int i = 0; i < drawMeshes.size(); i++) {
			for (int j = 0; j < drawMeshes[i].size(); j++) {
				drawMeshes[i][j] = drawMeshes[i][j].rotate(0.0f, rot);
			}
		}

		std::vector<int> toErase(0);
		std::vector<Vector2f> drawOffsets(0);

		int startMeshSize = drawMeshes.size();
		for (int j = 0; j < startMeshSize; j++) {
			if (utils2::polyIntersPolyCheck(offDest, drawMeshes[j])) {
				std::vector<utils2::BrokenPoly> drawBroken = utils2::breakPoly(offDest, drawMeshes[j]);

				std::vector<std::vector<Vector2f>> newDrawMeshParts(0);
				newDrawMeshParts.reserve(drawBroken.size());

				for (int k = 0; k < drawBroken.size(); k++) {
					drawOffsets.push_back(drawBroken[k].offset);

					std::vector<Vector2f> nDrawPart(0);
					nDrawPart.reserve(drawBroken[k].brokenPoly.size());

					for (int n = 0; n < drawBroken[k].brokenPoly.size(); n++) {
						nDrawPart.push_back(drawBroken[k].brokenPoly[n] + drawBroken[k].offset);
					}

					newDrawMeshParts.push_back(nDrawPart);
				}

				toErase.push_back(j);
				drawMeshes.insert(drawMeshes.end(), newDrawMeshParts.begin(), newDrawMeshParts.end());
			}
		}

		Debug::rtlog << startMeshSize << " " << drawMeshes.size() << " (" << (drawMeshes.size() - startMeshSize) << ")\n";

		for (int i = 0; i < toErase.size(); i++) {
			drawMeshes.erase(drawMeshes.begin() + toErase[i] - i);
		}

		int brokenMeshesStart = startMeshSize - toErase.size();

		/*std::vector<std::vector<Vector2f>> joined = utils2::joinAdjacent(std::vector<std::vector<Vector2f>>(drawMeshes.begin() + startMeshSize - toErase.size(), drawMeshes.end()));

		drawMeshes.erase(drawMeshes.begin() + brokenMeshesStart, drawMeshes.end());
		drawMeshes.insert(drawMeshes.begin() + brokenMeshesStart, joined.begin(), joined.end());*/

		for (int j = 0; j < drawMeshes.size(); j++) {
			for (int k = 0; k < drawMeshes[j].size(); k++) {
				Debug::drawLine(drawMeshes[j][k] * Consts::zoom, drawMeshes[j][(k + 1) % drawMeshes[j].size()] * Consts::zoom, j < brokenMeshesStart ? vColor4f::white : vColor4f::red);
			}
		}
#endif
	}

	std::vector<std::shared_ptr<Poly>> brokenPs(0);
	brokenPs.reserve(broken.size());

	for (int i = 0; i < broken.size(); i++) {
		if (consts::triangulateMeshes) {
#ifdef OPTIMIZE_CONVEX
			std::vector<std::vector<Vector2f>> nDrawMesh(0);

			if (utils2::isConvex(broken[i].brokenPoly)) {
				nDrawMesh = { broken[i].brokenPoly };
			}
			else {
				for (int j = 0; j < drawMeshes.size(); j++) {
					bool isPartOfBroken = false;
					for (int k = 0; k < drawMeshes[j].size() && !isPartOfBroken; k++) {
						bool vertEqualsAny = false;
						for (int n = 0; n < broken[i].brokenPoly.size() && !vertEqualsAny; n++) {
							vertEqualsAny = drawMeshes[j][k].equalsRange(broken[i].brokenPoly[n] + broken[i].offset, utils2::tolerance / 2.0f);
						}
						//if (vertEqualsAny) Debug::drawCircle(drawMeshes[j][k], 0.015f, vColor4f::red);
						isPartOfBroken = vertEqualsAny;
					}

					/*for (int k = 0; k < drawMeshes[j].size(); k++) {
						Debug::drawLine(drawMeshes[j][k]*Consts::zoom, drawMeshes[j][(k + 1) % drawMeshes[j].size()]*Consts::zoom, j < startMeshSize-toErase.size() ? vColor4f::white : vColor4f::red);
					}*/

					if (isPartOfBroken) {
						std::vector<std::vector<Vector2f>> nDrawMeshConvex;

						//Debug::rtlog << j << " " << startMeshSize << " " << toErase.size() << " (" << (startMeshSize - toErase.size()) << ")\n";
						if (j >= brokenMeshesStart && !utils2::isConvex(drawMeshes[j])) {
							/*for (int k = 0; k < drawMeshes[j].size(); k++) {
								Debug::drawLine(drawMeshes[j][k] * Consts::zoom, drawMeshes[j][(k + 1) % drawMeshes[j].size()] * Consts::zoom, vColor4f::blue);
							}*/
							//Debug::rtlog << "convex check\n";
							nDrawMeshConvex = utils2::makeConvex(drawMeshes[j]);
						}
						else {
							nDrawMeshConvex = { drawMeshes[j] };
						}

						for (int k = 0; k < nDrawMeshConvex.size(); k++) {
							std::vector<Vector2f> nDrawMeshOffset(0);
							nDrawMeshOffset.reserve(nDrawMeshConvex[k].size());
							for (int n = 0; n < nDrawMeshConvex[k].size(); n++) {
								nDrawMeshOffset.push_back(nDrawMeshConvex[k][n] - broken[i].offset);
							}

							nDrawMesh.push_back(nDrawMeshOffset);
						}
					}
				}
			}
			//Debug::rtlog <<  "size " << nDrawMesh.size() << " " << broken[i].brokenPoly.size() << "\n";
#endif
		}
		//Debug::rtlog << "broksize " << broken[i].brokenPoly.size() << " ";

		std::shared_ptr<Poly> p = std::make_shared<Poly>(pos+broken[i].offset, 0.0f);
		if (consts::triangulateMeshes) {
#ifdef OPTIMIZE_CONVEX
			if (broken[i].area < Consts::polyAreaThreshold) {
				Debug::rtlog << "broken area < threshold, retriangulating\n";
				p->setVerts(broken[i].brokenPoly);
			}
			else {
				if (nDrawMesh.size() > 0 && nDrawMesh.size() <= broken[i].brokenPoly.size()) {
					p->setVerts(broken[i].brokenPoly, nDrawMesh);
				}
				else {
					Debug::rtlog << "ndrawmeshsize outside acceptable range(" << nDrawMesh.size() << ", " << broken[i].brokenPoly.size() << "), retriangulating\n";
					p->setVerts(broken[i].brokenPoly);
				}
			}
#endif
		}
		
#ifndef OPTIMIZE_CONVEX
		//std::chrono::time_point<std::chrono::steady_clock> timerEarly = std::chrono::high_resolution_clock::now();
		
		p->setVerts(broken[i].brokenPoly);
		
		//std::chrono::time_point<std::chrono::steady_clock> timerNow = std::chrono::high_resolution_clock::now();
		//std::chrono::duration<float, std::nano> timeDiff = timerNow - timerEarly;
		//Debug::rtlog << "triangulation time " << timeDiff.count() / 1000000.0f << "ms\n";
#endif
		
		p->setColor(color);

		brokenPs.push_back(p);
		
	}

	if (consts::triangulateMeshes) {
#ifdef OPTIMIZE_CONVEX
		std::chrono::time_point<std::chrono::steady_clock> timerNow = chrono::high_resolution_clock::now();
		chrono::duration<float, nano> timeDiff = timerNow - timerEarly;
		Debug::rtlog << "triangulation time " << timeDiff.count() / 1000000.0f << "ms\n";
#endif
	}

	return brokenPs;
}

std::vector<utils2::LinePolyInters> Poly::lineIntersID(utils2::Line l) {
	return utils2::lineIntersPoly(utils2::Line(l.p1-pos, l.p2-pos), rVerts);
}

std::vector<std::shared_ptr<Poly>> Poly::slicePoly(utils2::Line l) {
	l.p1 -= pos; l.p2 -= pos;
	std::vector<utils2::BrokenPoly> brokenPs = utils2::slicePoly(l, rVerts);

	if (brokenPs.size() <= 0) return {};

	std::vector<std::shared_ptr<Poly>> broken(brokenPs.size());
	for (int i = 0; i < brokenPs.size(); i++) {
		broken[i] = std::make_shared<Poly>(pos + brokenPs[i].offset, 0.0f, brokenPs[i].brokenPoly);
		broken[i]->setColor(color);
	}

	return broken;
}

void Poly::addForce(Vector2f force) {
	vel += force / mass;
}

void Poly::addTorque(float torque) {
	angMom += torque/mass;
}

void Poly::forceUpdateVerts() {
	if (rVerts.size() != verts.size()) {
		rVerts = std::vector<Vector2f>(verts.size());
	}

	for (int i = 0; i < verts.size(); i++) {
		rVerts[i] = verts[i].rotate(0.0f, rot);
	}

	rBoundingBox = utils2::calcBoundingBox(rVerts);
	wBoundingBox = rBoundingBox + pos;
}

void Poly::updateVerts() {
	if (oldRot != rot) {
		forceUpdateVerts();
	}
}

void Poly::setPos(const Vector2f &pos) {
	if (this->pos != pos) {
		this->pos = pos;
		wBoundingBox = rBoundingBox + pos;
	}
}

void Poly::setRot(const float &rot) {
	if (this->rot != rot) {
		this->rot = rot;
		updateVerts();
		GameObject::setRot(rot);
	}
}

void Poly::update() {
	GameObject::update();

	setPos(pos + vel*gtime::deltaTime);
	vel -= vel * 2.0f*gtime::deltaTime;

	if (angMom != 0.0f) {
		setRot(rot + angMom * gtime::deltaTime);
		angMom -= angMom * 1.0f*gtime::deltaTime;
	}

	if (rot >= 360.0f || rot <= 0.0f) {
		setRot(fmod(rot, 360.0f));
	}


	//if (col) {
	//	col->update();
	//	if (col->collided) {
	//		wBoundingBox = rBoundingBox + pos;
	//		col->collided = false;
	//	}
	//}
}

void Poly::triangulateMesh() {
	drawMeshes = utils2::makeConvex(verts);
}

void Poly::draw(Vector2f camPos, float zoom) {
	//Debug::rtlog << boundingBox.pos << "\n";
	//Debug::drawQuad(Consts::project(wBoundingBox.pos),  wBoundingBox.size*Consts::zoom);
	if (wBoundingBox.intersects(consts::camBoundingBox)) {
		if (!isTrash || drawMeshes.size() == 0 || !consts::triangulateMeshes) {
			glColor4f(color.x, color.y, color.z, color.w);
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < rVerts.size(); i++) {
				Vector2f projected = consts::project(pos + rVerts[i]);
				glVertex2f(projected.x, projected.y);
			}
			glEnd();
		}

		if (consts::triangulateMeshes) {
			for (int i = 0; i < drawMeshes.size(); i++) {
				//glColor4f(color.x, color.y, color.z, color.w);
				if (drawColors.size() > i) {
					util::setColor4f(drawColors[i]);
				}
				else {
					util::setColor4f(color);
				}
				glBegin(GL_TRIANGLE_FAN);
				for (int j = 0; j < drawMeshes[i].size(); j++) {
					Vector2f projected = consts::project(pos + drawMeshes[i][j].rotate(0.0f, rot));
					glVertex2f(projected.x, projected.y);
				}
				glEnd();
			}

			if (!consts::flatShading && !isTrash) {
				for (int i = 0; i < drawMeshes.size(); i++) {
					util::setColor4f(color.colMul(0.6f));
					glBegin(GL_LINE_LOOP);
					for (int j = 0; j < drawMeshes[i].size(); j++) {
						Vector2f projected = consts::project(pos + drawMeshes[i][j].rotate(0.0f, rot));
						glVertex2f(projected.x, projected.y);
					}
					glEnd();
				}
			}
		}

		//Debug::drawText(std::to_string(area) + ", " + std::to_string(perimeter), Consts::project(pos), 0.5f);
	}
}
