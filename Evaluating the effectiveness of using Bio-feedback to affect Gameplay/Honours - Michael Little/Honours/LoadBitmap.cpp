#include "LoadBitmap.h"

HBITMAP LoadABitmap(LPSTR szFileName)
{
	return (HBITMAP)LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); //controls the bitmap's properties
}
