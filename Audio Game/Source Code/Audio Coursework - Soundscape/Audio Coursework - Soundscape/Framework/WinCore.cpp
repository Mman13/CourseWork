/*
	File:	WinCore.cpp
	Version:	2.2
	Date: 22nd april 2013; August 2012; 2006.
	Author:	Allan C. Milne

	Namespace:	AllanMilne
	Exposes:	WinCore implementation.
	Requires:	IState, Gametimer.

	Description:
	A basic Windows framework providing 
	*	initialization of the windows environment;
	*	Windows message loop handling;
	*	an application state frame process function with time delta information.

	notes:
	*	Any project using this class must have its character set to multibyte, not unicode.
	*	this implementation defines the escape key to exit the application.
	*	See WinCore.hpp for behaviour and use.
	*	See IState.hpp for behaviour and use of application state processing.
	* See GameTimer.h for details on the timer implementation used.

	this model  is intended as a basic framework for building your own Windows applications.
	You can, of course, use your own Windows framework to create these applications, this is a simple example and starting point.

*/

// system includes.
#include <windows.h>

// framework includes.
#include "IState.hpp"
#include "Gametimer.h"
#include "WinCore.hpp"

namespace AllanMilne 
{

//=== Windows message processing.
//=== Escape key terminates application.
LRESULT CALLBACK WndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		//--- check for escape key; if pressed then terminate application.
		if( wParam == VK_ESCAPE )
			DestroyWindow(hwnd);
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
} // end WndProc function.

//=== Constructor to set up strategy pattern.
WinCore::WinCore (IState *aState)
{
	mState = aState;
	mHwnd = NULL;
	mTimer = new GameTimer ();
} // end WinCore constructor.

//=== Destructor.
//=== Notes:
//=== 1. we do not call mState->Cleanup() since this is called in the RunApp() method.
//=== 2. We do not delete the mState field since this object was not responsible for creating it - it was passed to it in the constructor.
WinCore::~WinCore ()
{
	delete mTimer;
	mTimer = NULL;
} // end WinCore destructor.

//=== Create the application window; returns false if any errors detected.
//=== Calls the IState::Initialize() function via the strategy pattern; and resets the timer.
	bool WinCore::Initialize (
		const string &aTitle,		// Text to display in title bar.
		int aWidth,					// width of back-buffer window.
		int aHeight,				// height of back-buffer window.
		bool isWindowed,			// true=windowed; false=full screen.
		HINSTANCE anHInstance		// handle to calling process.
		)
{
	const char *title = aTitle.c_str();
	WNDCLASS wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)WndProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = anHInstance;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = (LPCSTR)title;
	if (!RegisterClass(&wc) ) 
	{
		MessageBox (NULL, TEXT (" RegisterClass() failed - Error registering application class with Windows."), TEXT ("WinCore::RunApp() - Failed"), MB_OK | MB_ICONERROR );
		return false;
	}
	HWND hwnd = 0;
	hwnd = CreateWindow ( (LPCSTR)title, (LPCSTR)title, 
		WS_EX_TOPMOST,
		0, 0, aWidth, aHeight,
		0 /*parent hwnd*/, 0 /* menu */, anHInstance, 0 /*extra*/); 
	if( !hwnd )
	{
		MessageBox(NULL, TEXT (" CreateWindow() failed - error creating window."), TEXT ("WinCore::RunApp() - Failed"), MB_OK | MB_ICONERROR );
		return false;
	}
	mHwnd = hwnd;
	ShowWindow (mHwnd, SW_SHOW);
	UpdateWindow (mHwnd);

	// Initialize state-wide resources in the state logic objec.
	// does not display message box for error - delegated to state logic class.
	bool ok = mState->Initialize(mHwnd);
	return ok;
} // end Initialize function.


//===  Encapsulates calling of the windows message processing function.
//=== Calls the application state object Setup() function then
	//=== calls ProcessFrame() function every time around the messsage loop.
//=== The state object Cleanup() function is called when the message loop is terminated.
void WinCore::RunApp ()
{
	//--- Setup and start state-wide processing  cycle resources.
	if (!(mState->Setup())) {
		MessageBox (NULL, TEXT (" IState::Setup failed - application aborted."), TEXT ("WinCore::RunApp() - Failed"), MB_OK | MB_ICONERROR );
		return;
	}
	mTimer->Reset ();		// start timing.

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	//--- The Windows message processing loop.
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage (&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// get elapsed time for the current call.
			mTimer->Tick ();
			//--- call the frame processing function. ---
			mState->ProcessFrame (mTimer->GetDeltaTime());
		}
	} // end while loop.
	//--- clean up game-wide resources we have allocated.
	mState->Cleanup ();
} // end RunApp function.

/* Version history.

=== 2.2
Revised to reflect replacement of IGameCore with IState.
IState::Initialize() called at end of WinCore::Initialize().
IState::Setup() called at start of WinCore::RunApp(), before starting the message loop.
Other housekeeping and refactoring.
Removed all reference to 'game'.

=== 2.1
Start() renamed as RunApp().
Moved call to game->SetupGame() from Initialize() function to RunApp().

=== 2.0.2
Removed check for NULL before calling delete since the delete operator is well-defined for null references anyway.

*/

} // end AllanMilne namespace.