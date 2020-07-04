/**
* @version:1.0
* @author kevinnan.org.cn  2018212114@mail.hfut.edu.cn
* @date 2020/7/2
* @Content:定义通用的一些函数用于调用
*/
#ifndef IMAGECOMPRESSION_TOOLS_H
#define IMAGECOMPRESSION_TOOLS_H

#include<iostream>
#include<string>
using namespace std;
typedef unsigned char uchar;
typedef unsigned int uint;

class Tools{
public:
	//字符转16进制
	static string Char2Hex(uchar c);

	static uchar * RGB2YUV(uchar * , uint , uint , uint );

	static uchar * YUV2RGB(uchar * , uint );
	
};

#endif
