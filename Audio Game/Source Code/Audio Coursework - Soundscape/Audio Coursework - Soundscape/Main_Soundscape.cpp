/*
	File name:	Main_Soundscape.cpp
	Version:	1.0
	Date:		15th February 2014.
	Author:		Michael Little (Based on and utilises code from Main_Walking by Allan C. Milne)


	Uses:	WinCore, IState, XACore, SoundscapeControl.

	Description:
	A main code file containing the Windows application entry point
	for use with windows/audio framework.
	*	See WinCore.hpp for details of the windows abstraction.
	*	see IState.hpp for details of the frame processing abstraction.
	*	see XACore.hpp for details of the audio engine abstraction.

*/
// system includes
#include <windows.h>
#include <memory>

// Framework includes
#include "Framework\WinCore.hpp"
#include "Framework\IState.hpp"
#include "Framework\XACore.hpp"
#include "Framework\waveFileManager.hpp"
using namespace AllanMilne;
using namespace AllanMilne::Audio;

// Application specific includes
#include "SoundscapeControl.hpp"

/////////////////////////////
// Application Entry point //
////////////////////////////////////////////////////////////////
int WINAPI WinMain (HINSTANCE hinstance, HINSTANCE prevInstance, 
				   PSTR cmdLine, int showCmd)
{
	std::unique_ptr<WinCore> windowApp;
	std::unique_ptr<IState> frameProcessor;

	// Create the singleton XACore object that initializes XAudio2 and X3DAudio.
	// Must do this before creating/initializing other objects since these may require XAudio2 functionality in creating sounds, etc.
	XACore::CreateInstance();
	if (XACore::GetStatus() != XACore::OK) 
	{
		MessageBox (NULL, TEXT ("Error initializing XAudio2 - application aborted."), TEXT ("Main Application"), MB_OK | MB_ICONERROR );
		return 0;
	}

	// create the object that encapsulates frame processing.
	frameProcessor.reset(new SoundscapeControl());

	// Create the WinCore object composed with the frame processor.
	windowApp.reset(new WinCore (frameProcessor.get()));

	// Initialize the WinCore object including creating the application window;
	// this will also call the IState::Initialize() method of the frame processor.
	bool ok = windowApp->Initialize (
				" Audio Coursework - Soundscape ",		// Windows title bar text.
				800, 600,			// width x height
				false,				// use full screen; change to true if windowed is required.
				hinstance );
	if (!ok) {
		MessageBox (NULL, TEXT ("Error occurred while initializing WinCore; application aborted."), TEXT (" Main Application"), MB_OK | MB_ICONERROR );
		return 0;
	}

	// Run the application Windows messsage loop and associated frame processing.
	windowApp->RunApp();


	///////////////////////
	// Delete resources ////////////////////////////////
	// NOTE. delete in the opposite order of creation ///////////////////////////
	// delete the WinCore instance explicitly before we delete other resources //
	/////////////////////////////////////////////////////////////////////////////
	windowApp.release();
	
	// as above, delete the IState object.
	frameProcessor.release();
	
	// Delete the XACore singleton instance - will clear up all XAudio2 resources.
	XACore::DeleteInstance();
	
	// Delete the WaveFileManager Instance to release all PCMWave objects that might have been created.
	WaveFileManager::DeleteInstance();
	//////////////////////////////////////////////////////////////////////////////

	return 0;
} // end WinMain function.

// end of code.