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

static vec4_t Vector_IntersectPlane(vec4_t& plane_p, vec4_t& plane_n, vec4_t& lineStart, vec4_t& lineEnd)
{
	plane_n = NormalizeVector(plane_n);
	float plane_d = -DotProduct(plane_n, plane_p);
	float ad = DotProduct(lineStart, plane_n);
	float bd = DotProduct(lineEnd, plane_n);
	float t = (-plane_d - ad) / (bd - ad);
	vec4_t lineStartToEnd = SubVectors(lineEnd, lineStart);
	vec4_t lineToIntersect = MulVector(lineStartToEnd, t);
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
	screenmat.m[2][2] = 1.0f;
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

static int ClipTriangleToPlane(vec4_t plane_p, vec4_t plane_n, triangle in_tri, triangle& out_tri1, triangle& out_tri2)
{
	// Make sure plane normal is indeed normal
	plane_n = NormalizeVector(plane_n);

	// Return signed shortest distance from point to plane, plane normal must be normalised
	auto dist = [&](vec4_t p)
	{
		return (DotProduct(p, plane_n) - DotProduct(plane_n, plane_p));
	};

	// Create two temporary storage arrays to classify points either side of plane
	// If distance sign is positive, point lies on "inside" of plane
	vec4_t inside_points[3];  int nInsidePointCount = 0;
	vec4_t outside_points[3]; int nOutsidePointCount = 0;

	// Get signed distance of each point in triangle to plane
	float d0 = (float)dist(in_tri.p[0]);
	float d1 = (float)dist(in_tri.p[1]);
	float d2 = (float)dist(in_tri.p[2]);

	if (d0 >= 0) { inside_points[nInsidePointCount++] = in_tri.p[0]; }
	else { outside_points[nOutsidePointCount++] = in_tri.p[0]; }
	if (d1 >= 0) { inside_points[nInsidePointCount++] = in_tri.p[1]; }
	else { outside_points[nOutsidePointCount++] = in_tri.p[1]; }
	if (d2 >= 0) { inside_points[nInsidePointCount++] = in_tri.p[2]; }
	else { outside_points[nOutsidePointCount++] = in_tri.p[2]; }

	// Now classify triangle points, and break the input triangle into 
	// smaller output triangles if required. There are four possible
	// outcomes...

	if (nInsidePointCount == 0)
	{
		// All points lie on the outside of plane, so clip whole triangle
		// It ceases to exist

		return 0; // No returned triangles are valid
	}

	if (nInsidePointCount == 3)
	{
		// All points lie on the inside of plane, so do nothing
		// and allow the triangle to simply pass through
		out_tri1 = in_tri;

		return 1; // Just the one returned original triangle is valid
	}

	if (nInsidePointCount == 1 && nOutsidePointCount == 2)
	{
		// Triangle should be clipped. As two points lie outside
		// the plane, the triangle simply becomes a smaller triangle

		// The inside point is valid, so keep that...
		out_tri1.p[0] = inside_points[0];

		// but the two new points are at the locations where the 
		// original sides of the triangle (lines) intersect with the plane
		out_tri1.p[1] = Vector_IntersectPlane(plane_p, plane_n, inside_points[0], outside_points[0]);
		out_tri1.p[2] = Vector_IntersectPlane(plane_p, plane_n, inside_points[0], outside_points[1]);

		return 1; // Return the newly formed single triangle
	}

	if (nInsidePointCount == 2 && nOutsidePointCount == 1)
	{
		// Triangle should be clipped. As two points lie inside the plane,
		// the clipped triangle becomes a "quad". Fortunately, we can
		// represent a quad with two new triangles

		// Copy appearance info to new triangles

		// The first triangle consists of the two inside points and a new
		// point determined by the location where one side of the triangle
		// intersects with the plane
		out_tri1.p[0] = inside_points[0];
		out_tri1.p[1] = inside_points[1];
		out_tri1.p[2] = Vector_IntersectPlane(plane_p, plane_n, inside_points[0], outside_points[0]);

		// The second triangle is composed of one of he inside points, a
		// new point determined by the intersection of the other side of the 
		// triangle and the plane, and the newly created point above
		out_tri2.p[0] = inside_points[1];
		out_tri2.p[1] = out_tri1.p[2];

		out_tri2.p[2] = Vector_IntersectPlane(plane_p, plane_n, inside_points[1], outside_points[0]);

		return 2; // Return two newly formed triangles which form a quad
	}
}