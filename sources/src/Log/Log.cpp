#include "Log/Log.h"

#include "Log/LogMessageData.h"
#include "Log/LogSink.h"

namespace Core
{
	Log& Log::Instance()
	{
		static Log instance;
		return instance;
	}

	void Log::Message(LogMessageSeverity severity, std::string message, size_t line, const char* file)
	{
		LogMessageData messageData;
		messageData.message = std::move(message);
		messageData.severity = severity;
		messageData.file = file;
		messageData.line = line;
		messageData.timepoint = std::chrono::system_clock::now();
		MessageImpl(std::move(messageData));
	}

	void Log::AddSink(SinkPtr sink)
	{
		_sinks.emplace_back(std::move(sink));
	}

	void Log::RemoveSink(SinkPtr sink)
	{
		auto sinkIt = std::find(_sinks.begin(), _sinks.end(), sink);
		_sinks.erase(sinkIt);
	}

	Log::Log()
	{
	}

	void Log::MessageImpl(LogMessageData messageData)
	{
		for (const auto & logSink : _sinks)
		{
			logSink->OnMessage(messageData);
		}
	}
}
