/**
* @version:1.0
* @author kevinnan.org.cn  2018212114@mail.hfut.edu.cn
* @date 2020/7/2
* @Content:定义bmp位图文件的图像信息
*/

#ifndef IMAGECOMPRESSION_IMAGE_H
#define IMAGECOMPRESSION_IMAGE_H

typedef unsigned int uint;
typedef unsigned char uchar;

class Image{
private:
	uchar * imageInfo;
	uchar * data;
	uint width;
	uint height;
	uint dataSize;
public:
	Image(uchar *, uchar *, uint, uint, uint);
	~Image();

	uchar * getImageInfo();
	uchar * getData();
	uint getWidth();
	uint getHeight();
	uint getDataSize();


};

#endif
