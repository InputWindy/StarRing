#include "pch.h"
#include "WindowsInput.h"

#include "Application.h"
#include <GLFW/glfw3.h>
namespace StarRing {
    Input* Input::ms_InputInstance = new WindowsInput();
}

bool StarRing::WindowsInput::IsKeyPressedImpl(unsigned int key_code)
{
    auto window = static_cast<GLFWwindow*>( Application::GetApplication().GetWindow().GetNativeWindow());
    
    auto state = glfwGetKey(window, key_code);
    
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool StarRing::WindowsInput::IsMouseBtnPressedImpl(unsigned int btn)
{
    auto window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());

    auto state = glfwGetMouseButton(window, btn);

    return state == GLFW_PRESS;
}

std::pair<float, float> StarRing::WindowsInput::GetMousePositionImpl()
{
    auto window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
     
    return pair<float, float>((float)xpos,(float)ypos);
}

float StarRing::WindowsInput::GetMouseXImpl()
{
    auto window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos,&ypos);

    return (float)xpos;
}

float StarRing::WindowsInput::GetMouseYImpl()
{
    auto window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    return (float)ypos;
}
