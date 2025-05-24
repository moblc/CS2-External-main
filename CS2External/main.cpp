#include <Windows.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <GLFW/glfw3.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <geom.h>
#include "Menu.h"
#include "Settings.h"
#include "Hack.h"
#include <filesystem>

RECT rect;
HWND game;
GLFWwindow* window;
bool insert_pressed = false;

#pragma comment(lib,"glfw3.lib")
#pragma comment(lib,"opengl32.lib")

#define IDR_TTF1 101
#define IDR_RCDATA1                     102

void* LoadEmbeddedFont(HINSTANCE hInstance, int resourceId, size_t& size) {
	HRSRC hRes = FindResource(hInstance, MAKEINTRESOURCE(resourceId), RT_RCDATA);
	if (!hRes) {
		std::cerr << "Failed to find resource." << std::endl;
		return nullptr;
	}

	HGLOBAL hResData = LoadResource(hInstance, hRes);
	if (!hResData) {
		std::cerr << "Failed to load resource." << std::endl;
		return nullptr;
	}

	size = SizeofResource(hInstance, hRes);
	return LockResource(hResData);
}

bool IsRunAsAdmin()
{
	BOOL isAdmin = FALSE;
	PSID adminGroup = NULL;
	SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
	if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminGroup))
	{
		CheckTokenMembership(NULL, adminGroup, &isAdmin);
		FreeSid(adminGroup);
	}
	return isAdmin;
}

void GLFWinit()
{
	glfwInit();
	glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
	glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
}

void GLFW_ImGuiInit()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	
	io.Fonts->AddFontFromFileTTF("c:/windows/Fonts/simhei.ttf",20.0f,nullptr, io.Fonts->GetGlyphRangesChineseFull());
	HINSTANCE hInstance = GetModuleHandle(NULL);
	size_t fontSize = 0;
	void* fontData = LoadEmbeddedFont(hInstance, IDR_RCDATA1, fontSize);
	ImFont* font = io.Fonts->AddFontFromMemoryTTF(fontData, fontSize, 16.0f);

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
}

void renderLoop()
{

	while (!glfwWindowShouldClose(glfwGetCurrentContext()))
	{
		if (GetAsyncKeyState(VK_DELETE))
			break;

		if (GetAsyncKeyState(VK_RSHIFT) & 0x8000)
		{
			if (!insert_pressed)
			{
				Menu::ToggleMenu();
				insert_pressed = true;
			}
		}
		else
		{
			insert_pressed = false;
		}

		glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, !Settings::showMenu);

		getGameRect(game,rect);
		glfwSetWindowPos(window, rect.left, rect.top);
		glfwSetWindowSize(window, width, height);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		Menu::MainMenu();
		Hack::hackloop();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}
}

void GLFW_ImGuiShutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}

void GLFW_Shutdown()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

int main()
{
	// 设置控制台代码页为 UTF-8
	SetConsoleOutputCP(65001);
	// 设置控制台输入代码页为 UTF-8
	SetConsoleCP(65001);

	if (!IsRunAsAdmin())
	{
		std::cout << "请以管理员身份运行" << std::endl;
		getchar();
		return 1;
	}

	std::cout << "右shift: 打开菜单" << std::endl;
	std::cout << "del键: 退出" << std::endl;

	game = FindWindowA(NULL, "Counter-Strike 2");
	getGameRect(game,rect);

	GLFWinit();
	window = glfwCreateWindow(width, height, "CS2绘制", NULL, NULL);
	glfwMakeContextCurrent(window);
	GLFW_ImGuiInit();	

	renderLoop();

	GLFW_ImGuiShutdown();
	GLFW_Shutdown();
	getchar();
}