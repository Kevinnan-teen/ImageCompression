/**
* @version:1.0
* @author kevinnan.org.cn  2018212114@mail.hfut.edu.cn
* @date 2020/7/2
* @Content:读取bmp位图文件(目前只能读取24位位图)
*/

#ifndef IMAGECOMPRESSION_IMAGEIO_H
#define IMAGECOMPRESSION_IMAGEIO_H

#include"Image.h"
#include"tools.h"
#include<iostream>
using namespace std;

class ImageIO{
public:
	//图像大小所在字节位置
	static const int imageSizePosition;

	//信息头大小所在字节位置
	static const int infoSizePosition;	

	//图像宽度大小所在字节位置
	static const int imageWidthPosition;

	//图像高度大小所在字节位置
	static const int imageHeightPosition;

	//bmp文件头大小为54字节
	static const int imageInfoHeadSize;

	//读bmp图像,返回图像信息
	static Image * readImage(string &);

	//将文件头和数据写入文件,也即恢复原图像
	static void writeImage(string & , Image *);

};

#endif
