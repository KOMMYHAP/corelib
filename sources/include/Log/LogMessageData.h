#pragma once
#include "LogMessageSeverity.h"

namespace Core
{
	struct LogMessageData
	{
		using TimePoint = std::chrono::system_clock::time_point;

		LogMessageSeverity		severity = LogMessageSeverity::Info;
		std::string				message;
		size_t					line = 0;			// may be 0
		const char *			file = nullptr;		// may be nullptr
		TimePoint				timepoint;			// may be zero for Log, but always filled for LogSink
	};
}
