#include "Log/DebuggerDisplaySink.h"

#include <sstream>
#include "Log/LogUtils.h"

#include "Windows.h"
#include "debugapi.h"

namespace Core
{
	void DebuggerDisplaySink::OnMessage(const LogMessageData& messageData)
	{
		std::stringstream ss;
		ss << messageData;
		auto message = ss.str();
		OutputDebugStringA(message.c_str());
	}
}
