#include "framework.h"
#include "MainWindow.h"
#include "Resource.h"
#include <dxgidebug.h>
#include <fstream>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
Vector3 mousePos;

MainWindow::MainWindow()
{

}

MainWindow::~MainWindow()
{
	DeleteBackBuffer();
	SAFE_RELEASE(m_device);
	SAFE_RELEASE(m_deviceContext);
	SAFE_RELEASE(m_swapChain);
	SAFE_RELEASE(m_renderTargetView);
}

void MainWindow::InitScene()
{
	// SceneManager 싱글톤으로 GetInstance()
	SCENEMANAGER;
}

void MainWindow::Update()
{
	SCENEMANAGER->Update();
}

void MainWindow::Render()
{
	// ImGui Frame 생성
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	// Scene 렌더
	{
		Clear(0.0f, 0.125f, 0.3f, 1.0f);
		SCENEMANAGER->Render();
	}

	// PostRender : 2D 렌더 (ImGui Font 등)
	{
		SCENEMANAGER->PostRender();
	}

	// Imgui 랜더링
	if (SCENEMANAGER->GetGUIActive())
	{
		bool isActive = true;
		ImGui::Begin(u8"한글", &isActive);
		SCENEMANAGER->GuiRender();
		ImGui::End();
	}
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	Present();
}

LRESULT MainWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		//switch (wParam)
		//{
		//case VK_ESCAPE:
		//	PostQuitMessage(0);
		//	break;
		//}
		break;

	case WM_SIZE:
		MainWindow::GetInstance()->OnSize();
		break;

	case WM_MOUSEMOVE:
		mousePos.x = (float)LOWORD(lParam);
		mousePos.y = (float)(MainWindow::GetInstance()->GetHeight()) - (float)HIWORD(lParam);
		mousePos.z = 0.0f;
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void MainWindow::OnSize()
{
	static int flag;

	RECT rect;
	GetClientRect(GetWindowHandler(), &rect);

	flag++;
	if (flag < 3) return;
	flag = 3;

	m_width = rect.right - rect.left;
	m_height = rect.bottom - rect.top;

	ResizeScreen((float)m_width, (float)m_height);
}

void MainWindow::CreateGameWindow(HINSTANCE hInstance, int width, int height)
{
	// MyRegisterClass 정의
	// Callback 받을 수 있음
	WNDCLASSEXW wcex;
	{
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_D3DONEBUTTONRUN));
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		if (width == 0 && height == 0)
			wcex.lpszMenuName = NULL;
		else
			wcex.lpszMenuName = MAKEINTRESOURCEW(IDI_D3DONEBUTTONRUN);
		wcex.lpszClassName = L"DX3D";
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

		RegisterClassExW(&wcex);
	}

	m_instance = hInstance; // 인스턴스 핸들을 전역 변수에 저장

	int dualX		= GetSystemMetrics(SM_CXVIRTUALSCREEN); // 듀얼스크린일 때 적용값
	int dualY		= GetSystemMetrics(SM_CYVIRTUALSCREEN); // 듀얼스크린일 때 적용값
	int ScreenX		= GetSystemMetrics(SM_CXSCREEN);
	int Offset		= 0;

	// 듀얼스크린일 때 적용값
	if (dualX > ScreenX)
		Offset = dualX - ScreenX + 240;

	// Full Screen
	if (width == 0 && height == 0)
	{
		int ScreenX = GetSystemMetrics(SM_CXSCREEN);
		int ScreenY = GetSystemMetrics(SM_CYSCREEN);

		m_wnd = CreateWindowW(L"DX3D", L"D3D",
			WS_POPUP,
			Offset,
			0,
			ScreenX, ScreenY,
			nullptr,
			nullptr,
			hInstance,
			nullptr);
	}
	else
	{
		m_wnd = CreateWindowW(L"DX3D", L"D3D", WS_OVERLAPPEDWINDOW,
			Offset,
			0,
			width, height,
			nullptr,
			nullptr,
			hInstance,
			nullptr);
	}

	RECT rect;
	GetClientRect(m_wnd, &rect);

	m_width  = rect.right - rect.left;
	m_height = rect.bottom - rect.top;

	// DirectX 초기화
	InitDevice();

	ShowWindow(m_wnd, SW_SHOWDEFAULT);
	UpdateWindow(m_wnd);
}

void MainWindow::Clear(float r, float g, float b, float alpha)
{
	float clearColor[4] = { r, g,  b,  alpha };

	SetViewPort();

	m_deviceContext->ClearRenderTargetView(m_renderTargetView, clearColor);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1, 0);
}

void MainWindow::Present()
{
	m_swapChain->Present(0, 0);
}

void MainWindow::SetRenderTarget()
{
	GetDC()->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);	// DSV생성
}

void MainWindow::InitDevice()
{
	UINT width = m_width;
	UINT height = m_height;

	// 1. D3D11CreateDeviceAndSwapChain 함수 실행 :Device, DeviceContext, SwapChain생성
	// 2. RenderTargetView 세팅
	// 3. BackBuffer 생성

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = m_wnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = true;

	vector<D3D_FEATURE_LEVEL> feature_level =
	{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,  // 11.0
			D3D_FEATURE_LEVEL_10_1,  // ps4_0,vs_4  GTX750
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3
	};

	D3D_FEATURE_LEVEL  seleted_feature_level;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT, // 3D+2D(글자)
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_swapChain,
		&m_device,
		&seleted_feature_level,   // 지원가능한 feature 중 최우선 선택
		&m_deviceContext
	);

	assert(SUCCEEDED(hr));
	CreateBackBuffer((float)width, (float)height);
}

void MainWindow::CreateBackBuffer(float width, float height)
{
	HRESULT hr;

	// GetBackBuffer And Create RTV	< -- 2D에선 RTV만 필요했지만 3D는 DSV도 추가해야 함
	{
		ID3D11Texture2D* backbufferPointer;
		hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backbufferPointer);
		assert(SUCCEEDED(hr));

		hr = GetDevice()->CreateRenderTargetView(backbufferPointer, NULL, &m_renderTargetView);
		assert(SUCCEEDED(hr));
		SAFE_RELEASE(backbufferPointer);
	}

	// Create Texture - DSV < -- 3D에 추가로 필요하게 된 기능 Depth Stencil =깊이버퍼
	// 이전엔 렌더링 순서대로 했을 때, 3D에선 Z값 기준으로 깊이를 측정해 렌더링한다
	{
		D3D11_TEXTURE2D_DESC desc = { 0 };
		desc.Width = (UINT)width;
		desc.Height = (UINT)height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		hr = GetDevice()->CreateTexture2D(&desc, NULL, &m_depthStencilBuffer);
		assert(SUCCEEDED(hr));
	}

	//Create DSV
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		hr = GetDevice()->CreateDepthStencilView(m_depthStencilBuffer, &desc, &m_depthStencilView);
		assert(SUCCEEDED(hr));
	}

	SetRenderTarget();
}

void MainWindow::DeleteBackBuffer()
{
	SAFE_RELEASE(m_depthStencilView);
	SAFE_RELEASE(m_renderTargetView);
	SAFE_RELEASE(m_depthStencilBuffer);
}

void MainWindow::ResizeScreen(float width, float height)
{
	if (width < 1 || height < 1)
		return;

	Font::Get()->Delete();
	DeleteBackBuffer();
	{
		DC->OMSetRenderTargets(0, 0, 0);
		HRESULT hr = m_swapChain->ResizeBuffers(0, (UINT)width, (UINT)height, DXGI_FORMAT_UNKNOWN, 0);
		assert(SUCCEEDED(hr));
	}
	CreateBackBuffer(width, height);
	
	// Font 세팅
	Font::Get()->AddColor("White", 1, 1, 1);
	Font::Get()->AddStyle("Default", L"배달의민족 주아", 80.0f);

	Font::Get()->SetColor("White");
	Font::Get()->SetStyle("Default");

	ENV->UpdateProjection();
}
void MainWindow::SetViewPort()
{
	D3D11_VIEWPORT viewport;
	viewport.Width = (float)m_width;
	viewport.Height = (float)m_height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	GetDC()->RSSetViewports(1, &viewport);
}

void MainWindow::ReportLiveObjects()
{
	using fPtrDXGIGetDebugInterface = HRESULT(__stdcall*)(const IID&, void**);

	HMODULE hMod = GetModuleHandle(L"Dxgidebug.dll");
	fPtrDXGIGetDebugInterface DXGIGetDebugInterface = (fPtrDXGIGetDebugInterface)GetProcAddress(hMod, "DXGIGetDebugInterface");

	IDXGIDebug* debug = nullptr;
	DXGIGetDebugInterface(__uuidof(IDXGIDebug), (void**)&debug);

	debug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_ALL);
	debug->Release();
}

