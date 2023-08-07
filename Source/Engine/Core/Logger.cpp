#include "Logger.h"
#include "FileIO.h"
namespace meow
{
	Logger g_logger(LogLevel::Info, &std::cout, "log.txt");

	bool Logger::Log(LogLevel Loglevel, const std::string& filename, int line)
	{
		if (Loglevel < m_Loglevel) return false;
		switch (Loglevel)
		{
		case LogLevel::Info:
			*this << "Info: ";
			break;
		case LogLevel::Warning:
			*this << "Warning: ";
			break;
		case LogLevel::Error:
			*this << "Error: ";
			break;
		case LogLevel::Assert:
			*this << "Assert: ";
			break;
		default:
			break;
		}
		*this << getFileName(filename) << " (" << line << ")";
		return true;
	}

	
}