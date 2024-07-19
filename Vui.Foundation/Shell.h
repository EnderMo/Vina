#pragma once
#include "DrawingContext.h"
namespace vui
{
    class PaintDesktop : DrawingContext::DrawingContextExtension
    {
        public:
        PaintDesktop(POINT pt)
        {

        }
        void operator()(DrawingContext& dc) override
        {
        }
    };
}