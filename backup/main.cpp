#include<iostream>
#include<string>
#include"Image.h"
#include"ImageIO.h"
#include"tools.h"

using namespace std;

int main(){
	string fileName = "../images/her.bmp";
	string yuvFileName = "../images/her_test.yuv";
	string recoverFileName = "../images/recoverHer.bmp";

	Image * image = ImageIO::readImage(fileName);

	for(int i = image->getDataSize() - 100; i < image->getDataSize(); i++){
		cout<<int(*(image->getData()+i))<<"\t";
	}
	cout<<endl;

	//写入压缩后文件
	uchar * YUVData = new uchar[image->getDataSize() / 2];
	YUVData = Tools::RGB2YUV(image->getData(), image->getWidth(), image->getHeight(), image->getDataSize());
	
	Image * yuvImage = new Image(image->getImageInfo(), YUVData, image->getWidth(), image->getHeight(), image->getDataSize()/2);

	ImageIO::writeImage(yuvFileName, yuvImage);

	
	//恢复文件
	Image * YUVImage2 = ImageIO::readImage(yuvFileName);

	uchar * BGRData = new uchar[YUVImage2->getDataSize()*2];


	BGRData = Tools::YUV2RGB(YUVImage2->getData(), YUVImage2->getDataSize());

	for(int i = image->getDataSize() - 100; i < image->getDataSize(); i++){
		cout<<int(BGRData[i])<<"\t";
	}
	cout<<endl;

	Image * recoverFile = new Image(YUVImage2->getImageInfo(), BGRData, YUVImage2->getWidth(),
		YUVImage2->getHeight(), YUVImage2->getDataSize()*2);

	ImageIO::writeImage(recoverFileName, recoverFile);



	#ifdef linux		
	cout<<"This is linux"<<endl;
	#endif

	#ifdef __WINDOWS_
	cout<<"This is Windows"<<endl;
	system("pause");
	#endif



	return 0;
}
