#include "stdafx.h"
#include "Tools.h"

template<typename T>
inline T limit(const T& value, const T& lower, const T&upper) {
	return ((value > upper) ? upper : ((value < lower) ? lower : value));
}

CTools::CTools()
{
}


CTools::~CTools()
{
}


void CTools::makeCrossPoint(LPVIDEOHDR lpVHdr, int xCenter, int yCenter, BITMAPINFO BmInfo)
{
	int i, j, index;
	for (i = xCenter - 9; i <= xCenter + 9; i++)
	{
		if (i<0 || i >= BmInfo.bmiHeader.biHeight) continue;
		index = i*BmInfo.bmiHeader.biWidth;
		*(lpVHdr->lpData + 3 * (index + yCenter)) = 0;
		*(lpVHdr->lpData + 3 * (index + yCenter) + 1) = 0;
		*(lpVHdr->lpData + 3 * (index + yCenter) + 2) = 0;
	}

	index = xCenter*BmInfo.bmiHeader.biWidth;
	for (j = yCenter - 9; j <= yCenter + 9; j++)
	{
		if (j<0 || j >= BmInfo.bmiHeader.biWidth) continue;
		*(lpVHdr->lpData + 3 * (index + j)) = 0;
		*(lpVHdr->lpData + 3 * (index + j) + 1) = 0;
		*(lpVHdr->lpData + 3 * (index + j) + 2) = 0;
	}


}




void CTools::rgb2hsv(LPVIDEOHDR lpVHdr, int index, double& fH, double& fS, double& fV)
{
	double fB, fG, fR;

	fB = *(lpVHdr->lpData + (index * 3)) / 255.0;
	fG = *(lpVHdr->lpData + (index * 3) + 1) / 255.0;
	fR = *(lpVHdr->lpData + (index * 3) + 2) / 255.0;

	double fCMax = MAX(MAX(fR, fG), fB);
	double fCMin = MIN(MIN(fR, fG), fB);
	double fDelta = fCMax - fCMin;

	if (fDelta > 0) {
		if (fCMax == fR) {
			fH = 60 * (fmod(((fG - fB) / fDelta), 6));
		}
		else if (fCMax == fG) {
			fH = 60 * (((fB - fR) / fDelta) + 2);
		}
		else if (fCMax == fB) {
			fH = 60 * (((fR - fG) / fDelta) + 4);
		}

		if (fCMax > 0) {
			fS = fDelta / fCMax;
		}
		else {
			fS = 0;
		}

		fV = fCMax;
	}
	else {
		fH = 0;
		fS = 0;
		fV = fCMax;
	}

	if (fH < 0) {
		fH = 360 + fH;
	}
}


BYTE CTools::getImage(LPVIDEOHDR lpVHdr, int index)
{
	return *(lpVHdr->lpData + index);
}
BYTE CTools::getImage(LPVIDEOHDR lpVHdr, BITMAPINFO BmInfo, int indexX, int indexY)
{
	if ( indexX >= BmInfo.bmiHeader.biWidth){
		cout << "overFlow:W" << endl;
		indexX = BmInfo.bmiHeader.biWidth - 1;
	}
	if (indexY >= BmInfo.bmiHeader.biHeight) {
		cout << "overFlow:H" << endl;
		indexX = BmInfo.bmiHeader.biHeight - 1;
	}
	return *(lpVHdr->lpData +indexY * BmInfo.bmiHeader.biWidth + indexX);
}


void CTools::setImage(LPVIDEOHDR lpVHdr, int index, BYTE input)
{

	*(lpVHdr->lpData + index) = input;
}

void CTools::setImage(LPVIDEOHDR lpVHdr, BITMAPINFO BmInfo, int indexX, int indexY, BYTE input) {
	if (indexX >= BmInfo.bmiHeader.biWidth) {
		cout << "overFlow:W" << endl;
		indexX = BmInfo.bmiHeader.biWidth - 1;
	}
	if (indexY >= BmInfo.bmiHeader.biHeight) {
		cout << "overFlow:H" << endl;
		indexX = BmInfo.bmiHeader.biHeight - 1;
	}
	*(lpVHdr->lpData + indexY * BmInfo.bmiHeader.biWidth + indexX) = input;

}



bool CTools::isBlue(double fH, double fS, double fV)
{
	return ((fH >= 210 && fH <= 270) && (fS >= 0.4 && fS <= 1) && (fV >= 0.2 && fV <= 1));
}


bool CTools::isGreen(double fH, double fS, double fV)
{
	return (fH >= 95 && fH <= 145) && (fS >= 0.3 && fS <= 1) && (fV >= 0.1 && fV <= 1);
}

bool CTools::isRed(double fH, double fS, double fV)
{
	return (fH >= 340 || fH <= 20) && (fS >= 0.6 && fS <= 1) && (fV >= 0.4 && fV <= 1);
}

bool CTools::isYellow(double fH, double fS, double fV)
{
	return (fH >= 35 && fH <= 85) && (fS >= 0.6 && fS <= 1) && (fV >= 0.4 && fV <= 1);
}


void CTools::makeLine(LPVIDEOHDR lpVHdr, BITMAPINFO BmInfo, int xStart, int xEnd, int yStart, int yEnd)
{
	int xS, yS, xE, yE;
	int nWidth = BmInfo.bmiHeader.biWidth;
	if (xStart == xEnd) // vertical
	{
		if (yStart < yEnd) { yS = yStart; yE = yEnd; }
		else { yS = yEnd; yE = yStart; }
		for (int r = yS; r <= yE; r++)
		{
			*(lpVHdr->lpData + 3 * (nWidth*xStart + r)) = 0;
			*(lpVHdr->lpData + 3 * (nWidth*xStart + r) + 1) = 0;
			*(lpVHdr->lpData + 3 * (nWidth*xStart + r) + 2) = 0;
		}
		return;
	}

	double a = (double)(yEnd - yStart) / (xEnd - xStart); // ±â¿ï±â
	int nHeight = BmInfo.bmiHeader.biHeight;

	if ((a>-1) && (a<1)) // almost vertical
	{
		if (xStart<xEnd) {
			xS = xStart;
			xE = xEnd;
			yS = yStart;
			yE = yEnd;
		}
		else
		{
			xS = xEnd;
			xE = xStart;
			yS = yEnd;
			yE = yStart;
		}
		for (int c = xS; c <= xE; c++)
		{
			int r = (int)(a*(c - xS) + yS + 0.5);
			if (r<0 || r >= nHeight)
				continue;
			*(lpVHdr->lpData + 3 * (nWidth*c + r)) = 0;
			*(lpVHdr->lpData + 3 * (nWidth*c + r) + 1) = 0;
			*(lpVHdr->lpData + 3 * (nWidth*c + r) + 2) = 0;
		}
	}
	else //almost horizontal
	{
		double invA = 1.0 / a;
		if (yStart < yEnd) {
			yS = yStart;
			yE = yEnd;
			xS = xStart;
			xE = xEnd;
		}
		else {
			yS = yEnd;
			yE = yStart;
			xS = xEnd;
			xE = xStart;
		}

		for (int r = yS; r <= yE; r++)
		{
			int c = (int)(invA * (r - yS) + xS + 0.5);
			if (r<0 || r >= nHeight)
				continue;
			*(lpVHdr->lpData + 3 * (nWidth*c + r)) = 0;
			*(lpVHdr->lpData + 3 * (nWidth*c + r) + 1) = 0;
			*(lpVHdr->lpData + 3 * (nWidth*c + r) + 2) = 0;
		}
	}
}
