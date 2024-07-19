#pragma once
#include "VinaComponents.hpp"
D2DVertexUIPanel* NewUIPanel = new D2DVertexUIPanel;
/*

     void GetFirstControlPoints(
            __in const std::vector<FLOAT>&rhs,
         __out std::vector<FLOAT>&x)
     {
         _ASSERTE(rhs.size() == x.size());
       int n = rhs.size();
      std::vector<FLOAT> tmp(n);    // Temp workspace.
   
     FLOAT b = 2.0f;
      x[0] = rhs[0] / b;
      for (int i = 1; i < n; i++) // Decomposition and forward substitution.
         {
                tmp[i] = 1 / b;
               b = (i < n - 1 ? 4.0f : 3.5f) - tmp[i];
             x[i] = (rhs[i] - x[i - 1]) / b;
          }
     for (int i = 1; i < n; i++)
         {
                x[n - i - 1] -= tmp[n - i] * x[n - i]; // Back substitution.
            }
    }

    void GetCurveControlPoints(
            __in const std::vector<D2D1_POINT_2F>&knots,
         __out std::vector<D2D1_POINT_2F>&firstCtrlPt,
        __out std::vector<D2D1_POINT_2F>&secondCtrlPt)
     {
        _ASSERTE((firstCtrlPt.size() == secondCtrlPt.size())
            && (knots.size() == firstCtrlPt.size() + 1));
    
    int n = knots.size() - 1;
        _ASSERTE(n >= 1);

if (n == 1)
    {
       // Special case: Bezier curve should be a straight line.
         // 3P1 = 2P0 + P3
        firstCtrlPt[0].x = (2 * knots[0].x + knots[1].x) / 3.0f;
             firstCtrlPt[0].y = (2 * knots[0].y + knots[1].y) / 3.0f;
    
          // P2 = 2P1 ¨C P0
      secondCtrlPt[0].x = 2 * firstCtrlPt[0].x - knots[0].x;
             secondCtrlPt[0].y = 2 * firstCtrlPt[0].y - knots[0].y;
             return;
        }

        // Right hand side vector
    std::vector<FLOAT> rhs(n);

    // Set right hand side X values
       for (int i = 1; i < (n - 1); ++i)
        {
              rhs[i] = 4 * knots[i].x + 2 * knots[i + 1].x;
           }
    rhs[0] = knots[0].x + 2 * knots[1].x;
        rhs[n - 1] = (8 * knots[n - 1].x + knots[n].x) / 2.0f;
        // Get first control points X-values
   std::vector<FLOAT> x(n);
      GetFirstControlPoints(rhs, x);
   
      // Set right hand side Y values
   for (int i = 1; i < (n - 1); ++i)
    {
          rhs[i] = 4 * knots[i].y + 2 * knots[i + 1].y;
    }
   rhs[0] = knots[0].y + 2 * knots[1].y;
      rhs[n - 1] = (8 * knots[n - 1].y + knots[n].y) / 2.0f;
      // Get first control points Y-values
   std::vector<FLOAT> y(n);
      GetFirstControlPoints(rhs, y);
  
      for (int i = 0; i < n; ++i)
      {
             // First control point
      firstCtrlPt[i] = D2D1::Point2F(x[i], y[i]);
             // Second control point
      if (i < (n - 1))
             {
                        secondCtrlPt[i] = D2D1::Point2F(2 * knots[i + 1].x - x[i + 1], 2 * knots[i + 1].y - y[i + 1]);
                    }
        else
            {
                         secondCtrlPt[i] = D2D1::Point2F((knots[n].x + x[n - 1]) / 2, (knots[n].y + y[n - 1]) / 2);
                   }
       }
    }

    HRESULT CreateBezierSpline(
        __in ID2D1Factory* pD2dFactory,
        __in const std::vector<D2D1_POINT_2F>& points,
        __out ID2D1PathGeometry** ppPathGeometry,ID2D1GeometrySink** spSink)
    {

        _ASSERTE(points.size() > 1);
        int n = points.size();
        std::vector<D2D1_POINT_2F> firstCtrlPt(n - 1);
        std::vector<D2D1_POINT_2F> secondCtrlPt(n - 1);
        GetCurveControlPoints(points, firstCtrlPt, secondCtrlPt);

        HRESULT hr = pD2dFactory->CreatePathGeometry(ppPathGeometry);

        if (FAILED(hr))
            return hr;


        hr = (*ppPathGeometry)->Open(&*spSink);
        if (SUCCEEDED(hr))
        {
            (*spSink)->SetFillMode(D2D1_FILL_MODE_WINDING);
            (*spSink)->BeginFigure(points[0], D2D1_FIGURE_BEGIN_FILLED);
            for (int i = 1; i < n; i++) {
                (*spSink)->AddBezier(D2D1::BezierSegment(firstCtrlPt[i - 1], secondCtrlPt[i - 1], points[i]));
            }(*spSink)->EndFigure(D2D1_FIGURE_END_OPEN);        (*spSink)->Close();
        }    return hr;
    }
    */
void MainUI(HWND hWnd, ID2D1HwndRenderTarget* hrt)
{
	RECT rc;
	GetClientRect(hWnd, &rc);
	D2DDrawSolidRect(hrt, 0, 0, rc.right, rc.bottom, VERTEXUICOLOR_DARKNIGHT);
    VertexUILayoutInfo lo;
    lo.Mode = vinaLayout::Left;
    lo.interval = 20;
    lo.x = 20;
    lo.y = 20;
    lo.uniCx =100;
    lo.uniCy = 100;
	NewUIPanel->Set(hWnd, hrt);
    
	//btn->Set(20, 20, 100, 40,L"QwQ");
	for (auto iter = comVina.begin(); iter != comVina.end(); ++iter)
	{
       
        if (std::holds_alternative<std::shared_ptr<VinaButton>>(iter->second))
        {
            NewUIPanel->Add(std::get<std::shared_ptr<VinaButton>>(iter->second));
        }
        if (std::holds_alternative<std::shared_ptr<VinaText>>(iter->second))
        {
            NewUIPanel->Add(std::get<std::shared_ptr<VinaText>>(iter->second));
        }
        if (std::holds_alternative<std::shared_ptr<VinaFAIcon>>(iter->second))
        {
            NewUIPanel->Add(std::get<std::shared_ptr<VinaFAIcon>>(iter->second));
        }
	}
   // static VinaSwitch* bs = new VinaSwitch;
    //VertexUICtlColor ci;
    //ci.activeBorderColor = VuiDarkenColor(ci.activeColor,40);
    //bs->Set(200, 400, 60, 30, ci);
    //NewUIPanel->Add(bs);
    /*
    ID2D1GeometrySink* pSink = NULL;
    m_pD2DFactory->CreatePathGeometry(&m_pPathGeometry);
    m_pPathGeometry->Open(&pSink);

      
        struct NSVGimage* image;
        image = nsvgParseFromFile("test.svg", "px", 96);
        NSVGshape* shape = NULL;
        NSVGpath* path = NULL;
        printf("size: %f x %f\n", image->width, image->height);
        // Use...
        std::vector<D2D1_POINT_2F>mpt;
        for (shape = image->shapes; shape != NULL; shape = shape->next) {
            for (path = shape->paths; path != NULL; path = path->next) {
                for (auto i = 0; i < path->npts - 1; i += 3) {
                    float* p = &path->pts[i * 2];
                    mpt.push_back(D2D1::Point2F(p[0], p[1]));
                    mpt.push_back(D2D1::Point2F(p[2], p[3]));
                    mpt.push_back(D2D1::Point2F(p[4], p[5]));
                    mpt.push_back(D2D1::Point2F(p[6], p[7]));
                   // drawCubicBez(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
                }
            }
        }

        CreateBezierSpline(m_pD2DFactory, mpt, &m_pPathGeometry, &pSink);
       
    ID2D1SolidColorBrush* testBrush = 0;
    hrt->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF(RGBToHex(RGB(255, 255, 255)), 1)),
        &testBrush
    );
    hrt->DrawGeometry(m_pPathGeometry, testBrush, 1.f);
    SafeRelease(&testBrush);
    SafeRelease(&pSink);
    SafeRelease(&m_pPathGeometry);

    nsvgDelete(image);
    */
   // hrt->FillGeometry(m_pPathGeometry, m_pLGBrush);
}