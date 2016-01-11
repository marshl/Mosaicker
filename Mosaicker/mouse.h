#pragma once

class Mouse
{
public:
	// Buttons on the mouse
	enum Buttons
	{
		BUTTON_LEFT,
		BUTTON_MIDDLE,
		BUTTON_RIGHT,

		BUTTON_COUNT
	};

	Mouse();

	virtual ~Mouse();

	// check button state
	bool Mouse::IsButtonUp(Buttons a_eButton) const
	{
		return m_bButtonUps[a_eButton] == false;
	}

	inline bool Mouse::IsButtonDown(Buttons a_eButton) const
	{
		return m_bButtonUps[a_eButton];
	}


	inline short Mouse::GetWheelMove()
	{
		short iMove = m_iWheel;
		m_iWheel = 0;
		return iMove;
	}

	void				GetMouseMove(int& a_irX, int& a_irY);

	void				GetMousePosition(int& a_irX, int& a_irY);
	void				SetMousePosition(int a_iX, int a_iY);

	void				GetMousePositionRelative(int& a_irX, int& a_irY);
	void				SetMousePositionRelative(int a_iX, int a_iY);

	void				ShowMouse(bool a_bShow) const;

private:

	int			m_iPrevPosX, m_iPrevPosY;
	short		m_iWheel;
	bool		m_bButtonUps[BUTTON_COUNT];









}; 
