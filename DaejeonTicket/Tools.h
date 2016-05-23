#pragma once
#include "vfw.h"
#include <iostream>
using namespace std;
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

class CTools
{
public:
	CTools();
	~CTools();
	void makeCrossPoint(LPVIDEOHDR lpVHdr, int xCenter, int yCenter, BITMAPINFO BmInfo);
	void rgb2hsv(LPVIDEOHDR lpVHdr, int index, double& fH, double& fS, double& fV);
	BYTE getImage(LPVIDEOHDR lpVHdr, int index);
	BYTE getImage(LPVIDEOHDR lpVHdr, BITMAPINFO BmInfo, int indexX, int indexY);

	void setImage(LPVIDEOHDR lpVHdr, int index, BYTE input);
	void setImage(LPVIDEOHDR lpVHdr, BITMAPINFO BmInfo, int indexX, int indexY, BYTE input);
	bool isBlue(double fH, double fS, double fV);
	bool isGreen(double fH, double fS, double fV);
	bool isRed(double fH, double fS, double fV);
	bool isYellow(double fH, double fS, double fV);
};
