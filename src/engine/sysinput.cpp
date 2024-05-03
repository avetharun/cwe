#include "engine/sysinput.h"
#include <GL/glfw3.h>
#include "utilities.h"
std::vector<void_1c_f> onKeyPress;
std::vector<void_1c_f> onKeyHold;
std::vector<void_1c_f> onKeyRelease;
std::vector<void_1c_f> & cw::InputManagement::Keyboard::getOnKeyPress() { return onKeyPress; }
std::vector<void_1c_f> & cw::InputManagement::Keyboard::getOnKeyRelease() { return onKeyRelease; }
std::vector<void_1c_f> & cw::InputManagement::Keyboard::getOnKeyHold() { return onKeyHold; }
void cw::InputManagement::Keyboard::EmulateKeyboardDown(unsigned char key) {
	keys[key] = true;
	keys_frame[key] = true;
	for (auto& handler : onKeyPress) {
		handler(key);
	}
	if (cw::lua::getLuaEvents().contains("on_key_press")) { for (auto& handler : cw::lua::getLuaEvents().at("on_key_press")) { handler(key); } }
}

void cw::InputManagement::Keyboard::EmulateKeyboardUp(unsigned char key) {
	keys[key] = false;
	for (auto& handler : onKeyRelease) {
		handler(key);
	}
	if (cw::lua::getLuaEvents().contains("on_key_release")) { for (auto& handler : cw::lua::getLuaEvents().at("on_key_release")) { handler(key); } }
}

bool cw::InputManagement::Keyboard::GetKey(int k) { // Is Key pressed?
	return keys[k];
}

bool cw::InputManagement::Keyboard::GetKeyAny(std::vector<int> ks) { // Is any Key pressed?
	bool b = false;
	for (int i = 0; i < ks.size(); i++) {
		b |= keys[ks[i]];
	}
	return b;
}

bool cw::InputManagement::Keyboard::GetKeyPressed(int k) { // Key pressed this frame?
	return keys_frame[k] && !keys_frame_last[k];
}

bool cw::InputManagement::Keyboard::GetKeyPressedAny(std::vector<int> ks) { // any Key pressed this frame?
	bool b = false;
	for (int i = 0; i < ks.size(); i++) {
		b |= keys_frame[ks[i]] && !keys_frame_last[ks[i]];
	}
	return b;
}

void cw::InputManagement::Keyboard::flush() {
	memset(keys, 0, 1024);
	memset(keys_frame, 0, 1024);
}

void cw::InputManagement::Keyboard::update() {
	memcpy(keys_frame_last, keys_frame, 1024);
	memcpy(keys_frame, keys, 1024);
	for (int i = 0; i < 1024; i++) {
		if (keys[i]) {
			for (auto& handler : onKeyHold) {
				handler(i);
			}

			if (cw::lua::getLuaEvents().contains("on_key_hold")) { for (auto& handler : cw::lua::getLuaEvents().at("on_key_hold")) { handler(i); } }
		}
	}
}


#include "utilities.h"

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	cw::InputManagement::Mouse::dx = cw::InputManagement::Mouse::x - xpos;
	cw::InputManagement::Mouse::dy = cw::InputManagement::Mouse::y - ypos;
	cw::InputManagement::Mouse::x = xpos;
	cw::InputManagement::Mouse::y = ypos;
	for (auto& callback : cw::InputManagement::Mouse::onMove) {
		callback(cw::InputManagement::Mouse::x, cw::InputManagement::Mouse::y);
	}
	if (cw::lua::getLuaEvents().contains("on_mouse_move")) { for (auto& handler : cw::lua::getLuaEvents().at("on_mouse_move")) { handler(xpos, ypos); } }
};
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (action) {
	case GLFW_PRESS: { cw::InputManagement::Keyboard::EmulateKeyboardDown(key); break; }
	case GLFW_RELEASE: { cw::InputManagement::Keyboard::EmulateKeyboardUp(key); break; }
	}
};
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	cw::InputManagement::Mouse::m1df = false;
	cw::InputManagement::Mouse::m2df = false;
	cw::InputManagement::Mouse::m3df = false;
	switch (button) {
	case GLFW_MOUSE_BUTTON_1: { cw::InputManagement::Mouse::m1d = cw::InputManagement::Mouse::m1df = action == GLFW_PRESS; break; }
	case GLFW_MOUSE_BUTTON_2: { cw::InputManagement::Mouse::m2d = cw::InputManagement::Mouse::m2df = action == GLFW_PRESS; break; }
	case GLFW_MOUSE_BUTTON_3: { cw::InputManagement::Mouse::m3d = cw::InputManagement::Mouse::m3df = action == GLFW_PRESS; break; }
	}
	for (auto& callback : cw::InputManagement::Mouse::onClick) {
		callback(cw::InputManagement::Mouse::x, cw::InputManagement::Mouse::y, cw::InputManagement::Mouse::m1d, cw::InputManagement::Mouse::m2d, cw::InputManagement::Mouse::m3d);
	}
	if (cw::lua::getLuaEvents().contains("on_mouse_click")) { for (auto& handler : cw::lua::getLuaEvents().at("on_mouse_click")) { handler(cw::InputManagement::Mouse::x, cw::InputManagement::Mouse::y, cw::InputManagement::Mouse::m1d, cw::InputManagement::Mouse::m2d, cw::InputManagement::Mouse::m3d); } }
};
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	cw::InputManagement::Mouse::sd = cw::InputManagement::Mouse::dy - yoffset;
	cw::InputManagement::Mouse::sy = yoffset;
	for (auto& callback : cw::InputManagement::Mouse::onScroll) {
		callback(cw::InputManagement::Mouse::sd);
	}
}
;
