#include "Logger.h"
#include <TlHelp32.h>

StructLog Logger::LOGCFG;

Logger::Logger(LogType type)
{
	m_logType = type;
	if (LOGCFG.Headers)
	{
		m_finalText = "";
		m_finalText += "[ " + GetLabel(type) + " ] ";
	}
}


void Logger::StartMTail()
{
	if(IsMTailRunning())
	{
		Logger(ltINFO) << "MTail already running";
		return;
	}

	Logger(ltINFO) << "Starting MTail";

	std::string params = " \"" + LOGCFG.GeneratedLogFilepath + "\" /start";

	ShellExecuteA(nullptr, nullptr, LOGCFG.MTailFilepath.c_str(),
		params.c_str(), nullptr, SW_SHOWDEFAULT);

}

void Logger::ClearFile()
{
	std::fstream FStream;
	FStream.open(LOGCFG.GeneratedLogFilepath, std::ios_base::trunc | std::ios_base::out);
	FStream.clear();
	FStream.close();
}

std::string Logger::GetDateTime()
{
	auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::stringstream ss;
	tm t{ 0 };
	(void)localtime_s(&t, &time);
	ss << std::put_time(&t, "[ %d-%m-%Y %H:%M:%S ] ");
	return ss.str();
}

std::string Logger::GetLabel(const LogType type)
{
	std::string str;
	switch (type)
	{
	case ltDEBUG: str = "DEBUG";
		break;
	case ltINFO:	str = "INFO";
		break;
	case ltWARN:	str = "WARN";
		break;
	case ltERROR:	str = "ERROR";
		break;
	}

	return str;
}

bool Logger::IsMTailRunning()
{
	bool running = false;
	PROCESSENTRY32 entry;

	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry))
		while (Process32Next(snapshot, &entry))
			if (!_wcsicmp(entry.szExeFile, L"mTAIL.exe"))
				running = true;

	CloseHandle(snapshot);

	return running;
}
