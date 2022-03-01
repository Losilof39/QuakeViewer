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

static void MultiplyMatrixVector(vec3_t& i, vec3_t& o, mat4& m)
{
	o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
	o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
	o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
	float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

	if (w != 0.0f)
	{
		o.x /= w; o.y /= w; o.z /= w;
	}
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

static mat4 MakeViewMatrix()
{

}