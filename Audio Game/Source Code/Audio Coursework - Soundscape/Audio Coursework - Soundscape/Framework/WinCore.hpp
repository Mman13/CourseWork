/*
	File:	winCore.hpp
	Version:	2.2
	Date: 22nd April 2013; August 2012; 2006.
	Author:	Allan C. Milne

	Namespace:	AllanMilne
	Exposes:	WinCore.
	Requires:	IState, Gametimer.

	Description:
	A class encapsulating a basic Windows framework
	with a strategy pattern used to incorporate application state processing.

	You should instantiate an object of this WinCore class,
	passing the pointer to an object of a class implementing IState as an argument to the constructor.

*/

#ifndef __WINCORE_HPP__
#define __WINCORE_HPP__

// system includes.
#include <windows.h>
#include <string>
using std::string;

// forward declaration of non-namespaced component.
class GameTimer;

namespace AllanMilne {

// forward declaration of AllanMilne namespace component.
	class IState;
	
class WinCore 
{
public:

	//--- constructor requires an IState object pointer for the strategy pattern.
	WinCore (IState*aState);
	//--- destructor should call the application state cleanup function.
	virtual ~WinCore ();

	//--- Create the application window; 
	//--- calls the application state object Initialize() function to create state-wide specific resources.
	//--- Returns false if any errors detected.
	bool Initialize (
		const string &aTitle,		// Text to display in title bar.
		int aWidth,					// width of back-buffer window.
		int aHeight,				// height of back-buffer window.
		bool isWindowed,			// true=windowed; false=full screen.
		HINSTANCE anHInstance		// handle to calling process.
	);

	//--- Run the application windows message processing loop.
	//--- Initialy calls the state object Setup() function; then ProcessFrame() function every time around the loop.
	//--- The state object Cleanup() function is called when the loop terminates.
	void RunApp ();


private:

	HWND mHwnd;				// Handle to current window.
	GameTimer *mTimer;		// Pointer to a timing class for use in calculating delta times between frames.
	IState*mState;			// pointer to state processing object.

}; // end WinCore class.

} // end AllanMilne namespace.

#endif