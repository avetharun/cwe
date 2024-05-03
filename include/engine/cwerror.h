
#pragma once
#include <cstdint>
#include <stack>
#include <string>
#ifndef cwlib_cwl_error_handler_hpp
#define cwlib_cwl_error_handler_hpp
struct cwError {
	typedef void (*void_1pc_1i32_f)(const char*, uint32_t);
	// Note: CW_NONE does NOT mean it won't send! Use CW_SILENT for that!!
	enum cwWarnings : uint32_t {
		CW_NONE =		 0b00000000000000000000000000000000,
		CW_ERROR =		 0b00000000000000000000000000000100,
		CW_WARN =		 0b00000000000000000000000000001000,
		CW_MESSAGE =	 0b00000000000000000000000000010000,
		CW_VERBOSE =	 0b00000000000000000000000000100000,
		CW_SILENT =		 0b00000000000000000000000001000000,
		CW_DEBUG =		 0b00000000000000000000000010000000,       
		CW_RETURN__ =	 0b10000000000000000000000000000000
	};
	// warning ID to human readable
	static const char* wtoh(uint32_t w);
	static inline std::stack<cwWarnings> WarningTypeStack = {};
	static inline cwWarnings warningState{};
	static inline std::string errorStr = "";
	static inline bool debug_enabled = true;
	static const char* geterror();

	// Return the current error state, or change if argument 0 is not CW_NONE
	static uint32_t sstate(cwWarnings state = CW_RETURN__);
	// Return the current or topmost error state
	static uint32_t gstate();
	static void pstate(cwWarnings state);
	static void push(cwWarnings state);
	
	static void postate();
	static void pop();
	static void serror(const char* err);
	static void serrof(const char* fmt, va_list args);
	static void serrof(const char* fmt, ...);
	static inline void_1pc_1i32_f errorHandler = nullptr;
};


#endif