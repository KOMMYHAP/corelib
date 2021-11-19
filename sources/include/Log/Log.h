#pragma once

#include "Log/LogMessageSeverity.h"

namespace Core
{
	struct LogMessageData;
	enum class LogMessageSeverity;
	class LogSink;

	class Log
	{
	public:
		using SinkPtr = std::shared_ptr<LogSink>;
		using Sinks = std::vector<SinkPtr>;

		Log(const Log &) = delete;
		Log(Log &&) = delete;

		static Log & Instance();

		void Message(LogMessageSeverity severity, std::string message, size_t line = 0, const char * file = nullptr);

		void AddSink(SinkPtr sink);
		void RemoveSink(SinkPtr sink);

	private:
		Log();

		void MessageImpl(LogMessageData messageData);

		Sinks				_sinks;
	};

	inline Log & GetLogger()
	{
		return Log::Instance();
	}
}
