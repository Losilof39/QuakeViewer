#pragma once
#include "quakedef.h"

class Camera
{
public:
	Camera(vec3_t &eye, vec3_t& at, vec3_t& up);
	void Proj(float FOV, float aspect, float z_near, float z_far);
	mat4 UpdateMVP(void);

private:
	vec3_t m_eye;
	vec3_t m_at;
	vec3_t m_orientation;
	vec3_t m_up;

	mat4 m_scale;
	mat4 m_proj;
	mat4 m_view;
};