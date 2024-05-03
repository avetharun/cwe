#pragma once
#include <string>
#include <GL/glfw3.h>
#include <nlohmann/json/json.hpp>
#include <chrono>
extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include <malloc.h>
#include <LuaBridge/LuaBridge.h>
enum TimeFormat {
	HOURS = 1 << 0,
	MINUTES = 1 << 1,
	SECONDS = 1 << 2,
	MILLIS = 1 << 3,
	MICROS = 1 << 4,
	DAYS = 1 << 5
};


struct EngineTimer {
	std::chrono::steady_clock::time_point START;
	std::chrono::steady_clock::time_point END;
	void start();
	void end();
	void setStart(std::chrono::steady_clock::time_point);
	void setEnd(std::chrono::steady_clock::time_point);
	long long micros();
	double millis();
	double seconds();
	double minutes();
	long hours();
	long days();
	std::string formatted(TimeFormat fmt);
};

struct Time {
	EngineTimer* timeSinceStart = (EngineTimer*)malloc(sizeof(EngineTimer));
	EngineTimer* deltaTimer = (EngineTimer*)malloc(sizeof(EngineTimer));
};
struct GameAppConfig {
	bool useVsync;
	int windowWidth;
	int windowHeight;
	bool isBorderless;
	bool isResizable;
	bool isFullscreen;
	std::string windowName;
	bool isVerbose;
	bool enableMargin;
	nlohmann::json appconfig;
	struct appcfg_bin {
		std::string autoload_path;
		std::string saved_data_path;
	};
	appcfg_bin binary_appconfig;
};
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
GLFWwindow* getWindow();


namespace cw {
	namespace lua {
		std::unordered_map<std::string, std::vector<luabridge::LuaRef>>& getLuaEvents();

		Time getEngineTimeHolders();
		double getDeltaTime();
		double getEngineRunTime();
		double getEngineRunTimeMs();
		lua_State* get_lua_state();
		bool lua_exec(const char* s);
		template <typename ... Args>
		bool invokeEventsFor(std::string name, Args... args) {
			if (!getLuaEvents().contains(name)) { return false; }
			for (auto& hdlr : getLuaEvents().at(name)) {
				hdlr(args...);
			}
			return true;
		}

	}
}
bool is_big_endian(void);
GameAppConfig getAppConfig();
int printf_if_verbose( char const* const _Format, ...);
int printlnf_if_verbose(char const* const _Format, ...);
void HideConsole();
void ShowConsole();
bool IsConsoleVisible();

// Port of Neofetch's get_term. Supports windows, but only used for Unix. Needed to hide/show windows. This will spawn a new window and assign a handle to it.
//std::string getTerminalName() {
//	std::string term_name = "Unknown";
//	bool hasFoundTerm = false;
//	auto envTermProgram = getenv("TERM_PROGRAM");
//	auto ssh_ttyEnv = getenv("SSH_TTY");
//	auto ssh_connEnv = getenv("SSH_CONNECTION");
//	if (ssh_connEnv) {
//		if (!ssh_ttyEnv) {
//			return "SSH; Failed to get SSH env";
//		}
//		return ssh_ttyEnv;
//	}
//	if (getenv("WT_SESSION")) {
//		return "Windows Terminal";
//	}
//	else {
//#ifdef _WIN32
//		return "Command Prompt (assumed)";
//#endif
//	}
//	// Quick check for MacOS
//	if (envTermProgram) {
//		std::string TERM_PROGRAM = envTermProgram;
//		if (strncmp(TERM_PROGRAM.c_str(), "iTerm.app", TERM_PROGRAM.size())) {
//			return "iTerm";
//		}
//		if (strncmp(TERM_PROGRAM.c_str(), "Terminal.app", TERM_PROGRAM.size())) {
//			return "iOS Terminal";
//		}
//		if (strncmp(TERM_PROGRAM.c_str(), "Hyper", TERM_PROGRAM.size())) {
//			return "Hyper";
//		}
//	}
//#ifdef __unix__
//	printf("NOT IMPLEMENTED!!!!");
//	assert(0);
//#endif
//}
//#ifdef __unix__

//#endif


