#pragma once

#include <sstream>

#include "LogMessageSeverity.h"
#include "Log.h"
#include "FormatUtils/GenericFormat.h"

namespace Core
{
	class LogStream
	{
	public:
		void Open(LogMessageSeverity sevetiry, size_t line = 0, const char * file = nullptr)
		{
			_sevetiry = sevetiry;
			_line = line;
			_file = file;
		}

		void Close()
		{
			GetLogger().Message(_sevetiry, _stringstream.str(), _line, _file);
		}

		template <class T>
		friend LogStream & operator<<(LogStream & stream, const T & arg)
		{
			using namespace FormatUtils;
			Serialize(stream._stringstream, arg);
			return stream;
		}

	private:
		LogMessageSeverity	_sevetiry = LogMessageSeverity::Error;
		size_t				_line = 0;
		const char *		_file = nullptr;
		std::ostringstream _stringstream;
	};

	class LogStreamHandler
	{
	public:
		LogStreamHandler(LogMessageSeverity sevetiry, size_t line = 0, const char * file = nullptr)
		{
			_logStream.Open(sevetiry, line, file);
		}
		~LogStreamHandler()
		{
			_logStream.Close();
		}

		LogStreamHandler(const LogStreamHandler &) = delete;
		LogStreamHandler(LogStreamHandler &&) = delete;

		LogStream & GetStream()
		{
			return _logStream;
		}

	private:
		LogStream		_logStream;
	};
}
