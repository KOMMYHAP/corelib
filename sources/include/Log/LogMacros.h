#pragma once
#include "LogMessageSeverity.h"
#include "LogStream.h"
#include "FormatUtils/StandardFormat.h"

#define LOG_DEBUG \
	CORE_LOG_IMPL(::Core::LogMessageSeverity::Debug)
#define LOG_INFO \
	CORE_LOG_IMPL(::Core::LogMessageSeverity::Info)
#define LOG_WARNING \
	CORE_LOG_IMPL(::Core::LogMessageSeverity::Warning)
#define LOG_ERROR \
	CORE_LOG_IMPL(::Core::LogMessageSeverity::Error)

#define CORE_LOG_IMPL(severity) \
	::Core::LogStreamHandler((severity), __LINE__, __FILE__).GetStream