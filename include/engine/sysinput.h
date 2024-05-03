#pragma once
#include <functional>
#include <string>
#include <alib.h>
#include <vector>
using void_1c_f = std::function<void(char)>;
namespace cw {
	namespace InputManagement {
		namespace Mouse {
			static float sy, sd{ 0 };
			static int x, y, dx, dy{ 0 };
			static bool m1d, m2d, m3d, mt1d, mt2d{ false };
			static bool m1df, m2df, m3df, mt1df, mt2df{ false };

			static const std::vector< std::function <void(int, int, bool, bool, bool)>> onClick;
			static const std::vector < std::function <void(float)>> onScroll;
			static const std::vector < void_2i_f > onMove;
			static const bool input_stolen;
		};
		namespace Keyboard {
			static inline bool input_stolen;
			static char keys[1024];
			static char keys_frame[1024];
			static char keys_frame_last[1024];
			std::vector<void_1c_f> & getOnKeyPress();
			std::vector<void_1c_f> & getOnKeyRelease();
			std::vector<void_1c_f> & getOnKeyHold();

			void EmulateKeyboardDown(unsigned char key);
			void EmulateKeyboardUp(unsigned char key);
			bool GetKey(int k);
			bool GetKeyAny(std::vector<int> ks);
			bool GetKeyPressed(int k);
			bool GetKeyPressedAny(std::vector<int> ks);
			void flush();
			void update();
		};
	};
}