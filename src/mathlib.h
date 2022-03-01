#pragma once

typedef struct vec3_t
{
    float x, y, z;
}vec3_t;

struct triangle
{
    vec3_t point[3];
};

struct mat4
{
    float m[4][4] = { 0 };
};

struct mesh
{
    std::vector<triangle> tris;
};

static vec3_t MultiplyMatrixVector(vec3_t& i, mat4& m)
{
	vec3_t o;

	o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
	o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
	o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
	float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

	if (w != 0.0f)
	{
		o.x /= w; o.y /= w; o.z /= w;
	}

	return o;
}

static vec3_t MulVector(vec3_t& i, float scalar)
{
	return { i.x * scalar, i.y * scalar, i.z * scalar };
}

static vec3_t AddVectors(vec3_t& a, vec3_t& b)
{
	return { a.x + b.x, a.y + b.y, a.z + b.z };
}

static vec3_t SubVectors(vec3_t& a, vec3_t& b)
{
	return { a.x - b.x, a.y - b.y, a.z - b.z };
}

static vec3_t NormalizeVector(vec3_t& i)
{
	float lenght = sqrtf(i.x * i.x + i.y * i.y + i.z * i.z);

	return { i.x /= lenght, i.y /= lenght, i.z /= lenght };
}

static float DotProduct(vec3_t& a, vec3_t& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

static vec3_t CrossProduct(vec3_t& a, vec3_t& b)
{
	vec3_t out;

	out.x = a.y * b.z - a.z * b.y;
	out.y = a.z * b.x - a.x * b.z;
	out.z = a.x * b.y - a.y * b.x;

	return out;
}

static mat4 Matrix_QuickInverse(mat4& m) // Only for Rotation/Translation Matrices
{
	mat4 matrix;
	matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
	matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
	matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
	matrix.m[3][3] = 1.0f;
	return matrix;
}

static mat4 MakeProjMatrix(float FOV, float aspect, float z_near, float z_far)
{
	mat4 proj;

	float FOV_rad = 1.0f / tanf(FOV * 0.5f / 180.0f * 3.14159f);

	proj.m[0][0] = aspect * FOV_rad;
	proj.m[1][1] = FOV_rad;
	proj.m[2][2] = z_far / (z_far - z_near);
	proj.m[3][2] = (-z_far * z_near) / (z_far - z_near);
	proj.m[2][3] = 1.0f;
	proj.m[3][3] = 0.0f;

	return proj;
}

static mat4 PointAt(vec3_t eye, vec3_t target, vec3_t up)
{
	// Calculate new forward direction
	vec3_t newForward = SubVectors(target, eye);
	newForward = NormalizeVector(newForward);

	// Calculate new Up direction
	vec3_t a = MulVector(newForward, DotProduct(up, newForward));
	vec3_t newUp = SubVectors(up, a);
	newUp = NormalizeVector(newUp);

	// New Right direction is easy, its just cross product
	vec3_t newRight = CrossProduct(newUp, newForward);

	// Construct Dimensioning and Translation Matrix	
	mat4 matrix;
	matrix.m[0][0] = newRight.x;	matrix.m[0][1] = newRight.y;	matrix.m[0][2] = newRight.z;	matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = newUp.x;		matrix.m[1][1] = newUp.y;		matrix.m[1][2] = newUp.z;		matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = newForward.x;	matrix.m[2][1] = newForward.y;	matrix.m[2][2] = newForward.z;	matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = eye.x;			matrix.m[3][1] = eye.y;			matrix.m[3][2] = eye.z;			matrix.m[3][3] = 1.0f;
	return matrix;
}

static mat4 MakeViewMatrix(vec3_t eye, vec3_t at, vec3_t up)
{
	mat4 pointat = PointAt(eye, at, up);

	return Matrix_QuickInverse(pointat);
}

static void AdjustToView(triangle& tri, int width, int height)
{
	// Scale into view
	tri.point[0].x += 1.0f; tri.point[0].y += 1.0f;
	tri.point[1].x += 1.0f; tri.point[1].y += 1.0f;
	tri.point[2].x += 1.0f; tri.point[2].y += 1.0f;

	// at this point the coordinates are from 0 to 2, so we divide in half (to get 0-1) and scale it
	for (int i = 0; i < 3; i++)
	{
		tri.point[i].x *= 0.5f * (float)width;
		tri.point[i].y *= 0.5f * (float)height;
	}
	/*tri.point[1].x *= 0.5f * (float)width;
	tri.point[1].y *= 0.5f * (float)height;
	tri.point[2].x *= 0.5f * (float)width;
	tri.point[2].y *= 0.5f * (float)height;*/
}