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




using KeyCode = uint16_t;

enum EnKeyCode : KeyCode
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	};


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
	KeyPressedEvent(KeyCode keyCode, bool isRepeat) :KeyEvent(keyCode), m_isRepeat(isRepeat){}

	bool IsRepeat() const { return m_isRepeat; }

	EVENT_CLASS_TYPE(KeyPressed)
private:
	bool m_isRepeat;
};

class KeyReleasedEvent :public KeyEvent
{
public:
	KeyReleasedEvent(KeyCode keyCode) :KeyEvent(keyCode){}

	EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent :public KeyEvent
{
public:
	KeyTypedEvent(KeyCode keyCode) :KeyEvent(keyCode){}

	EVENT_CLASS_TYPE(KeyTyped)
};