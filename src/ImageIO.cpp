/**
* @version:1.0
* @author kevinnan.org.cn  2018212114@mail.hfut.edu.cn
* @date 2020/7/2
* @Content:定义通用的一些函数用于调用
*/
#include"ImageIO.h"

const int ImageIO::imageSizePosition = 2;

const int ImageIO::infoSizePosition = 10;

const int ImageIO::imageWidthPosition = 18;

const int ImageIO::imageHeightPosition = 22;

const int ImageIO::imageInfoHeadSize = 54;

const int ImageIO::ylfInfoSize = 16;


Image * ImageIO::readImage(string & fileName){
	FILE * imageFile = fopen(fileName.c_str(), "rb+");	
	if(imageFile == nullptr){
		cout<<"空文件"<<endl;
		exit(EXIT_FAILURE);
	}

	//读文件头
	uchar * imageInfo = new uchar[imageInfoHeadSize];
	fread(imageInfo, sizeof(uchar), imageInfoHeadSize, imageFile);


	uint width, height, dataSize;
	string imageWidthStr, imageHeightStr;

	for(int i = imageWidthPosition; i < imageWidthPosition + 4; i++){
		imageWidthStr.insert(0, Tools::Char2Hex(imageInfo[i]));
	}
	width = stoi(imageWidthStr, nullptr, 16);

	for(int i = imageHeightPosition; i < imageHeightPosition + 4; i++){
		imageHeightStr.insert(0, Tools::Char2Hex(imageInfo[i]));
	}
	height = stoi(imageHeightStr, nullptr, 16);

	string bmpStr = "yuv";
	cout<<fileName.substr(fileName.length()-3, 3)<<endl;
	if(fileName.substr(fileName.length()-3, 3) == bmpStr){
		dataSize = (width * height * 3) / 2;
	}else{
		dataSize = width * height * 3;
	}
	
	//cout<<"dataSize:"<<dataSize<<endl;

	//读图像数据
	fseek(imageFile, imageInfoHeadSize, 0);
	uchar * data = new uchar[dataSize];
	fread(data, sizeof(uchar), dataSize, imageFile);
	fclose(imageFile);

	Image * image = new Image(imageInfo, data, width, height, dataSize);

	return image;
}


void ImageIO::writeImage(string & revocerFileName, Image * image){
	FILE * recover_file = fopen(revocerFileName.c_str(), "wb");
	fwrite(image->getImageInfo(), sizeof(uchar), imageInfoHeadSize, recover_file);
	fseek(recover_file, 0, -1);
	fwrite(image->getData(), sizeof(uchar), image->getDataSize(), recover_file);
	fclose(recover_file);
	cout<<"finish writing!"<<endl;
}


void ImageIO::read_Huffman_encode_file(const string& fileName, 
									   std::vector<uchar>& Data,
									   uint& weightMapKeySize,
									   uint& weightMapValueSize, 
									   uint& dataSize,
									   uint& sum_size,
									   uint& bit_sum_length){
	FILE * imageFile = fopen(fileName.c_str(), "rb+");	
	if(imageFile == nullptr){
		cout<<"空文件"<<endl;
		exit(EXIT_FAILURE);
	}

	//读bmp信息头
	uchar * imageInfo = new uchar[imageInfoHeadSize];
	fread(imageInfo, sizeof(uchar), imageInfoHeadSize, imageFile);

	//读霍夫曼编码信息头
	uchar* ylfInfo = new uchar[ylfInfoSize];
	fread(ylfInfo, sizeof(uchar), ylfInfoSize, imageFile);

	string weightMapKeyStr = "", weightMapValueStr = "", dataStr = "", bitSumLengthStr="";
	//int weightMapKeySize, weightMapValueSize, dataSize;

	for(int i = 0; i < 4; i++){
		weightMapKeyStr += Tools::Char2Hex(ylfInfo[i]);
	}
	//权重表key总bytes
	weightMapKeySize = stoi(weightMapKeyStr, nullptr, 16);

	for(int i = 4; i < 8; i++){
		weightMapValueStr += Tools::Char2Hex(ylfInfo[i]);
	}
	//权重表value总bytes
	weightMapValueSize = stoi(weightMapValueStr, nullptr, 16);

	for(int i = 8; i < 12; i++){
		dataStr += Tools::Char2Hex(ylfInfo[i]);
	}
	//霍夫曼编码数据总bytes
	dataSize = stoi(dataStr, nullptr, 16);

	for(uint i = 12; i < 16; i++){
		bitSumLengthStr += Tools::Char2Hex(ylfInfo[i]);
	}
	bit_sum_length = stoi(bitSumLengthStr, nullptr, 16);

	//ylf数据总bytes
	sum_size = weightMapKeySize + weightMapValueSize + dataSize;

	uchar * data = new uchar[sum_size];
	fread(data, sizeof(uchar), sum_size, imageFile);
	fclose(imageFile);

	for(int i = 0; i < sum_size; i++){
		Data.push_back(data[i]);
	}
}

Image* ImageIO::readlzwFile(string & fileName){
	FILE * imageFile = fopen(fileName.c_str(), "rb+");	
	if(imageFile == nullptr){
		cout<<"空文件"<<endl;
		exit(EXIT_FAILURE);
	}

	//读bmp文件头
	uchar * imageInfo = new uchar[imageInfoHeadSize];
	fread(imageInfo, sizeof(uchar), imageInfoHeadSize, imageFile);

	//读取lzw数据大小
	uchar * lzwSizeInfo = new uchar[4];
	fread(lzwSizeInfo, sizeof(uchar), 4, imageFile);

	uint encode_size;
	string encodeSizeStr = "";
	for(int i = 0; i < 4; i++){
		encodeSizeStr += Tools::Char2Hex(lzwSizeInfo[i]);
	}
	encode_size = stoi(encodeSizeStr, nullptr, 16);
	//cout<<"read_lzw_size::"<<encode_size<<endl;

	uchar* lzw_encode_data = new uchar[encode_size];
	fread(lzw_encode_data, sizeof(uchar), encode_size, imageFile);
	fclose(imageFile);

	Image* image = new Image(imageInfo, lzw_encode_data, 0, 0, encode_size);

	return image;
}
