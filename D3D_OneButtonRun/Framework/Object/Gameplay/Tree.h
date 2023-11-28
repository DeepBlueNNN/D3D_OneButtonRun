#pragma once
class Tree : public GameActor
{
public:
	Tree(string fbxName, string id);
	~Tree();

public:
	virtual void Update()	  override;
	virtual void Render()	  override;
	virtual void PreRender()  override {}
	virtual void PostRender() override {}
	virtual void GUIRender()  override;
	
};