#pragma once

#include <stdlib.h>

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>

//
// 更新进度的回调函数指针
//

typedef bool (__stdcall *PF_UPD_PROGRESS)(LPVOID pProgressInfo, int nTotal, int nComplete);

//
// 测试用户取消的回调函数指针
//

typedef bool (__stdcall *PF_TESTABORT)(void);

//
// 滤镜接口（用于多态）
//
class IFilter
{
protected:
	bool m_bMultiThreads;		//是否启用多线程
	int m_nThreadCount;			//用户建议的多线程数
	LPVOID m_pProgInfo;			//进度反馈的参数，传回给用户
	PF_UPD_PROGRESS m_pUpdProg; //进度反馈回调
	PF_TESTABORT m_pTestAbort; //测试取消回调

public:
	IFilter()
	{
		m_bMultiThreads = false;
		m_nThreadCount = 1;

		m_pProgInfo = NULL;
		m_pUpdProg = NULL;
		m_pTestAbort = NULL;
	}
	virtual ~IFilter() { };
	virtual void Init(LPVOID pInfo) { }; //用于设置参数
	virtual void Reset() { };

	bool GetMultiThreadEnabled() const {	return m_bMultiThreads; };
	int GetThreadCount() const { return m_nThreadCount; }
	PF_UPD_PROGRESS GetUpdateProgressFunc() const { return m_pUpdProg; };
	PF_TESTABORT GetTestAbortFunc() const { return m_pTestAbort; };

	void SetMultiThreads(bool bEnabled, int ThreadCount)
	{
		m_bMultiThreads = bEnabled;
		m_nThreadCount = ThreadCount;
	}

	void SetUpdateProgressFunc(PF_UPD_PROGRESS pF, LPVOID pInfo)
	{
		m_pUpdProg = pF;
		m_pProgInfo = pInfo;
	}

	void SetTestAbortFunc(PF_TESTABORT pF) { m_pTestAbort = pF; };

	virtual bool UpdateProgress(int nTotal, int nComplete) const
	{
		if(m_pUpdProg == NULL)
			return false;

		return m_pUpdProg(m_pProgInfo, nTotal, nComplete);
	}

	virtual bool TestAbort() const
	{
		if(m_pTestAbort == NULL)
			return false;

		return m_pTestAbort();
	}

	virtual bool Filter(LPCVOID pSrc, LPVOID pDest, int width, int height, int bpp)
	{
		return false;
	}
};

//
// Thread Function Declares;
//

template<typename T>
DWORD WINAPI GaussBlurThreadProc8(LPVOID lpParameters);

template<typename T>
DWORD WINAPI GaussBlurThreadProc24(LPVOID lpParameters);

//
// 传递到线程的参数
//
template<typename T>
class CGaussBlurThreadParams
{
public:
	int r;
	T* pTempl;
	LPBYTE pSrc;	//Src  位图的位图数据起点 （对所有线程，pSrc，pDest 是相同的）
	LPBYTE pDest;	//Dest 位图的位图数据起点

	int width;		//图像宽度
	int height;		//图像高度（已经被取绝对值）

	//处理的行范围：[rowBegin, rowEnd) ，不包括 rowEnd （每个线程不同）
	int rowBegin;
	int rowEnd;
	int stride;		//扫描行宽度（bytes）
	int pixelSize;	//像素大小 =bpp/8;

	bool bHorz;		//true-水平模糊，false-纵向模糊

public:
	CGaussBlurThreadParams() { };
	~CGaussBlurThreadParams() { };
};

//
// 高斯模糊算法
//

template<typename T>
class CGaussBlurFilter : public IFilter
{
protected:
	int m_r;		//完整模板正方形的边长为 (2 * r + 1)
	T m_sigma;		//高斯半径（方差的平方根）
	T* m_pTempl;	//模板T[r+1];

public:
	CGaussBlurFilter();
	virtual ~CGaussBlurFilter();

	int GetR() const { return m_r; };
	T GetSigma() const { return m_sigma; };

	//设置参数（此时会分配模板）
	void SetSigma(T sigma);

	//设置参数
	virtual void Init(LPVOID pInfo);

	//释放模板
	virtual void Reset();

	//处理图像
	virtual bool Filter(LPCVOID pSrc, LPVOID pDest, int width, int height, int bpp);
};

template<typename T>
CGaussBlurFilter<T>::CGaussBlurFilter()
{
	m_r = -1;
	m_sigma = (T)(-1);
	m_pTempl = NULL;
	m_bMultiThreads = false;
	m_nThreadCount = 0;
}

template<typename T>
CGaussBlurFilter<T>::~CGaussBlurFilter()
{
	if(m_pTempl != NULL)
		free(m_pTempl);
}

template<typename T>
void CGaussBlurFilter<T>::SetSigma(T sigma)
{
	int i;
	m_sigma = sigma;
	m_r = (int)(m_sigma * 3 + 0.5);
	if(m_r <= 0) m_r = 1;

	//分配模板
	LPVOID pOldTempl = m_pTempl;
	m_pTempl = (T*)realloc(m_pTempl, sizeof(T) * (m_r + 1));

	//分配失败？
	if(m_pTempl == NULL)
	{
		if(pOldTempl != NULL)
			free(pOldTempl);

		return;
	}
	
	//计算 p[0] 灰度值为1 的模板面
	T k1 = (T)((-0.5) / (m_sigma * m_sigma));
	for(i = 0; i <= m_r; i++)
		m_pTempl[i] = exp(k1 * i * i);

	//计算模板加权总和
	T sum = m_pTempl[0];
	for(i = 1; i <= m_r; i++)
	{
		sum += (m_pTempl[i] * 2);
	}
	
	//归一化
	sum = (T)(1.0 / sum); //取倒数
	for(i = 0; i <= m_r; i++)
		m_pTempl[i] *= sum;
}

//
// 在CGaussBlurFilter 中，pInfo 是一个指向 T sigma 的指针；
//
template<typename T>
void CGaussBlurFilter<T>::Init(LPVOID pInfo)
{
	T* pT = (T*)pInfo;
	SetSigma(*pT);
}

template<typename T>
void CGaussBlurFilter<T>::Reset()
{
	m_r = -1;
	m_sigma = (T)(-1.0);
	if(m_pTempl != NULL)
	{
		free(m_pTempl);
		m_pTempl = NULL;
	}
}

template<typename T>
bool CGaussBlurFilter<T>::Filter(LPCVOID pSrc, LPVOID pDest, int width, int height, int bpp)
{
	if(pSrc == NULL || pDest == NULL)
		return false;

	//只能处理 8, 24， 32 bpp
	if(bpp != 24 && bpp != 8 && bpp != 32)
		return false;

	if(m_r < 0 || m_pTempl == NULL)
		return false;

	int absHeight = (height >= 0) ? height : (-height);
	int stride = (width * bpp + 31)/ 32 * 4;
	int pixelSize = bpp / 8;
	int i, ThreadCount;
	DWORD dwTid;

	//申请缓冲区，存储中间结果
	LPVOID pTemp = malloc(stride * absHeight);
	if(pTemp == NULL)
		return false;

	//判断是否启用多线程处理
	if(m_bMultiThreads && m_nThreadCount > 1)
	{
		ThreadCount = min(m_nThreadCount, absHeight);

		CGaussBlurThreadParams<T> *p1 = new CGaussBlurThreadParams<T>[ThreadCount];
		HANDLE *pHandles = new HANDLE[ThreadCount];

		//水平方向
		for(i = 0; i < ThreadCount; i++)
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
			if(i == ThreadCount - 1)
				p1[i].rowEnd = absHeight;
			else
				p1[i].rowEnd = p1[i].rowBegin + absHeight / ThreadCount;

			p1[i].bHorz = true;

			//Committed StackSize = 512; 线程需要的栈的大小
			pHandles[i] = CreateThread(NULL, 512, 
				(bpp == 8)? GaussBlurThreadProc8<T> : GaussBlurThreadProc24<T>,
				(LPVOID)(&p1[i]), 0, &dwTid);
		}
		WaitForMultipleObjects(ThreadCount, pHandles, TRUE, INFINITE);
		for(i = 0; i < ThreadCount; i++)
			CloseHandle(pHandles[i]);

		UpdateProgress(100, 50);

		//垂直方向
		for(i = 0; i < ThreadCount; i++)
		{
			p1[i].pSrc = (LPBYTE)pTemp;
			p1[i].pDest = (LPBYTE)pDest;
			p1[i].bHorz = false;

			pHandles[i] = CreateThread(NULL, 512, 
				(bpp == 8)? GaussBlurThreadProc8<T> : GaussBlurThreadProc24<T>,
				(LPVOID)(&p1[i]), 0, &dwTid);
		}
		WaitForMultipleObjects(ThreadCount, pHandles, TRUE, INFINITE);
		for(i = 0; i < ThreadCount; i++)
			CloseHandle(pHandles[i]);

		delete[] p1;
		delete[] pHandles;
	}
	else
	{
		//单线程
		CGaussBlurThreadParams<T> params;

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

		if(bpp == 8)
			GaussBlurThreadProc8<T>(&params);
		else
			GaussBlurThreadProc24<T>(&params);

		UpdateProgress(100, 50);

		params.pSrc = (LPBYTE)pTemp;
		params.pDest = (LPBYTE)pDest;
		params.bHorz = false;
		
		if(bpp == 8)
			GaussBlurThreadProc8<T>(&params);
		else
			GaussBlurThreadProc24<T>(&params);
	}

	free(pTemp);
	UpdateProgress(100, 100);
	return true;
}

//线程的入口点: 处理 8 bpp 图像（灰度图像，全索引）
template<typename T>
DWORD WINAPI GaussBlurThreadProc8(LPVOID lpParameters)
{
	CGaussBlurThreadParams<T> *pInfo = (CGaussBlurThreadParams<T>*)lpParameters;

	T result;
	int row, col, subRow, subCol, MaxVal, x, x1;
	LPINT pSubVal, pRefVal;

	if(pInfo->bHorz)
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

	for(row = pInfo->rowBegin; row < pInfo->rowEnd; ++row)
	{
		for(col = 0; col < pInfo->width; ++col)
		{
			pDestPixel = pInfo->pDest + pInfo->stride * row + col;

			result = 0;

			subRow = row;
			subCol = col;

			for(x = -pInfo->r; x <= pInfo->r; x++)
			{
				//边界处理
				x1 = (x >= 0) ? x : (-x);
				*pSubVal = *pRefVal + x;
				if(*pSubVal < 0) *pSubVal = 0;
				else if(*pSubVal > MaxVal) *pSubVal = MaxVal;
				
				pSrcPixel = pInfo->pSrc + pInfo->stride * subRow + subCol;

				result += *pSrcPixel * pInfo->pTempl[x1];
			}
			*pDestPixel = (BYTE)result;
		}
	}
	return 0;
}

//线程的入口点: 处理 24 bpp 以上图像
template<typename T>
DWORD WINAPI GaussBlurThreadProc24(LPVOID lpParameters)
{
	CGaussBlurThreadParams<T> *pInfo = (CGaussBlurThreadParams<T>*)lpParameters;

	T result[3];
	int row, col, subRow, subCol, MaxVal, x, x1;
	LPINT pSubVal, pRefVal;

	if(pInfo->bHorz)
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

	for(row = pInfo->rowBegin; row < pInfo->rowEnd; ++row)
	{
		for(col = 0; col < pInfo->width; ++col)
		{
			pDestPixel = pInfo->pDest + pInfo->stride * row + pInfo->pixelSize * col;

			result[0] = 0;
			result[1] = 0;
			result[2] = 0;

			subRow = row;
			subCol = col;

			for(x = -pInfo->r; x <= pInfo->r; x++)
			{
				x1 = (x >= 0) ? x : (-x);
				*pSubVal = *pRefVal + x;

				//边界处理：Photoshop 采用的是方法1。
				//方法1：取边缘像素（图像边缘像素向内部扩散！）
				if(*pSubVal < 0) *pSubVal = 0;
				else if(*pSubVal > MaxVal) *pSubVal = MaxVal;

				//方法2：取当前像素（使得越靠近图像边缘的地方越清晰）
				/*
				if(*pSubVal < 0 || *pSubVal > MaxVal)
					*pSubVal = *pRefVal;
				*/
				
				pSrcPixel = pInfo->pSrc + pInfo->stride * subRow + pInfo->pixelSize * subCol;

				result[0] += pSrcPixel[0] * pInfo->pTempl[x1];
				(pSrcPixel[1]<255) ? result[1] += (pSrcPixel[1] + 1) * pInfo->pTempl[x1] : result[1] += pSrcPixel[1] * pInfo->pTempl[x1];
				result[2] += pSrcPixel[2] * pInfo->pTempl[x1];
			}
			pDestPixel[0] = (BYTE)result[0];
			pDestPixel[1] = (BYTE)result[1];
			pDestPixel[2] = (BYTE)result[2];
		}
	}
	return 0;
}