#include <parrlib/Quaternion.h>

Quaternion Quaternion::operator* (const Quaternion &q) const {
	float nW = w * q.w - x * q.x - y * q.y - z * q.z;
	float nX = x * q.w + w * q.x + y * q.z - z * q.y;
	float nY = y * q.w + w * q.y + z * q.x - x * q.z;
	float nZ = z * q.w + w * q.z + x * q.y - y * q.x;

	return Quaternion(nX, nY, nZ, nW);
}

Quaternion Quaternion::operator+ (const Quaternion &q) const {
	return Quaternion(x+q.x, y+q.y, z+q.z, w+q.w);
}

mat4 Quaternion::operator*(mat4 const& m) const { return toMatrix() * m; }

//Quaternion Quaternion::operator* (const Vector3f &v) {
//	float nW = -x * v.x - y * v.y - z * v.z;
//	float nX = w * v.x + y * v.z - z * v.y;
//	float nY = w * v.y + z * v.x - x * v.z;
//	float nZ = w * v.z + x * v.y - y * v.x;
//
//	return Quaternion(nX, nY, nZ, nW);
//}

Vector3f Quaternion::operator*(const Vector3f &v) const {
	float num = x * 2.0f;
	float num2 = y * 2.0f;
	float num3 = z * 2.0f;
	float num4 = x * num;
	float num5 = y * num2;
	float num6 = z * num3;
	float num7 = x * num2;
	float num8 = x * num3;
	float num9 = y * num3;
	float num10 = w * num;
	float num11 = w * num2;
	float num12 = w * num3;

	Vector3f result;
	result.x = (1.0f - (num5 + num6)) * v.x + (num7 - num12) * v.y + (num8 + num11) * v.z;
	result.y = (num7 + num12) * v.x + (1.0f - (num4 + num6)) * v.y + (num9 - num10) * v.z;
	result.z = (num8 - num11) * v.x + (num9 + num10) * v.y + (1.0f - (num4 + num5)) * v.z;
	return result;
}

Quaternion& Quaternion::operator*= (const Quaternion &q) {
	float nW = w * q.w - x * q.x - y * q.y - z * q.z;
	float nX = x * q.w + w * q.x + y * q.z - z * q.y;
	float nY = y * q.w + w * q.y + z * q.x - x * q.z;
	float nZ = z * q.w + w * q.z + x * q.y - y * q.x;

	w = nW;
	x = nX;
	y = nY;
	z = nZ;

	return *this;
}

//Quaternion Quaternion::inverse() {
//	return Quaternion(1.0f/x, 1.0f/y, 1.0f/z, 1.0f/w);
//}

Quaternion Quaternion::scale(float s) const {
	return Quaternion(x*s, y*s, z*s, w*s);
}

Quaternion Quaternion::inverse() const {
	return conjugate().scale(1.0f/(x*x + y*y + z*z + w*w));
}

float Quaternion::getTheta(float angle) const {
	return util::toRadians(angle);
}

float Quaternion::magnitude() const {
	return sqrt((x*x) + (y*y) + (z*z) + (w*w));
}


Quaternion Quaternion::normalized() const {
	float magnitude2 = magnitude();
	return Quaternion(x / magnitude2, y / magnitude2, z / magnitude2, w / magnitude2);
}

//Quaternion Quaternion::conjugate() {
//	return Quaternion(-x, -y, -z -w);
//}

Quaternion Quaternion::conjugate() const {
	return Quaternion(-x, -y, -z, w);
}

Vector3f Quaternion::direction() const {
	Quaternion q = *this*Vector3f(0.0f,0.0f,1.0f);

	return Vector3f(q.x, q.y, q.z).normalized();
}

Quaternion Quaternion::fromMatrix(Matrix4f mat) const {
	Quaternion q;

	float trace = mat.matrix[0][0] + mat.matrix[1][1] + mat.matrix[2][2]; // I removed + 1.0f; see discussion with Ethan
	if (trace > 0.0f) {// I changed M_EPSILON to 0
		float s = 0.5f / sqrtf(trace + 1.0f);
		q.w = 0.25f / s;
		q.x = (mat.matrix[2][1] - mat.matrix[1][2]) * s;
		q.y = (mat.matrix[0][2] - mat.matrix[2][0]) * s;
		q.z = (mat.matrix[1][0] - mat.matrix[0][1]) * s;
	}
	else {
		if (mat.matrix[0][0] > mat.matrix[1][1] && mat.matrix[0][0] > mat.matrix[2][2]) {
			float s = 2.0f * sqrtf(1.0f + mat.matrix[0][0] - mat.matrix[1][1] - mat.matrix[2][2]);
			q.w = (mat.matrix[2][1] - mat.matrix[1][2]) / s;
			q.x = 0.25f * s;
			q.y = (mat.matrix[0][1] + mat.matrix[1][0]) / s;
			q.z = (mat.matrix[0][2] + mat.matrix[2][0]) / s;
		}
		else if (mat.matrix[1][1] > mat.matrix[2][2]) {
			float s = 2.0f * sqrtf(1.0f + mat.matrix[1][1] - mat.matrix[0][0] - mat.matrix[2][2]);
			q.w = (mat.matrix[0][2] - mat.matrix[2][0]) / s;
			q.x = (mat.matrix[0][1] + mat.matrix[1][0]) / s;
			q.y = 0.25f * s;
			q.z = (mat.matrix[1][2] + mat.matrix[2][1]) / s;
		}
		else {
			float s = 2.0f * sqrtf(1.0f + mat.matrix[2][2] - mat.matrix[0][0] - mat.matrix[1][1]);
			q.w = (mat.matrix[1][0] - mat.matrix[0][1]) / s;
			q.x = (mat.matrix[0][2] + mat.matrix[2][0]) / s;
			q.y = (mat.matrix[1][2] + mat.matrix[2][1]) / s;
			q.z = 0.25f * s;
		}
	}

	return q;
}

Matrix4f Quaternion::toMatrix() const {
	Matrix4f mat;

	mat.setupRotation(0.0f, 0.0f, 0.0f);

	float sqw = w*w;
	float sqx = x*x;
	float sqy = y*y;
	float sqz = z*z;

	// invs (inverse square length) is only required if quaternion is not already normalised
	float invs = 1 / (sqx + sqy + sqz + sqw);
	mat.matrix[0][0] = (sqx - sqy - sqz + sqw)*invs; // since sqw + sqx + sqy + sqz =1/invs*invs
	mat.matrix[1][1] = (-sqx + sqy - sqz + sqw)*invs;
	mat.matrix[2][2] = (-sqx - sqy + sqz + sqw)*invs;

	float tmp1 = x*y;
	float tmp2 = z*w;
	mat.matrix[1][0] = 2.0f * (tmp1 + tmp2)*invs;
	mat.matrix[0][1] = 2.0f * (tmp1 - tmp2)*invs;

	tmp1 = x*z;
	tmp2 = y*w;
	mat.matrix[2][0] = 2.0f * (tmp1 - tmp2)*invs;
	mat.matrix[0][2] = 2.0f * (tmp1 + tmp2)*invs;
	tmp1 = y*z;
	tmp2 = x*w;
	mat.matrix[2][1] = 2.0f * (tmp1 + tmp2)*invs;
	mat.matrix[1][2] = 2.0f * (tmp1 - tmp2)*invs;

	return mat;
}

// just in case you need that function also
Quaternion Quaternion::createFromAxisAngle(Vector3f axis, float angle) const {
	float halfAngle = angle * 0.5f;
	float s = (float)sin(halfAngle);
	Quaternion q;
	q.x = axis.x * s;
	q.y = axis.y * s;
	q.z = axis.z * s;
	q.w = (float)cos(halfAngle);
	return q;
}

Quaternion Quaternion::lookAt(Vector3f sourcePoint, Vector3f destPoint, Vector3f up) const {
	vec3 forward = vec3(0.f, 0.f, 1.f);
	vec3 forwardVector = (destPoint - sourcePoint).normalized();

	float dot = forward.dot(forwardVector);

	if (abs(dot - (-1.f)) < 0.000001f){
		return Quaternion(3.1415926535897932f, up);
	}
	if (abs(dot - (1.f)) < 0.000001f){
		return Quaternion(1.0f, 0.f);
	}

	float rotAngle = (float)acos(dot);
	vec3 rotAxis = forward.cross(forwardVector);
	rotAxis = rotAxis.normalized();

	return createFromAxisAngle(rotAxis, rotAngle);
}

Quaternion Quaternion::fromEuler(float x, float y, float z) const {
	/*float c1 = cos(x / 2.0f);
	float s1 = sin(x / 2.0f);
	float c2 = cos(y / 2.0f);
	float s2 = sin(y / 2.0f);
	float c3 = cos(z / 2.0f);
	float s3 = sin(z / 2.0f);
	float c1c2 = c1 * c2;
	float s1s2 = s1 * s2;

	Quaternion q;
	q.w = c1c2 * c3 - s1s2 * s3;
	q.x = c1c2 * s3 + s1s2 * c3;
	q.y = s1 * c2*c3 + c1 * s2*s3;
	q.z = c1 * s2*c3 - s1 * c2*s3;
	return q;*/

	// Assuming the angles are in radians.
	float c1 = cos(x);
	float s1 = sin(x);
	float c2 = cos(y);
	float s2 = sin(y);
	float c3 = cos(z);
	float s3 = sin(z);
	Quaternion q;
	q.w = sqrt(1.0f + c1 * c2 + c1 * c3 - s1 * s2 * s3 + c2 * c3) / 2.0f;
	float w4 = (4.0f * q.w);
	q.x = (c2 * s3 + c1 * s3 + s1 * s2 * c3) / w4;
	q.y = (s1 * c2 + s1 * c3 + c1 * s2 * s3) / w4;
	q.z = (-s1 * s3 + c1 * s2 * c3 + s2) / w4;
	return q;
	
}

Vector3f Quaternion::toEuler()  const {
	Vector3f v;

	float sqw = w*w;
	float sqx = x*x;
	float sqy = y*y;
	float sqz = z*z;
	float unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
	float test = x*y + z*w;
	if (test > 0.499*unit) { // singularity at north pole
		v.x = 2.0f * atan2(x, w);
		v.y = cst::PI / 2.0f;
		v.z = 0.0f;
		return v;
	}
	else if (test < -0.499*unit) { // singularity at south pole
		v.x = -2.0f * atan2(x, w);
		v.y = -cst::PI / 2.0f;
		v.z = 0.0f;
		return v;
	}
	
	v.x = atan2(2.0f * y*w - 2.0f * x*z, sqx - sqy - sqz + sqw);
	v.y = asin(2.0f * test / unit);
	v.z = atan2(2.0f * x*w - 2.0f * y*z, -sqx + sqy - sqz + sqw);

	return v;
}

void Quaternion::setX(float angle) {
	x = util::toRadians(angle);
}

void Quaternion::setY(float angle) {
	y = util::toRadians(angle);
}

void Quaternion::setZ(float angle) {
	z = util::toRadians(angle);
}


float Quaternion::getX()  const {
	return util::toDegrees(x);
}

float Quaternion::getY() const {
	return util::toDegrees(y);
}

float Quaternion::getZ() const {
	return util::toDegrees(z);
}


float Quaternion::getAngle() const {
	Quaternion norm = *this;
	if (w > 1.0f) norm = normalized();
	return 2.0f * acos(norm.w);
}

Vector3f Quaternion::getAxis() const {
	Quaternion norm = *this;
	if(w > 1.0f) norm = normalized();

	Vector3f v;

	double s = sqrt(1.0f - w*w); // assuming quaternion normalised then w is less than 1, so term always positive.
	if (s < 0.001f) { // test to avoid divide by zero, s is always positive due to sqrt
	  // if s close to zero then direction of axis not important
		v.x = norm.x; // if it is important that axis is normalised then replace with x=1; y=z=0;
		v.y = norm.y;
		v.z = norm.z;
	}
	else {
		v.x = norm.x / s; // normalise axis
		v.y = norm.y / s;
		v.z = norm.z / s;
	}

	return v;
}


std::string Quaternion::toStringRadians() const {
	std::ostringstream ss;
	ss << "(" << x << ", " << y << ", " << z << ", " << w << ")";
	return ss.str();
}

std::string Quaternion::toString() const {
	std::ostringstream ss;
	ss << "(" << getX() << ", " << getY() << ", " << getZ() << ", " << w << ")";
	return ss.str();
}


Quaternion::Quaternion(){

}

Quaternion::Quaternion(vec3 const& v) {
	this->x = getTheta(v.x);
	this->y = getTheta(v.y);
	this->z = getTheta(v.z);
	this->w = 1.0f;
}

Quaternion::Quaternion(float angle, vec3 const& axis) {
	x = axis.x * sin(angle / 2.0f);
	y = axis.y * sin(angle / 2.0f);
	z = axis.z * sin(angle / 2.0f);
	w = cos(angle / 2.0f);
}

Quaternion::Quaternion(float x, float y, float z, float w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}


Quaternion::~Quaternion(){

}

mat4 operator*(mat4 const& m, Quaternion const& q) {
	return m * q.toMatrix();
}

std::ostream& operator<<(std::ostream& os, Quaternion const& q) {
	os << "(" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << ")";
	return os;
}

std::wostream& operator<<(std::wostream& os, Quaternion const& q) {
	os << L"(" << q.x << L", " << q.y << L", " << q.z << L", " << q.w << L")";
	return os;
}
