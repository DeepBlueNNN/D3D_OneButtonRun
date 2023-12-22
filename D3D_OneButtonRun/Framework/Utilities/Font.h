#pragma once

class Font : public Singleton<Font>
{
public:
    Font();
    ~Font();

public:
    void AddColor(string key, float r, float g, float b);
    void AddStyle(string key, wstring font, float size = 20.0f,
        DWRITE_TEXT_ALIGNMENT alignment = DWRITE_TEXT_ALIGNMENT_LEADING,
        DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH stretch = DWRITE_FONT_STRETCH_NORMAL);

public:
    bool CheckStyleValidity(string key);
    bool CheckColorValidity(string key);

public:
    void SetColor(string key);
    void SetStyle(string key);

    void RenderText(wstring text, Float2 pos, Float2 size = Float2());
    void RenderText(string text, Float2 pos, Float2 size = Float2());

    ID2D1DeviceContext* GetDC() { return m_context; }

    wstring ChangeWString(string value);

private:
    ID2D1Factory1* m_factory;
    IDWriteFactory* m_writeFactory;

    IDXGIDevice* m_dxgiDevice;

    ID2D1Device* m_device;
    ID2D1DeviceContext* m_context;

    IDXGISurface* m_dxgiSurface;

    ID2D1Bitmap1* m_targetBitmap;

    map<string, ID2D1SolidColorBrush*> m_brushes;
    map<string, IDWriteTextFormat*> m_formats;

    ID2D1SolidColorBrush* m_curBrush;
    IDWriteTextFormat* m_curFormat;
};