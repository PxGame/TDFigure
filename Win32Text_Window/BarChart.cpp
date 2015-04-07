#include "BarChart.h"


CBarChart::CBarChart()
{
	m_tdfItems.clear();
	m_hWidth = 0;
	m_itemWidth = 0;
	m_itemMergin = 0;
	m_heightMergin = 0;
	m_nItemCount = 0;
	m_nMaxValue = 0;
	m_nMinValue = 0;	
	m_nFontMaxSize = 20;
	m_nFontMinSize = 12;
	m_nYValueMultiple = 0;
	m_bShowBrokenLine =  FALSE;
}


CBarChart::~CBarChart()
{
}

BOOL CBarChart::Init(HDC hdc, RECT rtScope, int nMargin, vector<BARCHARTITEM> items, int itemMergin, int hWidth)
{
	//hWidth为以立方体宽为斜边的等边直角三角形的直角边长
	BOOL bRet = FALSE;
	bRet = CTDFigure::Init(hdc, rtScope, nMargin);
	if (bRet == FALSE)
	{
		return FALSE;
	}

	m_tdfItems = items;

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

	if (itemMergin != -1)
	{
		m_itemMergin = itemMergin;	
		m_itemWidth = m_nValidXLength / m_nItemCount - m_itemMergin;

		if (m_itemWidth <= 0)
		{
			return FALSE;
		}
	}
	else
	{
		m_itemMergin = m_nValidXLength / m_nItemCount / 3;
		m_itemWidth = 2 * m_itemMergin;
	}

	if (hWidth != -1)
	{
		m_hWidth = hWidth;
	}
	else
	{
		m_hWidth = m_itemMergin / 2;
	}

	if (m_nMaxValue != 0)
	{
		int maxValue = m_nMaxValue;
		if (m_nMaxValue <= 10)
		{
			maxValue = 10;
			m_nYValueMultiple = 1;
		}
		else
		{
			m_nYValueMultiple = 1;
			while (maxValue / 10 != 0)
			{
				m_nYValueMultiple *= 10;
				maxValue /= 10;
			}

			if (m_nMaxValue >= 100)
			{
				m_nYValueMultiple /= 10;
			}

			maxValue = m_nMaxValue;
			int n = maxValue % m_nYValueMultiple;
			if (n != 0)
			{
				maxValue += (m_nYValueMultiple - n);
			}
		}
		
		m_nMaxValue = maxValue;
		m_heightMergin = (double)m_nValidYLength / (double)m_nMaxValue;
	}
	else
	{
		m_heightMergin = -1;
	}

	return TRUE;
}

BOOL CBarChart::DrawDc()
{
	RECT rtBkg = { 0, 0, m_nWidth, m_nHeight };
	FillRect(m_bufHdc, &rtBkg, (HBRUSH)GetStockObject(WHITE_BRUSH));
	DrawLine();
	DrawXY();
	DrawItem();
	return BitBlt(m_hdc, m_rtScope.left, m_rtScope.top, m_nWidth, m_nHeight, m_bufHdc, 0, 0, SRCCOPY);
}

BOOL CBarChart::DrawCube(POINT ptPos, int length, int hWidth, int height, COLORREF  rgbColor)
{
	if (length <= 0 || hWidth <= 0 || height <= 0)
	{
		return FALSE;
	}

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
	if (m_heightMergin <= 0)
	{
		return FALSE;
	}

	HBRUSH hBrush;
	HBRUSH oldHBrush;
	hBrush = CreateSolidBrush(RGB(255, 255, 150));
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

	HFONT font = NULL;
	HFONT oldFont = NULL;
	int fontSize = (int)(m_nMaxValue / 10 * m_heightMergin / 2) < m_nFontMinSize ? m_nFontMinSize : (int)(m_nMaxValue / 10 * m_heightMergin / 2) > m_nFontMaxSize ? m_nFontMaxSize : (int)(m_nMaxValue / 10 * m_heightMergin / 2);

	LOGFONT logft;
	ZeroMemory(&logft, sizeof(logft));
	//CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT))->GetLogFont(&logft);//获取系统字体
	logft.lfWidth = 0;							//字体宽度
	logft.lfHeight = fontSize;							//字体高度
	logft.lfWeight = 5;						//字体粗细
	logft.lfItalic = FALSE;						//是否斜体
	logft.lfUnderline = FALSE;						//是否有下划线
	logft.lfStrikeOut = FALSE;						//是否有删除线
	logft.lfCharSet = CHINESEBIG5_CHARSET;					//字符集
	logft.lfOutPrecision = OUT_DEFAULT_PRECIS;			//输出精度
	logft.lfClipPrecision = CLIP_DEFAULT_PRECIS;			//剪切精度
	logft.lfQuality = DEFAULT_QUALITY;				//输出质量
	logft.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;	//字体的间距和家族
	logft.lfOrientation = 0;
	logft.lfEscapement = 0;
	_tcscpy_s(logft.lfFaceName, 32, TEXT("system"));					//字体类型
	font = CreateFontIndirect(&logft);
	oldFont = (HFONT)SelectObject(m_bufHdc, font);

	TCHAR strBuf[10] = { 0 };
	SIZE size = { 0 };

	for (int i = 0, j = 0, lineValue = m_nMaxValue / 10 * j; i <= m_nMaxValue; i++, lineValue = m_nMaxValue / 10 * j)
	{
		if (i == lineValue)
		{
			POINT tBegin = { m_ptZero.x, (LONG)(m_ptZero.y - lineValue * m_heightMergin) };

			_itot_s(i, strBuf, 10, 10);
			::GetTextExtentPoint(m_bufHdc, strBuf, _tcslen(strBuf), &size);
			TextOut(m_bufHdc, tBegin.x - size.cx - 5, tBegin.y - size.cy / 2, strBuf, _tcslen(strBuf));

			MoveToEx(m_bufHdc, tBegin.x, tBegin.y, NULL);
			LineTo(m_bufHdc, tBegin.x + m_hWidth, tBegin.y - m_hWidth);
			LineTo(m_bufHdc, tBegin.x + m_hWidth + m_nValidXLength, tBegin.y - m_hWidth);
			j++;
		}
	}

	SelectObject(m_bufHdc, oldFont);
	DeleteObject(font);

	SelectObject(m_bufHdc, OldHPen);
	DeleteObject(hPen);
	return TRUE;
}

BOOL CBarChart::DrawItem()
{
	if (m_heightMergin <= 0)
	{
		return FALSE;
	}

	HFONT font = NULL;
	HFONT oldFont = NULL;
	int fontSize = (m_itemWidth / 4) < m_nFontMinSize ? m_nFontMinSize : (m_itemWidth / 4) > m_nFontMaxSize ? m_nFontMaxSize : (m_itemWidth / 4);

	LOGFONT logft;
	ZeroMemory(&logft, sizeof(logft));
	//CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT))->GetLogFont(&logft);//获取系统字体
	logft.lfWidth = 0;							//字体宽度
	logft.lfHeight = fontSize;							//字体高度
	logft.lfWeight = 5;						//字体粗细
	logft.lfItalic = FALSE;						//是否斜体
	logft.lfUnderline = FALSE;						//是否有下划线
	logft.lfStrikeOut = FALSE;						//是否有删除线
	logft.lfCharSet = CHINESEBIG5_CHARSET;					//字符集
	logft.lfOutPrecision = OUT_DEFAULT_PRECIS;			//输出精度
	logft.lfClipPrecision = CLIP_DEFAULT_PRECIS;			//剪切精度
	logft.lfQuality = DEFAULT_QUALITY;				//输出质量
	logft.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;	//字体的间距和家族
	logft.lfOrientation = 0;
	logft.lfEscapement = 0;
	_tcscpy_s(logft.lfFaceName, 32, TEXT("system"));					//字体类型
	font = CreateFontIndirect(&logft);
	oldFont = (HFONT)SelectObject(m_bufHdc, font);

	COLORREF oldClr;
	TCHAR strBuf[10] = { 0 };
	SIZE size = { 0 };
	int height = 0;
	BOOL bRet = FALSE;
	vector<BARCHARTITEM>::iterator tbegin = m_tdfItems.begin();
	vector<BARCHARTITEM>::iterator tend = m_tdfItems.end();
	POINT ptPos = m_ptZero;
	POINT linePos = { ptPos.x + m_itemWidth / 2, ptPos.y - height };
	POINT oldLinePos = {m_ptZero.x + m_hWidth / 2, m_ptZero.y - m_hWidth / 2};

	while (tbegin != tend)
	{
		ptPos.x += m_itemMergin;
		height = (int)((*tbegin).item.value * m_heightMergin);

		if (m_bShowBrokenLine == TRUE)
		{
			Ellipse(m_bufHdc, linePos.x - 2, linePos.y - 2, linePos.x + 2, linePos.y + 2);
			linePos.x = ptPos.x + m_itemWidth / 2 + m_hWidth / 2;
			linePos.y = ptPos.y - height - m_hWidth / 2;
			MoveToEx(m_bufHdc, oldLinePos.x, oldLinePos.y, NULL);
			LineTo(m_bufHdc, linePos.x, linePos.y);
			oldLinePos = linePos;
		}

		bRet = DrawCube(ptPos, m_itemWidth, m_hWidth, height, (*tbegin).rgbColor);
		if (bRet == FALSE)
		{
			POINT mainPos[4] = { 
				ptPos, 
				{ ptPos.x + m_hWidth, ptPos.y - m_hWidth}, 
				{ ptPos.x + m_hWidth + m_itemWidth, ptPos.y - m_hWidth },
				{ ptPos.x + m_itemWidth, ptPos.y }
			};

			HBRUSH hBrush;
			HBRUSH oldHBrush;
			hBrush = CreateSolidBrush((*tbegin).rgbColor);
			oldHBrush = (HBRUSH)SelectObject(m_bufHdc, hBrush);

			POINT topRhombus[4] = { mainPos[0], mainPos[1], mainPos[2], mainPos[3] };
			Polygon(m_bufHdc, topRhombus, 4);

			SelectObject(m_bufHdc, oldHBrush);
			DeleteObject(hBrush);
		}

		oldClr = SetTextColor(m_bufHdc, (*tbegin).rgbColor);

		::GetTextExtentPoint(m_bufHdc, (*tbegin).item.name.c_str(), (*tbegin).item.name.length(), &size);

		for (int i = 0, line = 0, length = (*tbegin).item.name.length(); i < length; i += 2, line++)
			TextOut(m_bufHdc, ptPos.x + (m_itemWidth - size.cx / length * 2) / 2, ptPos.y + 5 + line * size.cy, (*tbegin).item.name.c_str() + i, 2);

		_itot_s((*tbegin).item.value, strBuf, 10, 10);
		::GetTextExtentPoint(m_bufHdc, strBuf, _tcslen(strBuf), &size);
		TextOut(m_bufHdc, ptPos.x + (m_itemWidth - size.cx) / 2, ptPos.y - height - m_hWidth - size.cy - 5, strBuf, _tcslen(strBuf));

		SetTextColor(m_bufHdc, oldClr);

		ptPos.x += m_itemWidth;
		tbegin++;
	}

	SelectObject(m_bufHdc, oldFont);
	DeleteObject(font);

	return TRUE;
}

void CBarChart::SetXYName(TSTRING strXName, TSTRING strYName)
{
	m_strXName = strXName;
	m_strYName = strYName;
}

void CBarChart::ShowBrokenLine(BOOL bMode)
{
	m_bShowBrokenLine = bMode;
}
