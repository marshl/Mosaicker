#include "Mouse.h"

#include <Windows.h>

Mouse::Mouse()
:    m_iPrevPosX(0)
,    m_iPrevPosY(0)
{
    m_iWheel = 0;

    memset( m_bButtonUps, 0, sizeof(bool) * BUTTON_COUNT );
}


Mouse::~Mouse()
{
}


void Mouse::GetMousePosition(int& a_irX, int& a_irY)
{
    POINT oCursor;
    GetCursorPos(&oCursor);

    a_irX = oCursor.x;
    a_irY = oCursor.y;
}


void Mouse::GetMousePositionRelative(int& a_irX, int& a_irY)
{
    POINT oCursor;
    GetCursorPos(&oCursor);

    a_irX = oCursor.x;
    a_irY = oCursor.y;
}


void Mouse::SetMousePosition(int a_iX, int a_iY)
{
    SetCursorPos(a_iX,a_iY);
    m_iPrevPosX = a_iX;
    m_iPrevPosY =a_iY;
}


void Mouse::SetMousePositionRelative( int a_iX, int a_iY )
{
    POINT oCursor;
    oCursor.x = a_iX;
    oCursor.y = a_iY;

    SetCursorPos (oCursor.x, oCursor.y );

    m_iPrevPosX =a_iX;
    m_iPrevPosY = a_iY;
}


void Mouse::GetMouseMove(int& a_irX, int& a_irY)
{
    POINT oCursor;
    GetCursorPos(&oCursor);

    a_irX = oCursor.x - m_iPrevPosX;
    a_irY = oCursor.y - m_iPrevPosY;

    m_iPrevPosX = oCursor.x;
    m_iPrevPosY = oCursor.y;
}

void Mouse::ShowMouse(bool a_bShow) const
{
    ShowCursor(a_bShow);
}