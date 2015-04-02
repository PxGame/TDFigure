#include "TDFigure.h"

CTDFigure::CTDFigure()
{
	m_hdc = NULL;
	m_bufHdc = NULL;
	m_bufBitmap = NULL;
	m_penAxis = NULL;

	SetRect(&m_rtScope, 0, 0, 0, 0);
	m_nMargin = 0;
	m_strXName = TEXT("X");
	m_strYName = TEXT("Y");

	m_nArrowSize = 10;
	m_nFontZone = 20;

	m_nWidth = 0;
	m_nHeight = 0;
	m_ptZero.x = 0;
	m_ptZero.y = 0;
	m_nXLength = 0;
	m_nYLength = 0;

	m_oldPenAxis = NULL;
	m_oldBufBitmap = NULL;
}

CTDFigure::~CTDFigure()
{
	if (NULL != m_penAxis)
	{
		DeleteObject(m_penAxis);
	}
	if (NULL != m_bufHdc)
	{
		DeleteObject(m_bufHdc);
	}
	if (NULL != m_bufBitmap)
	{
		DeleteObject(m_bufBitmap);
	}
}

BOOL CTDFigure::Init(HDC hdc, RECT rtScope, int nMargin)
{
	m_hdc = hdc;
	m_bufHdc = CreateCompatibleDC(m_hdc);
	SetBkMode(m_bufHdc, TRANSPARENT);

	m_rtScope = rtScope;
	m_nMargin = nMargin;

	m_nWidth = m_rtScope.right - m_rtScope.left;
	if (m_nWidth <= 0)
	{
		return FALSE;
	}

	m_nHeight = m_rtScope.bottom - m_rtScope.top;
	if (m_nHeight <= 0)
	{
		return FALSE;
	}

	m_ptZero.x = m_nMargin;
	m_ptZero.y = m_nHeight - m_nMargin;
	m_nXLength = m_nWidth - 2 * m_nMargin;
	if (m_nXLength <= 0)
	{
		return FALSE;
	}
	m_nYLength = m_nHeight - 2 * m_nMargin;
	if (m_nYLength <= 0)
	{
		return FALSE;
	}
	m_nValidXLength = (int)(m_nXLength * 0.9f);
	if (m_nValidXLength <= 0)
	{
		return FALSE;
	}
	m_nValidYLength = (int)(m_nYLength * 0.9f);
	if (m_nValidYLength <= 0)
	{
		return FALSE;
	}

	m_bufBitmap = CreateCompatibleBitmap(m_hdc, m_nWidth, m_nHeight);
	if (m_bufBitmap == NULL)
	{
		return FALSE;
	}
	m_oldBufBitmap = (HBITMAP)SelectObject(m_bufHdc, m_bufBitmap);

	m_penAxis = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	if (m_penAxis == NULL)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CTDFigure::DrawDc()
{
	FillRect(m_bufHdc, &m_rtScope, (HBRUSH)GetStockObject(WHITE_BRUSH));
	DrawXY();
	return BitBlt(m_hdc, m_rtScope.left, m_rtScope.top, m_nWidth, m_nHeight, m_bufHdc, 0, 0, SRCCOPY);
	//BLENDFUNCTION bldt;
	//bldt.BlendOp = AC_SRC_OVER;
	//bldt.BlendFlags = 0;
	//bldt.AlphaFormat = AC_SRC_ALPHA;
	//bldt.SourceConstantAlpha = 255;
	//return AlphaBlend(m_hdc, m_rtScope.left, m_rtScope.top, m_nWidth, m_nHeight,
	//	m_bufHdc, 0, 0, m_nWidth, m_nHeight, bldt);
}

BOOL CTDFigure::DrawXY()
{
	POINT bufPt = { 0, 0 };
	m_oldPenAxis = (HPEN)SelectObject(m_bufHdc, m_penAxis);

	MoveToEx(m_bufHdc, m_ptZero.x, m_ptZero.y, NULL);

	bufPt.x = m_ptZero.x + m_nXLength;
	bufPt.y = m_ptZero.y;
	LineTo(m_bufHdc, bufPt.x, bufPt.y);
	MoveToEx(m_bufHdc, bufPt.x, bufPt.y, NULL);
	LineTo(m_bufHdc, bufPt.x - m_nArrowSize, bufPt.y - m_nArrowSize);
	MoveToEx(m_bufHdc, bufPt.x, bufPt.y, NULL);
	LineTo(m_bufHdc, bufPt.x - m_nArrowSize, bufPt.y + m_nArrowSize);

	MoveToEx(m_bufHdc, m_ptZero.x, m_ptZero.y, NULL);

	bufPt.x = m_ptZero.x ;
	bufPt.y = m_ptZero.y - m_nYLength;
	LineTo(m_bufHdc, bufPt.x, bufPt.y);
	MoveToEx(m_bufHdc, bufPt.x, bufPt.y, NULL);
	LineTo(m_bufHdc, bufPt.x - m_nArrowSize, bufPt.y + m_nArrowSize);
	MoveToEx(m_bufHdc, bufPt.x, bufPt.y, NULL);
	LineTo(m_bufHdc, bufPt.x + m_nArrowSize, bufPt.y + m_nArrowSize);

	SelectObject(m_bufHdc, m_oldPenAxis);

	if (!m_strXName.empty())
	{
		TextOut(m_bufHdc,
			m_nWidth - m_nMargin,
			m_nHeight - m_nMargin + m_nArrowSize,
			m_strXName.c_str(),
			m_strXName.length());
	}

	if (!m_strYName.empty())
	{
		SIZE size;
		::GetTextExtentPoint(m_bufHdc, m_strYName.c_str(), m_strYName.length(), &size);

		TextOut(m_bufHdc,
			m_nMargin - size.cx - m_nArrowSize < 0 ? 0 : m_nMargin - size.cx - m_nArrowSize,
			m_nMargin - m_nArrowSize,
			m_strYName.c_str(),
			m_strYName.length());
	}

	return TRUE;
}

