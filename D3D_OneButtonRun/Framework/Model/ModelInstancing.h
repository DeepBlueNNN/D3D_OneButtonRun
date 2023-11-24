#pragma once
class ModelInstancing : public Model
{
public:
	ModelInstancing(string name);
	~ModelInstancing();

public:
	virtual void Update() override;
	virtual void Render() override;
	void GUIRender();

	Transform* Add();

private:
	vector<Transform*>	m_transforms;
	InstanceData		m_instanceDatas[MAX_INSTANCE];

	VertexBuffer*	m_instanceBuffer;
	UINT			m_drawCount = 0;

};