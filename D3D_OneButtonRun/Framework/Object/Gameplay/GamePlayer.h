#pragma once
class GamePlayer : public GameActor
{
public:
	GamePlayer();
	~GamePlayer();

public:
	virtual void Update()	  override;
	virtual void Render()	  override;
	virtual void PreRender()  override {}
	virtual void PostRender() override {}
	virtual void GUIRender()  override;

private:
	

};