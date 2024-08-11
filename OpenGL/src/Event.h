#pragma once


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
	None						= 0,
	EventCategoryApplication	= 1 << 0,
	EventCategoryInput			= 1 << 1,
	EventCategoryKeyboard		= 1 << 2,
	EventCategoryMouse			= 1 << 3,
	EventCategoryMouseButton	= 1 << 4
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


#define EVENT_BIND_FUNC(func) [this](auto&&... args)->decltype(auto) {return this->func(std::forward<decltype(args)>(args)...);}

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


using MouseCode = uint16_t;
enum : MouseCode
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	};

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
	MouseMoveEvent(double xpos, double ypos):m_xPos(xpos),m_yPos(ypos){}


	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	EVENT_CLASS_TYPE(MouseMoved)

	double m_xPos, m_yPos;
};

class MouseScrolledEvent :public Event
{
public:
	MouseScrolledEvent(double xoffset, double yoffset) :m_xOffect(xoffset), m_yOffect(yoffset) {}


	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
	EVENT_CLASS_TYPE(MouseScrolled)

	double m_xOffect, m_yOffect;
};

class KeyEvent :public Event
{
public:
	EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

protected:
	KeyEvent(int keyCode, int mods) :m_keyCode(keyCode),m_mod(mods),m_isRepeat(false) {}
	
	int m_keyCode, m_mod;
	bool m_isRepeat;
};

class KetPressedEvent :public KeyEvent
{
public:
	KetPressedEvent(int keyCode, int mods) :KeyEvent(keyCode, mods) {}

	EVENT_CLASS_TYPE(KeyPressed)
};