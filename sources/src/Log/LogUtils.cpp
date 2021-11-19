#include "Log/LogUtils.h"
#include "Log/LogMessageData.h"

namespace Core
{
	std::ostream& operator<<(std::ostream & ostream, const LogMessageData& messageData)
	{
		auto time = std::chrono::system_clock::to_time_t(messageData.timepoint);
		
		ostream
			<< '[' << std::put_time(std::localtime(&time), "%T") << "] "
			<< '[' << messageData.severity << "] ";

		if (messageData.severity == LogMessageSeverity::Error &&
			messageData.file && messageData.line)
		{
			auto filename = std::filesystem::path(messageData.file).filename();
			
			ostream << '(' << filename << " on " << messageData.line << " line) ";
		}

		ostream << messageData.message << '\n';

		return ostream;
	}

	std::ostream& operator<<(std::ostream & ostream, LogMessageSeverity severity)
	{
		switch (severity)
		{
		case LogMessageSeverity::Debug:
			ostream << "Debug";
			break;
		case LogMessageSeverity::Info:
			ostream << "Info";
			break;
		case LogMessageSeverity::Warning:
			ostream << "Warning";
			break;
		case LogMessageSeverity::Error:
			ostream << "Error";
			break;
		default:
			ostream << "Unknown";
			break;
		}
		return ostream;
	}
}
