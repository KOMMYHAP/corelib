#include "Asserts/Asserts.h"
#include "Log/Log.h"

namespace
{
	Core::AssertHandler		g_assertHandler = &Core::DefaultAssertionHandler;
}

namespace Core
{
	void DebugBreak()
	{
#ifdef _DEBUG
		__debugbreak();
#endif
	}

	void HandleAssertion(const std::string& message, int line, const char* file)
	{
		if (g_assertHandler)
		{
			g_assertHandler(message, line, file);
		}
	}

	AssertHandler SetAssertionHandler(AssertHandler handler)
	{
		return std::exchange(g_assertHandler, std::move(handler));
	}

	void DefaultAssertionHandler(const std::string& message, int line, const char* file)
	{
		GetLogger().Message(LogMessageSeverity::Error, message, line, file);
		DebugBreak();
	}
}
