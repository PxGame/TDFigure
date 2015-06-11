#include "PieDiagram.h"


CPieDiagram::CPieDiagram()
{
	SetRect(&m_rtScope, 0, 0, 0, 0);
	m_nMargin = 0;
	m_nRadius = 0;
	m_ptZero.x = 0;
	m_ptZero.y = 0;

	m_hdc = NULL;
	m_bufHdc = NULL;
	m_bufBitmap = NULL;
	m_oldBufBitmap = NULL;
}


CPieDiagram::~CPieDiagram()
{
	SelectObject(m_bufHdc, m_oldBufBitmap);
	DeleteObject(m_bufBitmap);
	DeleteDC(m_bufHdc);
}

BOOL CPieDiagram::Init(HDC hdc, RECT rtScope, int nMargin, vector<PIEDIAGRAMITEM> items)
{
	m_nWidth = rtScope.right - rtScope.left;
	m_nHeight = rtScope.bottom - rtScope.top;
	if (m_nWidth - nMargin <= 0 || m_nHeight - nMargin <= 0)
	{
		return FALSE;
	}

	m_hdc = hdc;
	m_bufBitmap = CreateCompatibleBitmap(m_hdc, m_nWidth, m_nHeight);
	m_bufHdc = CreateCompatibleDC(m_hdc);

	SetBkMode(m_bufHdc, TRANSPARENT);
	m_oldBufBitmap = (HBITMAP)SelectObject(m_bufHdc, m_bufBitmap);

	m_nMargin = nMargin;
	m_vtItems = items;

	if (m_nWidth > m_nHeight)
	{
		m_nRadius = m_nHeight / 2 - m_nMargin;
	}
	else
	{
		m_nRadius = m_nWidth / 2 - m_nMargin;
	}

	m_ptZero.x = m_nMargin + m_nRadius;
	m_ptZero.y = m_ptZero.x;

	return TRUE;
}

BOOL CPieDiagram::DrawDc()
{
	RECT rtBkg = { 0, 0, m_nWidth, m_nHeight };
	FillRect(m_bufHdc, &rtBkg, (HBRUSH)GetStockObject(WHITE_BRUSH));

	DrawItem();

	return BitBlt(m_hdc, m_rtScope.left, m_rtScope.top, m_nWidth, m_nHeight, m_bufHdc, 0, 0, SRCCOPY);
}

BOOL CPieDiagram::DrawItem()
{
	HBRUSH oldHBrush;
	HBRUSH newHBrush;

	Ellipse(m_bufHdc, m_ptZero.x - m_nRadius, m_ptZero.y - m_nRadius, m_ptZero.x + m_nRadius, m_ptZero.y + m_nRadius);
	map<PIEDIAGRAMITEM, double, 


	return TRUE;
}
