/*****************************************************************//**
 * \paragraph CWE: 2D game engine built upon ImGui and GLFW
 * 
 * 
 * \author Feintha
 * \date   April 2024
 *********************************************************************/


#include <lua.h>
#include "engine/binparse.h"
#include <iostream>
#include <GL/glfw3.h>
#include <bit>
#include "imgui/imgui.h"
#include "imgui/imgui_uielement.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <stdio.h>
#include <nlohmann/json/json.hpp>
#include "argparse.hpp"
#include "utilities.h"
#include <alib.h>
#include "engine/sysinput.h"


auto f = std::function<void(unsigned char)>([&](unsigned char) {
	printf("abc\n");
});
static void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "CW: GLFW Error %d: %s\n", error, description);
	ShowConsole();
}
#ifndef NDEBUG
// Production builds should set NDEBUG=1
#define NDEBUG false
#else
#define NDEBUG true
#endif

#ifndef DEBUG
#define DEBUG !NDEBUG
#endif

void impl_lua();


static GameAppConfig CONFIG;
static argparse::ArgumentParser parser = argparse::ArgumentParser("cwe");

GameAppConfig getAppConfig() {
	return CONFIG;
}
void setConsoleState(bool state) {
	if (state) {
		ShowConsole();
	}
	else {
		HideConsole();
	}
}
GLFWwindow* window;
GLFWwindow* getWindow()
{
	return window;
}
void test(char) { printf("tested\n"); };
// When margin = true and window width or height are differnt than their default, we need to 
bool isWindowSizeRedone = false;
bool showsConsole = false;
int main(int argc, char** argv) {
	getEngineTimeHolders().timeSinceStart->start();
	parser.add_description("If window is resizable, any area outside of the specified width, height will be black. This can be disabled with the --margin argument");
	parser.add_argument("--window_width", "-w").nargs(1, 1).default_value(512).help("Specifies the window's default width. This applies to any borders that may be drawn from resizing.").store_into(CONFIG.windowWidth);
	parser.add_argument("--window_height", "-h").nargs(1, 1).default_value(512).help("Specifies the window's default height. This applies to any borders that may be drawn from resizing.").store_into(CONFIG.windowHeight);
	auto& resizing_group = parser.add_mutually_exclusive_group();
	resizing_group.add_argument("--borderless", "-b").default_value(false).implicit_value(true).help("Specifies if the window should be borderless").store_into(CONFIG.isBorderless);
	resizing_group.add_argument("--resizable", "-q").default_value(true).implicit_value(true).help("Specifies if the window should be resizable").store_into(CONFIG.isResizable);
	parser.add_argument("--window_name", "-t").default_value("Game").help("Specifies the title of the window").store_into(CONFIG.windowName);
	parser.add_argument("--verbose", "-v").default_value(true).implicit_value(true).help("Specifies whether to print debug messages to the console").store_into(CONFIG.isVerbose);
	parser.add_argument("--console").default_value(true).implicit_value(true).help("Shows or hides the console").store_into(showsConsole);
	parser.add_group("Rendering").add_argument("--margin").default_value(true).implicit_value(true).help("Specifies whether to black out edges of the screen when a predefined window size is set and window is resizable"
						).store_into(CONFIG.enableMargin);
	parser.add_argument("--use_vsync", "-z").default_value(true).implicit_value(true).help("Specifies if the game should use VSync").store_into(CONFIG.useVsync);


	bool isGameDefinedByArgs = false;
	parser.add_group("Single-file game loading").add_argument("--autoload").default_value("").help("Path to the autoload file. File can either be a .cw[a,l,t] or a lua script.").nargs(1, 1);
	parser.add_argument("--saved_data").default_value("").help("Path to the saved data path. Writing out gamedata from runtime will write here.").store_into(CONFIG.binary_appconfig.saved_data_path).nargs(1, 1);
	bool testParser = false;
#if NDEBUG
	parser.add_argument("--test_parsing").default_value(false).implicit_value(true).hidden().store_into(testParser);
#endif
	std::string gameloader;
	parser.add_group("Loader can be of .cw[a,l,t]. Lua scripts are not supported.").add_argument("--loader").default_value("assets/main.cwt").store_into(gameloader).help("Main loader file for the game. If the --autoload param is present, this will do nothing.").nargs(1, 1);
	EngineTimer t1;
	t1.start();
	if (!alib_file_exists(gameloader.c_str()) &&!CONFIG.binary_appconfig.autoload_path.empty() && !parser.is_used("--autoload")) {
		printf("Failed to initialize game, loader \"%s\" doesn't exist!\n", gameloader.c_str());
	} else if (!parser.is_used("--autoload") && alib_file_exists(gameloader.c_str())){
		CONFIG.appconfig = nlohmann::json::parse(alib_file_read(gameloader.c_str()));
		std::string autoload = CONFIG.appconfig["autoload"].get<std::string>();
		CONFIG.binary_appconfig.autoload_path= CONFIG.appconfig["autoload"];
		if (CONFIG.appconfig.contains("saved_data")) {
			CONFIG.binary_appconfig.saved_data_path = CONFIG.appconfig["saved_data"];
		}
	}
	else if (!alib_file_exists(gameloader.c_str())) {
		if (!parser.is_used("--autoload")) {
			// Load debug scene (aka editor)
		} else printf("Failed to initialize game, loader \"%s\" doesn't exist!\n", gameloader.c_str());
	}
	parser.parse_args(argc, argv);
	printlnf_if_verbose("Parsed arguments.\nInitializing...");

	if (testParser) {
		printlnf_if_verbose("Testing binparse impl...");
		cw::BinarySerializer::Writer w;
		int chkInt = 37112 ^ time(NULL);
		char chkChr = 182 ^ time(NULL);
		w.write_uint(chkInt);
		w.write_ubyte(chkChr);
		std::string s = "\"TEST\"";
		w.write_string(s);
		unsigned long long chkLng = -327132434 ^ time(NULL);
		w.write_ulong(chkLng);
		cw::BinarySerializer::Reader r = w.to_reader();
		printf("\nunsigned int: Found %u, expected %u\n", r.read_uint(), chkInt);
		printf("unsigned byte: Found %hhu, expected %hhu\n", r.read_ubyte(), chkChr);
		printf("string: Found %s, expected %s\n", r.read_string().c_str(), s.c_str());
		printf("unsigned long: Found %llu, expected %llu\n", r.read_ulong(), chkLng);
	}
	glfwSetErrorCallback(glfw_error_callback);
	setConsoleState(showsConsole);
	if (!glfwInit()) {
		printlnf_if_verbose("Failed to initialize GLFW! ");
		return 1;
	}
	printlnf_if_verbose("GLFW Initialized");
	setConsoleState(parser.get<bool>("--console"));

	// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	const char* glsl_version = "#version 100";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	glfwWindowHint(GLFW_RESIZABLE, CONFIG.isResizable);
	glfwWindowHint(GLFW_DECORATED, !CONFIG.isBorderless);

	// Create window with graphics context
	window = glfwCreateWindow(CONFIG.windowWidth, CONFIG.windowHeight, CONFIG.windowName.c_str(), nullptr, nullptr);
	if (window == nullptr) {
		printlnf_if_verbose("Failed to create GLFW window");
		return 1;
	}

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);


	glfwSetWindowSizeLimits(window, CONFIG.windowWidth, CONFIG.windowHeight, GLFW_DONT_CARE, GLFW_DONT_CARE);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(CONFIG.useVsync); // Enable vsync

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	impl_lua();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_IsSRGB;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	printf_if_verbose("Created ImGui context");
	t1.end();
	printlnf_if_verbose("Engine took %f milliseconds to start", t1.millis());

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	bool isEmscriptenUsed = false;
#ifdef __EMSCRIPTEN__
	ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
	isEmscriptenUsed = true;
#endif
	ImGui_ImplOpenGL3_Init(glsl_version);
	printlnf_if_verbose("Initialized ImGui for %s", isEmscriptenUsed ? "Emscripten, GLFW, and OpenGL3" : "GLFW and OpenGL3");

	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	EngineTimer t2;
	// Main loop
	printf("%zd", cw::InputManagement::Keyboard::getOnKeyPress().size());
#ifdef __EMSCRIPTEN__
	// For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
	// You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
	io.IniFilename = nullptr;
	EMSCRIPTEN_MAINLOOP_BEGIN
#else
	while (!glfwWindowShouldClose(window))
#endif
	{
		t2.start();
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();
		cw::lua::invokeEventsFor("on_update");

		// Start the Dear ImGui frame
		cw::lua::invokeEventsFor("on_gui");
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		cw::lua::invokeEventsFor("on_render");
		// Internally, we use ImGui to render everything!
		// Rendering



		cw::lua::invokeEventsFor("_util_before_imgui_render_frame");
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		cw::lua::invokeEventsFor("_util_after_clear_color");
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		cw::lua::invokeEventsFor("on_pre_swap");
		glfwSwapBuffers(window);
		cw::lua::invokeEventsFor("on_before_input_updated");
		cw::InputManagement::Keyboard::update();
#ifdef WIN32
		Sleep(0);
#endif
		t2.end();
		*getEngineTimeHolders().deltaTimer = t2;
	}
#ifdef __EMSCRIPTEN__
	EMSCRIPTEN_MAINLOOP_END;
#endif

	cw::lua::invokeEventsFor("on_exit");
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
