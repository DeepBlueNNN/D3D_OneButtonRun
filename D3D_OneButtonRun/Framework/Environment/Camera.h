#pragma once

struct Ray
{
    Vector3 pos;
    Vector3 dir;
    Ray()
    {

    }
    Ray(Vector3 pos, Vector3 dir)
    {
        this->pos = pos;
        this->dir = dir;
    }
};

/////////////////////////////////////////////////////////////////
//                        View 생성 담당	    			       //
//                        Camera Control                       //
/////////////////////////////////////////////////////////////////

class Camera : public Transform
{
public:
    Camera();
    ~Camera();

public:
    /// <summary>
    /// 카메라 트렌스폼 초기화
    /// </summary>
    void InitCamera();
    void Update();
    void GUIRender();

    void SetView();
    void SetTarget(Transform* target) { m_target = target; }    

    Vector3 ScreenToWorld(Vector3 pos);
    Vector3 WorldToScreen(Vector3 pos);

    Ray ScreenPointToRay(Vector3 screenPoint);

private:
    void FreeMode();

private:
    ViewBuffer* m_viewBuffer = nullptr;
    Matrix      m_view;
    Matrix      m_projection;

    float m_moveSpeed = 10.0f;
    float m_rotSpeed = 5.0f;

    Transform*  m_target = nullptr;    
};