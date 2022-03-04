#pragma once

typedef struct vec3_t
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 1.0f;
}vec3_t;

struct triangle
{
    vec3_t p[3];
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

	o.x = i.x * m.m[0][0] + i.y * m.m[0][1] + i.z * m.m[0][2] + i.w * m.m[0][3];
	o.y = i.x * m.m[1][0] + i.y * m.m[1][1] + i.z * m.m[1][2] + i.w * m.m[1][3];
	o.z = i.x * m.m[2][0] + i.y * m.m[2][1] + i.z * m.m[2][2] + i.w * m.m[2][3];
	o.w = i.x * m.m[3][0] + i.y * m.m[3][1] + i.z * m.m[3][2] + i.w * m.m[3][3];

	return o;
}

static mat4 XRotationMatrix(float angle)
{
	mat4 xrot;

	xrot.m[0][0] = cosf(angle);
	xrot.m[0][1] = -sinf(angle);
	xrot.m[1][0] = sin(angle);
	xrot.m[1][1] = cos(angle);
	xrot.m[2][2] = 1.0f;
	xrot.m[3][3] = 1.0f;

	return xrot;
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

static mat4 ScreenSpaceMatrix(int half_width, int half_height)
{
	mat4 screenmat;

	screenmat.m[0][0] = half_width; screenmat.m[0][3] = half_width;
	screenmat.m[1][1] = -half_height; screenmat.m[1][3] = half_height;
	screenmat.m[3][3] = 1.0f;

	return screenmat;
}