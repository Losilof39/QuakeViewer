//#include "Camera.h"
//
//void Camera::Init(int width, int height, glm::vec3 position)
//{
//	m_width = width;
//	m_height = height;
//	m_position = position;
//}
//
//void Camera::InitProjMatrix(float FOV, float near_plane, float far_plane)
//{
//	//m_view = glm::lookAt(m_position, m_position + m_orientation, m_up);
//	m_projection = glm::perspective(glm::radians(FOV), 1.0f, near_plane, far_plane);
//}
//
//void Camera::UpdateViewMatrix(void)
//{
//	m_MVP = m_projection;
//}
//
//glm::mat4 Camera::GetMVP(void)
//{
//	return m_MVP;
//}
