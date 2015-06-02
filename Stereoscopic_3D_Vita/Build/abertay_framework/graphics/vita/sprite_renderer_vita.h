#ifndef _ABFW_SPRITE_RENDERER_VITA_H
#define _ABFW_SPRITE_RENDERER_VITA_H

#include <graphics/sprite_renderer.h>
#include <maths/matrix44.h>
#include <gxm.h>

namespace abfw
{
	// forward declarations
	class Platform;
	class TextureVita;

class SpriteRendererVita : public SpriteRenderer
{
public:
	SpriteRendererVita(const Platform& platform);
	~SpriteRendererVita();

	void Begin(bool clear);
	void End();
	void DrawSprite(const Sprite& sprite);
private:
	SceGxmShaderPatcherId colouredSpriteVertexProgramId;
	SceGxmShaderPatcherId colouredSpriteFragmentProgramId;
	SceUID colouredSpriteVerticesUid;
	SceUID colouredSpriteIndicesUid;
	SceGxmVertexProgram *colouredSpriteVertexProgram;
	SceGxmFragmentProgram *colouredSpriteFragmentProgram;
	struct SpriteVertex * colouredSpriteVertices;
	uint16_t * colouredSpriteIndices;
	const SceGxmProgramParameter *wvpParam;
	const SceGxmProgramParameter *spriteDataParam;


	// default texture
	TextureVita*	default_texture_;
};

}

#endif // _ABFW_SPRITE_RENDERER_VITA_H