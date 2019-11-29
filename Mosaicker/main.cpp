#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <ShellAPI.h>
#include <thread>

#include <WindowsX.h>

#include <Magick++.h>

#include "common.h"
#include "vector2.h"
#include "imagefield.h"
#include "colour.h"
#include "Texture.h"

void Render();
void InitialiseOpenGL();
void LoadTiles();
bool FindImageToLoad(std::string& _filename);
void RecalculateTextureField();
void PrintInstructions();
int GetUserIndex(int _length);
void RecalculateTextureRow(int _rowIndex);

////// Global Variables
HDC g_HDC;        // global device context
bool fullScreen = false;
bool previousKeysPressed[256];
bool currentKeysPressed[256];// holds true for keys that are pressed    

const int THREAD_COUNT = 10;

bool showingLines;

Vector2 mousePos;
Vector2 clickedPos;
Vector2 currentOffset;
Vector2 totalOffset;

float zoomClickPoint = 0.0f;
float totalZoom = 1.0f;
float currentZoom = 0.0f;
bool mouseDown = false;

int windowWidth = 600;
int windowHeight = 600;

enum RENDERMODE
{
	RENDERMODE_IMAGE,
	RENDERMODE_TILES,
	RENDERMODE_REPLACE,
};
RENDERMODE renderMode = RENDERMODE_IMAGE;

int tesellationTilesHigh = 1;
int tesellationTilesWide = 1; // Dependant on tiles high

//ImageField* mainImageField;
Texture* mainTexture;

//std::vector<ImageField*> tileData;
std::vector<Texture*> tileData;

//ImageField*** textureField;
Texture*** textureField;
//std::multimap<ImageField*, Vector2> textureMap;
std::multimap<Texture*, Vector2> textureMap;

float ColourDiff(const Magick::Color c1, const Magick::Color c2)
{
	return abs(c1.greenQuantum() - c2.greenQuantum()) + abs(c1.blueQuantum() - c2.blueQuantum()) + abs(c1.greenQuantum() - c2.greenQuantum());
}

// function to set the pixel format for the device context
void SetupPixelFormat(HDC hDC)
{
	int nPixelFormat;                    // our pixel format index

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),    // size of structure
		1,                                // default version
		PFD_DRAW_TO_WINDOW |            // window drawing support
		PFD_SUPPORT_OPENGL |            // OpenGL support
		PFD_DOUBLEBUFFER,                // double buffering support
		PFD_TYPE_RGBA,                    // RGBA color mode
		32,                                // 32 bit color mode
		0, 0, 0, 0, 0, 0,                // ignore color bits, non-palettized mode
		0,                                // no alpha buffer
		0,                                // ignore shift bit
		0,                                // no accumulation buffer
		0, 0, 0, 0,                        // ignore accumulation bits
		16,                                // 16 bit z-buffer size
		0,                                // no stencil buffer
		0,                                // no auxiliary buffer
		PFD_MAIN_PLANE,                    // main drawing plane
		0,                                // reserved
		0, 0, 0 };                        // layer masks ignored

	nPixelFormat = ChoosePixelFormat(hDC, &pfd);    // choose best matching pixel format

	SetPixelFormat(hDC, nPixelFormat, &pfd);        // set pixel format to device context
}

// the Windows Procedure event handler
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HGLRC hRC;                    // rendering context
	static HDC hDC;                        // device context

	switch (message)
	{
	case WM_CREATE:                    // window is being created
	{
		hDC = GetDC(hwnd);            // get current window's device context
		g_HDC = hDC;
		SetupPixelFormat(hDC);        // call our pixel format setup function

		// create rendering context and make it current
		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);

		return 0;
	}
	case WM_CLOSE:                    // windows is closing
	{
		// deselect rendering context and delete it
		wglMakeCurrent(hDC, NULL);
		wglDeleteContext(hRC);

		// send WM_QUIT to message queue
		PostQuitMessage(0);

		return 0;
	}
	case WM_SIZE:
	{
		windowHeight = HIWORD(lParam);        // retrieve width and height
		windowWidth = LOWORD(lParam);

		if (windowHeight == 0)                    // don't want a divide by zero
		{
			windowHeight = 1;
		}

		glViewport(0, 0, windowWidth, windowHeight);    // reset the viewport to new dimensions
		glMatrixMode(GL_PROJECTION);        // set projection matrix current matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();                    // reset projection matrix

		// calculate aspect ratio of window
		gluOrtho2D(0, (GLfloat)windowWidth / (GLfloat)windowHeight, 0, 1.0);

		glMatrixMode(GL_MODELVIEW);            // set modelview matrix
		glLoadIdentity();                    // reset modelview matrix

		return 0;
	}
	case WM_KEYDOWN:
	{
		// is a key pressed?
		currentKeysPressed[wParam] = true;

		if (wParam == 0x31)
		{
			renderMode = RENDERMODE_IMAGE;
		}
		else if (wParam == 0x32)
		{
			renderMode = RENDERMODE_TILES;
		}
		else if (wParam == 0x33)
		{
			renderMode = RENDERMODE_REPLACE;
			RecalculateTextureField();
		}
		else if (wParam == 'Q')
		{
			tesellationTilesHigh = max(tesellationTilesHigh - 5, 1);

			std::cout << "Tesellation: " << tesellationTilesWide << " x " << tesellationTilesHigh << "\n";
			tesellationTilesWide = (tesellationTilesHigh * mainTexture->width()) / mainTexture->height();
			tesellationTilesWide = max(1, tesellationTilesWide);

			if (renderMode == RENDERMODE_REPLACE)
			{
				RecalculateTextureField();
			}
		}
		else if (wParam == 'W')
		{
			tesellationTilesHigh = min(tesellationTilesHigh + 5, mainTexture->width());
			tesellationTilesHigh = min(tesellationTilesHigh, mainTexture->height());

			tesellationTilesWide = (tesellationTilesHigh * mainTexture->width()) / mainTexture->height();
			std::cout << "Tesellation: " << tesellationTilesWide << " x " << tesellationTilesHigh << "\n";
			tesellationTilesWide = max(1, tesellationTilesWide);

			if (renderMode == RENDERMODE_REPLACE)
			{
				RecalculateTextureField();
			}
		}
		else if (wParam == 'R')
		{
			totalOffset.SetZero();
			totalZoom = 1.0f;
		}
		else if (wParam == 'T')
		{
			PrintInstructions();
		}
		else if (wParam == 'E')
		{
			showingLines = !showingLines;
		}
		return 0;
	}
	case WM_KEYUP:
	{
		currentKeysPressed[wParam] = false;
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		mouseDown = true;
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);

		clickedPos.Set(x, y);
		mousePos.Set(x, y);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);

		mousePos.Set(x, y);
		zoomClickPoint = (float)y;
		return 0;
	}
	case WM_LBUTTONUP:
	{
		totalOffset += currentOffset;
		currentOffset.SetZero();
		return 0;
	}
	case WM_RBUTTONUP:
	{
		totalZoom += currentZoom;
		zoomClickPoint = 0.0f;
		currentZoom = 0.0f;
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		mousePos.Set(x, y);
		if (wParam & MK_LBUTTON)
		{
			currentOffset = (clickedPos - mousePos) / totalZoom;
		}
		if (wParam & MK_RBUTTON)
		{
			currentZoom = (zoomClickPoint - mousePos.y) * 0.01f;
		}
		return 0;
	}
	default:
	{
		break;
	}
	}

	return (DefWindowProc(hwnd, message, wParam, lParam));
}

// the main windows entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	AllocConsole();
	FILE* file;
	freopen_s(&file, "CON", "w", stdout);
	freopen_s(&file, "CON", "r", stdin);

	std::string filename;

	if (FindImageToLoad(filename) == false)
	{
		std::cout << "Could not load image\n";
		exit(1);
	}

	WNDCLASSEX windowClass;        // window class
	HWND     hwnd;            // window handle
	MSG         msg;                // message
	bool     done;            // flag saying when our app is complete
	DWORD     dwExStyle;        // Window Extended Style
	DWORD     dwStyle;            // Window Style
	RECT     windowRect;

	// temp var's
	int bits = 32;

	windowRect.left = (long)0;                        // Set Left Value To 0
	windowRect.right = (long)windowWidth;                    // Set Right Value To Requested Width
	windowRect.top = (long)0;                            // Set Top Value To 0
	windowRect.bottom = (long)windowHeight;                    // Set Bottom Value To Requested Height

	// fill out the window class structure
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);    // default icon
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);        // default arrow
	windowClass.hbrBackground = NULL;                                // don't need background
	windowClass.lpszMenuName = NULL;                                // no menu
	windowClass.lpszClassName = "QuickMosaic";
	windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);        // windows logo small icon

	// register the windows class
	if (!RegisterClassEx(&windowClass))
		return 0;

	if (fullScreen)                                // fullscreen?
	{
		DEVMODE dmScreenSettings;                    // device mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = windowWidth;        // screen width
		dmScreenSettings.dmPelsHeight = windowHeight;        // screen height
		dmScreenSettings.dmBitsPerPel = bits;        // bits per pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// 
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// setting display mode failed, switch to windowed
			MessageBox(NULL, "Display mode failed", NULL, MB_OK);
			fullScreen = false;
		}
	}

	if (fullScreen)                            // Are We Still In Fullscreen Mode?
	{
		dwExStyle = WS_EX_APPWINDOW;                // Window Extended Style
		dwStyle = WS_POPUP;                        // Windows Style
		ShowCursor(FALSE);                        // Hide Mouse Pointer
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;    // Window Extended Style
		dwStyle = WS_OVERLAPPEDWINDOW;                    // Windows Style
	}

	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);    // Adjust Window To True Requested Size

	// class registered, so now create our window
	hwnd = CreateWindowEx
	(
		NULL,                                    // extended style
		"quickmosaic",                            // class name
		"QuickMosaic",        // app name
		dwStyle | WS_CLIPCHILDREN |
		WS_CLIPSIBLINGS,
		0, 0,                                    // x,y coordinate
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,    // width, height
		NULL,                                    // handle to parent
		NULL,                                    // handle to menu
		hInstance,                            // application instance
		NULL
	);                                // no extra params

   // check if window creation failed ( hwnd would equal NULL )
	if (!hwnd)
		return 0;

	ShowWindow(hwnd, SW_SHOW);            // display the window
	UpdateWindow(hwnd);                    // update the window

	done = false;                        // intialize the loop condition variable
	InitialiseOpenGL();                        // initialize OpenGL

	LoadTiles();

	BITMAPINFOHEADER header;
	unsigned char* data = Common::LoadBitmapFile(filename.c_str(), &header);
	mainTexture = new Texture(GL_TEXTURE_2D, filename);
	mainTexture->Load();

	//textureField = new ImageField** [mainTexture->width()];
	textureField = new Texture * *[mainTexture->width()];
	for (int i = 0; i < mainTexture->width(); ++i)
	{
		//textureField[i] = new ImageField*[mainTexture->height()];
		textureField[i] = new Texture * [mainTexture->height()];
	}

	while (done == false)
	{
		PeekMessage(&msg, hwnd, NULL, NULL, PM_REMOVE);

		if (msg.message == WM_QUIT)        // do we receive a WM_QUIT message?
		{
			done = true;                // if so, time to quit the application
			break;
		}
		TranslateMessage(&msg);    // translate and dispatch to event queue
		DispatchMessage(&msg);

		if (currentKeysPressed[VK_ESCAPE])
		{
			done = true;
			break;
		}

		float dt = Common::GetDeltaTime();

		Render();
		Sleep(1000 / 24);
	}

	if (fullScreen)
	{
		ChangeDisplaySettings(NULL, 0);        // If So Switch Back To The Desktop
		ShowCursor(TRUE);                    // Show Mouse Pointer
	}


	for (int i = 0; i < mainTexture->width(); ++i)
	{
		delete textureField[i];
	}

	delete textureField;

	for (unsigned int i = 0; i < tileData.size(); ++i)
	{
		delete tileData[i];
	}

	FreeConsole();
	return msg.wParam;
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	float zoom = 2.0f * (currentZoom + totalZoom);

	float imageAspect = (float)mainTexture->width() / (float)mainTexture->height();
	float tileAspect = (float)tesellationTilesWide / (float)tesellationTilesHigh;

	switch (renderMode)
	{
	case RENDERMODE_IMAGE:
	{
		glColor3f(1.0f, 1.0f, 1.0f);

		//glBindTexture( GL_TEXTURE_2D, mainImageField->texture );
		mainTexture->Bind(NULL);
		glPushMatrix();

		glTranslatef(-1.0f, -1.0f, 0.0f);
		glTranslatef(-(totalOffset.x + currentOffset.x) / windowWidth, (totalOffset.y + currentOffset.y) / windowHeight, 0.0f);

		glTranslatef((totalOffset.x + currentOffset.x) / windowWidth, -(totalOffset.y + currentOffset.y) / windowHeight, 0.0f);
		glScalef(zoom, zoom, 1);
		glTranslatef(-(totalOffset.x + currentOffset.x) / windowWidth, (totalOffset.y + currentOffset.y) / windowHeight, 0.0f);

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f * imageAspect, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f * imageAspect, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 1.0f);
		glEnd();
		break;
	}
	case RENDERMODE_TILES:
	{
		glBindTexture(GL_TEXTURE_2D, NULL);
		glTranslatef(-1.0f, -1.0f, 0.0f);
		glTranslatef(-(totalOffset.x + currentOffset.x) / windowWidth, (totalOffset.y + currentOffset.y) / windowHeight, 0.0f);

		glTranslatef((totalOffset.x + currentOffset.x) / windowWidth, -(totalOffset.y + currentOffset.y) / windowHeight, 0.0f);
		glScalef(zoom, zoom, 1);
		glTranslatef(-(totalOffset.x + currentOffset.x) / windowWidth, (totalOffset.y + currentOffset.y) / windowHeight, 0.0f);

		glBegin(GL_QUADS);
		float cellWidth = (float)mainTexture->width() / (float)tesellationTilesWide;
		float cellHeight = (float)mainTexture->height() / (float)tesellationTilesHigh;
		for (int y = 0; y < tesellationTilesHigh; ++y)
		{
			for (int x = 0; x < tesellationTilesWide; ++x)
			{
				Magick::Color c = mainTexture->GetAverageInArea(x * cellWidth, y * cellHeight, cellWidth, cellHeight);
				//c = *mainImageField->GetPixel(x,y);

				float d = 2 << MAGICKCORE_QUANTUM_DEPTH;
				glColor4f(c.redQuantum() / d, c.greenQuantum() / d, c.blueQuantum() / d, c.alphaQuantum() / d);
				//glColor3f( float(x)/(float)tesellation, float(y)/float(tesellation), 1.0f );

				Magick::Quantum q = c.redQuantum();

				glVertex2f((float(x) / (float)tesellationTilesWide) * tileAspect, float(y) / tesellationTilesHigh);
				glVertex2f((float(x + 1) / (float)tesellationTilesWide) * tileAspect, float(y) / tesellationTilesHigh);
				glVertex2f((float(x + 1) / (float)tesellationTilesWide) * tileAspect, float(y + 1) / tesellationTilesHigh);
				glVertex2f((float(x) / (float)tesellationTilesWide) * tileAspect, float(y + 1) / tesellationTilesHigh);
			}
		}
		glEnd();
		break;
	}
	case RENDERMODE_REPLACE:
	{
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(-1.0f, -1.0f, 0.0f);
		glTranslatef(-(totalOffset.x + currentOffset.x) / windowWidth, (totalOffset.y + currentOffset.y) / windowHeight, 0.0f);

		glTranslatef((totalOffset.x + currentOffset.x) / windowWidth, -(totalOffset.y + currentOffset.y) / windowHeight, 0.0f);
		glScalef(zoom, zoom, 1);
		glTranslatef(-(totalOffset.x + currentOffset.x) / windowWidth, (totalOffset.y + currentOffset.y) / windowHeight, 0.0f);

		if (textureMap.size() > 0)
		{
			GLuint texture = NULL;
			std::multimap<Texture*, Vector2>::iterator iter;
			//for ( std::multimap<ImageField*, Vector2>::iterator iter = textureMap.begin(); iter != textureMap.end(); ++iter )
			for (iter = textureMap.begin(); iter != textureMap.end(); ++iter)
			{
				if (iter->first == nullptr)
				{
					continue;
				}

				//if ( texture != iter->first->texture )
				if (texture != iter->first->GetTexture())
				{
					if (texture != NULL)
					{
						glEnd();
					}

					texture = iter->first->GetTexture();

					glBindTexture(GL_TEXTURE_2D, texture);
					glBegin(GL_QUADS);
				}

				Vector2 pos = iter->second;

				glTexCoord2f(0.0f, 1.0f);    glVertex2f(pos.x / tesellationTilesWide * tileAspect, pos.y / tesellationTilesHigh);
				glTexCoord2f(1.0f, 1.0f);    glVertex2f((pos.x + 1) / tesellationTilesWide * tileAspect, pos.y / tesellationTilesHigh);
				glTexCoord2f(1.0f, 0.0f);    glVertex2f((pos.x + 1) / tesellationTilesWide * tileAspect, (pos.y + 1) / tesellationTilesHigh);
				glTexCoord2f(0.0f, 0.0f);    glVertex2f(pos.x / tesellationTilesWide * tileAspect, (pos.y + 1) / tesellationTilesHigh);
			}
		}
		glEnd();
		break;
	}
	default:
	{
		break;
	}
	}

	if (showingLines)
	{
		glColor3f(0, 0, 0);
		glBegin(GL_LINES);
		for (int x = 0; x < tesellationTilesWide; ++x)
		{
			glVertex2f(float(x) / float(tesellationTilesWide) * tileAspect, 0);
			glVertex2f(float(x) / float(tesellationTilesWide) * tileAspect, 1);
		}
		for (int y = 0; y < tesellationTilesHigh; ++y)
		{
			glVertex2f(0, float(y) / float(tesellationTilesHigh));
			glVertex2f(tileAspect, float(y) / float(tesellationTilesHigh));
		}
		glEnd();
	}

	glFlush();
	SwapBuffers(g_HDC);
}

void InitialiseOpenGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);        // clear to black
	glEnable(GL_TEXTURE_2D);                    // enable 2D texturing
}


bool FindImageToLoad(std::string& filename)
{
	unsigned int fileCount = 0;
	WIN32_FIND_DATA findData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	hFind = FindFirstFile("images\\*", &findData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		std::cout << "Error opening images folder.\n";
		return "";
	}

	std::vector<std::string> filenames;
	do
	{
		std::string str = findData.cFileName;
		if (str != "."
			&& str != ".."
			&& str.find(".png") != string::npos)
		{
			std::cout << fileCount << ") " << str << "\n";
			std::string filename = "images/" + str;
			filenames.push_back(filename);
			++fileCount;
		}
	} while (FindNextFile(hFind, &findData) != 0);

	FindClose(hFind);

	if (filenames.empty())
	{
		std::cout << "No .png files found in images folder.\n";
		return false;
	}

	int fileIndex = GetUserIndex(filenames.size());
	filename = filenames[fileIndex];
	return true;
}


int GetUserIndex(int length)
{
	while (true)
	{
		std::cout << "Enter file index: ";
		int index = -1;
		std::cin >> index;

		std::cout << "I:" << index << "\n";
		if (index < 0 || index >= length)
		{
			std::cout << "Enter a number between 0 and " << length << "\n";
		}
		else
		{
			return index;
		}
	}

	return 0;
}

void LoadTiles()
{
	WIN32_FIND_DATA findData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	hFind = FindFirstFile("tiles\\*", &findData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		std::cout << "Error: invalid path\n";
		return;
	}

	do
	{
		std::string str = findData.cFileName;
		if (str != "."
			&& str != ".."
			&& str.find(".png") != string::npos)
		{
			std::cout << str << "\n";
			std::string filename = "tiles/" + str;
			//BITMAPINFOHEADER header;
			//unsigned char* data = Common::LoadBitmapFile( (char*)filename.c_str(), &header );

			//if ( data == nullptr )
			//    continue;

			/*ImageField* tile = new ImageField( header.biWidth, header.biHeight );
			tile->bitmapInfo = header;
			tile->texture = Common::LoadBitmapAsTexture( data, &header );
			tile->LoadData( data );
			tile->average = tile->GetAverage( 0, 0, 1, 1 );
			tile->filename = str;
			tileData.push_back( tile );*/
			Texture* tile = new Texture(GL_TEXTURE_2D, filename);
			tile->Load();
			tileData.push_back(tile);
		}
	} while (FindNextFile(hFind, &findData) != 0);

	FindClose(hFind);
}

void RecalculateTextureField()
{
	textureMap.clear();

	for (int y = 0; y < tesellationTilesHigh; ++y)
	{
		for (int x = 0; x < tesellationTilesWide; ++x)
		{
			textureField[y][x] = nullptr;
		}
	}

	std::thread* threads[THREAD_COUNT];
	for (int i = 0; i < THREAD_COUNT; ++i)
	{
		threads[i] = new std::thread(RecalculateTextureRow, i);
	}

	for (int i = 0; i < THREAD_COUNT; ++i)
	{
		threads[i]->join();
	}

	for (int i = 0; i < THREAD_COUNT; ++i)
	{
		delete threads[i];
	}

	for (int y = 0; y < tesellationTilesHigh; ++y)
	{
		for (int x = 0; x < tesellationTilesWide; ++x)
		{
			if (textureField[y][x] != nullptr)
			{
				//textureMap.insert( std::pair<ImageField*, Vector2>( textureField[y][x], Vector2( (float)x, (float)y ) ) );
				auto texturePair = std::pair<Texture*, Vector2>(textureField[y][x], Vector2(x, y));
				textureMap.insert(texturePair);
			}
		}
	}
}

void RecalculateTextureRow(int rowIndex)
{
	for (int y = tesellationTilesHigh / THREAD_COUNT * rowIndex;
		y < (int)((float)tesellationTilesHigh / (float)THREAD_COUNT * float(rowIndex + 1)); ++y)
	{
		for (int x = 0; x < tesellationTilesWide; ++x)
		{
			Magick::Color c = mainTexture->GetAverageInArea(x, y, tesellationTilesWide, tesellationTilesHigh);

			float diff = ColourDiff(c, tileData[0]->GetAverageColour());//ColourDiff( c, tileData[0]->average );
			//ImageField* closestField = nullptr;
			Texture* closestField = nullptr;

			//if ( c.alphaQuantum() > 0 ) // Less than zero and it is a transparent cell
			{
				closestField = tileData[0];
				for (unsigned int i = 1; i < tileData.size(); ++i)
				{
					//float tDiff = ColorDiff( c, tileData[i]->average );
					float tDiff = ColourDiff(c, tileData[i]->GetAverageColour());

					if (tDiff < diff)
					{
						diff = tDiff;
						closestField = tileData[i];
					}
				}
			}

			textureField[y][x] = closestField;
		}
	}
}

void PrintInstructions()
{
	/*std::string filename( mainTexture->GetFilename().begin() + mainTexture->GetFilename().find( '/' )+1,
		mainTexture->GetFilename().begin() + mainTexture->GetFilename().rfind(".") );
	std::ofstream fout( filename + "_mosaic.txt" );

	for ( int i = 0; i < (int)tileData.size(); ++i )
	{
		tileData[i]->count = 0;
	}

	for ( int y = 0; y < tesellationTilesHigh; ++y )
	{
		fout << "Row " << y + 1 << "\n";

		ImageField* blockType = textureField[y][0];
		int blockCount = 0;

		for ( int x = 0; x < tesellationTilesWide; ++x )
		{
			ImageField* field = textureField[y][x];

			if ( field != nullptr )
			{
				++field->count;
			}

			if ( blockType != field )
			{

				std::string temp = blockType == nullptr ? "Blank" : blockType->filename.substr( 0, blockType->filename.find(".bmp") );
				fout << blockCount << " x " << temp << "\n";
				blockType = field;
				blockCount = 0;
			}
			++blockCount;
		}
		std::string temp = blockType == nullptr ? "Blank" : blockType->filename.substr( 0, blockType->filename.find(".bmp") );
		fout << blockCount << " x " << temp << "\n\n";
	}

	fout << "\n";

	for ( int i = 0; i < (int)tileData.size(); ++i )
	{
		ImageField* field = tileData[i];
		if ( field->count != 0 )
		{
			std::string temp = field->filename.substr( 0, field->filename.find(".bmp") );
			fout << field->count << " x " << temp << "\n";
		}
	}

	fout.close();*/
}