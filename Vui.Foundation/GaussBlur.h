#pragma once
#include <cmath>
#include <minwindef.h>
#include <processthreadsapi.h>
#include <synchapi.h>
#include <handleapi.h>
#include <WinBase.h>
#include <functional>
#include "DrawingContext.h"
#include <thread>
#include "Misc.h"
namespace vui
{
	//
	// 高斯模糊算法
	//

	class GaussBlur : public DrawingContext::DrawingContextExtension
	{
	private:
		int m_r;		  //完整模板正方形的边长为 (2 * r + 1)
		double m_sigma;	  //高斯半径（方差的平方根）
		double *m_pTempl; //模板T[r+1];
		DrawingContext *src;

		RECT rt;
		int ratio;
		int clr;
		std::function<bool()> *fbcb;
		//
		// 传递到线程的参数
		//
		struct GaussBlurThreadParams
		{
			int r;
			double *pTempl;
			LPBYTE pSrc;  // Src  位图的位图数据起点 （对所有线程，pSrc，pDest 是相同的）
			LPBYTE pDest; // Dest 位图的位图数据起点

			int width;	//图像宽度
			int height; //图像高度（已经被取绝对值）

			//处理的行范围：[rowBegin, rowEnd) ，不包括 rowEnd （每个线程不同）
			int rowBegin;
			int rowEnd;
			int stride;	   //扫描行宽度（bytes）
			int pixelSize; //像素大小 =bpp/8;

			bool bHorz; // true-水平模糊，false-纵向模糊
		};
		bool Filter(unsigned char *const pSrc, unsigned char *pDest, unsigned int width, unsigned int height, int bpp)
		{
			if (pSrc == NULL || pDest == NULL)
				return false;

			//只能处理 8, 24， 32 bpp
			if (bpp != 24 && bpp != 8 && bpp != 32)
				return false;

			if (m_r < 0 || m_pTempl == NULL)
				return false;

			unsigned int absHeight = height;
			unsigned int stride = (width * bpp + 31) / 32 * 4;
			unsigned int pixelSize = bpp / 8;
			unsigned int i, ThreadCount;
			DWORD dwTid;

			//申请缓冲区，存储中间结果
			auto pTemp = new unsigned char[stride * absHeight];
			if (pTemp == 0)
				return false;

			//判断是否启用多线程处理
			if (stride * absHeight >= 200 * 200 * 4)
			{
				ThreadCount = min(std::thread::hardware_concurrency(), absHeight);

				GaussBlurThreadParams *p1 = new GaussBlurThreadParams[ThreadCount];
				HANDLE *pHandles = new HANDLE[ThreadCount];

				//水平方向
				for (i = 0; i < ThreadCount; i++)
				{
					p1[i].pSrc = (LPBYTE)pSrc;
					p1[i].pDest = (LPBYTE)pTemp;
					p1[i].width = width;
					p1[i].height = absHeight;
					p1[i].stride = stride;
					p1[i].pixelSize = pixelSize;
					p1[i].r = m_r;
					p1[i].pTempl = m_pTempl;

					//分配子任务
					p1[i].rowBegin = absHeight / ThreadCount * i;

					//高度不一定能被 ThreadCount 整除！所以最后一个任务的结尾必须显式指定！
					if (i == ThreadCount - 1)
						p1[i].rowEnd = absHeight;
					else
						p1[i].rowEnd = p1[i].rowBegin + absHeight / ThreadCount;

					p1[i].bHorz = true;

					// Committed StackSize = 512; 线程需要的栈的大小
					pHandles[i] = CreateThread(NULL, 512,
											   (bpp == 8) ? GaussBlurThreadProc8 : GaussBlurThreadProc24,
											   (LPVOID)(&p1[i]), 0, &dwTid);
				}
				WaitForMultipleObjects(ThreadCount, pHandles, TRUE, INFINITE);
				for (i = 0; i < ThreadCount; i++)
					CloseHandle(pHandles[i]);
				//垂直方向
				for (i = 0; i < ThreadCount; i++)
				{
					p1[i].pSrc = (LPBYTE)pTemp;
					p1[i].pDest = (LPBYTE)pDest;
					p1[i].bHorz = false;

					pHandles[i] = CreateThread(NULL, 512,
											   (bpp == 8) ? GaussBlurThreadProc8 : GaussBlurThreadProc24,
											   (LPVOID)(&p1[i]), 0, &dwTid);
				}
				WaitForMultipleObjects(ThreadCount, pHandles, TRUE, INFINITE);
				for (i = 0; i < ThreadCount; i++)
					CloseHandle(pHandles[i]);

				delete[] p1;
				delete[] pHandles;
			}
			else
			{
				//单线程
				GaussBlurThreadParams params;

				params.pSrc = (LPBYTE)pSrc;
				params.pDest = (LPBYTE)pTemp;
				params.width = width;
				params.height = absHeight;
				params.stride = stride;
				params.pixelSize = pixelSize;
				params.r = m_r;
				params.pTempl = m_pTempl;
				params.rowBegin = 0;
				params.rowEnd = absHeight;
				params.bHorz = true;

				if (bpp == 8)
					GaussBlurThreadProc8(&params);
				else
					GaussBlurThreadProc24(&params);

				params.pSrc = (LPBYTE)pTemp;
				params.pDest = (LPBYTE)pDest;
				params.bHorz = false;

				if (bpp == 8)
					GaussBlurThreadProc8(&params);
				else
					GaussBlurThreadProc24(&params);
			}

			delete[] pTemp;
			return true;
		}
		//设置参数（此时会分配模板）
		void SetSigma(double sigma)
		{
			m_sigma = sigma;
			m_r = static_cast<int>(round(m_sigma * 3));
			if (m_r <= 0)
				m_r = 1;

			//分配模板
			if (!m_pTempl)
			{
				delete[] m_pTempl;
				m_pTempl = 0;
			}

			//分配失败？
			if (!(m_pTempl = new double[m_r]))
			{
				throw;
			}

			//计算 p[0] 灰度值为1 的模板面
			double k1 = -0.5 / pow(m_sigma, 2);
			for (int i = 0; i <= m_r; i++)
				m_pTempl[i] = exp(k1 * i * i);

			//计算模板加权总和
			double sum = m_pTempl[0];
			for (int i = 1; i <= m_r; i++)
				sum += (m_pTempl[i] * 2);

			//归一化
			sum = 1. / sum; //取倒数
			for (int i = 0; i <= m_r; i++)
				m_pTempl[i] *= sum;
		}

	public:
		GaussBlur(double sigma, DrawingContext &sdc, RECT rt = RECT{0}, int downscaleratio = 1, int fallbackclr = RGB(vuicolor.ctl_blur_r, vuicolor.ctl_blur_g, vuicolor.ctl_blur_b), std::function<bool()> *usefallback = nullptr)
		{
			SetSigma(sigma);
			src = &sdc;
			this->rt = rt;
			ratio = downscaleratio;
			clr = fallbackclr;
			fbcb = usefallback;
		}
		~GaussBlur()
		{
			if (!m_pTempl)
				delete m_pTempl;
			m_pTempl = 0;
		}
		void operator()(DrawingContext &dc) override
		{
			if (fbcb == nullptr || (*fbcb)())
			{
				auto &hsrc = *src;
				auto sz = SIZE{rt.right - rt.left, rt.bottom - rt.top};
				DrawingContext tmpsrc{sz};
				DrawingContext tmpdst{sz};
				tmpsrc.CopyFrom(hsrc, POINT{0}, POINT{rt.left, rt.top}, sz);
				auto& sbmp = tmpsrc.GetBitmap();
				auto& dbmp = tmpdst.GetBitmap();
				vui::dout << "GaussBlur for size{" << sz.cx << "," << sz.cy << "} 32-bits bmp." << "\n";                                                                                                     \
																													LARGE_INTEGER start_time;                                                                                               \
																													QueryPerformanceCounter(&start_time);
				Filter(sbmp.lock(),dbmp.lock(),sz.cx ,sz.cy,32);
				LARGE_INTEGER end_time;             \
					QueryPerformanceCounter(&end_time); \
					LARGE_INTEGER freq;                 \
					QueryPerformanceFrequency(&freq);   \
					vui::dout << "GaussBlur took " << double((end_time.QuadPart - start_time.QuadPart) * 1000000) / freq.QuadPart / 1000 << "ms\n";
				sbmp.unlock();
				dbmp.unlock();
				dc.CopyFrom(tmpdst,POINT{rt.left, rt.top},POINT{0},sz);
			}
			else
			{
				FillRect(dc.handle(), &rt, vui::SolidBrush(clr).brush()); // 纯色回滚
			}
		}

		//线程的入口点: 处理 8 bpp 图像（灰度图）
		static inline DWORD WINAPI GaussBlurThreadProc8(LPVOID lpParameters)
		{
			GaussBlurThreadParams *pInfo = (GaussBlurThreadParams *)lpParameters;

			double result;
			int row, col, subRow, subCol, MaxVal, x, x1;
			LPINT pSubVal, pRefVal;

			if (pInfo->bHorz)
			{
				//水平方向
				pSubVal = &subCol;
				pRefVal = &col;
				MaxVal = pInfo->width - 1;
			}
			else
			{
				//垂直方向
				pSubVal = &subRow;
				pRefVal = &row;
				MaxVal = pInfo->height - 1;
			}

			LPBYTE pSrcPixel = NULL;
			LPBYTE pDestPixel = NULL;

			for (row = pInfo->rowBegin; row < pInfo->rowEnd; ++row)
			{
				for (col = 0; col < pInfo->width; ++col)
				{
					pDestPixel = pInfo->pDest + pInfo->stride * row + col;

					result = 0;

					subRow = row;
					subCol = col;

					for (x = -pInfo->r; x <= pInfo->r; x++)
					{
						//边界处理
						x1 = (x >= 0) ? x : (-x);
						*pSubVal = *pRefVal + x;
						if (*pSubVal < 0)
							*pSubVal = 0;
						else if (*pSubVal > MaxVal)
							*pSubVal = MaxVal;

						pSrcPixel = pInfo->pSrc + pInfo->stride * subRow + subCol;

						result += *pSrcPixel * pInfo->pTempl[x1];
					}
					*pDestPixel = (BYTE)result;
				}
			}
			return 0;
		}

		//线程的入口点: 处理 32 bpp 图像（真彩色）
		static inline DWORD WINAPI GaussBlurThreadProc24(LPVOID lpParameters)
		{
			GaussBlurThreadParams *pInfo = (GaussBlurThreadParams *)lpParameters;
			double result[3];
			int row, col, subRow, subCol, MaxVal, x, x1;
			LPINT pSubVal, pRefVal;

			if (pInfo->bHorz)
			{
				//水平方向
				pSubVal = &subCol;
				pRefVal = &col;
				MaxVal = pInfo->width - 1;
			}
			else
			{
				//垂直方向
				pSubVal = &subRow;
				pRefVal = &row;
				MaxVal = pInfo->height - 1;
			}

			LPBYTE pSrcPixel = NULL;
			LPBYTE pDestPixel = NULL;

			for (row = pInfo->rowBegin; row < pInfo->rowEnd; ++row)
			{
				for (col = 0; col < pInfo->width; ++col)
				{
					pDestPixel = pInfo->pDest + pInfo->stride * row + pInfo->pixelSize * col;

					result[0] = 0;
					result[1] = 0;
					result[2] = 0;

					subRow = row;
					subCol = col;

					for (x = -pInfo->r; x <= pInfo->r; x++)
					{
						x1 = (x >= 0) ? x : (-x);
						*pSubVal = *pRefVal + x;

						//边界处理：Photoshop 采用的是方法1。
						//方法1：取边缘像素（图像边缘像素向内部扩散！）
						if (*pSubVal < 0)
							*pSubVal = 0;
						else if (*pSubVal > MaxVal)
							*pSubVal = MaxVal;

						//方法2：取当前像素（使得越靠近图像边缘的地方越清晰）
						/*
						if(*pSubVal < 0 || *pSubVal > MaxVal)
							*pSubVal = *pRefVal;
						*/

						pSrcPixel = pInfo->pSrc + pInfo->stride * subRow + pInfo->pixelSize * subCol;

						result[0] += pSrcPixel[0] * pInfo->pTempl[x1];
						result[1] += pSrcPixel[1] * pInfo->pTempl[x1];
						result[2] += pSrcPixel[2] * pInfo->pTempl[x1];
					}
					pDestPixel[0] = (BYTE)result[0];
					pDestPixel[1] = (BYTE)result[1];
					pDestPixel[2] = (BYTE)result[2];
				}
			}
			return 0;
		}
	};
}