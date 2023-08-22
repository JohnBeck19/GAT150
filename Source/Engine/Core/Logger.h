#pragma once
#include "Framework/Singleton.h"
#include <string>
#include <cassert>
#include <fstream>
#include <iostream>

#ifdef _DEBUG
#define INFO_LOG(msg)		{ if (meow::Logger::Instance().Log(meow::LogLevel::Info,__FILE__, __LINE__))		{meow::Logger::Instance() << msg << "\n";}}
#define WARNING_LOG(msg)	{ if (meow::Logger::Instance().Log(meow::LogLevel::Warning,__FILE__, __LINE__))	{meow::Logger::Instance()<< msg << "\n";}}
#define ERROR_LOG(msg)		{ if (meow::Logger::Instance().Log(meow::LogLevel::Error,__FILE__, __LINE__))		{meow::Logger::Instance() << msg << "\n";}}
#define ASSERT_LOG(condition, msg) { if ( !condition && meow::Logger::Instance().Log(meow::LogLevel::Assert,__FILE__, __LINE__))	{meow::Logger::Instance() << msg << "\n";} assert(condition); }
#else
#define INFO_LOG(msg) {}
#define WARNING_LOG(msg) {}
#define ERROR_LOG(msg) {}
#define ASSERT_LOG(msg) {}

#endif // _DEBUG



namespace meow 
{
	enum class LogLevel
	{
		Info,
		Warning,
		Error,
		Assert
	};
	class Logger : public Singleton<Logger>
	{
	public:
		Logger(LogLevel Loglevel = LogLevel::Info, std::ostream* ostream = &std::cout, const std::string& filename = "Log.txt") :
			m_Loglevel{Loglevel},
			m_ostream{ ostream }
		{
			if(!filename.empty()) m_fstream.open(filename);
		}

		bool Log(LogLevel Loglevel, const std::string& filename, int line);

		template<typename T>
		Logger& operator << (T value);

	private:
		LogLevel m_Loglevel;
		std::ostream* m_ostream = nullptr;
		std::ofstream m_fstream;
	
	};

	template<typename T>
	inline Logger& Logger::operator<<(T value)
	{
		if (m_ostream) *m_ostream << value;

		if (m_fstream.is_open()) {

			m_fstream << value;
			m_fstream.flush();
		}
		return *this;
	}
}
