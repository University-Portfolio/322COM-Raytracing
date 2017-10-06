#pragma once
#include <SDL.h>


/**
* Enum of all supported codes
*/
enum Key
{
	KV_UNKNOWN = SDL_Scancode::SDL_SCANCODE_UNKNOWN,
	
	KV_A = SDL_Scancode::SDL_SCANCODE_A,
	KV_B = SDL_Scancode::SDL_SCANCODE_B,
	KV_C = SDL_Scancode::SDL_SCANCODE_C,
	KV_D = SDL_Scancode::SDL_SCANCODE_D,
	KV_E = SDL_Scancode::SDL_SCANCODE_E,
	KV_F = SDL_Scancode::SDL_SCANCODE_F,
	KV_G = SDL_Scancode::SDL_SCANCODE_G,
	KV_H = SDL_Scancode::SDL_SCANCODE_H,
	KV_I = SDL_Scancode::SDL_SCANCODE_I,
	KV_J = SDL_Scancode::SDL_SCANCODE_J,
	KV_K = SDL_Scancode::SDL_SCANCODE_K,
	KV_L = SDL_Scancode::SDL_SCANCODE_L,
	KV_M = SDL_Scancode::SDL_SCANCODE_M,
	KV_N = SDL_Scancode::SDL_SCANCODE_N,
	KV_O = SDL_Scancode::SDL_SCANCODE_O,
	KV_P = SDL_Scancode::SDL_SCANCODE_P,
	KV_Q = SDL_Scancode::SDL_SCANCODE_Q,
	KV_R = SDL_Scancode::SDL_SCANCODE_R,
	KV_S = SDL_Scancode::SDL_SCANCODE_S,
	KV_T = SDL_Scancode::SDL_SCANCODE_T,
	KV_U = SDL_Scancode::SDL_SCANCODE_U,
	KV_V = SDL_Scancode::SDL_SCANCODE_V,
	KV_W = SDL_Scancode::SDL_SCANCODE_W,
	KV_X = SDL_Scancode::SDL_SCANCODE_X,
	KV_Y = SDL_Scancode::SDL_SCANCODE_Y,
	KV_Z = SDL_Scancode::SDL_SCANCODE_Z,

	KV_UP = SDL_Scancode::SDL_SCANCODE_UP,
	KV_DOWN = SDL_Scancode::SDL_SCANCODE_DOWN,
	KV_LEFT = SDL_Scancode::SDL_SCANCODE_LEFT,
	KV_RIGHT = SDL_Scancode::SDL_SCANCODE_RIGHT,

	KV_1 = SDL_Scancode::SDL_SCANCODE_1,
	KV_2 = SDL_Scancode::SDL_SCANCODE_2,
	KV_3 = SDL_Scancode::SDL_SCANCODE_3,
	KV_4 = SDL_Scancode::SDL_SCANCODE_4,
	KV_5 = SDL_Scancode::SDL_SCANCODE_5,
	KV_6 = SDL_Scancode::SDL_SCANCODE_6,
	KV_7 = SDL_Scancode::SDL_SCANCODE_7,
	KV_8 = SDL_Scancode::SDL_SCANCODE_8,
	KV_9 = SDL_Scancode::SDL_SCANCODE_9,
	KV_0 = SDL_Scancode::SDL_SCANCODE_0,

	KV_F1 = SDL_Scancode::SDL_SCANCODE_F1,
	KV_F2 = SDL_Scancode::SDL_SCANCODE_F2,
	KV_F3 = SDL_Scancode::SDL_SCANCODE_F3,
	KV_F4 = SDL_Scancode::SDL_SCANCODE_F4,
	KV_F5 = SDL_Scancode::SDL_SCANCODE_F5,
	KV_F6 = SDL_Scancode::SDL_SCANCODE_F6,
	KV_F7 = SDL_Scancode::SDL_SCANCODE_F7,
	KV_F8 = SDL_Scancode::SDL_SCANCODE_F8,
	KV_F9 = SDL_Scancode::SDL_SCANCODE_F9,
	KV_F10 = SDL_Scancode::SDL_SCANCODE_F10,
	KV_F11 = SDL_Scancode::SDL_SCANCODE_F11,
	KV_F12 = SDL_Scancode::SDL_SCANCODE_F12,

	
	KV_RETURN = SDL_Scancode::SDL_SCANCODE_RETURN,
	KV_ESCAPE = SDL_Scancode::SDL_SCANCODE_ESCAPE,
	KV_BACKSPACE = SDL_Scancode::SDL_SCANCODE_BACKSPACE,
	KV_TAB = SDL_Scancode::SDL_SCANCODE_TAB,
	KV_SPACE = SDL_Scancode::SDL_SCANCODE_SPACE,

	KV_MINUS = SDL_Scancode::SDL_SCANCODE_MINUS,
	KV_EQUALS = SDL_Scancode::SDL_SCANCODE_EQUALS,

	KV_MAX = SDL_Scancode::SDL_NUM_SCANCODES,
};


class Keyboard
{
private:
	friend class Window;

	/// Tables to store key states where true is down and false is up
	bool m_currentStates[Key::KV_MAX]{ false };
	bool m_previousStates[Key::KV_MAX]{ false };

protected:
	/**
	* Update the current (un)pressed state for the key
	* @param key		Current key in question
	* @param isPressed	True if key is down
	* @returns True if button is pressed down
	*/
	inline void UpdateKeystate(const Key key, bool isPressed)
	{
		m_currentStates[key] = isPressed;
	}

	/**
	* Callback for when a new frame starts
	*/
	inline void HandleNewFrame() 
	{
		// Copy current states to previous states
		for (int i = 0; i < Key::KV_MAX; ++i)
			m_previousStates[i] = m_currentStates[i];
	}

public:
	/**
	* Is the given key currently down
	* @param key		Current key in question
	* @returns True if button is pressed down
	*/
	inline bool IsKeyDown(const Key key) const
	{
		return m_currentStates[key];
	}

	/**
	* Has the given key been released on this frame
	* @param key		Current key in question
	* @returns True if button state has just changed to up
	*/
	inline bool IsKeyReleased(const Key key) const
	{
		return !m_currentStates[key] && m_previousStates[key];
	}

	/**
	* Has the given key been pressed on this frame
	* @param key		Current key in question
	* @returns True if button state has just changed to down
	*/
	inline bool IsKeyPressed(const Key key) const 
	{
		return m_currentStates[key] && !m_previousStates[key];
	}
};

