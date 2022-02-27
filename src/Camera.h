#pragma once
#include "quakedef.h"

class Camera
{
public:

	void Init(int width, int height, glm::vec3 position);
	void InitProjMatrix(float FOV, float near_plane, float far_plane);
	void UpdateViewMatrix(void);

	glm::mat4 GetMVP(void);

private:

	int m_width, m_height;
	float m_yaw, m_pitch;

	glm::vec3 m_position;
	glm::vec3 m_up = { 0.0f, 0.0f, 1.0f};
	glm::vec3 m_right = { 1.0f, 0.0f, 0.0f };
	glm::vec3 m_orientation = { 0.0f, 1.0f, 0.0f };

	glm::mat4 m_model = glm::mat4(1.0f);
	glm::mat4 m_view;
	glm::mat4 m_projection;

	// Model * View * Projection matrix
	glm::mat4 m_MVP;
};