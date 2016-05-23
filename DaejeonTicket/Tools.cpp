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
	for (i = xCenter - 15; i <= xCenter + 15; i++)
	{
		if (i<0 || i >= BmInfo.bmiHeader.biHeight) continue;
		index = i*BmInfo.bmiHeader.biWidth;
		*(lpVHdr->lpData + 3 * (index + yCenter)) = 0;
		*(lpVHdr->lpData + 3 * (index + yCenter) + 1) = 0;
		*(lpVHdr->lpData + 3 * (index + yCenter) + 2) = 0;
	}

	index = xCenter*BmInfo.bmiHeader.biWidth;
	for (j = yCenter - 15; j <= yCenter + 15; j++)
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

