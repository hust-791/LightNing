#pragma once
#include "LN/Core/KeyCodes.h"

namespace LN
{
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = 1 << 0,
		EventCategoryInput = 1 << 1,
		EventCategoryKeyboard = 1 << 2,
		EventCategoryMouse = 1 << 3,
		EventCategoryMouseButton = 1 << 4
	};

#define EVENT_CLASS_TYPE(type)	static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual std::string GetName() const override { return #type;}

#define EVENT_CLASS_CATEGORY(category)	virtual int GetCategoryFlags()const override {return EventCategory::category;}


	class Event
	{
	public:
		bool Handled = false;

	public:
		virtual ~Event() = default;

		virtual EventType GetEventType() const = 0;
		virtual std::string GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;

		bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	};


#define LN_EVENT_BIND_FUNC(func) [this](auto&&... args)->decltype(auto) {return this->func(std::forward<decltype(args)>(args)...);}

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& e) :m_event(e) {}

		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (m_event.GetEventType() == T::GetStaticType())
			{
				m_event.Handled |= func(static_cast<T&>(m_event));
				return true;
			}
			return false;
		}
	private:
		Event& m_event;
	};

#pragma region MouseEvent

	class MouseButtonEvent : public Event
	{
	public:
		MouseCode GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
	protected:
		MouseButtonEvent(const MouseCode button)
			: m_Button(button) {}

		MouseCode m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(const MouseCode button)
			: MouseButtonEvent(button) {}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(const MouseCode button)
			: MouseButtonEvent(button) {}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

	class MouseMoveEvent :public Event
	{
	public:
		MouseMoveEvent(double xpos, double ypos) :m_xPos(xpos), m_yPos(ypos) {}

		float GetX() const { return (float)m_xPos; }
		float GetY() const { return (float)m_yPos; }


		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
			EVENT_CLASS_TYPE(MouseMoved)

	private:
			double m_xPos, m_yPos;
	};

	class MouseScrolledEvent :public Event
	{
	public:
		MouseScrolledEvent(double xoffset, double yoffset) :m_xOffect(xoffset), m_yOffect(yoffset) {}

		float GetXOffset() const { return (float)m_xOffect; }
		float GetYOffset() const { return (float)m_yOffect; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
			EVENT_CLASS_TYPE(MouseScrolled)

	private:
			double m_xOffect, m_yOffect;
	};

#pragma endregion

#pragma region KeyEvent

	class KeyEvent :public Event
	{
	public:
		KeyCode GetKeyCode() const { return m_keyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(KeyCode keyCode) :m_keyCode(keyCode) {}

		KeyCode m_keyCode;
	};

	class KeyPressedEvent :public KeyEvent
	{
	public:
		KeyPressedEvent(KeyCode keyCode, bool isRepeat) :KeyEvent(keyCode), m_isRepeat(isRepeat) {}

		bool IsRepeat() const { return m_isRepeat; }

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		bool m_isRepeat;
	};

	class KeyReleasedEvent :public KeyEvent
	{
	public:
		KeyReleasedEvent(KeyCode keyCode) :KeyEvent(keyCode) {}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent :public KeyEvent
	{
	public:
		KeyTypedEvent(KeyCode keyCode) :KeyEvent(keyCode) {}

		EVENT_CLASS_TYPE(KeyTyped)
	};

#pragma endregion

#pragma region WindowEvent

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }


		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_Width, m_Height;
	};


	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

#pragma endregion
}
