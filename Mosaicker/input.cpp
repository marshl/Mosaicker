#include "input.h"
#include <iostream>

/*
EInput* EInput::selfRef = nullptr;

EInput* EInput::Instantiate()
{
	if(EInput::selfRef == nullptr)
	{
		EInput::selfRef = new EInput();
	}
	return EInput::selfRef;
}

bool EInput::Destroy()
{
	if(EInput::selfRef != nullptr)
	{
		delete EInput::selfRef;
		EInput::selfRef = false;
		return true;
	}
	return false;
}


void EInput::AddEvent(SDL_Event sdlEvent)
{
	switch(sdlEvent.type)
	{
	case  SDL_KEYDOWN:
	{
		EInput::selfRef->currentKeys[sdlEvent.key.keysym.sym] = true;
		break;
	};
	case SDL_KEYUP:
	{
		EInput::selfRef->currentKeys[sdlEvent.key.keysym.sym] = false;
		break;
	}
	case SDL_MOUSEBUTTONDOWN:
	{
		EInput::selfRef->currentMouseButtons[ sdlEvent.button.button ] = true;
		break;
	}
	case SDL_MOUSEBUTTONUP:
	{
		EInput::selfRef->currentMouseButtons[ sdlEvent.button.button ] = false;
		break;
	}
	default:
		break;
	};
}

void EInput::Update()
{
	memcpy(EInput::selfRef->previousKeys, EInput::selfRef->currentKeys, sizeof(bool) * SDLK_LAST);

	int x,y;
	SDL_GetMouseState(&x, &y);
	EInput::selfRef->previousMousePosition = EInput::selfRef->currentMousePosition;
	EInput::selfRef->currentMousePosition.Set(float(x), float(y));

	memcpy(EInput::selfRef->previousMouseButtons, EInput::selfRef->currentMouseButtons, sizeof(bool) * NUM_MOUSE_BUTTONS);
}


bool EInput::GetKey(SDLKey key)
{
	return EInput::selfRef->currentKeys[key];
}

bool EInput::GetKeyDown(SDLKey key)
{
	return EInput::selfRef->currentKeys[key]
	&& !EInput::selfRef->previousKeys[key];
}

bool EInput::GetKeyUp(SDLKey key)
{
	return !EInput::selfRef->currentKeys[key];
}

bool EInput::GetKeyReleased(SDLKey key)
{
	return !EInput::selfRef->currentKeys[key]
	&& EInput::selfRef->previousKeys[key];
}


bool EInput::GetMouse(int button)
{
	return EInput::selfRef->currentMouseButtons[button];
}

bool EInput::GetMouseDown(int button)
{
	return EInput::selfRef->currentMouseButtons[button]
		&& !EInput::selfRef->previousMouseButtons[button];
}

bool EInput::GetMouseUp( int button )
{
	return !EInput::selfRef->currentMouseButtons[ button ];
}

bool EInput::GetMouseReleased( int button )
{
	return !EInput::selfRef->currentMouseButtons[ button ]
		&& EInput::selfRef->previousMouseButtons[ button ];
}


Vector2 EInput::GetCurrentMousePosition()
{
	return EInput::selfRef->currentMousePosition;
}

float EInput::GetCurrentMouseX()
{
	return float(EInput::selfRef->currentMousePosition.x);
}

float EInput::GetCurrentMouseY()
{
	return float(EInput::selfRef->currentMousePosition.y);
}

Vector2 EInput::GetPreviousMousePosition()
{
	return EInput::selfRef->previousMousePosition;
}

float EInput::GetPreviousMouseX()
{
	return EInput::selfRef->previousMousePosition.x;
}

float EInput::GetPreviousMouseY()
{
	return EInput::selfRef->previousMousePosition.y;
}


Vector2 EInput::GetMouseMovement()
{
	return EInput::selfRef->currentMousePosition - EInput::selfRef->previousMousePosition;
}

float EInput::GetMouseMovementX()
{
	return EInput::selfRef->currentMousePosition.x - EInput::selfRef->previousMousePosition.x;
}

float EInput::GetMouseMovementY()
{
	return EInput::selfRef->currentMousePosition.y - EInput::selfRef->previousMousePosition.y;
}


EInput::EInput()
{
	memset(this->currentKeys, (int)false, sizeof(bool) * SDLK_LAST);
	memset(this->previousKeys, (int)false, sizeof(bool) * SDLK_LAST);
	memset(this->currentMouseButtons, (int)false, sizeof(bool) * NUM_MOUSE_BUTTONS);
	memset(this->previousMouseButtons, (int)false, sizeof(bool) * NUM_MOUSE_BUTTONS);
}

EInput::~EInput()
{

}
*/