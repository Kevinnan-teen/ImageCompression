/**
* @version:1.0
* @author kevinnan.org.cn  2018212114@mail.hfut.edu.cn
* @date 2020/7/2
* @Content:定义bmp位图文件的图像信息
*/

#include"Image.h"
#include<iostream>

Image::Image(uchar * imageInfo, uchar * data, uint width, uint height, uint dataSize){
	this->imageInfo = imageInfo;
	this->data = data;
	this->width = width;
	this->height = height;
	this->dataSize = dataSize;
}


Image::~Image(){
	delete []imageInfo;
	delete []data;
}


uchar * Image::getImageInfo(){
	return this->imageInfo;
}


uchar * Image::getData(){
	return this->data;
}


uint Image::getWidth(){
	return this->width;
}


uint Image::getHeight(){
	return this->height;
}


uint Image::getDataSize(){
	return this->dataSize;
}


