#include <camera2D.hpp>

Camera2D::Camera2D()
    : m_Viewport(0)
    , m_Position(0)
    , m_Rotation()
    , m_ProjectionMatrix(1)
    , m_ViewMatrix(1)
    , m_ViewDirty(false)
{}

Camera2D::Camera2D( int screenWidth, int screenHeight )
    : m_Viewport( 0, 0, screenWidth, screenHeight )
    , m_Position(0)
    , m_Rotation()
    , m_ProjectionMatrix(1)
    , m_ViewMatrix(1)
    , m_ViewDirty( false )
{

}

void Camera2D::SetViewport( int x, int y, int width, int height )
{
    m_Viewport = glm::vec4( x, y, width, height );
}

glm::vec4 Camera2D::GetViewport() const
{
    return m_Viewport;
}

void Camera2D::SetProjectionRH( float left, float right, float top, float bottom, float zNear, float zFar )
{
    m_ProjectionMatrix = glm::ortho( left, right, top, bottom, zNear, zFar );
}

void Camera2D::ApplyViewMatrix()
{
    UpdateViewMatrix();
}

void Camera2D::SetPosition( const glm::vec3& pos )
{
    m_Position = pos;
    m_ViewDirty = true;
}

glm::vec3 Camera2D::GetPosition() const
{
    return m_Position;
}

void Camera2D::Translate( const glm::vec3& delta, bool local /* = true */ )
{
    if ( local )
    {
        m_Position += m_Rotation * delta;
    }
    else
    {
        m_Position += delta;
    }
    m_ViewDirty = true;
}

void Camera2D::SetRotation( const glm::quat& rot )
{
    m_Rotation = rot;
    m_ViewDirty = true;
}

glm::quat Camera2D::GetRotation() const
{
    return m_Rotation;
}

void Camera2D::SetEulerAngles( const glm::vec3& eulerAngles )
{
    m_Rotation = glm::quat(glm::radians(eulerAngles));
}

glm::vec3 Camera2D::GetEulerAngles() const
{
    return glm::degrees(glm::eulerAngles( m_Rotation ));
}

void Camera2D::Rotate( const glm::quat& rot )
{
    m_Rotation = m_Rotation * rot;
    m_ViewDirty = true;
}

glm::mat4 Camera2D::GetProjectionMatrix()
{
    return m_ProjectionMatrix;
}

glm::mat4 Camera2D::GetViewMatrix()
{
    UpdateViewMatrix();
    return m_ViewMatrix;
}

void Camera2D::UpdateViewMatrix()
{
    if ( m_ViewDirty )
    {
        glm::mat4 translate = glm::translate(-m_Position);
        // Since we know the rotation matrix is orthonormalized, we can simply 
        // transpose the rotation matrix instead of inversing.
        glm::mat4 rotate = glm::transpose(glm::toMat4(m_Rotation));

        m_ViewMatrix = rotate * translate;

        m_ViewDirty = false;
    }
}
