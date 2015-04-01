#include "BarChart.h"


CBarChart::CBarChart()
{
}


CBarChart::~CBarChart()
{
}

BOOL CBarChart::Init(HDC hdc, RECT rtScope, int nMargin, int hWidth, vector<BARCHARTITEM> items, int itemMergin)
{
	//hWidth为以立方体宽为斜边的等边直角三角形的直角边长
	CTDFigure::Init(hdc, rtScope, nMargin);
	m_hWidth = hWidth;
	m_tdfItems = items;
	m_itemMergin = itemMergin;
	m_nItemCount = m_tdfItems.size();
	if (m_nItemCount <= 0)
	{
		return FALSE;
	}
	m_nMaxValue = m_tdfItems[0].item.value;
	m_nMinValue = m_tdfItems[0].item.value;
	for (int i = 1; i < m_nItemCount; i++)
	{
		if (m_nMaxValue < m_tdfItems[i].item.value)
		{
			m_nMaxValue = m_tdfItems[i].item.value;
		}
		if (m_nMinValue > m_tdfItems[i].item.value)
		{
			m_nMinValue = m_tdfItems[i].item.value;
		}
	}
	m_itemWidth = m_nValidXLength / m_nItemCount - m_itemMergin - m_hWidth;
	if (m_itemWidth <= 0)
	{
		return FALSE;
	}

	m_heightMergin = m_nValidYLength / m_nMaxValue;
	if (m_heightMergin <= 0)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CBarChart::DrawDc()
{
	FillRect(m_bufHdc, &m_rtScope, (HBRUSH)GetStockObject(WHITE_BRUSH));
	DrawLine();
	DrawXY();
	DrawXYName(TEXT("X"), TEXT("Y"));
	DrawItem();
	return BitBlt(m_hdc, m_rtScope.left, m_rtScope.top, m_nWidth, m_nHeight, m_bufHdc, 0, 0, SRCCOPY);
}

BOOL CBarChart::DrawCube(POINT ptPos, int length, int hWidth, int height, COLORREF  rgbColor)
{
	//hWidth为以立方体宽为斜边的等边直角三角形的直角边长
	POINT mainPos[6] = { 0 };
	mainPos[0].x = ptPos.x;
	mainPos[0].y = ptPos.y - height;
	mainPos[1].x = ptPos.x + hWidth;
	mainPos[1].y = ptPos.y - height - hWidth;
	mainPos[2].x = ptPos.x + length + hWidth;
	mainPos[2].y = ptPos.y - height - hWidth;
	mainPos[3].x = ptPos.x + length;
	mainPos[3].y = ptPos.y - height;
	mainPos[4].x = ptPos.x + length;
	mainPos[4].y = ptPos.y;
	mainPos[5].x = ptPos.x + length + hWidth;
	mainPos[5].y = ptPos.y - hWidth;

	TRIVERTEX trives[2];
	trives[0].x = ptPos.x;
	trives[0].y = ptPos.y;
	trives[0].Alpha = 0xff00;
	trives[0].Red = MAKEWORD(0x00, GetRValue(rgbColor));
	trives[0].Green = MAKEWORD(0x00, GetGValue(rgbColor));
	trives[0].Blue = MAKEWORD(0x00, GetBValue(rgbColor));
	trives[1].x = mainPos[3].x;
	trives[1].y = mainPos[3].y;
	trives[1].Alpha = 0xff00;
	trives[1].Red = 0xff00;
	trives[1].Green = 0xff00;
	trives[1].Blue = 0xff00;
	// 定义渐变区为矩形，并确定其点
	GRADIENT_RECT rects[1];
	rects[0].UpperLeft = 0;
	rects[0].LowerRight = 1;
	GradientFill(m_bufHdc, trives, 2, rects, 1, GRADIENT_FILL_RECT_V);

	MoveToEx(m_bufHdc, ptPos.x, ptPos.y, NULL);
	LineTo(m_bufHdc, mainPos[0].x, mainPos[0].y);
	MoveToEx(m_bufHdc, ptPos.x, ptPos.y, NULL);
	LineTo(m_bufHdc, mainPos[4].x, mainPos[4].y);

	HBRUSH hBrush;
	HBRUSH oldHBrush;
	hBrush = CreateSolidBrush(rgbColor);
	oldHBrush = (HBRUSH)SelectObject(m_bufHdc, hBrush);

	POINT topRhombus[4] = { mainPos[0], mainPos[1], mainPos[2], mainPos[3] };
	Polygon(m_bufHdc, topRhombus, 4);


	SelectObject(m_bufHdc, oldHBrush);
	DeleteObject(hBrush);
	int nGray = 50;
	hBrush = CreateSolidBrush(RGB(
		GetRValue(rgbColor) - nGray < 0 ? 0 : GetRValue(rgbColor) - nGray,
		GetGValue(rgbColor) - nGray < 0 ? 0 : GetGValue(rgbColor) - nGray,
		GetBValue(rgbColor) - nGray < 0 ? 0 : GetBValue(rgbColor) - nGray
		));
	oldHBrush = (HBRUSH)SelectObject(m_bufHdc, hBrush);

	POINT rightRhombus[4] = { mainPos[2], mainPos[3], mainPos[4], mainPos[5] };
	Polygon(m_bufHdc, rightRhombus, 4);

	SelectObject(m_bufHdc, oldHBrush);
	DeleteObject(hBrush);
	return TRUE;
}

BOOL CBarChart::DrawLine()
{
	HBRUSH hBrush;
	HBRUSH oldHBrush;
	hBrush = CreateSolidBrush(RGB(255,255,150));
	oldHBrush = (HBRUSH)SelectObject(m_bufHdc, hBrush);

	POINT mainPos[4] = { 0 };
	mainPos[0].x = m_ptZero.x;
	mainPos[0].y = m_ptZero.y - m_nValidYLength;
	mainPos[1].x = m_ptZero.x + m_hWidth;
	mainPos[1].y = m_ptZero.y - m_nValidYLength - m_hWidth;
	mainPos[2].x = m_ptZero.x + m_hWidth;
	mainPos[2].y = m_ptZero.y - m_hWidth;
	mainPos[3].x = m_ptZero.x;
	mainPos[3].y = m_ptZero.y;

	POINT leftRhombus[4] = { mainPos[0], mainPos[1], mainPos[2], mainPos[3] };
	Polygon(m_bufHdc, leftRhombus, 4);

	SelectObject(m_bufHdc, oldHBrush);
	DeleteObject(hBrush);

	MoveToEx(m_bufHdc, mainPos[2].x, mainPos[2].y, NULL);
	LineTo(m_bufHdc, mainPos[2].x + m_nValidXLength, mainPos[2].y);
	LineTo(m_bufHdc, m_ptZero.x + m_nValidXLength, m_ptZero.y);

	MoveToEx(m_bufHdc, mainPos[1].x, mainPos[1].y, NULL);
	LineTo(m_bufHdc, mainPos[1].x + m_nValidXLength, mainPos[1].y);
	LineTo(m_bufHdc, mainPos[1].x + m_nValidXLength, mainPos[1].y + m_nValidYLength);

	HPEN hPen;
	HPEN OldHPen;
	hPen = CreatePen(PS_DOT, 1, RGB(221, 221, 221));
	OldHPen = (HPEN)SelectObject(m_bufHdc, hPen);

	for (int i = 1, j = 1; i * m_heightMergin < m_nValidYLength; i++)
	{
		if (i  == m_nMaxValue / 10 * j)
		{
			MoveToEx(m_bufHdc, m_ptZero.x, m_ptZero.y - i * m_heightMergin, NULL);
			LineTo(m_bufHdc, m_ptZero.x + m_hWidth, m_ptZero.y - m_hWidth - i * m_heightMergin);
			LineTo(m_bufHdc, m_ptZero.x + m_hWidth + m_nValidXLength, m_ptZero.y - m_hWidth - i * m_heightMergin);
			j++;
		}
	}

	SelectObject(m_bufHdc, OldHPen);
	DeleteObject(hPen);
	return TRUE;
}

BOOL CBarChart::DrawItem()
{
	vector<BARCHARTITEM>::iterator tbegin = m_tdfItems.begin();
	vector<BARCHARTITEM>::iterator tend = m_tdfItems.end();
	POINT ptPos = m_ptZero;
	while (tbegin != tend)
	{
		ptPos.x += m_itemMergin;

		DrawCube(ptPos, m_itemWidth, m_hWidth, (*tbegin).item.value * (int)m_heightMergin, (*tbegin).rgbColor);

		ptPos.x += m_itemWidth;
		ptPos.x += m_hWidth;
		tbegin++;
	}

	return TRUE;
}
