#include "pch.h"
#include "Screen.h"
#include <iostream>

const Vector2& ScreenSingleton::GetWindowResolution()
{
    return windowSize;
}

float ScreenSingleton::GetAspectRatio()
{
    float ratio = windowSize.x / windowSize.y;
    return isnan(ratio) ? 0 : ratio;
}

void ScreenSingleton::SetCursorVisibility(bool visible)
{
    glfwSetInputMode(window, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}
