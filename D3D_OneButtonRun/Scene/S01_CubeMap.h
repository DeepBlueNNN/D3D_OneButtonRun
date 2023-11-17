#pragma once
class S01_CubeMap : public Scene
{
public:
	S01_CubeMap();
	~S01_CubeMap();

public:
	virtual  void Update()      override;
	virtual  void Render()      override;
	virtual  void PreRender()   override;
	virtual  void PostRender()  override;
	virtual  void GUIRender()   override;

private:
	Sky* m_sky;
};