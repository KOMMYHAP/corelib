#include "gtest/gtest.h"
#include "Log/Log.h"
#include "Log/LogMacros.h"
#include "Log/LogMessageData.h"
#include "Log/LogSink.h"

class HistoryLogSink : public Core::LogSink
{
public:
	void OnMessage(const Core::LogMessageData& messageData) override
	{
		_messageData.emplace_back(messageData);
	}

	const Core::LogMessageData * GetLastMessageData() const
	{
		if (_messageData.empty())
		{
			return nullptr;
		}
		return &_messageData.back();
	}
	
	const std::vector<Core::LogMessageData> & GetAllMessageData() const
	{
		return _messageData;
	}

	void Reset()
	{
		_messageData.clear();
	}

private:
	std::vector<Core::LogMessageData>	_messageData;
};

class LogTestFixture : public testing::Test
{
public:
	void SetUp() override
	{
		_logSink = std::make_shared<HistoryLogSink>();
		Core::GetLogger().AddSink(_logSink);
	}

	void TearDown() override
	{
		Core::GetLogger().RemoveSink(_logSink);
	}

	const auto & GetLogSink() const
	{
		return _logSink;
	}

	auto CreateStreamHandler()
	{
		return Core::LogStreamHandler(_severity, _line, _file);
	};

	size_t GetLine() const
	{
		return _line;
	}

	const char * GetFile() const
	{
		return _file;
	}

	auto GetSeverity() const
	{
		return _severity;
	}

private:
	std::shared_ptr<HistoryLogSink>		_logSink;
	Core::LogMessageSeverity			_severity = Core::LogMessageSeverity::Info;
	const size_t						_line = __LINE__;
	const char *						_file = __FILE__;
};

TEST(LogTest, LoggerInstance)
{
	auto * ptr1 = &Core::GetLogger();
	auto * ptr2 = &Core::GetLogger();

	EXPECT_EQ(ptr1, ptr2);

	ptr1 = &Core::Log::Instance();
	ptr2 = &Core::Log::Instance();
	EXPECT_EQ(ptr1, ptr2);

	EXPECT_EQ(&Core::Log::Instance(), &Core::GetLogger());
}

TEST(LogTest, LoggerAddAndRemoveSink)
{
	auto historyLogSink = std::make_shared<HistoryLogSink>();
	Core::GetLogger().AddSink(historyLogSink);

	ASSERT_EQ(historyLogSink->GetLastMessageData(), nullptr);
	Core::GetLogger().Message(Core::LogMessageSeverity::Info, "");
	EXPECT_NE(historyLogSink->GetLastMessageData(), nullptr);

	Core::GetLogger().RemoveSink(historyLogSink);
	
	historyLogSink->Reset();
	ASSERT_EQ(historyLogSink->GetLastMessageData(), nullptr);
	Core::GetLogger().Message(Core::LogMessageSeverity::Info, "");
	EXPECT_EQ(historyLogSink->GetLastMessageData(), nullptr);
}

TEST(LogTest, LoggerSendOneMessage)
{
	auto historyLogSink = std::make_shared<HistoryLogSink>();
	Core::GetLogger().AddSink(historyLogSink);

	int line = 42;
	const char * file = "LogTest.cpp";
	std::string message = "Test";
	auto severity = Core::LogMessageSeverity::Info;
	auto timeNow = Core::LogMessageData::TimePoint::clock::now();

	ASSERT_EQ(historyLogSink->GetLastMessageData(), nullptr);
	Core::GetLogger().Message(severity, message, line, file);
	ASSERT_NE(historyLogSink->GetLastMessageData(), nullptr);

	auto & data = *historyLogSink->GetLastMessageData();
	EXPECT_EQ(data.line, static_cast<size_t>(line));
	EXPECT_EQ(data.file, file);
	EXPECT_EQ(std::string_view(data.file), std::string_view(file));
	EXPECT_EQ(data.message, message);
	EXPECT_EQ(data.severity, severity);
	EXPECT_NE(data.timepoint.time_since_epoch(), std::chrono::system_clock::duration::zero());
	EXPECT_GE(data.timepoint, timeNow);
}

TEST(LogTest, LoggerSendMoreMessages)
{
	auto historyLogSink = std::make_shared<HistoryLogSink>();
	Core::GetLogger().AddSink(historyLogSink);

	Core::LogMessageData originMessage;
	originMessage.message = "Test";
	originMessage.file = "LogTest.cpp";
	originMessage.line = 42;
	originMessage.severity = Core::LogMessageSeverity::Info;
	originMessage.timepoint = Core::LogMessageData::TimePoint::clock::now();

	auto CheckMessage = [&](const Core::LogMessageData & receivedMessage)
	{
		return std::string_view(originMessage.file) == std::string_view(receivedMessage.file) &&
			originMessage.line == receivedMessage.line &&
			originMessage.message == receivedMessage.message &&
			originMessage.severity == receivedMessage.severity &&
			receivedMessage.timepoint >= originMessage.timepoint;
	};
	
	const size_t n = 10;
	for (size_t i = 0; i < n; ++i)
	{
		Core::GetLogger().Message(originMessage.severity, originMessage.message, originMessage.line, originMessage.file);

		auto & history = historyLogSink->GetAllMessageData();
		EXPECT_EQ(history.size(), i + 1);
		EXPECT_EQ(CheckMessage(history[i]), true);
	}
}

TEST_F(LogTestFixture, LogStreamHandler)
{
	auto timepoint = Core::LogMessageData::TimePoint::clock::now();
	CreateStreamHandler().GetStream() << "Hello, my dear friend";
	auto * message = GetLogSink()->GetLastMessageData();
	EXPECT_NE(message, nullptr);
	EXPECT_EQ(message->message, "Hello, my dear friend");
	EXPECT_EQ(message->severity, GetSeverity());
	EXPECT_EQ(message->file, GetFile());
	EXPECT_EQ(message->line, GetLine());
	EXPECT_GT(message->timepoint, timepoint);
}

TEST_F(LogTestFixture, LogStreamHandlerManyArgs)
{
	CreateStreamHandler().GetStream() << "Hello" << ',' << " my dear " << std::string_view("friend");
	auto * message = GetLogSink()->GetLastMessageData();
	EXPECT_NE(message, nullptr);
	EXPECT_EQ(message->message, "Hello, my dear friend");
}

