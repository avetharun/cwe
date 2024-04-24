

#include <iostream>
#include <GL/glfw3.h>
#include <bit>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <stdio.h>
#include <nlohmann/json/json.hpp>
#include "argparse.hpp"
#include "utilities.h"
static void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
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
// When margin = true and window width or height are differnt than their default, we need to 
bool isWindowSizeRedone = false;
bool showsConsole = false;
int main(int argc, char** argv) {

	if constexpr (std::endian::native == std::endian::big)
		std::cout << "big-endian\n";
	else if constexpr (std::endian::native == std::endian::little)
		std::cout << "little-endian\n";
	else
		std::cout << "mixed-endian\n";
	printf("Starting cwe. Big endian? %d\n", is_big_endian());
	parser.add_description("If window is resizable, any area outside of the specified width, height will be black. This can be disabled with the --margin argument");
	parser.add_argument("--window_width", "-w").default_value(512).help("Specifies the window's default width.").store_into(CONFIG.windowWidth);
	parser.add_argument("--window_height", "-h").default_value(512).help("Specifies the window's default height.").store_into(CONFIG.windowHeight);
	auto& resizing_group = parser.add_mutually_exclusive_group();
	resizing_group.add_argument("--borderless", "-b").default_value(false).implicit_value(true).help("Specifies if the window should be borderless").store_into(CONFIG.isBorderless);
	resizing_group.add_argument("--resizable", "-q").default_value(true).implicit_value(true).help("Specifies if the window should be resizable").store_into(CONFIG.isResizable);
	parser.add_argument("--use_vsync", "-z").default_value(true).implicit_value(true).help("Specifies if the game should use VSync").store_into(CONFIG.useVsync);
	parser.add_argument("--window_name", "-t").default_value("Game").help("Specifies the title of the window").store_into(CONFIG.windowName);
	parser.add_argument("--verbose", "-v").default_value(true).implicit_value(true).help("Specifies whether to print debug messages to the console").store_into(CONFIG.isVerbose);
	parser.add_argument("--console").default_value(true).implicit_value(true).help("Shows or hides the console").store_into(showsConsole);
	parser.add_argument("--margin").default_value(true).implicit_value(true).help("Specifies whether to black out edges of the screen when a predefined window size is set and window is resizable"
						).store_into(CONFIG.enableMargin);
	parser.parse_args(argc, argv);
	printlnf_if_verbose("Parsed arguments.\nInitializing...");
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

	//glfwWindowHint(GLFW_RESIZABLE, CONFIG.isResizable);
	glfwWindowHint(GLFW_DECORATED, !CONFIG.isBorderless);
	// Create window with graphics context
	GLFWwindow* window = glfwCreateWindow(CONFIG.windowWidth, CONFIG.windowHeight, CONFIG.windowName.c_str(), nullptr, nullptr);
	if (window == nullptr) {
		printlnf_if_verbose("Failed to create GLFW window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(CONFIG.useVsync); // Enable vsync

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	printf_if_verbose("Created ImGui context");

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

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	// - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != nullptr);

	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Main loop
#ifdef __EMSCRIPTEN__
	// For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
	// You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
	io.IniFilename = nullptr;
	EMSCRIPTEN_MAINLOOP_BEGIN
#else
	while (!glfwWindowShouldClose(window))
#endif
	{
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		ImGui::ShowDemoWindow();
		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);
			ImGui::ColorButton("TEST", ImGui::ColorConvertU32ToFloat4(0x000000ff));
			
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}

		// 3. Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		//if (CONFIG.enableMargin) {
			int imDisplayW = io.DisplaySize.x;
			int imDisplayH = io.DisplaySize.y;
			float imDisplayWF = io.DisplaySize.x;
			float imDisplayHF = io.DisplaySize.y;
			if (imDisplayW != CONFIG.windowWidth) {
				//printlnf_if_verbose("Width of window is different! Expected %d, found %d", CONFIG.windowWidth, imDisplayW);
				int diff = imDisplayW - CONFIG.windowWidth;
				float halfDiff = diff / 2;
				// What should happen: a fully black rect
				// What actually happens: nothing displays (except when I add a slight amount of color (0c)
				// (0xAABBGGRR)
				ImGui::GetForegroundDrawList()->AddRectFilled({ 0,0 }, { halfDiff,imDisplayHF }, 0x0c0000ff, 0);
				// However, the following creates a black rect:
				// (0xAABBGGRR)
				ImGui::GetForegroundDrawList()->AddRectFilled({ imDisplayWF - halfDiff,0 }, { imDisplayWF,imDisplayHF }, 0xff000000, 0);
			}
			if (imDisplayH != CONFIG.windowHeight) {
				//printlnf_if_verbose("Height of window is different! Expected %d, found %d", CONFIG.windowHeight, imDisplayH);
			}
		//}
		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}
#ifdef __EMSCRIPTEN__
	EMSCRIPTEN_MAINLOOP_END;
#endif

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
