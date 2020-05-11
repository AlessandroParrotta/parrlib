#include <parrlib/Matrix4f.h>

#include <parrlib/OtherUtil.h>
#include <parrlib/stringutils.h>
#include <parrlib/constants.h>

//Matrix4f Matrix4f::identity() {
//	float matrix[4][4];
//	for (int i = 0; i < 4; i++) {
//		for (int j = 0; j < 4; j++) {
//			if (i == j) matrix[i][j] = 1.f;
//			else		matrix[i][j] = 0.f;
//		}
//	}
//	return Matrix4f(matrix);
//}

Matrix4f Matrix4f::operator+ (const Matrix4f &other) const {
	Matrix4f result = Matrix4f();

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.matrix[i][j] = this->matrix[i][j] + other.matrix[i][j];
		}
	}

	return result;
}

Matrix4f Matrix4f::operator- (const Matrix4f &other) const {
	Matrix4f result = Matrix4f();

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.matrix[i][j] = this->matrix[i][j] - other.matrix[i][j];
		}
	}

	return result;
}

Matrix4f Matrix4f::operator* (const Matrix4f &other) const {
	Matrix4f result = Matrix4f();

	//for (int i = 0; i < 4; i++) {
	//	for (int j = 0; j < 4; j++) {
	//		//result.matrix[i][j] =	this->matrix[i][0] * other.matrix[0][j] +
	//		//						this->matrix[i][1] * other.matrix[1][j] +
	//		//						this->matrix[i][2] * other.matrix[2][j] +
	//		//						this->matrix[i][3] * other.matrix[3][j];
	//		result.matrix[i][j] = 0.f;
	//		for (int k = 0; k < 4; k++) {
	//			result.matrix[i][j] += this->matrix[i][k] * other.matrix[k][j];
	//		}
	//	}
	//}
	
	//inverted order, to sync behaviour with GLM
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			//result.matrix[i][j] =	this->matrix[i][0] * other.matrix[0][j] +
			//						this->matrix[i][1] * other.matrix[1][j] +
			//						this->matrix[i][2] * other.matrix[2][j] +
			//						this->matrix[i][3] * other.matrix[3][j];
			result.matrix[i][j] = 0.f;
			for (int k = 0; k < 4; k++) {
				result.matrix[i][j] += other.matrix[i][k] * this->matrix[k][j];
			}
		}
	}

	//Result[0]
	//row major
	//result.matrix[0][0] = (this->matrix[0][0] * other.matrix[0][0] + this->matrix[1][0] * other.matrix[0][1] + this->matrix[2][0] * other.matrix[0][2] + this->matrix[3][0] * other.matrix[0][3]);
	//result.matrix[0][1] = (this->matrix[0][1] * other.matrix[0][0] + this->matrix[1][1] * other.matrix[0][1] + this->matrix[2][1] * other.matrix[0][2] + this->matrix[3][1] * other.matrix[0][3]);
	//result.matrix[0][2] = (this->matrix[0][2] * other.matrix[0][0] + this->matrix[1][2] * other.matrix[0][1] + this->matrix[2][2] * other.matrix[0][2] + this->matrix[3][2] * other.matrix[0][3]);
	//result.matrix[0][3] = (this->matrix[0][3] * other.matrix[0][0] + this->matrix[1][3] * other.matrix[0][1] + this->matrix[2][3] * other.matrix[0][2] + this->matrix[3][3] * other.matrix[0][3]);
	
	//column major
	//result.matrix[0][0] = (this->matrix[0][0] * other.matrix[0][0] + this->matrix[0][1] * other.matrix[1][0] + this->matrix[0][2] * other.matrix[2][0] + this->matrix[0][3] * other.matrix[3][0]);
	//result.matrix[1][0] = (this->matrix[1][0] * other.matrix[0][0] + this->matrix[1][1] * other.matrix[1][0] + this->matrix[1][2] * other.matrix[2][0] + this->matrix[1][3] * other.matrix[3][0]);
	//result.matrix[2][0] = (this->matrix[2][0] * other.matrix[0][0] + this->matrix[2][1] * other.matrix[1][0] + this->matrix[2][2] * other.matrix[2][0] + this->matrix[2][3] * other.matrix[3][0]);
	//result.matrix[3][0] = (this->matrix[3][0] * other.matrix[0][0] + this->matrix[3][1] * other.matrix[1][0] + this->matrix[3][2] * other.matrix[2][0] + this->matrix[3][3] * other.matrix[3][0]);
	

	
	return result;
}

Vector4f Matrix4f::operator* (const Vector4f &other) const {
	//float tOther[4] = {other.x, other.y, other.z, other.w};
	//float result[4] = {0.0f,0.0f,0.0f,0.0f};

	//for (int i = 0; i < 4; i++) {
	//	for (int j = 0; j < 4; j++) {
	//		result[i] += matrix[i][j]*tOther[j];
	//	}
	//}

	//return Vector4f(result[0], result[1], result[2], result[3]);

	vec4 ris;

	//ris = {
	//	other.x * this->matrix[0][0] + other.y * this->matrix[0][1] + other.z * this->matrix[0][2] + other.w * this->matrix[0][3],
	//	other.x * this->matrix[1][0] + other.y * this->matrix[1][1] + other.z * this->matrix[1][2] + other.w * this->matrix[1][3],
	//	other.x * this->matrix[2][0] + other.y * this->matrix[2][1] + other.z * this->matrix[2][2] + other.w * this->matrix[2][3],
	//	other.x * this->matrix[3][0] + other.y * this->matrix[3][1] + other.z * this->matrix[3][2] + other.w * this->matrix[3][3],
	//};
	
	ris = {
		other.x * this->matrix[0][0] + other.y * this->matrix[1][0] + other.z * this->matrix[2][0] + other.w * this->matrix[3][0],
		other.x * this->matrix[0][1] + other.y * this->matrix[1][1] + other.z * this->matrix[2][1] + other.w * this->matrix[3][1],
		other.x * this->matrix[0][2] + other.y * this->matrix[1][2] + other.z * this->matrix[2][2] + other.w * this->matrix[3][2],
		other.x * this->matrix[0][3] + other.y * this->matrix[1][3] + other.z * this->matrix[2][3] + other.w * this->matrix[3][3],
	};

	return ris;
}

Vector4f Matrix4f::operator* (const Vector3f& v) const { return *this * vec4(v, 1.f); }

Matrix4f Matrix4f::operator* (const float& f) const {
	Matrix4f res;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			res[i][j] *= f;
		}
	}

	return res;
}


Matrix4f Matrix4f::transposed()const {
	Matrix4f m;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m.matrix[i][j] = matrix[j][i];
		}
	}
	return m;
}

//works for matrices NxX, just replace 4 with N
// Function to get cofactor of A[p][q] in temp[][]. n is current 
// dimension of A[][] 
void Matrix4f::getCofactor(float A[4][4], float temp[4][4], int p, int q, int n)
{
	int i = 0, j = 0;

	// Looping for each element of the matrix 
	for (int row = 0; row < n; row++)
	{
		for (int col = 0; col < n; col++)
		{
			//  Copying into temporary matrix only those element 
			//  which are not in given row and column 
			if (row != p && col != q)
			{
				temp[i][j++] = A[row][col];

				// Row is filled, so increase row index and 
				// reset col index 
				if (j == n - 1)
				{
					j = 0;
					i++;
				}
			}
		}
	}
}

/* Recursive function for finding determinant of matrix.
   n is current dimension of A[][]. */
int Matrix4f::determinant(float A[4][4], int n)
{
	int D = 0; // Initialize result 

	//  Base case : if matrix contains single element 
	if (n == 1)
		return A[0][0];

	float temp[4][4]; // To store cofactors 

	int sign = 1;  // To store sign multiplier 

	 // Iterate for each element of first row 
	for (int f = 0; f < n; f++)
	{
		// Getting Cofactor of A[0][f] 
		getCofactor(A, temp, 0, f, n);
		D += sign * A[0][f] * determinant(temp, n - 1);

		// terms are to be added with alternate sign 
		sign = -sign;
	}

	return D;
}

// Function to get adjoint of A[N][N] in adj[N][N]. 
void Matrix4f::adjoint(float A[4][4], float adj[4][4])
{
	if (4 == 1)
	{
		adj[0][0] = 1;
		return;
	}

	// temp is used to store cofactors of A[][] 
	int sign = 1;
	float temp[4][4];

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			// Get cofactor of A[i][j] 
			getCofactor(A, temp, i, j, 4);

			// sign of adj[j][i] positive if sum of row 
			// and column indexes is even. 
			sign = ((i + j) % 2 == 0) ? 1 : -1;

			// Interchanging rows and columns to get the 
			// transpose of the cofactor matrix 
			adj[j][i] = (sign) * (determinant(temp, 4 - 1));
		}
	}
}

Matrix4f Matrix4f::inverted()const {
	//from https://github.com/g-truc/glm/blob/master/glm/detail/func_matrix.inl

	float Coef00 = this->matrix[2][2] * this->matrix[3][3] - this->matrix[3][2] * this->matrix[2][3];
	float Coef02 = this->matrix[1][2] * this->matrix[3][3] - this->matrix[3][2] * this->matrix[1][3];
	float Coef03 = this->matrix[1][2] * this->matrix[2][3] - this->matrix[2][2] * this->matrix[1][3];

	float Coef04 = this->matrix[2][1] * this->matrix[3][3] - this->matrix[3][1] * this->matrix[2][3];
	float Coef06 = this->matrix[1][1] * this->matrix[3][3] - this->matrix[3][1] * this->matrix[1][3];
	float Coef07 = this->matrix[1][1] * this->matrix[2][3] - this->matrix[2][1] * this->matrix[1][3];

	float Coef08 = this->matrix[2][1] * this->matrix[3][2] - this->matrix[3][1] * this->matrix[2][2];
	float Coef10 = this->matrix[1][1] * this->matrix[3][2] - this->matrix[3][1] * this->matrix[1][2];
	float Coef11 = this->matrix[1][1] * this->matrix[2][2] - this->matrix[2][1] * this->matrix[1][2];

	float Coef12 = this->matrix[2][0] * this->matrix[3][3] - this->matrix[3][0] * this->matrix[2][3];
	float Coef14 = this->matrix[1][0] * this->matrix[3][3] - this->matrix[3][0] * this->matrix[1][3];
	float Coef15 = this->matrix[1][0] * this->matrix[2][3] - this->matrix[2][0] * this->matrix[1][3];

	float Coef16 = this->matrix[2][0] * this->matrix[3][2] - this->matrix[3][0] * this->matrix[2][2];
	float Coef18 = this->matrix[1][0] * this->matrix[3][2] - this->matrix[3][0] * this->matrix[1][2];
	float Coef19 = this->matrix[1][0] * this->matrix[2][2] - this->matrix[2][0] * this->matrix[1][2];

	float Coef20 = this->matrix[2][0] * this->matrix[3][1] - this->matrix[3][0] * this->matrix[2][1];
	float Coef22 = this->matrix[1][0] * this->matrix[3][1] - this->matrix[3][0] * this->matrix[1][1];
	float Coef23 = this->matrix[1][0] * this->matrix[2][1] - this->matrix[2][0] * this->matrix[1][1];

	vec4 Fac0(Coef00, Coef00, Coef02, Coef03);
	vec4 Fac1(Coef04, Coef04, Coef06, Coef07);
	vec4 Fac2(Coef08, Coef08, Coef10, Coef11);
	vec4 Fac3(Coef12, Coef12, Coef14, Coef15);
	vec4 Fac4(Coef16, Coef16, Coef18, Coef19);
	vec4 Fac5(Coef20, Coef20, Coef22, Coef23);

	vec4 Vec0(this->matrix[1][0], this->matrix[0][0], this->matrix[0][0], this->matrix[0][0]);
	vec4 Vec1(this->matrix[1][1], this->matrix[0][1], this->matrix[0][1], this->matrix[0][1]);
	vec4 Vec2(this->matrix[1][2], this->matrix[0][2], this->matrix[0][2], this->matrix[0][2]);
	vec4 Vec3(this->matrix[1][3], this->matrix[0][3], this->matrix[0][3], this->matrix[0][3]);

	vec4 Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
	vec4 Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
	vec4 Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
	vec4 Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

	vec4 SignA(+1, -1, +1, -1);
	vec4 SignB(-1, +1, -1, +1);
	Matrix4f Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

	//vec4 Row0(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

	//vec4 Dot0(this->matrix[0][0] * Row0.x, this->matrix[0][1] * Row0.y, this->matrix[0][2] * Row0.z, this->matrix[0][3] * Row0.w);
	//float Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

	//float OneOverDeterminant = 1.f / Dot1;

	//std::cout << "INV" << Inverse << "\n";

	//Matrix4f inv = Inverse * OneOverDeterminant;
	//std::cout << "INVERTED" << inv << "\n";

	return Inverse;
}

//Matrix4f Matrix4f::inverted() {
//	float t[4][4];
//	inverse(matrix, t);
//	return Matrix4f(t);
//}

void Matrix4f::setupIdentity() {
	matrix[0][0] = 1.0f;	matrix[0][1] = 0.0f;	matrix[0][2] = 0.0f;	matrix[0][3] = 0.0f;
	matrix[1][0] = 0.0f;	matrix[1][1] = 1.0f;	matrix[1][2] = 0.0f;	matrix[1][3] = 0.0f;
	matrix[2][0] = 0.0f;	matrix[2][1] = 0.0f;	matrix[2][2] = 1.0f;	matrix[2][3] = 0.0f;
	matrix[3][0] = 0.0f;	matrix[3][1] = 0.0f;	matrix[3][2] = 0.0f;	matrix[3][3] = 1.0f;
}

void Matrix4f::setupTranslation(float x, float y, float z) {
	matrix[0][0] = 1.0f;	matrix[0][1] = 0.0f;	matrix[0][2] = 0.0f;	matrix[0][3] = x;
	matrix[1][0] = 0.0f;	matrix[1][1] = 1.0f;	matrix[1][2] = 0.0f;	matrix[1][3] = y;
	matrix[2][0] = 0.0f;	matrix[2][1] = 0.0f;	matrix[2][2] = 1.0f;	matrix[2][3] = z;
	matrix[3][0] = 0.0f;	matrix[3][1] = 0.0f;	matrix[3][2] = 0.0f;	matrix[3][3] = 1.0f;
}

void Matrix4f::setupRotation(float x, float y, float z) {
	Matrix4f rx = Matrix4f();
	Matrix4f ry = Matrix4f();
	Matrix4f rz = Matrix4f();

	x = outl::toRadians(x);
	y = outl::toRadians(y);
	z = outl::toRadians(z);

	rx.matrix[0][0] = 1.0f;		rx.matrix[0][1] = 0.0f;		rx.matrix[0][2] = 0.0f;		rx.matrix[0][3] = 0.0f;
	rx.matrix[1][0] = 0.0f;		rx.matrix[1][1] = cos(x);	rx.matrix[1][2] = -sin(x);	rx.matrix[1][3] = 0.0f;
	rx.matrix[2][0] = 0.0f;		rx.matrix[2][1] = sin(x);	rx.matrix[2][2] = cos(x);	rx.matrix[2][3] = 0.0f;
	rx.matrix[3][0] = 0.0f;		rx.matrix[3][1] = 0.0f;		rx.matrix[3][2] = 0.0f;		rx.matrix[3][3] = 1.0f;

	ry.matrix[0][0] = cos(y);	ry.matrix[0][1] = 0.0f;		ry.matrix[0][2] = -sin(y);	ry.matrix[0][3] = 0.0f;
	ry.matrix[1][0] = 0.0f;		ry.matrix[1][1] = 1.0f;		ry.matrix[1][2] = 0.0f;		ry.matrix[1][3] = 0.0f;
	ry.matrix[2][0] = sin(y);	ry.matrix[2][1] = 0.0f;		ry.matrix[2][2] = cos(y);	ry.matrix[2][3] = 0.0f;
	ry.matrix[3][0] = 0.0f;		ry.matrix[3][1] = 0.0f;		ry.matrix[3][2] = 0.0f;		ry.matrix[3][3] = 1.0f;

	rz.matrix[0][0] = cos(z);	rz.matrix[0][1] = -sin(z);	rz.matrix[0][2] = 0.0f;		rz.matrix[0][3] = 0.0f;
	rz.matrix[1][0] = sin(z);	rz.matrix[1][1] = cos(z);	rz.matrix[1][2] = 0.0f;		rz.matrix[1][3] = 0.0f;
	rz.matrix[2][0] = 0.0f;		rz.matrix[2][1] = 0.0f;		rz.matrix[2][2] = 1.0f;		rz.matrix[2][3] = 0.0f;
	rz.matrix[3][0] = 0.0f;		rz.matrix[3][1] = 0.0f;		rz.matrix[3][2] = 0.0f;		rz.matrix[3][3] = 1.0f;

	Matrix4f result = rz*rx*ry;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			matrix[i][j] = result.matrix[i][j];
		}
	}
}

void Matrix4f::setupRotationAxis(float angle, vec3 const& axis) {
	float u = axis.x, v = axis.y, w = axis.z;
	float L = (u * u + v * v + w * w);
	angle = angle * cst::PI / 180.0; //converting to radian value
	float u2 = u * u;
	float v2 = v * v;
	float w2 = w * w;

	matrix[0][0] = (u2 + (v2 + w2) * cos(angle)) / L;
	matrix[0][1] = (u * v * (1 - cos(angle)) - w * sqrt(L) * sin(angle)) / L;
	matrix[0][2] = (u * w * (1 - cos(angle)) + v * sqrt(L) * sin(angle)) / L;
	matrix[0][3] = 0.0;

	matrix[1][0] = (u * v * (1 - cos(angle)) + w * sqrt(L) * sin(angle)) / L;
	matrix[1][1] = (v2 + (u2 + w2) * cos(angle)) / L;
	matrix[1][2] = (v * w * (1 - cos(angle)) - u * sqrt(L) * sin(angle)) / L;
	matrix[1][3] = 0.0;

	matrix[2][0] = (u * w * (1 - cos(angle)) - v * sqrt(L) * sin(angle)) / L;
	matrix[2][1] = (v * w * (1 - cos(angle)) + u * sqrt(L) * sin(angle)) / L;
	matrix[2][2] = (w2 + (u2 + v2) * cos(angle)) / L;
	matrix[2][3] = 0.0;

	matrix[3][0] = 0.0;
	matrix[3][1] = 0.0;
	matrix[3][2] = 0.0;
	matrix[3][3] = 1.0;
}

void Matrix4f::setupScale(float x, float y, float z) {
	matrix[0][0] = x;		matrix[0][1] = 0.0f;	matrix[0][2] = 0.0f;	matrix[0][3] = 0.0f;
	matrix[1][0] = 0.0f;	matrix[1][1] = y;		matrix[1][2] = 0.0f;	matrix[1][3] = 0.0f;
	matrix[2][0] = 0.0f;	matrix[2][1] = 0.0f;	matrix[2][2] = z;		matrix[2][3] = 0.0f;
	matrix[3][0] = 0.0f;	matrix[3][1] = 0.0f;	matrix[3][2] = 0.0f;	matrix[3][3] = 1.0f;
}

void Matrix4f::setupTranslation(Vector3f const& trasl) {
	setupTranslation(trasl.x, trasl.y, trasl.z);
}

void Matrix4f::setupRotation(Vector3f const& rot) {
	setupRotation(rot.x, rot.y, rot.z);
}

void Matrix4f::setupScale(Vector3f const& scale) {
	setupScale(scale.x, scale.y, scale.z);
}

void Matrix4f::setupPerspective(float fov, float aspect, float znear, float zfar) {
	float D2R = cst::PI / 180.0f;
	float yScale = 1.0f / tan(D2R * fov / 2.0f);
	float xScale = yScale / aspect;
	float nearmfar = znear - zfar;

	matrix[0][0] = xScale;	matrix[0][1] = 0.0f;	matrix[0][2] = 0.0f;						matrix[0][3] = 0.0f;
	matrix[1][0] = 0.0f;	matrix[1][1] = yScale;	matrix[1][2] = 0.0f;						matrix[1][3] = 0.0f;
	matrix[2][0] = 0.0f;	matrix[2][1] = 0.0f;	matrix[2][2] = (zfar + znear) / nearmfar;		matrix[2][3] = -1.0f;
	matrix[3][0] = 0.0f;	matrix[3][1] = 0.0f;	matrix[3][2] = 2.0f * zfar*znear / nearmfar;	matrix[3][3] = 0.0f;

	//float ar = MyContext::orthoAspect==0.0f? ((float)constants::WIDTH / (float)constants::HEIGHT) : MyContext::orthoAspect;
	//float tanHalfFOV = (float)tan(util::toRadians(fov / 2.0f));
	//float zRange = znear - zfar;

	//matrix[0][0] = 1.0f / (tanHalfFOV  * ar);		matrix[0][1] = 0.0f;					matrix[0][2] = 0.0f;						matrix[0][3] = 0.0f;
	//matrix[1][0] = 0.0f;							matrix[1][1] = 1.0f / tanHalfFOV;		matrix[1][2] = 0.0f;						matrix[1][3] = 0.0f;
	//matrix[2][0] = 0.0f;							matrix[2][1] = 0.0f;					matrix[2][2] = (-znear - zfar) / zRange;	matrix[2][3] = 2.0f * zfar * znear / zRange;
	//matrix[3][0] = 0.0f;							matrix[3][1] = 0.0f;					matrix[3][2] = 1.0f;						matrix[3][3] = 0.0f;

	/*float m[] = {
		xScale, 0.0f,	0.0f,						0.0f,
		0.0f,	yScale, 0.0f,						0.0f,
		0.0f,	0.0f,	(far + near) / nearmfar,	-1.0f,
		0.0f,	0.0f,	2.0f * far*near / nearmfar, 0.0f
	};*/
}

void Matrix4f::setupCamera(Vector3f const& forward, Vector3f const& up) {
	Vector3f f = forward;
	//f = f.normalized();

	Vector3f r = up;
	//r = r.normalized();
	r = r.cross(f);

	Vector3f u = f.cross(r);

	matrix[0][0] = r.x;		matrix[0][1] = r.y;		matrix[0][2] = r.z;		matrix[0][3] = 0.0f;
	matrix[1][0] = u.x;		matrix[1][1] = u.y;		matrix[1][2] = u.z;		matrix[1][3] = 0.0f;
	matrix[2][0] = f.x;		matrix[2][1] = f.y;		matrix[2][2] = f.z;		matrix[2][3] = 0.0f;
	matrix[3][0] = 0.0f;	matrix[3][1] = 0.0f;	matrix[3][2] = 0.0f;	matrix[3][3] = 1.0f;
}

vec3 Matrix4f::toEuler() const {
	float sy = sqrt(matrix[0][0] * matrix[0][0] + matrix[1][0] * matrix[1][0]);

	bool singular = sy < 1e-6f; // If

	float x, y, z;
	if (!singular){
		x = atan2(matrix[2][1], matrix[2][2]);
		y = atan2(-matrix[2][0], sy);
		z = atan2(matrix[1][0], matrix[0][0]);
	}
	else{
		x = atan2(-matrix[1][2], matrix[1][1]);
		y = atan2(-matrix[2][0], sy);
		z = 0.f;
	}
	return vec3(x, y, z);
}


vec3 Matrix4f::translationv() const {
	return vec3(
		matrix[3][0],
		matrix[3][1],
		matrix[3][2]
	);
}
vec3 Matrix4f::scalingv() const {
	return vec3(
		vec3(matrix[0][0], matrix[0][1], matrix[0][2]).magnitude(),
		vec3(matrix[1][0], matrix[1][1], matrix[1][2]).magnitude(),
		vec3(matrix[2][0], matrix[2][1], matrix[2][2]).magnitude()
	);
}

Matrix4f Matrix4f::translation() const{
	Matrix4f res = 1.f;
	res[3][0] = matrix[3][0];
	res[3][1] = matrix[3][1];
	res[3][2] = matrix[3][2];
	return res;
}

Matrix4f Matrix4f::rotation() const {
	vec3 sc = scalingv();
	Matrix4f res = 1.f;
	res[0][0] = matrix[0][0] / sc.x;
	res[0][1] = matrix[0][1] / sc.y;
	res[0][2] = matrix[0][2] / sc.z;

	res[1][0] = matrix[1][0] / sc.x;
	res[1][1] = matrix[1][1] / sc.y;
	res[1][2] = matrix[1][2] / sc.z;
	
	res[2][0] = matrix[2][0] / sc.x;
	res[2][1] = matrix[2][1] / sc.y;
	res[2][2] = matrix[2][2] / sc.z;

	//res[0][0] = matrix[0][0] / sc.x;
	//res[1][0] = matrix[1][0] / sc.y;
	//res[2][0] = matrix[2][0] / sc.z;

	//res[0][1] = matrix[0][1] / sc.x;
	//res[1][1] = matrix[1][1] / sc.y;
	//res[2][1] = matrix[2][1] / sc.z;
	//
	//res[0][2] = matrix[0][2] / sc.x;
	//res[1][2] = matrix[1][2] / sc.y;
	//res[2][2] = matrix[2][2] / sc.z;

	return res;
}

Matrix4f Matrix4f::scaling() const {
	Matrix4f res = 1.f;
	res[0][0] = vec3(matrix[0][0], matrix[0][1], matrix[0][2]).magnitude();
	res[1][1] = vec3(matrix[1][0], matrix[1][1], matrix[1][2]).magnitude();
	res[2][2] = vec3(matrix[2][0], matrix[2][1], matrix[2][2]).magnitude();
	
	//res[0][0] = vec3(matrix[0][0], matrix[1][0], matrix[2][0]).magnitude();
	//res[1][1] = vec3(matrix[0][1], matrix[1][1], matrix[2][1]).magnitude();
	//res[2][2] = vec3(matrix[0][2], matrix[1][2], matrix[2][2]).magnitude();
	return res;
}


Matrix4f::Matrix4f(){

}

Matrix4f::Matrix4f(float matrix[4][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			this->matrix[i][j] = matrix[i][j];
		}
	}
}

//this lets me do mat4 mat = 1.f and get the identity matrix for example
Matrix4f::Matrix4f(float f) {
	//for (int i = 0; i < 4; i++) {
	//	for (int j = 0; j < 4; j++) {
	//		this->matrix[i][j] = f;
	//	}
	//}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			this->matrix[i][j] = 0.f;
		}
	}

	matrix[0][0] = f;
	matrix[1][1] = f;
	matrix[2][2] = f;
	matrix[3][3] = f;
}

Matrix4f::Matrix4f(Vector4f const& r0, Vector4f const& r1, Vector4f const& r2, Vector4f const& r3) {
	matrix[0][0] = r0.x; matrix[0][1] = r0.y; matrix[0][2] = r0.z; matrix[0][3] = r0.w;
	matrix[1][0] = r1.x; matrix[1][1] = r1.y; matrix[1][2] = r1.z; matrix[1][3] = r1.w;
	matrix[2][0] = r2.x; matrix[2][1] = r2.y; matrix[2][2] = r2.z; matrix[2][3] = r2.w;
	matrix[3][0] = r3.x; matrix[3][1] = r3.y; matrix[3][2] = r3.z; matrix[3][3] = r3.w;
}

Matrix4f::~Matrix4f(){

}

std::string Matrix4f::toString() const {
	std::stringstream ss;
	ss << "(\n";

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			ss << matrix[i][j];
			ss << " ";
		}
		ss << "\n";
	}
	ss << ")";
	return ss.str();
}
std::ostream& operator<<(std::ostream& os, const Matrix4f& mat) { return os << mat.toString(); }
std::wostream& operator<<(std::wostream& os, const Matrix4f& mat){ return os << stru::towstr(mat.toString()); }

namespace pmat4 {
	mat4 translate(vec3 const& v) {
		mat4 mat;
		mat.setupTranslation(v);
		mat = mat.transposed();
		return mat;
	}
	mat4 rotate(float angle, vec3 const& axis) {
		mat4 mat;
		mat.setupRotationAxis(angle, axis);
		mat = mat.transposed();
		return mat;
	}
	mat4 rotateEuler(vec3 const& rot) { return rotate(rot.x, v3ax::riv3) * rotate(rot.y, v3ax::upv3) * rotate(rot.z, v3ax::fwv3); }
	mat4 scale(vec3 const& v) {
		mat4 mat;
		mat.setupScale(v);
		return mat;
	}

	mat4 lookAt(vec3 const& eye, vec3 const& center, vec3 const& up) {
		//from https://github.com/g-truc/glm/blob/master/glm/ext/matrix_transform.inl
		//RH
		vec3 f = (center - eye).normalized();
		vec3 s = f.cross(up).normalized();
		vec3 u = s.cross(f);

		mat4 mat(1.f);
		mat[0][0] = s.x;
		mat[1][0] = s.y;
		mat[2][0] = s.z;
		mat[0][1] = u.x;
		mat[1][1] = u.y;
		mat[2][1] = u.z;
		mat[0][2] = -f.x;
		mat[1][2] = -f.y;
		mat[2][2] = -f.z;
		mat[3][0] = -s.dot(eye);
		mat[3][1] = -u.dot(eye);
		mat[3][2] = f.dot(eye);

		//LH
		//vec3 f = (center - eye).normalized();
		//vec3 s = up.cross(f).normalized();
		//vec3 u = f.cross(s);

		//mat4 mat(1.f);
		//mat[0][0] = s.x;
		//mat[1][0] = s.y;
		//mat[2][0] = s.z;
		//mat[0][1] = u.x;
		//mat[1][1] = u.y;
		//mat[2][1] = u.z;
		//mat[0][2] = f.x;
		//mat[1][2] = f.y;
		//mat[2][2] = f.z;
		//mat[3][0] = -s.dot(eye);
		//mat[3][1] = -u.dot(eye);
		//mat[3][2] = -f.dot(eye);

		return mat;
	}
	mat4 camera(vec3 const& forward, vec3 const& up) {
		mat4 mat;
		mat.setupCamera(forward, up);
		return mat;
	}
	mat4 perspective(float fov, float aspect, float znear, float zfar) {
		mat4 mat;
		mat.setupPerspective(fov, aspect, znear, zfar);
		//mat = mat.transposed();
		return mat;
	}

	mat4 ortho(float left, float right, float bottom, float top, float znear, float zfar) {
		//From https://github.com/g-truc/glm/blob/master/glm/ext/matrix_clip_space.inl

		//LH_ZO
		//mat4 mat(1.f);

		//mat[0][0] = 2.f / (right - left);
		//mat[1][1] = 2.f / (top - bottom);
		//mat[2][2] = 1.f / (zfar - znear);
		//mat[3][0] = -(right + left) / (right - left);
		//mat[3][1] = -(top + bottom) / (top - bottom);
		//mat[3][2] = -znear / (zfar - znear);

		//LH_NO
		//mat4 mat(1.f);
		//mat[0][0] = 2.f / (right - left);
		//mat[1][1] = 2.f / (top - bottom);
		//mat[2][2] = 2.f / (zfar - znear);
		//mat[3][0] = -(right + left) / (right - left);
		//mat[3][1] = -(top + bottom) / (top - bottom);
		//mat[3][2] = -(zfar + znear) / (zfar - znear);

		//RH_ZO
		//mat4 mat(1.f);
		//mat[0][0] = 2.f / (right - left);
		//mat[1][1] = 2.f / (top - bottom);
		//mat[2][2] = -1.f / (zfar - znear);
		//mat[3][0] = -(right + left) / (right - left);
		//mat[3][1] = -(top + bottom) / (top - bottom);
		//mat[3][2] = -znear / (zfar - znear);

		//RH_NO
		mat4 mat(1.f);
		mat[0][0] = 2.f / (right - left);
		mat[1][1] = 2.f / (top - bottom);
		mat[2][2] = -2.f / (zfar - znear);
		mat[3][0] = -(right + left) / (right - left);
		mat[3][1] = -(top + bottom) / (top - bottom);
		mat[3][2] = -(zfar + znear) / (zfar - znear);

		return mat;
	}

	mat4 inverse(mat4 const& m) {
		//from https://github.com/g-truc/glm/blob/master/glm/detail/func_matrix.inl

		float Coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
		float Coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
		float Coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

		float Coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
		float Coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
		float Coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

		float Coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
		float Coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
		float Coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

		float Coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
		float Coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
		float Coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

		float Coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
		float Coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
		float Coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

		float Coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
		float Coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
		float Coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

		vec4 Fac0(Coef00, Coef00, Coef02, Coef03);
		vec4 Fac1(Coef04, Coef04, Coef06, Coef07);
		vec4 Fac2(Coef08, Coef08, Coef10, Coef11);
		vec4 Fac3(Coef12, Coef12, Coef14, Coef15);
		vec4 Fac4(Coef16, Coef16, Coef18, Coef19);
		vec4 Fac5(Coef20, Coef20, Coef22, Coef23);

		vec4 Vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
		vec4 Vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
		vec4 Vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
		vec4 Vec3(m[1][3], m[0][3], m[0][3], m[0][3]);

		vec4 Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
		vec4 Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
		vec4 Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
		vec4 Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

		vec4 SignA(+1, -1, +1, -1);
		vec4 SignB(-1, +1, -1, +1);
		Matrix4f Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

		vec4 Row0(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

		vec4 Dot0(m[0][0] * Row0.x, m[0][1] * Row0.y, m[0][2] * Row0.z, m[0][3] * Row0.w);
		float Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

		float OneOverDeterminant = 1.f / Dot1;

		return Inverse * OneOverDeterminant;
	}
}

//MAPPING
//
/*

a e i 0
b f j 0
c g k 0
d h l 1

a(0,0) e(0,1) i(0,2) 0
b(1,0) f(1,1) j(1,2) 0
c(2,0) g(2,1) k(2,2) 0
d(3,0) h(3,1) l(3,2) 1

*/
