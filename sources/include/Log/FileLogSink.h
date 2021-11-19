#pragma once
#include "LogSink.h"

namespace Core
{
	class FileLogSink : public LogSink
	{
	public:
		void Init(const std::filesystem::path & filepath);
		void OnMessage(const LogMessageData& messageData) override;

	private:
		std::string			_filename;
	};
}
