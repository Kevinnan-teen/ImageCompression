#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <bitset>
#include "Image.h"
#include "ImageIO.h"
#include "tools.h"
#include "LZWcompress.h"

using namespace std;
typedef unsigned int uint;


void encode(){
	//BGR转YUV　+ LZW压缩算法
	string fileName = "../images/bmp_test.bmp";		//可改为你自己的图片路径
	string saveFileName = "../images/lzw.ylf";		

	Image * image = ImageIO::readImage(fileName);

	uchar * YUVdata = new uchar[image->getDataSize() / 2];
	YUVdata = Tools::RGB2YUV(image->getData(), image->getWidth(), image->getHeight(), image->getDataSize());

	LZWcompress * lzw = new LZWcompress(YUVdata, image->getDataSize() / 2);
	lzw->LZW_encode();
	//lzw->LZW_decode();

	lzw->get_lzw_encode();	//得到输出编码的char型数据

	Image * lzwFile = new Image(image->getImageInfo(), lzw->get_lzw_encode(), 
								0, 0, lzw->get_encode_size()+4);

	ImageIO::writeImage(saveFileName, lzwFile);
}

void decode(){
	//BGR转YUV　+ LZW压缩算法
	string fileName = "../images/lzw.ylf";
	string recoverFileName = "../images/recoverImage.bmp";

	Image* lzwFile = ImageIO::readlzwFile(fileName);

	cout<<lzwFile->getDataSize()<<endl;

	LZWcompress * lzw = new LZWcompress();
	lzw->set_encode_data(lzwFile->getData(), lzwFile->getDataSize());
	lzw->LZW_decode();

	//lzw解码得到YUV数据
	uchar* YUVdata = new uchar[lzw->get_decode_size()];
	YUVdata = lzw->get_lzw_decode();


	uchar * BGRData = new uchar[lzw->get_decode_size()*2];

	BGRData = Tools::YUV2RGB(YUVdata, lzw->get_decode_size());


	Image * recoverFile = new Image(lzwFile->getImageInfo(), BGRData, 0,
		0, lzw->get_decode_size()*2);

	ImageIO::writeImage(recoverFileName, recoverFile);

}

void encode_2(){
	//只包括LZW压缩算法
	string fileName = "../images/bmp_test.bmp";
	string saveFileName = "../images/lzw_test.lzw";

	Image * image = ImageIO::readImage(fileName);


	LZWcompress * lzw = new LZWcompress(image->getData(), image->getDataSize());
	lzw->LZW_encode();
	//lzw->LZW_decode();

	lzw->get_lzw_encode();	//得到输出编码的char型数据

	Image * lzwFile = new Image(image->getImageInfo(), lzw->get_lzw_encode(), 
								0, 0, lzw->get_encode_size()+4);

	ImageIO::writeImage(saveFileName, lzwFile);
}

void decode_2(){
	//只包括LZW压缩算法
	string fileName = "../images/lzw_test.lzw";
	string recoverFileName = "../images/recoverImage_2.bmp";

	Image* lzwFile = ImageIO::readlzwFile(fileName);

	cout<<lzwFile->getDataSize()<<endl;

	LZWcompress * lzw = new LZWcompress();
	lzw->set_encode_data(lzwFile->getData(), lzwFile->getDataSize());
	lzw->LZW_decode();


	Image * recoverFile = new Image(lzwFile->getImageInfo(), lzw->get_lzw_decode(), 0,
		0, lzw->get_decode_size());

	ImageIO::writeImage(recoverFileName, recoverFile);
}

int main(){
	// encode()和decode()函数为BGR转YUV　+ LZW算法对图像进行压缩，为有损压缩
	// encode_2()和decode_2()函数为 LZW算法对图像进行压缩，为无损压缩
	encode();
	decode();
	encode_2();
	decode_2();



	#ifdef linux		
	cout<<"This is linux"<<endl;
	#endif

	#ifdef __WINDOWS_
	cout<<"This is Windows"<<endl;
	system("pause");
	#endif



	return 0;
}
