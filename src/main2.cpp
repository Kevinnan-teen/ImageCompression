#include<iostream>
#include<string>
#include<vector>
#include"Image.h"
#include"ImageIO.h"
#include"tools.h"
#include "LZWcompress.h"

using namespace std;

int main(){
	string fileName = "../images/bmp_test.bmp";
	string yuvFileName = "../images/yuv_test.yuv";
	string recoverFileName = "../images/recoverImage.bmp";

	Image * image = ImageIO::readImage(fileName);

	// for(int i = image->getDataSize() - 100; i < image->getDataSize(); i++){
	// 	cout<<int(*(image->getData()+i))<<"\t";
	// }
	// cout<<endl;

	//写入压缩后文件
	uchar * YUVdata = new uchar[image->getDataSize() / 2];
	YUVdata = Tools::RGB2YUV(image->getData(), image->getWidth(), image->getHeight(), image->getDataSize());

	LZWcompress * lzw = new LZWcompress(YUVdata, image->getDataSize() / 2);
	lzw->LZW_encode();
	lzw->LZW_decode();


	// LZWcompress * lzw2 = new LZWcompress();
	// lzw2->LZW_encode();
	// lzw2->LZW_decode();



	#ifdef linux		
	cout<<"This is linux"<<endl;
	#endif

	#ifdef __WINDOWS_
	cout<<"This is Windows"<<endl;
	system("pause");
	#endif



	return 0;
}
