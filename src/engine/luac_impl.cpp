
extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "engine/sysinput.h"
#include "engine/binparse.h"
#include "engine/cwerror.h"
#include "engine/physics.h"
#include <GLFW/glfw3.h>

#include <LuaBridge/LuaBridge.h>
#include <LuaBridge/Vector.h>
#include <LuaBridge/Array.h>
#include <LuaBridge/Map.h>
#include <LuaBridge/UnorderedMap.h>
#include <LuaBridge/Optional.h>
#include <LuaBridge/List.h>
#include <utilities.h>
#include <engine/cwabt.h>

std::unordered_map<std::string, std::vector<luabridge::LuaRef>> LUA_EVENTS;

std::unordered_map<std::string, std::vector<luabridge::LuaRef>> & const cw::lua::getLuaEvents() { return LUA_EVENTS; }
void assignInputData(luabridge::Namespace globalNS);
static void _lua_print(std::string input) {
	cwError::serror(input.c_str());
}
static std::string g_keyToName(int keycode) { 
	return glfwGetKeyName(keycode, 0);
}
lua_State *L = luaL_newstate();
void addEventListener(std::string name, luabridge::LuaRef ref) {
	if (!ref.isFunction()) {
		std::string t_name = lua_typename(L, ref.type());
		luaL_error(L, "Expected function, found %s", t_name.c_str());
	}
	printlnf_if_verbose("Added 1 Event Listener to %s", name.c_str());
	if (LUA_EVENTS.contains(name)) {
		LUA_EVENTS.at(name).push_back(ref);
	}
	else {
		std::vector<luabridge::LuaRef> rV;
		rV.push_back(ref);
		LUA_EVENTS.emplace(name, rV);
	}
}
void removeEventListener(std::string name, luabridge::LuaRef ref) {
	if (!ref.isFunction()) {
		std::string t_name = lua_typename(L, ref.type());
		luaL_error(L, "Expected function, found %s", t_name.c_str());
	}
	printlnf_if_verbose("Removed 1 Event Listener from %s", name.c_str());
	std::vector<luabridge::LuaRef> vec;
	std::vector<luabridge::LuaRef>::iterator iter;
	if (LUA_EVENTS.contains(name)) {
		vec = LUA_EVENTS.at(name);
	}
	// edge case: if the event is already registered twice, this won't remove that one.
	while ((iter = vec.begin()) != vec.end()) {
		if (iter->m_ref == ref.m_ref) {
			vec.erase(iter);
			return;
		}
		iter++;
	}
}
void removeEventListenersFor(std::string name) {
	printlnf_if_verbose("Removed all Event Listeners for %s", name.c_str());
	if (LUA_EVENTS.contains(name)) {
		LUA_EVENTS.erase(name);
	}
}
std::vector<luabridge::LuaRef> getEventListenersFor(std::string name) {
	if (LUA_EVENTS.contains(name)) {
		return LUA_EVENTS.at(name);
	}
	return {};
}
template <auto name = "none">
void addEvent_T(luabridge::LuaRef ref) {
	addEventListener(name, ref);
}
luabridge::Namespace _G() { return luabridge::getGlobalNamespace(L); };
void impl_lua() {
	if (!L) { L = luaL_newstate(); }
	luaL_openlibs(L);
	luaopen_base(L);
	_G().addFunction("print", &_lua_print);

	_G().beginClass<ABTDataStub>("Json")
		.addFunction("setFloat", &ABTDataStub::set_float)
		.addFunction("addFloat", &ABTDataStub::add_float)
		.addFunction("getFloat", &ABTDataStub::get_float)
		.addFunction("setInt", &ABTDataStub::set_int)
		.addFunction("addInt", &ABTDataStub::add_int)
		.addFunction("getInt", &ABTDataStub::get_int)
		.addFunction("setString", &ABTDataStub::set_string)
		.addFunction("addString", &ABTDataStub::add_string)
		.addFunction("getString", &ABTDataStub::get_string)
		.addFunction("setBool", &ABTDataStub::set_bool)
		.addFunction("addBool", &ABTDataStub::add_bool)
		.addFunction("getBool", &ABTDataStub::get_bool)
		.addFunction("setCompound", &ABTDataStub::set_compound)
		.addFunction("addCompound", &ABTDataStub::add_compound)
		.addFunction("getCompound", &ABTDataStub::get_compound)
		.addFunction("clear", &ABTDataStub::clear)
		.addFunction("keys", &ABTDataStub::get_keys)
		.addFunction("contains", &ABTDataStub::m_contains)
		.addFunction("dump", &ABTDataStub::dump)
		.addConstructor<ABTDataStub(*)()>()
		.endClass();
	_G().beginNamespace("Serialization")
		.beginClass<ABT_RW<1>>("Text")
			.addData("data", &ABT_RW<1>::data)
			.addFunction("write_file", &ABT_RW<1>::compile)
			.addFunction("read_file", &ABT_RW<1>::decompile)
			.addFunction("dump", &ABT_RW<1>::getbuffer)
			.endClass()
		.beginClass<ABT_RW<2>>("Binary")
			.addData("data", &ABT_RW<2>::data)
			.addFunction("write_file", &ABT_RW<2>::compile)
			.addFunction("read_file", &ABT_RW<2>::decompile)
			.addFunction("dump", &ABT_RW<2>::getbuffer)
			.endClass()
		.beginClass<cw::BinarySerializer::Writer>("BufferedWriter")
			.addFunction("swap", &cw::BinarySerializer::Writer::to_reader)
			.addFunction("put_json", &cw::BinarySerializer::Writer::write_json)
			.addFunction("put_bson", &cw::BinarySerializer::Writer::write_bson)
			.addFunction("put_compound", &cw::BinarySerializer::Writer::write_compound)
			.addFunction("put_ubyte", &cw::BinarySerializer::Writer::write_ubyte)
			.addFunction("put_uchar", &cw::BinarySerializer::Writer::write_ubyte)
			.addFunction("put_ulong", &cw::BinarySerializer::Writer::write_ulong)
			.addFunction("put_ushort", &cw::BinarySerializer::Writer::write_ushort)
			.addFunction("put_uint", &cw::BinarySerializer::Writer::write_uint)
			.addFunction("put_sbyte", &cw::BinarySerializer::Writer::write_sbyte)
			.addFunction("put_schar", &cw::BinarySerializer::Writer::write_sbyte)
			.addFunction("put_slong", &cw::BinarySerializer::Writer::write_slong)
			.addFunction("put_sshort", &cw::BinarySerializer::Writer::write_sshort)
			.addFunction("put_sint", &cw::BinarySerializer::Writer::write_sint)
			.addFunction("put_bool", &cw::BinarySerializer::Writer::write_class<bool>)
			.addFunction("put_float", &cw::BinarySerializer::Writer::write_float)
			.addFunction("put_string", &cw::BinarySerializer::Writer::write_string)
			.endClass()
		.beginClass<cw::BinarySerializer::Reader>("BufferedReader")
			.addFunction("swap", &cw::BinarySerializer::Reader::to_writer)
			.addFunction("read_json", &cw::BinarySerializer::Reader::read_json)
			.addFunction("read_bson", &cw::BinarySerializer::Reader::read_bson)
			.addFunction("read_compound", &cw::BinarySerializer::Reader::read_compound)
			.addFunction("read_ubyte", &cw::BinarySerializer::Reader::read_ubyte)
			.addFunction("read_uchar", &cw::BinarySerializer::Reader::read_ubyte)
			.addFunction("read_ulong", &cw::BinarySerializer::Reader::read_ulong)
			.addFunction("read_ushort", &cw::BinarySerializer::Reader::read_ushort)
			.addFunction("read_uint", &cw::BinarySerializer::Reader::read_uint)
			.addFunction("read_sbyte", &cw::BinarySerializer::Reader::read_sbyte)
			.addFunction("read_schar", &cw::BinarySerializer::Reader::read_sbyte)
			.addFunction("read_slong", &cw::BinarySerializer::Reader::read_slong)
			.addFunction("read_sshort", &cw::BinarySerializer::Reader::read_sshort)
			.addFunction("read_sint", &cw::BinarySerializer::Reader::read_sint)
			.addFunction("read_bool", &cw::BinarySerializer::Reader::read_class<bool>)
			.addFunction("read_float", &cw::BinarySerializer::Reader::read_float)
			.addFunction("read_string", &cw::BinarySerializer::Reader::read_string)
			.endClass()
		.endNamespace();

	_G().beginClass<EngineTimer>("Timer")
		.addConstructor<void(*)(void)>()
		.addFunction("begin", &EngineTimer::start)
		.addFunction("end", &EngineTimer::end)
		.addFunction("seconds", &EngineTimer::seconds)
		.addFunction("milliseconds", &EngineTimer::millis)
		.addFunction("microseconds", &EngineTimer::micros)
		.addFunction("minutes", &EngineTimer::minutes)
		.addFunction("hours", &EngineTimer::hours)
		.addFunction("days", &EngineTimer::days)
		.addFunction("formatted", &EngineTimer::formatted)
		.endClass();
	_G().beginNamespace("Time")
		.addProperty("deltaTime",	cw::lua::getDeltaTime)
		.addProperty("runTime",		cw::lua::getEngineRunTime)
	.endNamespace();
	_G().beginNamespace("Event")
		.addFunction("getEventListeners", getEventListenersFor)
		.addFunction("addEventListener", addEventListener)
		.addFunction("removeEventListener", removeEventListener)
		.addFunction("removeEventListenersFor", removeEventListenersFor)
	.endNamespace();
	assignInputData(_G());
}
#define __asl_define_inputmodu(name, key) .addConstant(#name, key).addConstant(#key, key)
void assignInputData(luabridge::Namespace globalNS) {
	globalNS.beginNamespace("input")
		//.addFunction("on_key_press", &addEvent_T<"on_key_press">)
		//.addFunction("on_key_release", &addEvent_T<"on_key_release">)
		//.addFunction("on_key_hold", &addEvent_T<"on_key_hold">)
		.addFunction("key_to_name", g_keyToName)
		.addFunction("key_down", cw::InputManagement::Keyboard::GetKey)
		.addFunction("key_pressed", cw::InputManagement::Keyboard::GetKeyPressed)
#pragma region(glfw_keys)
		__asl_define_inputmodu(Key_a, GLFW_KEY_A)
		__asl_define_inputmodu(Key_b, GLFW_KEY_B)
		__asl_define_inputmodu(Key_c, GLFW_KEY_C)
		__asl_define_inputmodu(Key_d, GLFW_KEY_D)
		__asl_define_inputmodu(Key_e, GLFW_KEY_E)
		__asl_define_inputmodu(Key_f, GLFW_KEY_F)
		__asl_define_inputmodu(Key_g, GLFW_KEY_G)
		__asl_define_inputmodu(Key_h, GLFW_KEY_H)
		__asl_define_inputmodu(Key_i, GLFW_KEY_I)
		__asl_define_inputmodu(Key_j, GLFW_KEY_J)
		__asl_define_inputmodu(Key_k, GLFW_KEY_K)
		__asl_define_inputmodu(Key_l, GLFW_KEY_L)
		__asl_define_inputmodu(Key_m, GLFW_KEY_M)
		__asl_define_inputmodu(Key_n, GLFW_KEY_N)
		__asl_define_inputmodu(Key_o, GLFW_KEY_O)
		__asl_define_inputmodu(Key_p, GLFW_KEY_P)
		__asl_define_inputmodu(Key_q, GLFW_KEY_Q)
		__asl_define_inputmodu(Key_r, GLFW_KEY_R)
		__asl_define_inputmodu(Key_s, GLFW_KEY_S)
		__asl_define_inputmodu(Key_t, GLFW_KEY_T)
		__asl_define_inputmodu(Key_u, GLFW_KEY_U)
		__asl_define_inputmodu(Key_v, GLFW_KEY_V)
		__asl_define_inputmodu(Key_w, GLFW_KEY_W)
		__asl_define_inputmodu(Key_x, GLFW_KEY_X)
		__asl_define_inputmodu(Key_y, GLFW_KEY_Y)
		__asl_define_inputmodu(Key_z, GLFW_KEY_Z)

		__asl_define_inputmodu(Key_A, GLFW_KEY_A)
		__asl_define_inputmodu(Key_B, GLFW_KEY_B)
		__asl_define_inputmodu(Key_C, GLFW_KEY_C)
		__asl_define_inputmodu(Key_D, GLFW_KEY_D)
		__asl_define_inputmodu(Key_E, GLFW_KEY_E)
		__asl_define_inputmodu(Key_F, GLFW_KEY_F)
		__asl_define_inputmodu(Key_G, GLFW_KEY_G)
		__asl_define_inputmodu(Key_H, GLFW_KEY_H)
		__asl_define_inputmodu(Key_I, GLFW_KEY_I)
		__asl_define_inputmodu(Key_J, GLFW_KEY_J)
		__asl_define_inputmodu(Key_K, GLFW_KEY_K)
		__asl_define_inputmodu(Key_L, GLFW_KEY_L)
		__asl_define_inputmodu(Key_M, GLFW_KEY_M)
		__asl_define_inputmodu(Key_N, GLFW_KEY_N)
		__asl_define_inputmodu(Key_O, GLFW_KEY_O)
		__asl_define_inputmodu(Key_P, GLFW_KEY_P)
		__asl_define_inputmodu(Key_Q, GLFW_KEY_Q)
		__asl_define_inputmodu(Key_R, GLFW_KEY_R)
		__asl_define_inputmodu(Key_S, GLFW_KEY_S)
		__asl_define_inputmodu(Key_T, GLFW_KEY_T)
		__asl_define_inputmodu(Key_U, GLFW_KEY_U)
		__asl_define_inputmodu(Key_V, GLFW_KEY_V)
		__asl_define_inputmodu(Key_W, GLFW_KEY_W)
		__asl_define_inputmodu(Key_X, GLFW_KEY_X)
		__asl_define_inputmodu(Key_Y, GLFW_KEY_Y)
		__asl_define_inputmodu(Key_Z, GLFW_KEY_Z)

		__asl_define_inputmodu(Key_1, GLFW_KEY_1)
		__asl_define_inputmodu(Key_2, GLFW_KEY_2)
		__asl_define_inputmodu(Key_3, GLFW_KEY_3)
		__asl_define_inputmodu(Key_4, GLFW_KEY_4)
		__asl_define_inputmodu(Key_5, GLFW_KEY_5)
		__asl_define_inputmodu(Key_6, GLFW_KEY_6)
		__asl_define_inputmodu(Key_7, GLFW_KEY_7)
		__asl_define_inputmodu(Key_8, GLFW_KEY_8)
		__asl_define_inputmodu(Key_9, GLFW_KEY_9)
		__asl_define_inputmodu(Key_0, GLFW_KEY_0)

		__asl_define_inputmodu(Key_return, GLFW_KEY_ENTER)
		__asl_define_inputmodu(Key_enter, GLFW_KEY_ENTER)
		__asl_define_inputmodu(Key_escape, GLFW_KEY_ESCAPE)
		__asl_define_inputmodu(Key_backspace, GLFW_KEY_BACKSPACE)
		__asl_define_inputmodu(Key_tab, GLFW_KEY_TAB)
		__asl_define_inputmodu(Key_space, GLFW_KEY_SPACE)
		__asl_define_inputmodu(Key_minus, GLFW_KEY_MINUS)
		__asl_define_inputmodu(Key_equals, GLFW_KEY_EQUAL)
		__asl_define_inputmodu(Key_leftbracket, GLFW_KEY_LEFT_BRACKET)
		__asl_define_inputmodu(Key_rightbracket, GLFW_KEY_RIGHT_BRACKET)
		__asl_define_inputmodu(Key_backslash, GLFW_KEY_BACKSLASH)
		__asl_define_inputmodu(Key_forwardslash, GLFW_KEY_SLASH)
		__asl_define_inputmodu(Key_slash, GLFW_KEY_SLASH)
		__asl_define_inputmodu(Key_semicolon, GLFW_KEY_SEMICOLON)
		__asl_define_inputmodu(Key_apostrophe, GLFW_KEY_APOSTROPHE)
		__asl_define_inputmodu(Key_grave, GLFW_KEY_GRAVE_ACCENT)
		__asl_define_inputmodu(Key_comma, GLFW_KEY_COMMA)
		__asl_define_inputmodu(Key_period, GLFW_KEY_PERIOD)
		__asl_define_inputmodu(Key_capslock, GLFW_KEY_CAPS_LOCK)
		__asl_define_inputmodu(Key_F1, GLFW_KEY_F1)
		__asl_define_inputmodu(Key_F2, GLFW_KEY_F2)
		__asl_define_inputmodu(Key_F3, GLFW_KEY_F3)
		__asl_define_inputmodu(Key_F4, GLFW_KEY_F4)
		__asl_define_inputmodu(Key_F5, GLFW_KEY_F5)
		__asl_define_inputmodu(Key_F6, GLFW_KEY_F6)
		__asl_define_inputmodu(Key_F7, GLFW_KEY_F7)
		__asl_define_inputmodu(Key_F8, GLFW_KEY_F8)
		__asl_define_inputmodu(Key_F9, GLFW_KEY_F9)
		__asl_define_inputmodu(Key_F10, GLFW_KEY_F10)
		__asl_define_inputmodu(Key_F11, GLFW_KEY_F11)
		__asl_define_inputmodu(Key_F12, GLFW_KEY_F12)
		__asl_define_inputmodu(Key_F13, GLFW_KEY_F13)
		__asl_define_inputmodu(Key_F14, GLFW_KEY_F14)
		__asl_define_inputmodu(Key_F15, GLFW_KEY_F15)
		__asl_define_inputmodu(Key_F16, GLFW_KEY_F16)
		__asl_define_inputmodu(Key_F17, GLFW_KEY_F17)
		__asl_define_inputmodu(Key_F18, GLFW_KEY_F18)
		__asl_define_inputmodu(Key_F19, GLFW_KEY_F19)
		__asl_define_inputmodu(Key_F20, GLFW_KEY_F20)
		__asl_define_inputmodu(Key_F21, GLFW_KEY_F21)
		__asl_define_inputmodu(Key_F22, GLFW_KEY_F22)
		__asl_define_inputmodu(Key_F23, GLFW_KEY_F23)
		__asl_define_inputmodu(Key_F24, GLFW_KEY_F24)
		__asl_define_inputmodu(Key_printscreen, GLFW_KEY_PRINT_SCREEN)
		__asl_define_inputmodu(Key_scrolllock, GLFW_KEY_SCROLL_LOCK)
		__asl_define_inputmodu(Key_pause, GLFW_KEY_PAUSE)
		__asl_define_inputmodu(Key_insert, GLFW_KEY_INSERT)
		__asl_define_inputmodu(Key_home, GLFW_KEY_HOME)
		__asl_define_inputmodu(Key_pageup, GLFW_KEY_PAGE_UP)
		__asl_define_inputmodu(Key_delete, GLFW_KEY_DELETE)
		__asl_define_inputmodu(Key_end, GLFW_KEY_END)
		__asl_define_inputmodu(Key_pagedown, GLFW_KEY_PAGE_DOWN)
		__asl_define_inputmodu(Key_right, GLFW_KEY_RIGHT)
		__asl_define_inputmodu(Key_left, GLFW_KEY_LEFT)
		__asl_define_inputmodu(Key_down, GLFW_KEY_DOWN)
		__asl_define_inputmodu(Key_up, GLFW_KEY_UP)
		__asl_define_inputmodu(Key_lctrl, GLFW_KEY_LEFT_CONTROL)
		__asl_define_inputmodu(Key_lshift, GLFW_KEY_LEFT_SHIFT)
		__asl_define_inputmodu(Key_lalt, GLFW_KEY_LEFT_ALT)
		__asl_define_inputmodu(Key_lgui, GLFW_KEY_LEFT_SUPER)
		__asl_define_inputmodu(Key_rctrl, GLFW_KEY_RIGHT_CONTROL)
		__asl_define_inputmodu(Key_rshift, GLFW_KEY_RIGHT_SHIFT)
		__asl_define_inputmodu(Key_ralt, GLFW_KEY_RIGHT_ALT)
		__asl_define_inputmodu(Key_rgui, GLFW_KEY_RIGHT_SUPER)
		__asl_define_inputmodu(Key_unknown, GLFW_KEY_UNKNOWN)

#pragma endregion
		.endNamespace();

		cw::lua::lua_exec(
			R"V(

function abc(key)
	print("ABC")
end
j = Json()
j:setString("A", "B")
print(j:getString("A"))
Event.addEventListener(
	"on_key_press",
	abc
)

)V"


		);
}
Time engineTimeHolder;
Time cw::lua::getEngineTimeHolders() {
	return engineTimeHolder;
}

double cw::lua::getDeltaTime() {
	return engineTimeHolder.deltaTimer->millis();
}

double cw::lua::getEngineRunTime() {
	engineTimeHolder.timeSinceStart->end();
	return round(engineTimeHolder.timeSinceStart->seconds());
}

double cw::lua::getEngineRunTimeMs() {
	engineTimeHolder.timeSinceStart->end();
	return engineTimeHolder.timeSinceStart->millis();
}
lua_State *cw::lua::get_lua_state() { return L; }

bool cw::lua::lua_exec(const char* s) {
	auto L = cw::lua::get_lua_state();
	const int ret = luaL_dostring(L, s);

	if (ret != LUA_OK) {
		printf("Error: %s\n", lua_tostring(L, -1));
		lua_pop(L, 1); // pop error message
		return false;
	}

	return true;
}
