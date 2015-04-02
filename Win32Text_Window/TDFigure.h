#pragma once
# pragma comment(lib, "msimg32.lib")
# include <windows.h>
# include <string>
# include <string.h>
# include <tchar.h>
# include <wingdi.h>
using namespace std;

# ifdef UNICODE
#define TSTRING wstring
# else
#define TSTRING string
# endif

typedef struct _TDFItem
{
	int value;
	TSTRING name;
}TDFITEM, *PTDFITEM;

class CTDFigure
{
public:
	CTDFigure();
	~CTDFigure();

protected:
	RECT m_rtScope;
	int m_nMargin;
	int m_nXLength;
	int m_nYLength;
	int m_nValidXLength;
	int m_nValidYLength;
	int m_nArrowSize;
	int m_nFontZone;
	int m_nWidth;
	int m_nHeight;
	POINT m_ptZero;
	TSTRING m_strXName;
	TSTRING m_strYName;

	HDC m_hdc;
	HDC m_bufHdc;
	HBITMAP m_bufBitmap;
	HBITMAP m_OldBufBitmap;
	HPEN m_penAxis;
	HPEN m_oldPenAxis;

protected:
	BOOL Init(HDC hdc, RECT rtScope, int nMargin);
	BOOL DrawXY();

public:
	virtual BOOL DrawDc();
};

