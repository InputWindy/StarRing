#include "pch.h"
#include "OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>
StarRing::OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
	:m_ProjectionMatrix(ortho(left, right, bottom, top, -1.0f, 1.0f)),
	m_ViewMatrix(1.0f)
{
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;//glmÊÇ×ó³Ë
}

void StarRing::OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
{
	m_ProjectionMatrix = ortho(left, right, bottom, top, -1.0f, 1.0f);
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;//glmÊÇ×ó³Ë
}

void StarRing::OrthographicCamera::UpdateCamera()
{
	mat4 transform = translate(mat4(1.0f), m_Position) *
		rotate(mat4(1.0f), m_Rotation, vec3(0, 0, 1.0f));

	m_ViewMatrix = inverse(transform);
	m_ViewProjectionMatrix =m_ProjectionMatrix * m_ViewMatrix;//glmÊÇ×ó³Ë
}
