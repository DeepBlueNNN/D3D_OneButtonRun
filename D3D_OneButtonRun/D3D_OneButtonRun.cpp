// D3D_OneButtonRun.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "D3D_OneButtonRun.h"
#include <fstream>
////////////////////////////////////////
// TTF File Load
/////////////////////////////////////////
void  LoadFont()
{
	ImGuiIO& io = ImGui::GetIO();
	ImFont* font = nullptr;
	std::string font_file = "./TTF/NanumSquareB.ttf";
	//std::string font_file = "imgui/fonts/D2Coding.ttf";

	std::ifstream ifile;
	ifile.open(font_file);
	if (ifile)
	{
		std::cout << "file exists\n";
		font = io.Fonts->AddFontFromFileTTF(font_file.c_str(),
			16.0f, NULL, io.Fonts->GetGlyphRangesKorean());
	}
	else
	{
		std::cout << "file doesn't exist\n";
		font = io.Fonts->AddFontDefault();
	}
	IM_ASSERT(font != NULL);

}
//////////////////////////////////////////////////
// Window의 Main함수
// int main(int argc,char **argv)
//////////////////////////////////////////////////
int APIENTRY wWinMain(_In_     HINSTANCE hInstance,      // OS에서 생성된 Instance Handle
	_In_opt_ HINSTANCE hPrevInstance,  // Kill되었을때  예전 핸들
	_In_     LPWSTR    lpCmdLine,      // 명령어줄
	_In_     int       nCmdShow)       // Window를 어떻게 보이게 할 것인가?
{
	// Window Create, Direct3D 초기화
	MAIN->CreateGameWindow(hInstance, 1920, 1080); // 0,0 full mode

	// IMGUI 초기화
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui_ImplWin32_Init(MAIN->GetWindowHandler());
	ImGui_ImplDX11_Init(DEVICE, DC);
	LoadFont();   // TTF --> True Type Font
	ImGui::StyleColorsDark();


	// Scene은 SceneManager에 등록 사용
	// DX관련된 사항이 있기때문에 완전히 DX Init된후 Scene을  Create
	MAIN->InitScene();

	//Font를 세팅

	//Font::Get()->AddColor("White", 1, 1, 1);
	//Font::Get()->AddStyle("Default", L"배달의민족 주아");
	
	//Font::Get()->SetColor("White");
	//Font::Get()->SetStyle("Default");

	MSG msg;

	// 기본 메시지 루프입니다:
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			KEYBOARD->Update();
			MAIN->Update();
			MAIN->Render();
		}

	}

	return (int)msg.wParam;
}