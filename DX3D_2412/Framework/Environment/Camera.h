#pragma once

class Camera : public Transform
{
public:
    Camera();
    ~Camera();

    void Update();
    void Edit();

    void SetView(UINT slot = 1);
 
    class Ray ScreenPointToRay(const Vector3& screenPoint);

    Vector3 WorldToScreenPoint(Vector3 world);
   
    void TargetOptionSave(string file);
    void TargetOptionLoad(string file);

    bool ContainPoint(Vector3 point);
    bool ContainSphere(Vector3 center, float radius);

    void SetTarget(Transform* target) { this->target = target; }
    void UpdateInvenCamera();

    bool IsFollowMode() { return isFollowMode; }
   
    bool IsQuaterView() { return isQuaterView; }
   
    bool IsFPSView() { return isFPSView; }
   
    void SetQuaterView(bool isQuaterView) 
    { 
        this->isQuaterView = isQuaterView;
        isFPSView = !isQuaterView;
    }
   
    void SetFPSView(bool isFPSView)
    { 
        this->isFPSView = isFPSView; 
        isQuaterView = !isFPSView;
    }

private:
    void FreeMode();
    void FollowMode();

    void Frustum();

private:
    ViewBuffer* viewBuffer;
    MatrixBuffer* projectionBuffer;

    Matrix view;
    Matrix projection;

    bool isFollowMode = false;
    bool isQuaterView = false;
    bool isFPSView = false;
  
    float moveSpeed = 50.0f;
    float rotSpeed = 10.0f;

    Vector3 prevMousePos;

    Transform* target = nullptr;

    float distance = 20.0f;
    float height = 20.0f;
    float moveDamping = 5.0f;
    float rotDamping = 1.0f;
    float destRot = 0.0f;

    Vector3 destPos;
    Vector3 focusOffset;

    Matrix rotMatrix;

    bool isLookAtTargetX = true;
    bool isLookAtTargetY = true;

    char file[128];
    Vector3 planes[6];
    float a, b, c, d;
};