#pragma once
#include "LogMessageData.h"

namespace Core
{
	std::ostream & operator<<(std::ostream & ostream, const LogMessageData & messageData);
	std::ostream & operator<<(std::ostream & ostream, LogMessageSeverity severity);
}
