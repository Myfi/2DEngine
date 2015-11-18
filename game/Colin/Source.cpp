//==================================================================================================
// Codename: BladeWorks
// Basic game Window
// Version 1.0.1
//==================================================================================================
#include <windows.h>
#include <windowsx.h>
#include <iostream>
#include <fstream>
#include <string>
#include<vector>
// Prototypes


class NuObject {
public:
	int xPos;
	int yPos;
	int type;
	LPCSTR name;
	int num;
	LPARAM reference;
	//NuObject();

NuObject(HWND, LPCSTR, int, int, int, int, int);

int getYPos() {
	return yPos;
}
int* getXPos() {
	return &xPos;
}
int* getType() {
	return &type;
}
void ChangeYPos(int newVal) {
	yPos = newVal;
	return;
}
void ChangeXPos(int newVal) {
	xPos = newVal;
	return;
}
void ChangeType(int newVal) {
	type = newVal;
	return;
}
LPCSTR getName() {
	return name;
}
void ChangeName(LPCSTR newName) {
	name = newName;
	return;
}
int getNum() {
	return num;
}
LPARAM getReference() {
	return reference;
}
//NuObject(HWND parent, LPCSTR title, int x, int y);

};

#define ID_DATA 2
NuObject::NuObject(HWND parent, LPCSTR title, int typ, int x, int y, int ImX, int ImY) {
	//name = title;
	xPos = ImX;
	yPos = ImY;
	type = typ;
	name = title;
	reference = (LPARAM)CreateWindow(
		TEXT("BUTTON"),                /* Class Name */
		title,                /* Title */
		WS_VISIBLE | WS_CHILD,            /* Style */
		x, y,   /* Position */
		80, 20,   /* Size */
		parent,                           /* Parent */
		(HMENU)ID_DATA,                           /* No menu */
		NULL,                          /* Instance */
		0);
}
//NuObject::NuObject()
//{
	//new NuObject();
//};


int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT WINAPI MainWindowProc(HWND, UINT, WPARAM, LPARAM);
LRESULT WINAPI GameWindowProc(HWND, UINT, WPARAM, LPARAM);
LRESULT WINAPI GameWindowProc2(HWND, UINT, WPARAM, LPARAM);
LRESULT WINAPI Details(HWND, UINT, WPARAM, LPARAM);

using namespace std;
bool update = true;


bool CreateMainWindow(HINSTANCE, int);
bool AnotherInstance();
// Global Variables
HINSTANCE hinst;
HDC hdc;
RECT rect;
HWND mainWindow;
PAINTSTRUCT ps;
TCHAR ch = ' ';
HWND g_hwndChildren[4];
// Constants
const char CLASS_NAME[] = "2DEngineWindowClass";
const char CLASS_NAMEZ[] = "Tryit";
const char CLASS_NAM[] = "Again";
const char APP_TITLE[] = "2DEngine";
int dataStore[100][3];
int arrSize;
int ListCur;
vector <NuObject*> List[1];
vector <string> ListNames[1];

//=================
// Try initializing everyhting to Null
//=================


//==================================================================================================
// Create the Methods to overide windowsx.h methods here | Still need to edit the code, currently anothers
//==================================================================================================

void
OnSize(HWND hwnd, UINT state, int cx, int cy)
{
	//List[0] = new NuObject(NULL, NULL, NULL, NULL , NULL, NULL);
	//Start working on the general layout
	HDWP hdwp = BeginDeferWindowPos(4);
	int cxHalf = cx / 2;
	// Use this to define where the child windows should be drawn.
	if (hdwp) hdwp = DeferWindowPos(hdwp, g_hwndChildren[0],
		NULL, 0, 0, 250, cy-200,
		SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOACTIVATE);
	if (hdwp) hdwp = DeferWindowPos(hdwp, g_hwndChildren[1],
		NULL, 0, cy-200, cx, 200,
		SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOACTIVATE);
	if (hdwp) hdwp = DeferWindowPos(hdwp, g_hwndChildren[2],
		NULL, cx-250, 0, 250, cy-200,
		SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOACTIVATE);
	if (hdwp) hdwp = DeferWindowPos(hdwp, g_hwndChildren[3],
		NULL, 250, 0, cx-500, cy - 200,
		SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOACTIVATE);
	if (hdwp) EndDeferWindowPos(hdwp);

}

BOOL
OnCreate(HWND hwnd, LPCREATESTRUCT lpcs)
{
	WNDCLASSEX wcx = {};
	//List[0] = new NuObject(NULL, NULL, NULL, NULL, NULL, NULL);
	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = GameWindowProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hinst;
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcx.lpszClassName = CLASS_NAME; //Global Variable
	wcx.hIconSm = NULL;

	if (RegisterClassEx(&wcx) == 0)
		return false;

	WNDCLASSEX wcxx = {};
	//NObject temp = new NObject;
	wcxx.cbSize = sizeof(wcxx);
	wcxx.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wcxx.lpfnWndProc = GameWindowProc2;
	wcxx.cbClsExtra = 0;
	wcxx.cbWndExtra = 0;
	wcxx.hInstance = hinst;
	wcxx.hIcon = NULL;
	wcxx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcxx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcxx.lpszClassName = CLASS_NAMEZ; //Global Variable
	wcxx.hIconSm = NULL;

	if (RegisterClassEx(&wcxx) == 0)
		return false;

	WNDCLASSEX detailsWin = {};
	//NObject temp = new NObject;
	detailsWin.cbSize = sizeof(detailsWin);
	detailsWin.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	detailsWin.lpfnWndProc = Details;
	detailsWin.cbClsExtra = 0;
	detailsWin.cbWndExtra = 0;
	detailsWin.hInstance = hinst;
	detailsWin.hIcon = NULL;
	detailsWin.hCursor = LoadCursor(NULL, IDC_ARROW);
	detailsWin.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	detailsWin.lpszClassName = CLASS_NAM; //Global Variable
	detailsWin.hIconSm = NULL;
	
	if (RegisterClassEx(&detailsWin) == 0)
		return false;

	
	
	for (int i = 0; i < 2; i++) {
		g_hwndChildren[i] = CreateWindow(wcxx.lpszClassName, NULL,
			WS_VISIBLE | WS_CHILD | WS_BORDER, 0, 0, 0, 0,
			hwnd, NULL , hinst, 0);
		if (i == 1) {
			g_hwndChildren[3] =
				CreateWindow(
					wcx.lpszClassName,
					NULL, //Global Variable
					WS_VISIBLE | WS_CHILD | WS_BORDER,
					0,
					0,
					0,
					0,
					hwnd,
					NULL,
					hinst,
					0);

			//Create the Details Window

			g_hwndChildren[2] =
				CreateWindow(
					detailsWin.lpszClassName,
					NULL, //Global Variable
					WS_VISIBLE | WS_CHILD | WS_BORDER,
					0,
					0,
					0,
					0,
					hwnd,
					NULL,
					hinst,
					0);
		}
		if (!g_hwndChildren[i]) return FALSE;
	}
	return TRUE;
}
//==================================================================================================
// Where the Program Starts
//==================================================================================================
int CALLBACK
WinMain(HINSTANCE Instance,
	HINSTANCE PrevInstance,
	LPSTR CmdLine,
	int ShowCode)
{
	// MessageBox(0, "Hope this works", "Game", MB_OK|MB_ICONINFORMATION);

	// Creates a Mutex to Prevent Multiple Instances of the game
	if (AnotherInstance())
		return false;

	// Creates the Window
	if (!CreateMainWindow(Instance, ShowCode))
		return false;

	// Loops until done
	int done = 0;
	MSG msg;
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				done = 1;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}


//==================================================================================================
// Used to Create the Window
// Returns false is Error
//==================================================================================================
bool CreateMainWindow(HINSTANCE Instance, int ShowCode)
{
	WNDCLASS wc;
	HWND WindowHandle;
	wc.style = 0;
	wc.lpfnWndProc = MainWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hinst;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = TEXT("Scratch");

	if (!RegisterClass(&wc)) return FALSE;

	WindowHandle = CreateWindow(
		TEXT("Scratch"),                /* Class Name */
		TEXT("SCRATCH"),                /* Title */
		WS_OVERLAPPEDWINDOW,            /* Style */
		200, 200,   /* Position */
		1200, 600,   /* Size */
		NULL,                           /* Parent */
		NULL,                           /* No menu */
		hinst,                          /* Instance */
		0);                             /* No special parameters */

	ShowWindow(WindowHandle, ShowCode);

	UpdateWindow(WindowHandle);
	
	
	

	
	
	//if (!g_hwndChildren[3])
	//	return false;

	//ShowWindow(g_hwndChildren[3], ShowCode);

	//UpdateWindow(g_hwndChildren[3]);
	
	return true;
	
}
	
//==================================================================================================
// Windows event Callback Function
//==================================================================================================
	LRESULT WINAPI MainWindowProc(HWND Window,
		UINT msg,
		WPARAM wParam,
		LPARAM lParam)
	{
		LRESULT Result = 0;

		switch (msg)
		{
		HANDLE_MSG(Window, WM_SIZE, OnSize);
		HANDLE_MSG(Window, WM_CREATE, OnCreate);
		
		case WM_DESTROY:
		{
			OutputDebugStringA("WM_DESTROY\n");
			PostQuitMessage(0);
			return 0;
		} break;

		case WM_CLOSE: // The "X" At the Upper Right hand corner is clicked
		{
			OutputDebugStringA("WM_CLOSE\n");
			PostQuitMessage(0);
			return 0;
		} break;

		case WM_ACTIVATEAPP:
		{
			
			OutputDebugStringA("WM_ACTIVATEAPP\n");
		} break;
		
		default:
		{
			Result = DefWindowProc(Window, msg, wParam, lParam);
		} break;
		}

		return(Result);
	}
LRESULT WINAPI GameWindowProc(HWND Window,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	
	LRESULT Result = 0;

	switch (msg)
	{
	
	case WM_DESTROY:
	{
		OutputDebugStringA("WM_DESTROY\n");
		PostQuitMessage(0);
		return 0;
	} break;

	case WM_MOUSEACTIVATE:
	{
		//SetActiveWindow(Window);
		SetFocus(Window);
	}
	case WM_CHAR:
		switch (wParam)
		{
		case 0x08:
		case 0x0A:
		case 0x0D:
		case 0x1B:		//ESC
			MessageBeep((UINT)-1);
			return 0;
		default:
			ch = (TCHAR)wParam;
			InvalidateRect(Window, NULL, TRUE);
			return 0;
		} break;

	case WM_CLOSE: // The "X" At the Upper Right hand corner is clicked
	{
		OutputDebugStringA("WM_CLOSE\n");
		PostQuitMessage(0);
		return 0;
	} break;

	case WM_ACTIVATEAPP:
	{
		//OutputDebugStringA("WM_ACTIVATEAPP\n");
	} break;

	case WM_PAINT:
	{
		hdc = BeginPaint(Window, &ps);
		GetClientRect(Window, &rect);

		TextOut(hdc, rect.right / 2, rect.bottom / 2, &ch, 1);
		EndPaint(Window, &ps);
		return 0;
	} break;

	default:
	{
		Result = DefWindowProc(Window, msg, wParam, lParam);
	} break;
	}

	return(Result);
}

#define ID_ADD 1

/*void writeToFile() {
	ofstream outfile("test.txt", ios::app);
	LPCSTR name = "try it ";
	if (*List[0]->getName() == NULL) {
	outfile << "\n" << name << "1 " << "100 " << "170 ";
	}
	outfile.close();
	update = true;
}*/

//=========================
//Updates the List Windows
//=========================
void UpdateList(HWND Window) {
	ifstream myfile("test.txt");
	int run = 1;
	
	string name;
	while (myfile >> name) {

		std::string stemp = std::string(name.begin(), name.end());
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
		}

		run++;
	}
	arrSize = run - 1;
	myfile.close();
}

LRESULT WINAPI GameWindowProc2(HWND Window,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	LRESULT Result = 0;
	switch (msg)
	{
		
	case WM_CREATE: {
		GetClientRect(Window, &rect);
		CreateWindow(
			TEXT("BUTTON"),                /* Class Name */
			TEXT("Add Object"),                /* Title */
			WS_VISIBLE | WS_CHILD,            /* Style */
			0, rect.bottom,   /* Position */
			80, 20,   /* Size */
			Window,                           /* Parent */
			(HMENU) ID_ADD,                           /* No menu */
			NULL,                          /* Instance */
			0);
		//LPCSTR temp = "name";
		//NuObject button (Window, temp, 20, 10, 50, 50);
		UpdateList(Window);
		break;
	}
	case WM_COMMAND: {
		
		if (LOWORD(wParam == ID_ADD)) {
			//writeToFile();
			//call UpdateList
			UpdateList(Window);
			InvalidateRect(Window, NULL, TRUE);
			//MessageBox(Window, "Got Here", "OK", MB_OK);
		}
	case BN_CLICKED: {
		if (LOWORD(wParam == ID_DATA)) {

			SendMessage((HWND)g_hwndChildren[2], BN_CLICKED, (WPARAM)wParam, (LPARAM)lParam);

				
		}
		break;
	}
		break;
	}

	case WM_PAINT:
	{
		hdc = BeginPaint(Window, &ps);
		GetClientRect(Window, &rect);
		LPCSTR chr;
		int run = 0;
		while (run < arrSize){
			if (dataStore[run][0] == 1) {
				chr = "This";
			}
			else {
				chr = "that";
			}
			TextOut(hdc, dataStore[run][1], dataStore[run][2], chr, 4);
			run++;
		}
		EndPaint(Window, &ps);
		return 0;
	} break;

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
		//SetActiveWindow(Window);
		SetFocus(Window);
	}
	}

	return(Result);
}
#define ID_CHANGEFILE 4
HWND NameEditor;
HWND xEditor;
HWND yEditor;
LRESULT WINAPI Details(HWND Window,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	LRESULT Result = 0;

	switch (msg)
	{
		
	case WM_CREATE: {
		
		GetClientRect(Window, &rect);
		NameEditor = CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, 56, 10, 50, 18, Window, NULL, NULL,  0);
		xEditor = CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, 56, 30, 50, 18, Window, NULL, NULL, 0);
		yEditor = CreateWindow("EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, 56, 50, 50, 18, Window, NULL, NULL, 0);

		//The button to actually do things
		CreateWindow(
			TEXT("BUTTON"),                /* Class Name */
			TEXT("Change"),                /* Title */
			WS_VISIBLE | WS_CHILD,            /* Style */
			170, 0,   /* Position */
			80, 20,   /* Size */
			Window,                           /* Parent */
			(HMENU)ID_DATA,                           /* No menu */
			NULL,                          /* Instance */
			0);
		break;
	}
	case WM_COMMAND: {
		
		
		case BN_CLICKED: {
			switch (LOWORD(wParam))
			{
				case ID_DATA: {
					//SetWindowText(NameEditor, "Hello");
					
					for (int i = 0; i < arrSize; i++) {	
						//MessageBox(Window, "Got Here", "OK", MB_OK);
						if (List->at(i)->getReference() == lParam) {
							string name = ListNames->at(i);
							std::string stemp = std::string(name.begin(), name.end());
							LPCSTR sw = stemp.c_str();
							
							int y = List->at(i)->yPos;

							wstring temp = to_wstring(List->at(i)->xPos);
							//LPCSTR temp2 = temp.c_str;
							SetWindowText(NameEditor, sw);
							SetWindowTextW(xEditor, temp.c_str());
							SetWindowTextW(yEditor, temp.c_str());
						}
					}
					
					break;
				}
				case ID_CHANGEFILE: {
	
					break;
				}
			}
			
			break;
		}
						
		break;
	}

	
	case WM_PAINT:
	{
		hdc = BeginPaint(Window, &ps);
		GetClientRect(Window, &rect);
		
		EndPaint(Window, &ps);
		return 0;
	} break;

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
		//SetActiveWindow(Window);
		SetFocus(Window);
	}
	}

	return(Result);
}


//==================================================================================================
// Used to Check is another instance of the program is running
//==================================================================================================
bool AnotherInstance()
{
	HANDLE ourMutex;
	// Attempt to create a mutex using out unique string
	ourMutex = CreateMutex(NULL, true, "BLDEW0RKZ");

	if (GetLastError() == ERROR_ALREADY_EXISTS)
		return true;

	return false;
}

//====================================
// Write somehting into the text file
//====================================


