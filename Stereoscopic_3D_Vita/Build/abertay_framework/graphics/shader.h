#ifndef _ABFW_SHADER_H
#define _ABFW_SHADER_H

namespace abfw
{
	class Shader
	{
	public:
		Shader();
		virtual ~Shader();
		virtual void SetVertexFormat();
		virtual void SetUserParameters();
	};
}

#endif // _ABFW_SHADER_H