#pragma once
#include <math.h>
#include <vector>
#include <fstream>
#include <strstream>

#define M_PI 3.14159265359f

typedef struct vec3_t
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
}vec3_t;

typedef struct vec4_t
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 1.0f;
}vec4_t;

struct triangle
{
    vec4_t p[3];
};

// matrix[row][columns]
struct mat4
{
    float m[4][4] = { 0 };
};

struct mesh
{
    std::vector<triangle> tris;

	bool LoadFromObjectFile(std::string sFilename)
	{
		std::ifstream f(sFilename);
		if (!f.is_open())
			return false;

		// Local cache of verts
		std::vector<vec4_t> verts;

		while (!f.eof())
		{
			char line[128];
			f.getline(line, 128);

			std::strstream s;
			s << line;

			char junk;

			if (line[0] == 'v')
			{
				vec4_t v;
				s >> junk >> v.x >> v.y >> v.z;
				verts.push_back(v);
			}

			if (line[0] == 'f')
			{
				int f[3];
				s >> junk >> f[0] >> f[1] >> f[2];
				tris.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
			}
		}

		return true;
	}
};

static vec4_t MultiplyMatrixVector(vec4_t& i, mat4& m)
{
	vec4_t o;

	o.x = i.x * m.m[0][0] + i.y * m.m[0][1] + i.z * m.m[0][2] + i.w * m.m[0][3];
	o.y = i.x * m.m[1][0] + i.y * m.m[1][1] + i.z * m.m[1][2] + i.w * m.m[1][3];
	o.z = i.x * m.m[2][0] + i.y * m.m[2][1] + i.z * m.m[2][2] + i.w * m.m[2][3];
	o.w = i.x * m.m[3][0] + i.y * m.m[3][1] + i.z * m.m[3][2] + i.w * m.m[3][3];

	return o;
}

//static mat4 MultiplyMatrixVector(mat4& a, mat4& b)
//{
//	mat4 o;
//	
//	for (int i = 0; i < 4; i++)
//	{
//		for (int j = 0; j < 4; j++)
//		{
//			o.m[i][j] = a.m[]
//		}
//	}
//
//	return o;
//}

static vec3_t MulVector(vec3_t& i, float scalar)
{
	return { i.x * scalar, i.y * scalar, i.z * scalar };
}

static vec4_t MulVector(vec4_t& i, float scalar)
{
	return { i.x * scalar, i.y * scalar, i.z * scalar };
}

static vec3_t DivVector(vec3_t& i, float scalar)
{
	return { i.x / scalar, i.y / scalar, i.z / scalar };
}

static vec4_t DivVector(vec4_t& i, float scalar)
{
	return { i.x / scalar, i.y / scalar, i.z / scalar };
}

static vec3_t AddVectors(vec3_t& a, vec3_t& b)
{
	return { a.x + b.x, a.y + b.y, a.z + b.z };
}

static vec4_t AddVectors(vec4_t& a, vec4_t& b)
{
	return { a.x + b.x, a.y + b.y, a.z + b.z };
}

static vec3_t SubVectors(vec3_t& a, vec3_t& b)
{
	return { a.x - b.x, a.y - b.y, a.z - b.z };
}

static vec4_t SubVectors(vec4_t& a, vec4_t& b)
{
	return { a.x - b.x, a.y - b.y, a.z - b.z };
}

static vec3_t NormalizeVector(vec3_t& i)
{
	float lenght = sqrtf(i.x * i.x + i.y * i.y + i.z * i.z);

	return { i.x /= lenght, i.y /= lenght, i.z /= lenght };
}

static vec4_t NormalizeVector(vec4_t& i)
{
	float lenght = sqrtf(i.x * i.x + i.y * i.y + i.z * i.z);

	return { i.x /= lenght, i.y /= lenght, i.z /= lenght };
}

static float DotProduct(vec3_t& a, vec3_t& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

static float DotProduct(vec4_t& a, vec4_t& b)
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

static vec4_t CrossProduct(vec4_t& a, vec4_t& b)
{
	vec4_t out;

	out.x = a.y * b.z - a.z * b.y;
	out.y = a.z * b.x - a.x * b.z;
	out.z = a.x * b.y - a.y * b.x;

	return out;
}

static vec3_t Vector_IntersectPlane(vec3_t& plane_p, vec3_t& plane_n, vec3_t& lineStart, vec3_t& lineEnd)
{
	plane_n = NormalizeVector(plane_n);
	float plane_d = -DotProduct(plane_n, plane_p);
	float ad = DotProduct(lineStart, plane_n);
	float bd = DotProduct(lineEnd, plane_n);
	float t = (-plane_d - ad) / (bd - ad);
	vec3_t lineStartToEnd = SubVectors(lineEnd, lineStart);
	vec3_t lineToIntersect = MulVector(lineStartToEnd, t);
	return AddVectors(lineStart, lineToIntersect);
}

static mat4 IdentityMatrix(void)
{
	mat4 i;

	i.m[0][0] = 1.0f;
	i.m[1][1] = 1.0f;
	i.m[2][2] = 1.0f;
	i.m[3][3] = 1.0f;

	return i;
}

static mat4 ScaleMatrix(float scale)
{
	mat4 scale_mat4;

	scale_mat4.m[0][0] = scale;
	scale_mat4.m[1][1] = scale;
	scale_mat4.m[2][2] = scale;
	scale_mat4.m[3][3] = 1.0f;

	return scale_mat4;
}

static mat4 XRotationMatrix(float angle)
{
	mat4 zrot;

	zrot.m[0][0] = 1.0f;
	zrot.m[1][1] = cosf(angle);
	zrot.m[1][2] = -sinf(angle);
	zrot.m[2][1] = sinf(angle);
	zrot.m[2][2] = cosf(angle);
	zrot.m[3][3] = 1.0f;

	return zrot;
}

static mat4 ZRotationMatrix(float angle)
{
	mat4 xrot;

	xrot.m[0][0] = cosf(angle);
	xrot.m[0][1] = -sinf(angle);
	xrot.m[1][0] = sinf(angle);
	xrot.m[1][1] = cosf(angle);
	xrot.m[2][2] = 1.0f;
	xrot.m[3][3] = 1.0f;

	return xrot;
}

static mat4 YRotationMatrix(float angle)
{
	mat4 yrot;

	yrot.m[0][0] = cosf(angle);
	yrot.m[0][2] = sinf(angle);
	yrot.m[1][1] = 1.0f;
	yrot.m[2][0] = -sinf(angle);
	yrot.m[2][2] = cosf(angle);
	yrot.m[3][3] = 1.0f;

	return yrot;
}

static mat4 LookAt(vec3_t& eye, vec3_t& at, vec3_t& up)
{
	vec3_t diff = SubVectors(at, eye);
	vec3_t zaxis = NormalizeVector(diff);

	vec3_t crossed = CrossProduct(zaxis, up);

	vec3_t xaxis = NormalizeVector(crossed);
	vec3_t yaxis = CrossProduct(xaxis, zaxis);
	
	zaxis = MulVector(zaxis, -1.0f);
	
	mat4 viewMatrix;

	viewMatrix = {
		xaxis.x, xaxis.y, xaxis.z, -DotProduct(xaxis, eye),
		yaxis.x, yaxis.y, yaxis.z, -DotProduct(yaxis, eye),
		zaxis.x, zaxis.y, zaxis.z, -DotProduct(zaxis, eye),
		0, 0, 0, 1
	};
	
	return viewMatrix;
}

static mat4 MakeProjMatrix(float FOV, float aspect, float z_near, float z_far)
{
	mat4 proj;

	float rFOV = FOV * M_PI / 180.0f;
	float half_tan = tanf(0.5f * rFOV);
	float f = 1.0f / half_tan;

	proj.m[0][0] = f * (1.0f / aspect);
	proj.m[1][1] = f;
	proj.m[2][2] = -(z_far + z_near) / (z_far - z_near);
	proj.m[2][3] = -2.0f * z_far * z_near / (z_far - z_near);
	proj.m[3][2] = -1;

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

static void BarycentricCoords( float px, float py, float ax, float ay, float bx, float by, float cx, float cy, float* w1, float* w2, float* w3)
{
	float num = ax * (cy - ay) + (py - ay) * (cx - ax) - px * (cy - ay);
	float denom = (by - ay) * (cx - ax) - (bx - ax) * (cy - ay);

	*w1 = num / denom;
	*w2 = (py - ay - *w1 * (by - ay)) / (cy - ay);
	*w3 = 1 - *w2 - *w1;
}