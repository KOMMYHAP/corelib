#pragma once

namespace Core
{
	struct LogMessageData;

	class LogSink
	{
	public:
		virtual ~LogSink() = default;

		virtual void OnMessage(const LogMessageData & messageData) = 0;
	};
}
