#pragma once
#include "DrawingContext.h"

namespace vui
{
	void AverageBlt(DrawingContext &dest, POINT p, SIZE s, DrawingContext &src, POINT pSrc, int scale)
	{
		HDC dst = dest.handle();
		HDC sr = src.handle();
		HBITMAP srb = (HBITMAP)GetCurrentObject(sr, OBJ_BITMAP);
		BITMAPINFO bis{0};
		bis.bmiHeader.biSize = sizeof(bis.bmiHeader);
		GetDIBits(sr, srb, 0, 0, 0, &bis, DIB_RGB_COLORS);
		BYTE *lpSrcBits = new BYTE[bis.bmiHeader.biSizeImage];
		GetBitmapBits(srb, bis.bmiHeader.biSizeImage, lpSrcBits);
		HBITMAP srd = (HBITMAP)GetCurrentObject(dst, OBJ_BITMAP);
		BITMAPINFO bid{0};
		bid.bmiHeader.biSize = sizeof(bid.bmiHeader);
		GetDIBits(dst, srd, 0, 0, 0, &bid, DIB_RGB_COLORS);
		BYTE *lpDstBits = new BYTE[bid.bmiHeader.biSizeImage];
		GetBitmapBits(srd, bid.bmiHeader.biSizeImage, lpDstBits);

		for (int dY = p.y; dY < min(bid.bmiHeader.biHeight, p.y + s.cy); dY++)
		{
			for (int dX = p.x; dX < min(bid.bmiHeader.biWidth, p.x + s.cx); dX++)
			{
				int dI = (dX + dY * bid.bmiHeader.biWidth) * 4;
				int sI = ((dX - p.x) + (dY - p.y) * bis.bmiHeader.biWidth) * scale * 4;
				int resultRed = 0;
				int resultGreen = 0;
				int resultBlue = 0;
				for (int i = 0; i < scale; i++)
				{
					for (int j = 0; j < scale; j++)
					{
						int sIo = sI + i * bis.bmiHeader.biWidth * 4 + j * 4;
#ifdef _DEBUG
						if (static_cast<DWORD>(sIo) > bis.bmiHeader.biSizeImage)
							sIo = sI;
#endif
						resultRed += lpSrcBits[sIo + 2];
						resultGreen += lpSrcBits[sIo + 1];
						resultBlue += lpSrcBits[sIo];
					}
				}
				lpDstBits[dI + 2] = (BYTE)(resultRed / scale / scale);
				lpDstBits[dI + 1] = (BYTE)(resultGreen / scale / scale);
				lpDstBits[dI] = (BYTE)(resultBlue / scale / scale);
			}
		}
		delete[] lpSrcBits;
		SetBitmapBits(srd, bid.bmiHeader.biSizeImage, lpDstBits);
		delete[] lpDstBits;
	}

	class CopyFromUpscale : public DrawingContext::DrawingContextExtension
	{
		POINT pDst;
		SIZE sDst;
		POINT pSr;
		DrawingContext *pS;
		int sl;

	public:
		CopyFromUpscale(POINT p, SIZE s, DrawingContext &src, POINT pSrc = POINT{0}, int scale = 4)
		{
			pDst = p;
			sDst = s;
			pSr = pSrc;
			pS = &src;
			sl = scale;
		}
		void operator()(DrawingContext &dest) override
		{
			AverageBlt(dest, pDst, sDst, *pS, pSr, sl);
		}
	};
}