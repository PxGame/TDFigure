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
	int m_nFontMaxSize;
	int m_nFontMinSize;
	int m_nYValueMultiple;

	BOOL m_bShowBrokenLine;

private:
	BOOL DrawCube(POINT ptPos, int length, int hWidth, int height, COLORREF  rgbColor);
	BOOL DrawLine();
	BOOL DrawItem();

public:
	//hdc-��Ҫ���Ƶ�hdc, rtScope-��Χ��С��nMargin-�߽絽������ľ��룬items-ͳ�����ݼ��ϣ�itemMergin-ͳ��ͼ��ÿһ��֮��ļ��
	BOOL Init(HDC hdc, RECT rtScope, int nMargin, vector<BARCHARTITEM> items, int itemMergin = -1, int hWidth = -1);
	void SetXYName(TSTRING strXName, TSTRING strYName);
	void ShowBrokenLine(BOOL bMode);
	BOOL DrawDc();
};

