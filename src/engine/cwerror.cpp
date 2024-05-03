#include "engine/cwerror.h"
#include <alib.h>

// Return the current or topmost error state


// Return the current error state, or change if argument 0 is not CW_NONE


// warning ID to human readable


static void_1pc_1i32_f onError = [](const char* errv, uint32_t errs) {
	const char* __wtoh = ((errs == cwError::CW_NONE) ? "" : cwError::wtoh(errs));
	const char* __sep = ((errs == cwError::CW_NONE) ? "" : "|");
	printf("%s%s%s\n", __wtoh, __sep, errv);
};
const char* cwError::wtoh(uint32_t w) {
	switch (w) {
	default: return "UNKNOWN";
	case CW_NONE: return "";
	case CW_ERROR: return "ERROR";
	case CW_WARN: return "WARN";
	case CW_MESSAGE: return "Log";
	case CW_VERBOSE: return "V";
	case CW_SILENT: return "SILENT";
	case CW_DEBUG: return "DEBUG";
	}
}

const char* cwError::geterror() {
	return errorStr.c_str();
}

uint32_t cwError::sstate(cwWarnings state) {
	if (state & CW_RETURN__) { return warningState; }
	warningState = state;
	return state;
}

uint32_t cwError::gstate() {
	return WarningTypeStack.size() == 0 ? warningState : WarningTypeStack.top();
}

void cwError::pstate(cwWarnings state) {
	WarningTypeStack.push(state);
}

void cwError::push(cwWarnings state) {
	pstate(state);
}

void cwError::postate() {
	WarningTypeStack.pop();
}

void cwError::pop() {
	postate();
}

void cwError::serror(const char* err) {
	// return if debugging isn't enabled, and are sending a debug message.
	if (!debug_enabled && (warningState & CW_DEBUG)) {
		return;
	}
	errorStr = (char*)err;
	if (errorHandler == nullptr) {
		errorHandler = onError;
	}
	cwError::errorHandler(err, sstate());
}

void cwError::serrof(const char* fmt, va_list args) {
	// return if debugging isn't enabled, and are sending a debug message.
	if (!debug_enabled && (warningState & CW_DEBUG)) {
		return;
	}
	errorStr = alib_strfmtv(fmt, args);
	if (errorHandler == nullptr) {
		errorHandler = onError;
	}
	cwError::errorHandler(errorStr.c_str(), gstate());
}

void cwError::serrof(const char* fmt, ...) {
    // return if debugging isn't enabled, and are sending a debug message.
    if (!debug_enabled && (warningState & CW_DEBUG)) {
        return;
    }
    errorStr.clear();
    va_list args;
    va_start(args, fmt);
    errorStr.append(alib_strfmtv(fmt, args));
    va_end(args);
	if (errorHandler == nullptr) {
		errorHandler = onError;
	}
    cwError::errorHandler(errorStr.c_str(), gstate());
}
