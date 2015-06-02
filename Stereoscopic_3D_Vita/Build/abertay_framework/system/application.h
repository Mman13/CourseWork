#ifndef _ABFW_APPLICATION_H
#define _ABFW_APPLICATION_H

namespace abfw
{
	// forward declarations
	class Platform;

	class Application
	{
	public:
		Application(Platform& platform);
		virtual ~Application();
		void Run();
		virtual void Init() = 0;
		virtual void CleanUp() = 0;
		virtual bool Update(float ticks) = 0;
		virtual void Render() = 0;

	protected:
		class Platform& platform_;
	private:
		bool running_;
	};
}

#endif // _ABFW_APPLICATION_H