#ifndef INPUT_SYSTEM_H_
#define INPUT_SYSTEM_H_

#include <Windows.h>
#include <direct.h>
#include <dinput.h>

#define IS_USEKEYBOARD 1
#define IS_USEMOUSE 2

class Keyboard
{
public:
    Keyboard( LPDIRECTINPUT8 _pDI, HWND _hwnd );
    ~Keyboard();

    bool KeyDown( char _key ) { return this->m_keys[_key] & 0x80 ? true : false; }
    bool KeyUp( char _key ) { return this->m_keys[_key] & 0x80 ? false : true; }

    bool Update();

    void Clear() { ZeroMemory( m_keys, 256 * sizeof(char) ); }

    bool Acquire();
    bool Unacquire();

private:
    LPDIRECTINPUTDEVICE8 m_pDIDev;
    char m_keys[256];
};

class Mouse
{
public:
    Mouse( LPDIRECTINPUT8 _pDI, HWND _hwnd, bool _isExlusive = true );
    ~Mouse();

    bool ButtonDown( int _button ) { return this->m_state.rgbButtons[_button] & 0x80 ? true : false; }
    bool ButtonUp( int _button ) { return this->m_state.rgbButtons[_button] & 0x80 ? false : true; }
    int GetWheelMovement() { return m_state.lZ; }
    void GetMovement( int& _dx, int& _dy ) { _dx = m_state.lX;; _dy = m_state.lY; }

    bool Update();

    bool Acquire();
    bool Unacquire();

private:
    LPDIRECTINPUTDEVICE8 m_pDIDev;
    DIMOUSESTATE m_state;
};

class InputSystem
{
public:
    bool Initialise( HWND _hwnd, HINSTANCE _appInstance, bool _isExclusive = true, DWORD _flags = 0 );

    bool Shutdown();

    void AcquireAll();
    void UnacquireAll();

    Keyboard* GetKeyboard() { return this->m_pKeyboard; }
    Mouse* GetMouse() { return this->m_pMouse; }

    bool Update();

    bool KeyDown( char _key ) { return (this->m_pKeyboard && m_pKeyboard->KeyDown( _key )); }
    bool KeyUp( char _key ) { return (this->m_pKeyboard && m_pKeyboard->KeyUp( _key ) ); }

    bool ButtonDown( int _button ) { return ( this->m_pMouse && this->m_pMouse->ButtonUp( _button ) ); }
    bool ButtonUp( int _button ) { return ( this->m_pMouse && this->m_pMouse->ButtonUp( _button ) ); }

    void GetMouseMovement( int& dx, int& dy ) { if ( this->m_pMouse) this->m_pMouse->GetMovement( dx, dy ); }
    int GetMouseWheelMovement() { return m_pMouse ? m_pMouse->GetWheelMovement() : 0; }

private:
    Keyboard* m_pKeyboard;
    Mouse* m_pMouse;

    LPDIRECTINPUT8 m_pDI;
};




#endif