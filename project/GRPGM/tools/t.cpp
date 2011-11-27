
#include <windows.h>
#include <iostream>
#include <fstream>
#include <cstdio>
using namespace std;


int main(int argc, char * argv[])
{
	if(argc != 2)
	{
		cout << argv[0] << "  bmpfile" << endl;
		return 0;
	}
	
	ifstream	file(argv[1], ios::in | ios::binary);
	
	if(file.fail())
	{
		cout << "no such file" << endl;
		return 0;
	}
	
	cout << " read " << argv[1] << endl;
	
	BITMAPFILEHEADER	bmpHeader;
	BITMAPINFOHEADER	biHeader;
	
	file.read((char *)&bmpHeader, sizeof(BITMAPFILEHEADER));
	file.read((char *)&biHeader, sizeof(BITMAPINFOHEADER));
	file.close();
	
	printf("[FILE] bfType    = %c%c\n", bmpHeader.bfType & 0xff, bmpHeader.bfType >> 8 & 0xff);
	printf("[FILE] bfSize    = %d\n", bmpHeader.bfSize);
	printf("[FILE] bfOffBits = %d\n", bmpHeader.bfOffBits);
	printf("[INFO] biSize    = %d\n", biHeader.biSize);
	printf("[INFO] biWidth   = %d\n", biHeader.biWidth);
	printf("[INFO] biHeight  = %d\n", biHeader.biHeight);
	printf("[INFO] biPlanes  = %d\n", biHeader.biPlanes);
	printf("[INFO] biBitCount= %d\n", biHeader.biBitCount);
	printf("[INFO] biCompress= %d\n", biHeader.biCompression);
	printf("[INFO] biSizeImg = %d\n", biHeader.biSizeImage);
	
	return 0;
}

	