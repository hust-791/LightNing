#include "stdafx.h"
#include "LN/Core/Input.h"
#include <LN/Event/Event.h>
#include "Camera.h"

namespace LN {

	Camera::Camera(float fov, float aspectRatio, float nearClip, float farClip, CameraProjModel model)
			: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip), 
			m_Projection(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip)), m_ProjModel(model)
	{
		UpdateView();
	}

	void Camera::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;

		switch (m_ProjModel)
		{
			case CameraProjModel::en_Proj:
			{
				m_Projection = glm::ortho(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, -1.0f, 1.0f);
			} break;

			case CameraProjModel::en_Ortho:
			{
				m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
			} break;

			default: LN_CORE_ASSERT(false, "CameraProjModel Error !")
				break;
		}
	}

	void Camera::UpdateView()
	{
		// m_Yaw = m_Pitch = 0.0f; // Lock the camera's rotation
		m_Position = CalculatePosition();

		glm::quat orientation = GetOrientation();
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}

	void Camera::SetViewportSize(float width, float height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
		UpdateProjection();
	}

	glm::vec3 Camera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 Camera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 Camera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::vec3 Camera::CalculatePosition() const
	{
		return m_FocalPoint - GetForwardDirection() * m_Distance;
	}

	glm::quat Camera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}

	void Camera::MouseZoom(float delta)
	{
		m_Distance -= delta * ZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += GetForwardDirection();
			m_Distance = 1.0f;
		}
	}

	float Camera::ZoomSpeed() const
	{
		float distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}

	std::pair<float, float> Camera::PanSpeed() const
	{
		float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float Camera::RotationSpeed() const
	{
		return 0.8f;
	}

	void Camera::OnUpdate(Timestep ts)
	{

	}

	void Camera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(LN_EVENT_BIND_FUNC(Camera::OnMouseScroll));
		dispatcher.Dispatch<WindowResizeEvent>(LN_EVENT_BIND_FUNC(Camera::OnWindowResized));
	}

	bool Camera::OnMouseScroll(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);

		float delta = e.GetYOffset() * 0.1f;
		MouseZoom(delta);
		UpdateView();
		return false;
	}

	bool Camera::OnWindowResized(WindowResizeEvent& e)
	{
		SetViewportSize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}




	//---------------|--------------|----------------------
	//---------------| EditorCamera |----------------------
	//---------------|--------------|----------------------

	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip, CameraProjModel model)
		:Camera(fov, aspectRatio, nearClip, farClip, model)
	{
	}

	void EditorCamera::OnUpdate(Timestep ts)
	{
		Camera::OnUpdate(ts);

		if (Input::IsKeyPressed(Key::LeftAlt))
		{
			const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
			glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
			m_InitialMousePosition = mouse;

			if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
				MousePan(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
				MouseRotate(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
				MouseZoom(delta.y);
		}

		UpdateView();
	}

	void EditorCamera::OnEvent(Event& e)
	{
		Camera::OnEvent(e);
	}

	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
		m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
	}

	void EditorCamera::MouseRotate(const glm::vec2& delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * RotationSpeed();
		m_Pitch += delta.y * RotationSpeed();
	}



	//---------------|------------|----------------------
	//---------------| ViewCamera |----------------------
	//---------------|------------|----------------------


	void ViewCamera::OnUpdate(Timestep ts)
	{
		Camera::OnUpdate(ts);
	}

	void ViewCamera::OnEvent(Event& e)
	{
		Camera::OnEvent(e);
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseButtonPressedEvent>(LN_EVENT_BIND_FUNC(ViewCamera::OnMousePressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(LN_EVENT_BIND_FUNC(ViewCamera::OnMouseReleased));
		dispatcher.Dispatch<MouseMoveEvent>(LN_EVENT_BIND_FUNC(ViewCamera::OnMousMove));
		dispatcher.Dispatch<KeyPressedEvent>(LN_EVENT_BIND_FUNC(ViewCamera::OnKeyPressed));
	}

	bool ViewCamera::OnMousePressed(MouseButtonPressedEvent& e)
	{
		switch (e.GetMouseButton())
		{
		case Mouse::ButtonLeft:
		{
		}break;

		case Mouse::ButtonRight:
		{
		}break;

		default:
			break;
		}

		return false;
	}

	bool ViewCamera::OnMouseReleased(MouseButtonReleasedEvent& e)
	{
		return false;
	}

	bool ViewCamera::OnMousMove(MouseMoveEvent& e)
	{
		if (m_IsFirstEnter)
		{
			m_LastMousePosition = { e.GetX() ,e.GetY() };
			m_IsFirstEnter = false;
		}

		const glm::vec2& mouse{ e.GetX() ,e.GetY() };
		glm::vec2 delta = (mouse - m_LastMousePosition) * 0.003f;
		m_LastMousePosition = mouse;

		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * RotationSpeed();
		m_Pitch += delta.y * RotationSpeed();

		return false;
	}

	bool ViewCamera::OnKeyPressed(KeyPressedEvent& e)
	{
		switch (e.GetKeyCode())
		{
			case Key::W: 
			{		
				auto [xSpeed, ySpeed] = PanSpeed();
				m_FocalPoint += -GetUpDirection() * xSpeed * m_Distance;
			}break;

			case Key::A: 
			{
				auto [xSpeed, ySpeed] = PanSpeed();
				m_FocalPoint += GetRightDirection() * ySpeed * m_Distance;
			}break;

			case Key::S: 
			{
				auto [xSpeed, ySpeed] = PanSpeed();
				m_FocalPoint += GetUpDirection() * ySpeed * m_Distance;
			}break;

			case Key::D: 
			{
				auto [xSpeed, ySpeed] = PanSpeed();
				m_FocalPoint += GetRightDirection() * xSpeed * m_Distance;
			}break;


			default:	 
				break;
		}
		return false;
	}

}