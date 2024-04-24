#pragma once
#include <cstdarg>
#include <vadefs.h>
#include <cstdio>
#include <string>
struct GameAppConfig {
	bool useVsync;
	int windowWidth;
	int windowHeight;
	bool isBorderless;
	bool isResizable;
	std::string windowName;
	bool isVerbose;
	bool enableMargin;
};
bool is_big_endian(void)
{
	union {
		uint32_t i;
		char c[4];
	} bint = { 0x01020304 };

	return bint.c[0] == 1;
}
GameAppConfig getAppConfig();
int printf_if_verbose( char const* const _Format, ...) {
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
void HideConsole();
void ShowConsole();
bool IsConsoleVisible();

// Port of Neofetch's get_term. Supports windows, but only used for Unix. Needed to hide/show windows. This will spawn a new window and assign a handle to it.
std::string getTerminalName() {
	std::string term_name = "Unknown";
	bool hasFoundTerm = false;
	auto envTermProgram = getenv("TERM_PROGRAM");
	auto ssh_ttyEnv = getenv("SSH_TTY");
	auto ssh_connEnv = getenv("SSH_CONNECTION");
	if (ssh_connEnv) {
		if (!ssh_ttyEnv) {
			return "SSH; Failed to get SSH env";
		}
		return ssh_ttyEnv;
	}
	if (getenv("WT_SESSION")) {
		return "Windows Terminal";
	}
	else {
#ifdef _WIN32
		return "Command Prompt (assumed)";
#endif
	}
	// Quick check for MacOS
	if (envTermProgram) {
		std::string TERM_PROGRAM = envTermProgram;
		if (strncmp(TERM_PROGRAM.c_str(), "iTerm.app", TERM_PROGRAM.size())) {
			return "iTerm";
		}
		if (strncmp(TERM_PROGRAM.c_str(), "Terminal.app", TERM_PROGRAM.size())) {
			return "iOS Terminal";
		}
		if (strncmp(TERM_PROGRAM.c_str(), "Hyper", TERM_PROGRAM.size())) {
			return "Hyper";
		}
	}
#ifdef __unix__
	printf("NOT IMPLEMENTED!!!!");
	assert(0);
#endif
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

//#ifdef __unix__

//#endif