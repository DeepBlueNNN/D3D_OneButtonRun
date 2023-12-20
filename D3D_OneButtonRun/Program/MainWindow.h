#pragma once

/////////////////////////////////////////////////////////////////
//                   DirectX »ý¼º Class						   //
/////////////////////////////////////////////////////////////////

class  MainWindow
{
public:
	static MainWindow* GetInstance()
	{
		static MainWindow singleton;
		return &singleton;
	}

private:
	MainWindow();
	~MainWindow();

public:
	void   Update();
	void   Render();

public:
	static LRESULT CALLBACK		WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public: // Setter
	void						CreateGameWindow(HINSTANCE hinstance, int width, int height);
	void						Clear(float r, float g, float b, float alpha);
	void						Present();
	void						SetRenderTarget();
	void						OnSize();
	void						InitScene();

public: // Getter
	int							GetWidth()			{ return m_width; }
	int							GetHeight()			{ return m_height; }
	HWND						GetWindowHandler()	{ return m_wnd; }
	ID3D11Device*				GetDevice()			{ return m_device; }
	ID3D11DeviceContext*		GetDC()				{ return m_deviceContext; }
	IDXGISwapChain*				GetSwapChain()		{ return m_swapChain; }

private:
	void InitDevice();
	void CreateBackBuffer(float width, float height);
	void DeleteBackBuffer();
	void ResizeScreen(float width, float height);
	void SetViewPort();

public:
	void ReportLiveObjects();

private:
	ID3D11Device*             m_device = nullptr;
	ID3D11DeviceContext*      m_deviceContext = nullptr;

	IDXGISwapChain*           m_swapChain = nullptr;
	ID3D11RenderTargetView*   m_renderTargetView = nullptr;

	ID3D11DepthStencilView*   m_depthStencilView = nullptr;
	ID3D11Texture2D*          m_depthStencilBuffer = nullptr;
	ID3D11DepthStencilState*  m_depthStencilState = nullptr;

private:
	int		  m_width		= 0;        // Window Width
	int		  m_height		= 0;		// Window Height
	HWND	  m_wnd			= nullptr;
	HWND      m_introVideo	= nullptr;
	HINSTANCE m_instance	= nullptr;
};

