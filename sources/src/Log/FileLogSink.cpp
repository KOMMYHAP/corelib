#include "Log/FileLogSink.h"

#include <fstream>
#include "Log/LogUtils.h"
#include "Asserts/AssertMacros.h"

namespace Core
{
	void FileLogSink::Init(const std::filesystem::path& filepath)
	{
		_filename = filepath.string();
		std::ofstream ofs(_filename);
		Verify2(ofs.is_open(), "Failed to open filename.");
	}

	void FileLogSink::OnMessage(const LogMessageData& messageData)
	{
		std::ofstream ofs(_filename, std::ios_base::app);
		if (ofs.is_open())
		{
			ofs << messageData;
		}
	}
}
