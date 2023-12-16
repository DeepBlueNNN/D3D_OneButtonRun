#pragma once
class UIPanel
{
public:
	UIPanel() = default;
	~UIPanel() = default;

public:
	virtual void Update() {}
	virtual void Render() {}

public:	// Getter
	bool IsPanelOn() { return m_isPanelOn; }

public:	// Setter
	void SetPanelOn(bool set) { m_isPanelOn = set; }

protected:
	bool m_isPanelOn = false;
};