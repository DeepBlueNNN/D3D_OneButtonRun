#pragma once
class Scene_PlayRecord : public Scene
{
public:
	Scene_PlayRecord();
	~Scene_PlayRecord();

public:
	void Update()      override;
	void Render()      override;
	void ChangeScene() override;
	void PreRender()   override {}
	void PostRender()  override;
	void GUIRender()   override;

public:
	void UpdateRecords();
	void PrintRecords(int index);
	void ConvertRecordToString(UINT count, float time);

private:
	vector<UITexture*> m_textures;
	vector<Button*> m_buttons;

	vector<string> m_stageNames;
	vector<string> m_recordCounts;
	vector<string> m_recordTimes;
};