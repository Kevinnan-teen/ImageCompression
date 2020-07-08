#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <bitset>
#include "Image.h"
#include "ImageIO.h"
#include "tools.h"
#include "LZWcompress.h"
#include "HuffmanCompression.h"

using namespace std;
typedef unsigned int uint;

void encodeImage(unordered_map<uint, uint>& weightMap){
	string fileName = "../images/bmp_test.bmp";
	string huffmanFileName = "../images/huffman_result.ylh";
	Image * image = ImageIO::readImage(fileName);

	uchar * YUVdata = new uchar[image->getDataSize() / 2];
	YUVdata = Tools::RGB2YUV(image->getData(), image->getWidth(), image->getHeight(), image->getDataSize());
	//cout<<"YUV+end!"<<endl;
	LZWcompress * lzw = new LZWcompress(YUVdata, image->getDataSize() / 2);
	lzw->LZW_encode();
	HuffmanCompression * huffman2 = new HuffmanCompression(lzw->get_lzw_encode());
	//huffman2->Huffman_encode();

	Image * huffman_encode_image = new Image(image->getImageInfo(), huffman2->Huffman_encode(),
											 0,0,huffman2->get_encode_sum_size());

	weightMap =  huffman2->getWeightMap();

	ImageIO::writeImage(huffmanFileName, huffman_encode_image);
}

void decodeImage(unordered_map<uint, uint>& decodeWeightMap){
	string fileName = "../images/huffman_result.ylh";
	uint weightMapKeySize=0, weightMapValueSize=0, dataSize=0, sum_size=0, bit_sum_length;
	std::vector<uchar> data;
	ImageIO::read_Huffman_encode_file(fileName, data, weightMapKeySize, 
									  weightMapValueSize, 
									  dataSize,
									  sum_size,
									  bit_sum_length);
	cout<<"read_Huffman_encode_file_size:";
	cout<<dataSize<<endl;
	cout<<"read_bit_sum_length:"<<bit_sum_length<<endl;
	HuffmanCompression * huffman = new HuffmanCompression();
	std::vector<uint> recoverData;
	recoverData = huffman->huffman_decode_init(data, weightMapKeySize, weightMapValueSize, dataSize, bit_sum_length);

	cout<<"recoverData_size:"<<recoverData.size()<<endl;

	decodeWeightMap = huffman->getDecodeWeightMap();


	
}

int main(){
	unordered_map<uint, uint> weightMap, decodeWeightMap;
	encodeImage(weightMap);
	decodeImage(decodeWeightMap);
	cout<<weightMap.size()<<"\t"<<decodeWeightMap.size()<<endl;
	if(weightMap == decodeWeightMap){
		cout<<"right"<<endl;
	}else{
		cout<<"error"<<endl;
	}
	// string fileName = "../images/bmp_test.bmp";
	// string yuvFileName = "../images/yuv_test.yuv";
	// string recoverFileName = "../images/recoverImage.bmp";

	// Image * image = ImageIO::readImage(fileName);

	// // for(int i = image->getDataSize() - 100; i < image->getDataSize(); i++){
	// // 	cout<<int(*(image->getData()+i))<<"\t";
	// // }
	// // cout<<endl;

	// //写入压缩后文件
	// uchar * YUVdata = new uchar[image->getDataSize() / 2];
	// YUVdata = Tools::RGB2YUV(image->getData(), image->getWidth(), image->getHeight(), image->getDataSize());

	// LZWcompress * lzw = new LZWcompress(YUVdata, image->getDataSize() / 2);
	// lzw->LZW_encode();
	// lzw->LZW_decode();

	// HuffmanCompression * huffman2 = new HuffmanCompression(lzw->get_lzw_encode());


	// //HuffmanCompression * huffman2 = new HuffmanCompression();
	// // huffman2->build_Huffman_Tree();

	// // std::map<uint, string> huffman_code;
	// // huffman2->traverse_tree(huffman2->getRoot(), "", huffman_code);
	// // cout<<"code_result:"<<endl;
	// // // for(auto iter = huffman_code.begin(); iter != huffman_code.end(); iter++)
	// // // 	cout<<iter->first<<"\t"<<iter->second<<endl;
	// // cout<<"huffman_code_size:"<<huffman_code.size()<<endl;

	// huffman2->Huffman_encode();

	// cout<<"*******"<<endl;



	// LZWcompress * lzw2 = new LZWcompress();
	// lzw2->LZW_encode();
	// lzw2->LZW_decode();	

	// char b = '0';
	// cout<<int(b)<<endl;
	// Tools::bitset2char(bset);
	// //cout<<Tools::bitset2char(bset)<<endl;
	// // for(auto iter = Tools::bitset2char(bset).begin(); iter != Tools::bitset2char(bset).end(); iter++){
	// // 	cout<<int(*iter)<<"\t";
	// // }
	// cout<<endl<<Tools::char2bitset(Tools::bitset2char(bset))<<endl;





	#ifdef linux		
	cout<<"This is linux"<<endl;
	#endif

	#ifdef __WINDOWS_
	cout<<"This is Windows"<<endl;
	system("pause");
	#endif



	return 0;
}
