#include "InputSystem.h"

bool InputSystem::Initialise( HWND _hwnd, HINSTANCE _appInstance, bool _isExclusive, DWORD _flags )
{
    // Create the DI object
    if ( FAILED( DirectInput8Create( _appInstance, 
                                    DIRECTINPUT_VERSION,
                                    IID_IDirectInput8,
                                    (void**)&m_pDI,
                                    NULL ) ) )
    {
        return false;
    }

    if ( _flags & IS_USEKEYBOARD )
    {
        m_pKeyboard = new Keyboard( m_pDI, _hwnd );
        if ( m_pKeyboard == NULL )
        {
            return false;
        }
    }
    if ( _flags & IS_USEMOUSE )
    {
        m_pMouse = new Mouse( m_pDI, _hwnd, _isExclusive );
        if ( m_pMouse == NULL )
        {
            return false;
        }
    }
    return true;
}

bool InputSystem::Shutdown()
{
    UnacquireAll();
    if ( this->m_pKeyboard )
    {
        delete this->m_pKeyboard;
        this->m_pKeyboard = NULL;
    }

    if ( this->m_pMouse )
    {
        delete this->m_pMouse;
        this->m_pMouse = NULL;
    }

    if ( FAILED( m_pDI->Release() ) )
    {
        return false;
    }

    return true;
}

bool InputSystem::Update()
{
    if ( this->m_pKeyboard )
    {
        this->m_pKeyboard->Update();
    }
    if ( this->m_pMouse )
    {
        this->m_pMouse->Update();
    }
    return true;
}

void InputSystem::AcquireAll()
{
    if ( this->m_pKeyboard )
    {
        this->m_pKeyboard->Acquire();
    }
    if ( this->m_pMouse )
    {
        this->m_pMouse->Acquire();
    }
}

void InputSystem::UnacquireAll()
{
    if ( this->m_pKeyboard )
    {
        this->m_pKeyboard->Unacquire();
    }
    if ( this->m_pMouse )
    {
        this->m_pMouse->Unacquire();
    }
}






Keyboard::Keyboard( LPDIRECTINPUT8 _pDI, HWND _hwnd )
{
    if ( FAILED( _pDI->CreateDevice( GUID_SysKeyboard, &this->m_pDIDev, NULL ) ) )
    {
        // error processing
    }

    if ( FAILED( this->m_pDIDev->SetDataFormat( &c_dfDIKeyboard ) ) )
    {
        // error processing
    }

    if ( FAILED( this->m_pDIDev->SetCooperativeLevel( _hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE ) ) )
    {
        // error processing
    }

    if ( FAILED( this->m_pDIDev->Acquire() ) ) 
    {
        // error processing
    }

    Clear();
}

Keyboard::~Keyboard()
{
    if ( m_pDIDev )
    {
        m_pDIDev->Unacquire();
        m_pDIDev->Release();
    }
}

bool Keyboard::Update()
{
    if ( FAILED( m_pDIDev->GetDeviceState( sizeof(m_keys), (LPVOID)m_keys ) ) )
    {
        if ( FAILED( m_pDIDev->Acquire() ) )
        {
            return false;
        }
        if ( FAILED( m_pDIDev->GetDeviceState( sizeof(m_keys), (LPVOID)m_keys ) ) )
        {
            return false;
        }
    }
    return false;
}

bool Keyboard::Acquire()
{
    Clear();
    return !FAILED( m_pDIDev->Acquire() );
}

bool Keyboard::Unacquire()
{
    Clear();
    return !FAILED( m_pDIDev->Unacquire() );
}






Mouse::Mouse( LPDIRECTINPUT8 _pDI, HWND _hwnd, bool _isExclusive )
{
    if ( FAILED( _pDI->CreateDevice( GUID_SysMouse, &this->m_pDIDev, NULL ) ) )
    {
        // error processing
    }

    if ( FAILED( this->m_pDIDev->SetDataFormat( &c_dfDIMouse ) ) )
    {
        // error processing
    }

    DWORD flags;
    if ( _isExclusive )
    {
        flags = DISCL_FOREGROUND | DISCL_EXCLUSIVE | DISCL_NOWINKEY;
    }
    else
    {
        flags = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE;
    }

    if ( FAILED( this->m_pDIDev->SetCooperativeLevel( _hwnd, flags ) ) )
    {
        // error processing
    }

    if ( FAILED( this->m_pDIDev->Acquire() ) )
    {
        // error processing
    }

    if ( FAILED( this->m_pDIDev->GetDeviceState( sizeof(DIMOUSESTATE), &this->m_state ) ) )
    {
        // error processing
    }
}

Mouse::~Mouse()
{
    if ( this->m_pDIDev )
    {
        this->m_pDIDev->Unacquire();
        this->m_pDIDev->Release();
    }
}

bool Mouse::Update()
{
    if ( FAILED( this->m_pDIDev->GetDeviceState( sizeof(DIMOUSESTATE), &this->m_state ) ) )
    {
        if ( FAILED( this->m_pDIDev->Acquire() ) )
        {
            return false;
        }
        if ( FAILED( this->m_pDIDev->GetDeviceState( sizeof(DIMOUSESTATE), &this->m_state ) ) )
        {
            return false;
        }
    }
    return true;
}

bool Mouse::Acquire()
{
    return !FAILED( m_pDIDev->Acquire() );
}

bool Mouse::Unacquire()
{
    return !FAILED( this->m_pDIDev->Unacquire() );
}