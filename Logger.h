#pragma once
#include <chrono>
#include <fstream>
#include <iostream>
#include <comdef.h>
#include <iomanip>
#include <string>

enum LogType
{
	ltDEBUG,
	ltINFO,
	ltWARN,
	ltERROR
};

// Logger config
struct StructLog
{
	bool Headers = false;
	LogType Filterlevel = ltERROR;
	std::string GeneratedLogFilepath;
	std::string MTailFilepath;
};


class Logger
{
public:
	Logger() {}
	virtual ~Logger()
	{
		m_opened = false;
	}

	Logger(LogType type);

	template<class T>
	Logger& operator<<(const T& msg);

	static void StartMTail();

	static void ClearFile();

	static StructLog LOGCFG;

private:

	std::string GetDateTime();

	std::string GetLabel(const LogType type);

	static bool IsMTailRunning();

private:
	LogType m_logType = ltDEBUG;
	bool m_opened = false;

	std::string m_finalText;

	std::string m_tailFilepath;
};

template <class T>
Logger& Logger::operator<<(const T& msg)
{
	if (LOGCFG.Filterlevel >= m_logType)
	{
		m_finalText += GetDateTime();
		m_finalText += msg;
		m_finalText += "\n";
		m_opened = true;

		std::fstream FStream;

		FStream.open(LOGCFG.GeneratedLogFilepath, std::ios_base::app);

		FStream.write(m_finalText.c_str(), m_finalText.length());

		FStream.close();
	}
	return *this;
}


