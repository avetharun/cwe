#include "utilities.h"


bool is_big_endian(void)
{
	union {
		uint32_t i;
		char c[4];
	} bint = { 0x01020304 };

	return bint.c[0] == 1;
}

int printf_if_verbose(char const* const _Format, ...) {
	if (getAppConfig().isVerbose) {
		int _Result;
		va_list _ArgList;
		va_start(_ArgList, _Format);
		_Result = vfprintf(stdout, _Format, _ArgList);
		va_end(_ArgList);
		return _Result;
	}
	return 0;
}


// Utilities specific for Windows
#ifdef _WIN32 

#include <Windows.h>
void HideConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

void ShowConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);
}

bool IsConsoleVisible()
{
	return ::IsWindowVisible(::GetConsoleWindow()) != FALSE;
}
#endif

int printlnf_if_verbose(char const* const _Format, ...) {
	if (getAppConfig().isVerbose) {
		int _Result;
		va_list _ArgList;
		va_start(_ArgList, _Format);
		_Result = vfprintf(stdout, _Format, _ArgList);
		va_end(_ArgList);
		printf("\n");
		return _Result;
	}
	return 0;
}
void EngineTimer::start() {
	START = std::chrono::steady_clock::now();
}

void EngineTimer::end() {
	END = std::chrono::steady_clock::now();
}
void EngineTimer::setStart(std::chrono::steady_clock::time_point s) {
	START = s;
}

void EngineTimer::setEnd(std::chrono::steady_clock::time_point e) {
	END = e;
}

long long EngineTimer::micros() { return std::chrono::duration<double, std::micro>(END - START).count(); }

double EngineTimer::millis() { return std::chrono::duration<double, std::milli>(END - START).count(); }

double EngineTimer::seconds() { return std::chrono::duration<double>(END - START).count(); }

double EngineTimer::minutes() { return std::chrono::duration<double, std::ratio<60>>(END - START).count(); }

long EngineTimer::hours() { return std::chrono::duration_cast<std::chrono::hours>(END - START).count(); }

long EngineTimer::days() { return std::chrono::duration_cast<std::chrono::days>(END - START).count(); }

std::string EngineTimer::formatted(TimeFormat format) {
	std::stringstream str;
	bool hasAnother = false;
	if (format & TimeFormat::DAYS)    { str << days() <<	hasAnother ? ":" : ""; hasAnother = true; }
	if (format & TimeFormat::HOURS)   { str << hours() <<	hasAnother ? ":" : ""; hasAnother = true; }
	if (format & TimeFormat::MINUTES) { str << hours() <<	hasAnother ? ":" : ""; hasAnother = true; }
	if (format & TimeFormat::SECONDS) { str << seconds() <<	hasAnother ? ":" : ""; hasAnother = true; }
	if (format & TimeFormat::MILLIS)  { str << millis() <<	hasAnother ? ":" : ""; hasAnother = true; }
	if (format & TimeFormat::MICROS)  { str << micros() <<	hasAnother ? ":" : ""; hasAnother = true; }
	return str.str();
}
