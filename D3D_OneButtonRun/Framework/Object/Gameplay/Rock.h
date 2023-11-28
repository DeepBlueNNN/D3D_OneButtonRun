#pragma once
class Rock : public GameActor
{
public:
	Rock(string fbxName, string id);
	~Rock();

public:
	virtual void Update()	  override;
	virtual void Render()	  override;
	virtual void PreRender()  override {}
	virtual void PostRender() override {}
	virtual void GUIRender()  override;
};