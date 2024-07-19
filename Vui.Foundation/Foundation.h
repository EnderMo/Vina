#pragma once
// STL 依赖项 （TODO：使用自定义代码代替）
#include <string>

// 杂项
#include "Misc.h"

// Vui依赖的部分类（TODO：弃用这些类）
#include "VuiException.h"
#include "Encoding.h"

// Vui主题
#include "Theme.h"

// Vui动画缓动
#include "Animation.h"

// Vui Win32核心组件包装
#include "Dispatcher.h"
#include "Window.h"
#include "DrawingContext.h"

// Vui DC抗锯齿扩展
#include "AntiAliasing.h"

// Vui 弹出式菜单
#include "PopupMenu.h"

// Vui 系统托盘
#include "Tray.h"

namespace vui
{
    namespace consts
    {
        constexpr auto Version = "Alpha Internal Testing";
        constexpr int Major = 0;
        constexpr int Minor = 1;
        constexpr int Patch = 0;
        constexpr int Build = 0;
    }
}