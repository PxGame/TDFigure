#pragma once
# include "TDFigure.h"
# include <vector>
using namespace std;

typedef struct _BarChartItem
{
	TDFITEM item;
	COLORREF  rgbColor;
}BARCHARTITEM, *PBARCHARTITEM;

class CBarChart : protected CTDFigure
{
public:
	CBarChart();
	~CBarChart();

private:
	vector<BARCHARTITEM> m_tdfItems;
	int m_hWidth;
	int m_itemWidth;
	int m_itemMergin;
	double m_heightMergin;
	int m_nItemCount;
	int m_nMaxValue;
	int m_nMinValue;

private:
	BOOL DrawCube(POINT ptPos, int length, int hWidth, int height, COLORREF  rgbColor);
	BOOL DrawLine();
	BOOL DrawItem();

public:
	BOOL Init(HDC hdc, RECT rtScope, int nMargin, int hWidth, vector<BARCHARTITEM> items, int itemMergin);
	void SetXYName(TSTRING strXName, TSTRING strYName);
	BOOL DrawDc();
};

