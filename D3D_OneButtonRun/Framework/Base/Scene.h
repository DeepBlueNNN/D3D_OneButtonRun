#pragma once
//////////////////////////////////////////
// 추상화 Class 설계
// C++ Calss특징
//   1. 캡슐화 : 데이터,기능을 같은 장소에 둘 수 있다
//   2. 다형성 : override, overloading
//   3. 은닉화 : public,private, protected를 데이처,기능을 은닉화
//   4. 상속성/추상화
//  <추상화>
//      1. virtual 함수가 하나라도 있으면 추상화 class라고 한다
//      2. 객체를 생성할 수 없는 것
//      3. 왜 추상화 Class를 사용하는가?
//          --> 표준,여러멤버를 같이 사용 할수 있게
///////////////////////////////////////////////

class Scene
{
public:
	virtual void Update()		= 0; // 순수 가상 함수
	virtual void Render()		= 0; // 상속받는 클래스에서 반드시 Implement해야함
	virtual void ChangeScene()	{};
	virtual void PreRender()	{};
	virtual void PostRender()	{};
	virtual void GUIRender()	{};

public: // Setter(inline)
	void SetName(string name)		{ m_sceneName = name; }
	void SetNextName(string name)	{ m_nextSceneName = name; }
	void SetActive(bool active)		{ m_active = active; }
	void SetReset(bool value)		{ m_reset = value; }

public: // Getter
	string	GetName()				{ return m_sceneName; }
	auto	GetNextSceneName()		{ return m_nextSceneName; }
	bool	IsActive()				{ return m_active; }
	bool	IsDisplay()				{ return m_display; }
	int		GetSceneID()			{ return m_sceneID; }

protected:
	string	m_sceneName = "";		// 현재 Scene 이름
	string	m_nextSceneName = "";	// 다음 Scene 이름
	bool	m_active = true;		// 현재 Scene의 활성화 유무
	bool	m_reset = true;			// Goal 만났을때 잠시 현재의 Scene을 벗어난 후 ChangeScene이 호출 될 때 Reset
	bool	m_display = false;		// Map의 데이터를 표시 해줌
	int		m_sceneID = 0;
};

