#include <graphics/vita/sprite_renderer_vita.h>
#include <graphics/vita/texture_vita.h>
#include <graphics/sprite.h>
#include <maths/matrix44.h>
#include <system/vita/platform_vita.h>
#include <gxt.h>
#include <libdbg.h>
#include <display.h>


extern const SceGxmProgram _binary_textured_sprite_v_gxp_start;
extern const SceGxmProgram _binary_textured_sprite_f_gxp_start;

void *graphicsAlloc(SceKernelMemBlockType type, uint32_t size, uint32_t alignment, uint32_t attribs, SceUID *uid);
void graphicsFree(SceUID uid);

namespace abfw
{
struct SpriteVertex
{
	Vector2 position;
};

SpriteRendererVita::SpriteRendererVita(const Platform& platform) :
SpriteRenderer(platform),
colouredSpriteVertexProgramId(NULL),
colouredSpriteFragmentProgramId(NULL),
colouredSpriteVerticesUid(0),
colouredSpriteIndicesUid(0),
colouredSpriteVertexProgram(NULL),
colouredSpriteFragmentProgram(NULL),
wvpParam(NULL),
spriteDataParam(NULL),
default_texture_(NULL)
{
	Int32 err = SCE_OK;

	const PlatformVita& platform_vita = static_cast<const PlatformVita&>(platform_);
	SceGxmShaderPatcher* shader_patcher = platform_vita.shader_patcher();

	// clear screen shader programs
	// use embedded GXP files
	const SceGxmProgram *const colouredSpriteVertexProgramGxp	= &_binary_textured_sprite_v_gxp_start;
	const SceGxmProgram *const colouredSpriteFragmentProgramGxp	= &_binary_textured_sprite_f_gxp_start;

	// register programs with the patcher
	err = sceGxmShaderPatcherRegisterProgram(shader_patcher, colouredSpriteVertexProgramGxp, &colouredSpriteVertexProgramId);
	SCE_DBG_ASSERT(err == SCE_OK);
	err = sceGxmShaderPatcherRegisterProgram(shader_patcher, colouredSpriteFragmentProgramGxp, &colouredSpriteFragmentProgramId);
	SCE_DBG_ASSERT(err == SCE_OK);


	/* ---------------------------------------------------------------------
		9. Create the programs and data for the spinning triangle

		We define the spinning triangle vertex format here and create the
		vertex and fragment program.

		The vertex and index data is static, we allocate and write the data
		here too.
	   --------------------------------------------------------------------- */

	// get attributes by name to create vertex format bindings
	// first retrieve the underlying program to extract binding information
	const SceGxmProgramParameter *paramBasicPositionAttribute = sceGxmProgramFindParameterByName(colouredSpriteVertexProgramGxp, "aPosition");
	SCE_DBG_ASSERT(paramBasicPositionAttribute && (sceGxmProgramParameterGetCategory(paramBasicPositionAttribute) == SCE_GXM_PARAMETER_CATEGORY_ATTRIBUTE));
	const SceGxmProgramParameter *paramBasicColorAttribute = sceGxmProgramFindParameterByName(colouredSpriteVertexProgramGxp, "aColor");
	SCE_DBG_ASSERT(paramBasicColorAttribute && (sceGxmProgramParameterGetCategory(paramBasicColorAttribute) == SCE_GXM_PARAMETER_CATEGORY_ATTRIBUTE));

	// create shaded triangle vertex format
	SceGxmVertexAttribute colouredSpriteVertexAttributes[1];
	SceGxmVertexStream colouredSpriteVertexStreams[1];
	colouredSpriteVertexAttributes[0].streamIndex = 0;
	colouredSpriteVertexAttributes[0].offset = 0;
	colouredSpriteVertexAttributes[0].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
	colouredSpriteVertexAttributes[0].componentCount = 3;
	colouredSpriteVertexAttributes[0].regIndex = sceGxmProgramParameterGetResourceIndex(paramBasicPositionAttribute);
	colouredSpriteVertexStreams[0].stride = sizeof(abfw::SpriteVertex);
	colouredSpriteVertexStreams[0].indexSource = SCE_GXM_INDEX_SOURCE_INDEX_16BIT;

	// create shaded triangle shaders
	err = sceGxmShaderPatcherCreateVertexProgram(
		shader_patcher,
		colouredSpriteVertexProgramId,
		colouredSpriteVertexAttributes,
		1,
		colouredSpriteVertexStreams,
		1,
		&colouredSpriteVertexProgram);
	SCE_DBG_ASSERT(err == SCE_OK);

	// blended fragment program
	SceGxmBlendInfo	blendInfo;
	blendInfo.colorFunc = SCE_GXM_BLEND_FUNC_ADD;
	blendInfo.alphaFunc = SCE_GXM_BLEND_FUNC_ADD;
	blendInfo.colorSrc = SCE_GXM_BLEND_FACTOR_SRC_ALPHA;
	blendInfo.colorDst = SCE_GXM_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	blendInfo.alphaSrc = SCE_GXM_BLEND_FACTOR_SRC_ALPHA;
	blendInfo.alphaDst = SCE_GXM_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	blendInfo.colorMask = SCE_GXM_COLOR_MASK_ALL;

	err = sceGxmShaderPatcherCreateFragmentProgram(
		shader_patcher,
		colouredSpriteFragmentProgramId,
		SCE_GXM_OUTPUT_REGISTER_FORMAT_UCHAR4,
		MSAA_MODE,
		&blendInfo,
		colouredSpriteVertexProgramGxp,
		&colouredSpriteFragmentProgram);
	SCE_DBG_ASSERT(err == SCE_OK);

	// find vertex uniforms by name and cache parameter information
	wvpParam = sceGxmProgramFindParameterByName(colouredSpriteVertexProgramGxp, "wvp");
	SCE_DBG_ASSERT(wvpParam && (sceGxmProgramParameterGetCategory(wvpParam) == SCE_GXM_PARAMETER_CATEGORY_UNIFORM));
	spriteDataParam = sceGxmProgramFindParameterByName(colouredSpriteVertexProgramGxp, "sprite_data");
	SCE_DBG_ASSERT(spriteDataParam && (sceGxmProgramParameterGetCategory(spriteDataParam) == SCE_GXM_PARAMETER_CATEGORY_UNIFORM));

	// create shaded triangle vertex/index data
	colouredSpriteVertices = (SpriteVertex *)graphicsAlloc(
		SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE,
		4*sizeof(SpriteVertex),
		4,
		SCE_GXM_MEMORY_ATTRIB_READ,
		&colouredSpriteVerticesUid);
	colouredSpriteIndices = (uint16_t *)graphicsAlloc(
		SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE,
		4*sizeof(uint16_t),
		2,
		SCE_GXM_MEMORY_ATTRIB_READ,
		&colouredSpriteIndicesUid);

	colouredSpriteVertices[0].position.x = -0.5f;
	colouredSpriteVertices[0].position.y = -0.5f;
	colouredSpriteVertices[1].position.x = 0.5f;
	colouredSpriteVertices[1].position.y = -0.5f;
	colouredSpriteVertices[2].position.x = -0.5f;
	colouredSpriteVertices[2].position.y = 0.5f;
	colouredSpriteVertices[3].position.x = 0.5f;
	colouredSpriteVertices[3].position.y = 0.5f;

	colouredSpriteIndices[0] = 0;
	colouredSpriteIndices[1] = 1;
	colouredSpriteIndices[2] = 2;
	colouredSpriteIndices[3] = 3;

	default_texture_ = static_cast<TextureVita*>(Texture::CreateCheckerTexture(16, 1, platform_vita));

	projection_matrix_.OrthographicFrustumVita(0, platform_vita.width(), 0, platform_vita.height(), -1, 1);

}

SpriteRendererVita::~SpriteRendererVita()
{
	delete default_texture_;

	const PlatformVita& platform_vita = static_cast<const PlatformVita&>(platform_);
	SceGxmShaderPatcher* shader_patcher = platform_vita.shader_patcher();


	sceGxmShaderPatcherReleaseFragmentProgram(shader_patcher, colouredSpriteFragmentProgram);
	sceGxmShaderPatcherReleaseVertexProgram(shader_patcher, colouredSpriteVertexProgram);
	graphicsFree(colouredSpriteIndicesUid);
	graphicsFree(colouredSpriteVerticesUid);

	sceGxmShaderPatcherUnregisterProgram(shader_patcher, colouredSpriteFragmentProgramId);
	sceGxmShaderPatcherUnregisterProgram(shader_patcher, colouredSpriteVertexProgramId);
}

void SpriteRendererVita::Begin(bool clear)
{
	const PlatformVita& platform_vita = static_cast<const PlatformVita&>(platform_);
	platform_vita.BeginScene();

	if(clear)
		platform_vita.Clear();

	SceGxmContext* context = platform_vita.context();

	// render the rotating triangle
	sceGxmSetVertexProgram(context, colouredSpriteVertexProgram);
	sceGxmSetFragmentProgram(context, colouredSpriteFragmentProgram);
}

void SpriteRendererVita::End()
{
	const PlatformVita& platform_vita = static_cast<const PlatformVita&>(platform_);
	platform_vita.EndScene();
}

void SpriteRendererVita::DrawSprite(const Sprite& sprite)
{
	Matrix44 sprite_data;
	BuildSpriteShaderData(sprite, sprite_data);
	
	const PlatformVita& platform_vita = static_cast<const PlatformVita&>(platform_);
	SceGxmContext* context = platform_vita.context();

	const SceGxmTexture* texture = NULL;
	if(sprite.texture())
		texture = &static_cast<const TextureVita*>(sprite.texture())->texture();
	else
		texture = &default_texture_->texture();

	// make textures wrap
	sceGxmTextureSetUAddrMode(const_cast<SceGxmTexture*>(texture), SCE_GXM_TEXTURE_ADDR_REPEAT);
	sceGxmTextureSetVAddrMode(const_cast<SceGxmTexture*>(texture), SCE_GXM_TEXTURE_ADDR_REPEAT);

	SCE_DBG_ASSERT(texture != NULL);
	sceGxmSetFragmentTexture(context, 0, texture);

	// set the vertex program constants
	void *vertexDefaultBuffer;
	sceGxmReserveVertexDefaultUniformBuffer(context, &vertexDefaultBuffer);
	sceGxmSetUniformDataF(vertexDefaultBuffer, wvpParam, 0, 16, (float*)&projection_matrix_);
	sceGxmSetUniformDataF(vertexDefaultBuffer, spriteDataParam, 0, 16, (float*)&sprite_data);

	// draw the sprite geometry
	sceGxmSetVertexStream(context, 0, colouredSpriteVertices);
	sceGxmDraw(context, SCE_GXM_PRIMITIVE_TRIANGLE_STRIP, SCE_GXM_INDEX_FORMAT_U16, colouredSpriteIndices, 4);
}

}