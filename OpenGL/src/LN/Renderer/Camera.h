#pragma once

namespace LN {

    class Event;
    class MouseScrolledEvent;
    class WindowResizeEvent;
    class MouseMoveEvent;
    class KeyPressedEvent;

    enum class CameraProjModel
    {
        en_Proj = 0, en_Ortho
    };

    class Camera
    {
    public:
        Camera() = default;
        Camera(float fov, float aspectRatio, float nearClip, float farClip, CameraProjModel model = CameraProjModel::en_Proj);

        virtual ~Camera() = default;

        virtual void OnEvent(Event& e);
        virtual void OnUpdate(float ts);

        inline float GetYaw() const { return m_Yaw; }
        inline float GetPitch() const { return m_Pitch; }
        inline float GetDistance() const { return m_Distance; }
        inline void  SetDistance(float distance) { m_Distance = distance; }
        inline const glm::vec3& GetPosition() const { return m_Position; }
        inline const glm::mat4& GetProjection() const { return m_Projection; }
        inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        inline void  setCameraProjModel(CameraProjModel model) { m_ProjModel = model; }
        inline glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

        glm::vec3 GetUpDirection() const;
        glm::vec3 GetRightDirection() const;
        glm::vec3 GetForwardDirection() const;
        glm::quat GetOrientation() const;

        void SetViewportSize(float width, float height);
    protected:
        void UpdateProjection();
        void UpdateView();

        float ZoomSpeed() const;
        void MouseZoom(float delta);

        bool OnMouseScroll(MouseScrolledEvent& e);
        bool OnWindowResized(WindowResizeEvent& e);


        glm::vec3 CalculatePosition() const;


    protected:
        float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

        glm::mat4 m_ViewMatrix;
        glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
        glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };

        float m_Distance = 10.0f;
        float m_Pitch = 0.0f, m_Yaw = 0.0f;

        float m_ZoomLevel = 1.0f;

        float m_ViewportWidth = 1280, m_ViewportHeight = 720;

        glm::mat4 m_Projection = glm::mat4(1.0f);
        CameraProjModel m_ProjModel;
    };



    class EditorCamera : public Camera
    {
    public:
        EditorCamera() = default;
        EditorCamera(float fov, float aspectRatio, float nearClip, float farClip, CameraProjModel model = CameraProjModel::en_Proj);

        void OnUpdate(float ts) override;
        void OnEvent(Event& e) override;

    private:
        void MousePan(const glm::vec2& delta);
        void MouseRotate(const glm::vec2& delta);

        std::pair<float, float> PanSpeed() const;
        float RotationSpeed() const;

    private:
        glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };
    };



    class ViewCamera :public Camera
    {
        ViewCamera() = default;
        ViewCamera(float fov, float aspectRatio, float nearClip, float farClip, CameraProjModel model = CameraProjModel::en_Proj);

        void OnUpdate(float ts) override;
        void OnEvent(Event& e) override;

    private:
        bool OnMouseScroll(MouseScrolledEvent& e);
        bool OnWindowResized(WindowResizeEvent& e);
        bool OnMousePressed(MouseButtonPressedEvent& e);
        bool OnMouseReleased(MouseButtonReleasedEvent& e);
        bool OnMousMove(MouseMoveEvent& e);
        bool OnKeyPressed(KeyPressedEvent& e);

    private:
        glm::vec2 m_LastMousePosition = { 0.0f, 0.0f };
    };
}