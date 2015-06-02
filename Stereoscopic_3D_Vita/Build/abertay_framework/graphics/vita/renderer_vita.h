#ifndef _RENDERER_VITA_H
#define _RENDERER_VITA_H

#include "framework.h"
#include "sprite_vita.h"
#include "matrix44.h"
#include <gxm.h>


/*	Define the width and height to render at the native resolution on ES2
	hardware.
*/
#define DISPLAY_WIDTH				960
#define DISPLAY_HEIGHT				544
#define DISPLAY_STRIDE_IN_PIXELS	1024

/*	Define the libgxm color format to render to.  This should be kept in sync
	with the display format to use with the SceDisplay library.
*/
#define DISPLAY_COLOR_FORMAT		SCE_GXM_COLOR_FORMAT_A8B8G8R8
#define DISPLAY_PIXEL_FORMAT		SCE_DISPLAY_PIXELFORMAT_A8B8G8R8

/*	Define the number of back buffers to use with this sample.  Most applications
	should use a value of 2 (double buffering) or 3 (triple buffering).
*/
#define DISPLAY_BUFFER_COUNT		3

/*	Define the maximum number of queued swaps that the display queue will allow.
	This limits the number of frames that the CPU can get ahead of the GPU,
	and is independent of the actual number of back buffers.  The display
	queue will block during sceGxmDisplayQueueAddEntry if this number of swaps
	have already been queued.
*/
#define DISPLAY_MAX_PENDING_SWAPS	2

/*	Define the MSAA mode.  This can be changed to 4X on ES1 hardware to use 4X
	multi-sample anti-aliasing, and can be changed to 4X or 2X on ES2 hardware.
*/
#define MSAA_MODE					SCE_GXM_MULTISAMPLE_NONE

/*	Set this macro to 1 to manually allocate the memblock for the render target.
*/
#define MANUALLY_ALLOCATE_RT_MEMBLOCK		0

// Helper macro to align a value
#define ALIGN(x, a)					(((x) + ((a) - 1)) & ~((a) - 1))



// Data structure for clear geometry
struct ClearVertex
{
	float x;
	float y;
};


/*	Data structure to pass through the display queue.  This structure is
	serialized during sceGxmDisplayQueueAddEntry, and is used to pass
	arbitrary data to the display callback function, called from an internal
	thread once the back buffer is ready to be displayed.

	In this example, we only need to pass the base address of the buffer.
*/
struct DisplayData
{
	void *address;
};

class RendererVita
{
public:
	RendererVita(Int32& return_value);
	~RendererVita();

	void Begin();
	void End();
	void DrawSprite(const SpriteVita* const sprite);

	SceGxmShaderPatcherId RegisterShaderProgram(const SceGxmProgram* program);
private:
	Int32 Init();
	void CleanUp();

	void InitGxmLibrary();
	void CreateRendereringContext();
	void CreateRenderTarget();
	void AllocateDisplayBuffers();
	void AllocateDepthBuffer();
	void CreateShaderPatcher();
	void InitClearScreen();
	void InitSpriteRenderer();


	SceUID vdmRingBufferUid;
	SceUID vertexRingBufferUid;
	SceUID fragmentRingBufferUid;
	SceUID fragmentUsseRingBufferUid;
	void* hostMem;
	SceGxmContext *context;
	SceGxmRenderTarget *renderTarget;
	SceUID displayBufferUid[DISPLAY_BUFFER_COUNT];
	void *displayBufferData[DISPLAY_BUFFER_COUNT];
	SceGxmSyncObject *displayBufferSync[DISPLAY_BUFFER_COUNT];
	SceGxmColorSurface displaySurface[DISPLAY_BUFFER_COUNT];
	SceUID depthBufferUid;
	SceGxmDepthStencilSurface depthSurface;
	SceGxmShaderPatcher *shaderPatcher;
	SceUID patcherVertexUsseUid;
	SceUID patcherFragmentUsseUid;
	SceUID patcherBufferUid;


	SceGxmShaderPatcherId clearVertexProgramId;
	SceGxmShaderPatcherId clearFragmentProgramId;
	SceUID clearVerticesUid;
	SceUID clearIndicesUid;
	SceGxmVertexProgram *clearVertexProgram;
	SceGxmFragmentProgram *clearFragmentProgram;
	ClearVertex * clearVertices;
	uint16_t * clearIndices;

	SceGxmShaderPatcherId colouredSpriteVertexProgramId;
	SceGxmShaderPatcherId colouredSpriteFragmentProgramId;
	SceUID colouredSpriteVerticesUid;
	SceUID colouredSpriteIndicesUid;
	SceGxmVertexProgram *colouredSpriteVertexProgram;
	SceGxmFragmentProgram *colouredSpriteFragmentProgram;
	SpriteVertex * colouredSpriteVertices;
	uint16_t * colouredSpriteIndices;
	const SceGxmProgramParameter *wvpParam;
	const SceGxmProgramParameter *spriteDataParam;

	uint32_t backBufferIndex;
	uint32_t frontBufferIndex;


	// default texture
	TextureVita*	default_texture_;
	// test texture
//	SceUID							testTextureUid;
//	uint8_t							*testTextureData;
//	SceGxmTexture					testTexture;

	Matrix44 wvp;
};

#endif // _RENDERER_VITA_H