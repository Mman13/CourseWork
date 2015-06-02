#include <system/d3d11/platform_d3d11.h>
#include "s3d.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	// initialisation
	abfw::PlatformD3D11 platform(hInstance, 960, 544, false, true);

	S3D myApp(platform);
	myApp.Run();

	return 0;
}