#pragma once

namespace Core
{
	void DebugBreak();
	
	void HandleAssertion(const std::string & message, int line, const char * file);

	using AssertHandler = std::function<void(const std::string & /*message*/, int /*line*/, const char * /*file*/)>;
	AssertHandler SetAssertionHandler(AssertHandler handler);

	void DefaultAssertionHandler(const std::string & message, int line, const char * file);
}