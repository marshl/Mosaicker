#pragma once

#include <Windows.h>
#include "vector2.h"
#include "mouse.h"
/*
class EInput
{
public:

	static EInput* Instantiate();
	static bool Destroy();
	
	static void AddEvent( SDL_Event sdlEvent );
	static void Update();

	static bool GetKey( SDLKey key );
	static bool GetKeyDown( SDLKey key );
	static bool GetKeyUp( SDLKey key );
	static bool GetKeyReleased( SDLKey key );
	
	static bool GetMouse(int button);
	static bool GetMouseDown(int button);
	static bool GetMouseUp(int button);
	static bool GetMouseReleased(int button);

	static Vector2 GetCurrentMousePosition();
	static float GetCurrentMouseX();
	static float GetCurrentMouseY();

	static Vector2 GetPreviousMousePosition();
	static float GetPreviousMouseX();
	static float GetPreviousMouseY();

	static Vector2 GetMouseMovement();
	static float GetMouseMovementX();
	static float GetMouseMovementY();

private:
	static EInput* selfRef;
	
	// TODO convert to arrays of int32 and index in using bit shifting (possibly too much effort for too little gain)
	bool previousKeys[SDLK_LAST];
	bool currentKeys[SDLK_LAST];

	bool previousMouseButtons[NUM_MOUSE_BUTTONS];
	bool currentMouseButtons[NUM_MOUSE_BUTTONS];

	Vector2 currentMousePosition;
	Vector2 previousMousePosition;

	EInput();
	~EInput();
};*/