#define _CRTDBG_MAP_ALLOC       // for detecting memory leaks
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdlib.h>             // for detecting memory leaks
#include <crtdbg.h>             // for detecting memory leaks
#include "engine.h"
#include <iostream>
#include <fstream>
#include <windowsx.h>
#include <vector>
#include "NuObject.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <tchar.h>
#include <stdio.h>


// Function prototypes
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HWND &, HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);

LRESULT WINAPI Object_List(HWND, UINT, WPARAM, LPARAM);
LRESULT WINAPI Object_Details(HWND, UINT, WPARAM, LPARAM);


HWND g_hwndChildren[4];
HWND NameEditor;
HWND fileLoad;
HWND xEditor;
HWND yEditor;

RECT rect;

using namespace std;
int CurEdit = -1;
int arrSize;
string file;
int infoVal = 0;

//==================================================================================================
// These hold the pieces of information for each created object, the ListNames is a companion to
// List as LPCSTR and string conversion is annoying.
// =================================================================================================
vector <NuObject*> List[1];
vector <string> ListNames[1];

// Game pointer
Engine *game = NULL;
HWND hwnd = NULL;

//=============================================================================
// Starting point for a Windows application
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// Check for memory leak if debug build
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	MSG msg;

	// Create the game, sets up message handler
	game = new Engine;

	// Create the window
	if (!CreateMainWindow(hwnd, hInstance, nCmdShow))
		return 1;

	try {
		game->initialize(hwnd);     // throws GameError

									// main message loop
		int done = 0;
		while (!done)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				// look for quit message
				if (msg.message == WM_QUIT)
					done = 1;

				// decode and pass messages on to WinProc
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
				game->run(hwnd);    // run the game loop
									// also check the value of the current game ID for the info window, call info window if changed
			if (infoVal != game->current_asset) {
				//call the needed things to change the value of the infobox
				infoVal = game->current_asset;
				SendMessage((HWND)g_hwndChildren[1], WM_PAINT, NULL, NULL);
			}
		}
		SAFE_DELETE(game);     // free memory before exit
		return msg.wParam;
	}
	catch (const GameError &err)
	{
		game->deleteAll();
		DestroyWindow(hwnd);
		MessageBox(NULL, err.getMessage(), "Error", MB_OK);
	}
	catch (...)
	{
		game->deleteAll();
		DestroyWindow(hwnd);
		MessageBox(NULL, "Unknown error occured in game.", "Error", MB_OK);
	}

	SAFE_DELETE(game);     // free memory before exit
	return 0;
}

//delete all current vector variables

void clearVect() {
	for (int i = 0; i < arrSize; i++) {
		DestroyWindow(List->at(i)->getReference());
	}
	List->clear();
	ListNames->clear();
	return;
}

//=============================================================================
// Read files from the directory
//=============================================================================
void UpdateList(HWND Window) {

	/*int run = 1;
	string name;
	while (myfile >> name) {
	std::string stemp = std::string(name.begin(), name.end());
	//MessageBox(Window, "Got Here", "OK", MB_OK);
	LPCSTR sw = stemp.c_str();
	int type;
	myfile >> type;
	int x;
	myfile >> x;
	int y;
	myfile >> y;
	if (List->size() < run) {
	NuObject *temp = new NuObject(Window, sw, type, 10, run * 20, x, y);
	List->push_back(temp);
	ListNames->push_back(name);
	MessageBox(Window, "Got Here", "OK", MB_OK);
	}
	run++;
	}
	arrSize = run - 1;
	*/
	clearVect();
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	string y = string(buffer).substr(0, string(buffer).find_last_of("\\/"));
	y = y.substr(0, y.find_last_of("\\/"));
	y = y.append("\\Saves\\*");

	WIN32_FIND_DATA FileData;
	HANDLE hFind;
	LPCSTR pos = y.c_str();
	hFind = FindFirstFile(pos, &FileData);
	int run = 1;
	while (FindNextFile(hFind, &FileData) != 0) {
		string yz = FileData.cFileName;

		//MessageBox(Window, yy, "OK", MB_OK);
		string tempname = string(FileData.cFileName).substr(string(FileData.cFileName).find_last_of("."), 5);

		LPCSTR postemp = tempname.c_str();
		if (tempname == ".txt") {
			yz = string(FileData.cFileName).substr(0, string(FileData.cFileName).find_last_of("."));
			ListNames->push_back(yz);
			LPCSTR yy = yz.c_str();
			//LPCSTR name = (string(FileData.cFileName).substr(0, string(FileData.cFileName).find_last_of("."))).c_str();
			//throw(GameError(gameErrorNS::FATAL_ERROR, " || "));
			NuObject *temp = new NuObject(Window, yy, 10, 20 + run * 20);

			List->push_back(temp);
			run++;
		}
	}
	arrSize = run - 1;
}

LRESULT WINAPI Object_Detail(HWND Window,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	LRESULT Result = 0;
	switch (msg)
	{

	case WM_CREATE: {
	}
	case BN_CLICKED: {

	}
	case WM_PAINT: {
		{
			InvalidateRect(Window, NULL, TRUE);
			PAINTSTRUCT ps;
			HDC hdct = BeginPaint(Window, &ps);
			if(infoVal == 0)TextOut(hdct, 10 /* X */, 0 /* Y */, "Ground: Walk on it", 18 /* Number of chars */);
			if(infoVal == 1)TextOut(hdct, 10 /* X */, 0 /* Y */, "Bird: Jump on it, or die", 24 /* Number of chars */);
			if (infoVal == 2)TextOut(hdct, 10 /* X */, 0 /* Y */, "Player: Choose Start Point", 26 /* Number of chars */);
			if (infoVal == 3)TextOut(hdct, 10 /* X */, 0 /* Y */, "EndFlag: Choose End Point", 25 /* Number of chars */);
			if (infoVal == 4)TextOut(hdct, 10 /* X */, 0 /* Y */, "Spike: Kills you", 16 /* Number of chars */);
			EndPaint(Window, &ps);
			//MessageBox(Window, "Got Here", "OK", MB_OK);
		}
		break;
	}
	default:
	{
		Result = DefWindowProc(Window, msg, wParam, lParam);
	} break;
	}
	return(Result);

}
HWND fileMake;

LRESULT WINAPI Object_List(HWND Window,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	LRESULT Result = 0;
	switch (msg)
	{

	case WM_CREATE: {

		fileLoad = CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, 10, 10, 80, 20, Window, NULL, NULL, 0);

		//Create the Create New Object button (this may be removed)

		CreateWindow(
			TEXT("BUTTON"),					/* Class Name */
			TEXT("Load File"),             /* Title */
			WS_VISIBLE | WS_CHILD,          /* Style */
			100, 0,					/* Position */
			80, 20,							/* Size */
			Window,                         /* Parent */
			(HMENU)ID_LOAD,                 /* No menu */
			NULL,							/* Instance */
			0);


		CreateWindow(
			TEXT("BUTTON"),					/* Class Name */
			TEXT("Create File"),             /* Title */
			WS_VISIBLE | WS_CHILD,          /* Style */
			100, 20,					/* Position */
			80, 20,							/* Size */
			Window,                         /* Parent */
			(HMENU)ID_CREATE,                 /* No menu */
			NULL,							/* Instance */
			0);
		//
		UpdateList(Window);
		break;
	}
	case WM_COMMAND: {

		if (LOWORD(wParam == ID_ADD)) {
			//writeToFile();
			//call UpdateList
			//addObj(0, 0, 1);
			//UpdateList(Window);
			InvalidateRect(Window, NULL, TRUE);
			//MessageBox(Window, "Got Here", "OK", MB_OK);
		}
	case BN_CLICKED: {
		if (LOWORD(wParam == ID_DATA)) {
			//MessageBox(Window, "Got Here", "OK", MB_OK);
			//Grab what button was pressed, and set the CurEdit var to it's array position
			for (int i = 0; i < arrSize; i++) {
				if ((LPARAM)lParam == (LPARAM)List->at(i)->getReference()) {
					LPCSTR namechanfe = ListNames->at(i).c_str();
					SetWindowText(fileLoad, namechanfe);
					//MessageBox(Window, "Got Here", "OK", MB_OK);
					//CurEdit = i;
				}
			}

			//SendMessage((HWND)g_hwndChildren[2], BN_CLICKED, NULL , NULL);


		}
		if (LOWORD(wParam == ID_LOAD)) {

			char sbuffer[20];
			GetWindowText(fileLoad, sbuffer, 20);
			for (int i = 0; i < arrSize; i++) {
				if (std::string(sbuffer) == ListNames->at(i)) {
					std::string path = "Saves\\";
					path.append(sbuffer);
					path.append(".txt");
					//throw(GameError(gameErrorNS::FATAL_ERROR, path));
					game->changeFile(path.c_str());
				}
			}
			//The path should be correct for the given file
			//LPCSTR fiflePath = path.c_str();
			//ifstream myfile(path);
			//if (!myfile.fail()) {
			//	MessageBox(Window, fiflePath, "OK", MB_OK);
			//}
			//file = sbuffer;
			//set that char array to the window text

			//eraseVectors();
			//UpdateList(Window);

		}
		if (LOWORD(wParam == ID_CREATE)) {

			char sbuffer[20];
			GetWindowText(fileLoad, sbuffer, 20);

			char buffer[MAX_PATH];
			GetModuleFileName(NULL, buffer, MAX_PATH);
			string y = string(buffer).substr(0, string(buffer).find_last_of("\\/"));
			y = y.substr(0, y.find_last_of("\\/"));
			y = y.append("\\Saves\\");
			y = y.append(sbuffer);
			y = y.append(".txt");
			//throw(GameError(gameErrorNS::FATAL_ERROR, y));

			ofstream file(y);
			file << 6 << " " << 0 << " " << 0 << "\n";
			file << 3 << " " << 200 << " " << 100 << "\n";
			file << 2 << " " << 100 << " " << 100 << "\n";
			file.close();

			std::string path = "Saves\\";
			path.append(sbuffer);
			path.append(".txt");
			//throw(GameError(gameErrorNS::FATAL_ERROR, path));
			game->changeFile(path.c_str());
			SetWindowText(hwnd, "this");
			//char sbuffer[20];
			//GetWindowText(fileLoad, sbuffer, 20);
			//std::string path = "Saves\\";
			//path.append(sbuffer);
			//throw(GameError(gameErrorNS::FATAL_ERROR, path));
			//game->changeFile(path.c_str());


			//The path should be correct for the given file
			//LPCSTR fiflePath = path.c_str();
			//ifstream myfile(path);
			//if (!myfile.fail()) {
			//	MessageBox(Window, fiflePath, "OK", MB_OK);
			//}
			//file = sbuffer;
			//set that char array to the window text

			//eraseVectors();
			UpdateList(Window);

		}
		break;
	}
					 break;
	}

	case WM_ACTIVATEAPP:
	{
		OutputDebugStringA("WM_ACTIVATEAPP\n");
	} break;

	default:
	{
		Result = DefWindowProc(Window, msg, wParam, lParam);
	} break;
	case WM_MOUSEACTIVATE:
	{
		SetFocus(Window);
	}
	}

	return(Result);
}
//=============================================================================
// window event callback function
//=============================================================================
LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return (game->messageHandler(hwnd, msg, wParam, lParam));
}

//=============================================================================
// Create the window
// Returns: false on error
//=============================================================================
bool CreateMainWindow(HWND &hwnd, HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcx;

	// Fill in the window class structure with parameters 
	// that describe the main window. 
	wcx.cbSize = sizeof(wcx);           // size of structure 
	wcx.style = CS_HREDRAW | CS_VREDRAW;    // redraw if size changes 
	wcx.lpfnWndProc = WinProc;          // points to window procedure 
	wcx.cbClsExtra = 0;                 // no extra class memory 
	wcx.cbWndExtra = 0;                 // no extra window memory 
	wcx.hInstance = hInstance;          // handle to instance 
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);   // predefined arrow 
	wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);    // black background 
	wcx.lpszMenuName = NULL;           // name of menu resource 
	wcx.lpszClassName = CLASS_NAME;     // name of window class 
	wcx.hIconSm = NULL;                 // small class icon 

										// Register the window class. 
										// RegisterClassEx returns 0 on error.
	if (RegisterClassEx(&wcx) == 0)    // if error
		return false;

	//set up the screen in windowed or fullscreen mode?
	DWORD style;
	if (FULLSCREEN)
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	else
		style = WS_OVERLAPPEDWINDOW;

	// Create window
	hwnd = CreateWindow(
		CLASS_NAME,             // name of the window class
		GAME_TITLE,             // title bar text
		style,                  // window style
		CW_USEDEFAULT,          // default horizontal position of window
		CW_USEDEFAULT,          // default vertical position of window
		GAME_WIDTH,             // width of window
		GAME_HEIGHT,            // height of the window
		(HWND)NULL,            // no parent window
		(HMENU)NULL,           // no menu
		hInstance,              // handle to application instance
		(LPVOID)NULL);         // no window parameters

							   //WindClass for the ObjectDetails Sidebar
	WNDCLASSEX list = {};
	list.cbSize = sizeof(list);
	list.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	list.lpfnWndProc = Object_List;
	list.cbClsExtra = 0;
	list.cbWndExtra = 0;
	list.hInstance = hInstance;
	list.hIcon = NULL;
	list.hCursor = LoadCursor(NULL, IDC_ARROW);
	list.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	list.lpszClassName = CLASS_NAMEZ;
	list.hIconSm = NULL;

	if (RegisterClassEx(&list) == 0)
		return false;

	WNDCLASSEX details = {};
	details.cbSize = sizeof(list);
	details.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	details.lpfnWndProc = Object_Detail;
	details.cbClsExtra = 0;
	details.cbWndExtra = 0;
	details.hInstance = hInstance;
	details.hIcon = NULL;
	details.hCursor = LoadCursor(NULL, IDC_ARROW);
	details.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	details.lpszClassName = CLASS_NAM;
	details.hIconSm = NULL;

	if (RegisterClassEx(&details) == 0)
		return false;


	//The list window
	g_hwndChildren[0] =
		CreateWindow(
			list.lpszClassName,
			"File Browser",
			WS_VISIBLE | WS_BORDER,
			200,
			400,
			210,
			500,
			NULL,
			NULL,
			hInstance,
			0);

	//The list window
	g_hwndChildren[1] =
		CreateWindow(
			details.lpszClassName,
			"Type Details",
			WS_VISIBLE | WS_BORDER,
			700,
			400,
			220,
			70,
			NULL,
			NULL,
			hInstance,
			0);

	// if there was an error creating the window
	if (!hwnd)
		return false;

	if (!FULLSCREEN)             // if window
	{
		// Adjust window size so client area is GAME_WIDTH x GAME_HEIGHT
		RECT clientRect;
		GetClientRect(hwnd, &clientRect);   // get size of client area of window
		MoveWindow(hwnd,
			0,                                           // Left
			0,                                           // Top
			GAME_WIDTH + (GAME_WIDTH - clientRect.right),    // Right
			GAME_HEIGHT + (GAME_HEIGHT - clientRect.bottom), // Bottom
			TRUE);                                       // Repaint the window
	}

	// Show the window
	ShowWindow(hwnd, nCmdShow);

	return true;
}




