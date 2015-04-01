#include "TDFigure.h"

CTDFigure::CTDFigure()
{
	m_hdc = NULL;
	m_bufHdc = NULL;
	m_bufBitmap = NULL;
	m_penAxis = NULL;


	m_rtScope = { 0, 0, 0, 0 };
	m_nMargin = 0;
	m_strXName = TEXT("");
	m_strYName = TEXT("");

	m_nArrowSize = 10;
	m_nFontZone = 20;

	m_nWidth = 0;
	m_nHeight = 0;
	m_ptZero = { 0, 0 };
	m_nXLength = 0;
	m_nYLength = 0;

	m_oldPenAxis = NULL;
	m_OldBufBitmap = NULL;
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

	ChangeSize(rtScope, nMargin);

	m_bufBitmap = CreateCompatibleBitmap(m_hdc, m_nWidth, m_nHeight);
	m_penAxis = CreatePen(PS_SOLID, 2, RGB(0,0,0));

	return TRUE;
}

BOOL CTDFigure::ChangeSize(RECT rtScope, int nMargin)
{
	m_rtScope = rtScope;
	if (-1 != nMargin)
	{
		m_nMargin = nMargin;
	}

	m_nWidth = m_rtScope.right - m_rtScope.left;
	m_nHeight = m_rtScope.bottom - m_rtScope.top;

	if (m_bufBitmap != NULL)
	{
		SelectObject(m_bufHdc, m_OldBufBitmap);
		DeleteObject(m_bufBitmap);
		m_bufBitmap = NULL;
	}

	m_ptZero.x = m_nMargin;
	m_ptZero.y = m_nHeight - m_nMargin;
	m_nXLength = m_nWidth - 2 * m_nMargin;
	m_nYLength = m_nHeight - 2 * m_nMargin;
	m_nValidXLength = (int)(m_nXLength * 0.9f);
	m_nValidYLength = (int)(m_nYLength * 0.9f);

	m_bufBitmap = CreateCompatibleBitmap(m_hdc, m_nWidth, m_nHeight);
	m_OldBufBitmap = (HBITMAP)SelectObject(m_bufHdc, m_bufBitmap);

	return TRUE;
}

BOOL CTDFigure::DrawDc()
{
	FillRect(m_bufHdc, &m_rtScope, (HBRUSH)GetStockObject(WHITE_BRUSH));
	DrawXY();
	DrawXYName(TEXT("X"), TEXT("Y"));
	return BitBlt(m_hdc, m_rtScope.left, m_rtScope.top, m_nWidth, m_nHeight, m_bufHdc, 0, 0, SRCCOPY);
	//BLENDFUNCTION bldt;
	//bldt.BlendOp = AC_SRC_OVER;
	//bldt.BlendFlags = 0;
	//bldt.AlphaFormat = AC_SRC_ALPHA;
	//bldt.SourceConstantAlpha = 255;
	//return AlphaBlend(m_hdc, m_rtScope.left, m_rtScope.top, m_nWidth, m_nHeight,
	//	m_bufHdc, 0, 0, m_nWidth, m_nHeight, bldt);
}

BOOL CTDFigure::DrawXY(RECT rtScope, int nMargin)
{
	if (rtScope.bottom != -1)
	{
		m_rtScope = rtScope;
	}
	if (nMargin != -1)
	{
		m_nMargin = nMargin;
	}

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
	return TRUE;
}

BOOL CTDFigure::DrawXYName(TSTRING strXName, TSTRING strYName)
{
	m_strXName = strXName;
	m_strYName = strYName;

	//LOGFONT logft;
	//ZeroMemory(&logft, sizeof(logft));
	////CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT))->GetLogFont(&logft);//获取系统字体
	//logft.lfWidth = 12;							//字体宽度
	//logft.lfHeight = 0;							//字体高度
	//logft.lfWeight = 5;						//字体粗细
	//logft.lfItalic = FALSE;						//是否斜体
	//logft.lfUnderline = FALSE;						//是否有下划线
	//logft.lfStrikeOut = FALSE;						//是否有删除线
	//logft.lfCharSet = CHINESEBIG5_CHARSET;					//字符集
	//logft.lfOutPrecision = OUT_DEFAULT_PRECIS;			//输出精度
	//logft.lfClipPrecision = CLIP_DEFAULT_PRECIS;			//剪切精度
	//logft.lfQuality = DEFAULT_QUALITY;				//输出质量
	//logft.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;	//字体的间距和家族
	//logft.lfOrientation = 0;
	//logft.lfEscapement = 0;
	//_tcscpy_s(logft.lfFaceName, 32, TEXT("system"));					//字体类型
	//HFONT font;
	//HFONT oldFont;
	//font = CreateFontIndirect(&logft);
	//oldFont = (HFONT)SelectObject(m_bufHdc, font);

	TextOut(m_bufHdc, 
		m_nWidth - m_nMargin, 
		m_nHeight - m_nMargin + m_nArrowSize, 
		m_strXName.c_str(), 
		m_strXName.length());

	SIZE size;
	::GetTextExtentPoint(m_bufHdc, m_strYName.c_str(), m_strYName.length(), &size);

	TextOut(m_bufHdc, 
		m_nMargin - size.cx - m_nArrowSize < 0 ? 0 : m_nMargin - size.cx - m_nArrowSize,
		m_nMargin - m_nArrowSize, 
		m_strYName.c_str(), 
		m_strYName.length());

	//SelectObject(m_bufHdc, oldFont);
	//DeleteObject(font);
	return TRUE;
}
