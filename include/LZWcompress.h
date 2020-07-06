/**
* @version:1.0
* @author kevinnan.org.cn  2018212114@mail.hfut.edu.cn
* @date 2020/7/5
* @Content:将YUV数据通过LZW空间压缩算法进一步压缩
*/

#ifndef IMAGECOMPRESSION_LZWCOMPRESSION_H
#define IMAGECOMPRESSION_LZWCOMPRESSION_H
typedef unsigned char uchar;
typedef unsigned int uint;

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <exception>

#include "tools.h"
using namespace std;

class LZWcompress{
private:
	std::vector<string> YUVdata_n;
	std::map<string, uint> encode_map;
	std::map<uint, string> decode_map;
	std::vector<uint> lzw_encode_output;
	std::vector<string> lzw_decode_output;

public:
	LZWcompress(uchar * YUVdata, uint size);
	LZWcompress();
	void display();
	void encode_init();
	void decode_init();
	void LZW_encode();
	void LZW_decode();
	


};

#endif