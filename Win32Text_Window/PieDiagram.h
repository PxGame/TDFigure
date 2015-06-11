#pragma once

# include <windows.h>
# include <string>
# include <string.h>
# include <tchar.h>
# include <vector>
# include <map>
using namespace std;

# ifdef UNICODE
#define TSTRING wstring
# else
#define TSTRING string
# endif

typedef struct _PieDiagramItem
{
	int value;
	TSTRING name;
	COLORREF  rgbColor;
	bool operator < (const  PIEDIAGRAMITEM &  other)  const
	{
		return value < other.value;
	}
	bool operator == (const  PIEDIAGRAMITEM &  other)  const
	{
		return value == other.value;
	}
	bool operator > (const  PIEDIAGRAMITEM &  other)  const
	{
		return value > other.value;
	}
}PIEDIAGRAMITEM, *PPIEDIAGRAMITEM;

class CPieDiagram
{
public:
	CPieDiagram();
	~CPieDiagram();

protected:	
	RECT m_rtScope;
	int m_nWidth;
	int m_nHeight;
	int m_nMargin;
	int m_nRadius;
	POINT m_ptZero;

	vector<PIEDIAGRAMITEM> m_vtItems;
		
	HDC m_hdc;
	HDC m_bufHdc;
	HBITMAP m_bufBitmap;
	HBITMAP m_oldBufBitmap;
	
protected:
	BOOL DrawItem();

public:
	BOOL Init(HDC hdc, RECT rtScope, int nMargin, vector<PIEDIAGRAMITEM> items);
	BOOL DrawDc();
};

