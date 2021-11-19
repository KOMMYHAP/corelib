#pragma once
#include "LogSink.h"

namespace Core
{
	class DebuggerDisplaySink : public LogSink
	{
	public:
		void OnMessage(const LogMessageData& messageData) override;
	};
}
