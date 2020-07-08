/**
* @version:1.0
* @author kevinnan.org.cn  2018212114@mail.hfut.edu.cn
* @date 2020/7/2
* @Content:定义通用的一些函数用于调用
*/

#include"tools.h"

string Tools::Char2Hex(uchar c){
	const std::string hex = "0123456789ABCDEF";
	string ret;
	ret.push_back(hex[(c >> 4) & 0xf]); //取二进制高四位
	ret.push_back(hex[c & 0xf]);        //取二进制低四位
	return ret;
}


uchar * Tools::RGB2YUV(uchar * data, uint width, uint height, uint dataSize){
	uint YUVPixelNum = dataSize / 2;
	int * Y_Weight_Vector = new int[width * height];
	int * U_Weight_Vector = new int[(width * height) / 4];
	int * V_Weight_Vector = new int[(width * height) / 4];

	int Y_sum = 0, U_sum = 0, V_sum = 0;
	int Y, U, V;

	for(uint i = 0; i < height; i++){
		for(uint j = 0; j < width; j++){
			Y = 0.114 * data[i*width*3+j*3] + 
				0.587 * data[i*width*3+j*3+1] + 0.299 * data[i*width*3+j*3+2];			
			Y_Weight_Vector[Y_sum++] = int(Y);

			if(j % 4 == 0){
				U = 128 + 0.5 * data[i*width*3+j*3] - 
					0.3313 * data[i*width*3+j*3+1] - 0.1687 * data[i*width*3+j*3+2];
				U_Weight_Vector[U_sum++] = int(U);

			}
			if((j-2) % 4 == 0){
				V = 128 - 0.0813 * data[i*width*3+j*3] - 
					0.4187 * data[i*width*3+j*3+1] + 0.5 * data[i*width*3+j*3+2];
				V_Weight_Vector[V_sum++] = int(V);
			}
		}	
	}

	uchar * YUVData = new uchar[YUVPixelNum];
	for(uint i = 0; i < YUVPixelNum; i++){
		if(i < Y_sum){
			YUVData[i] = uchar(Y_Weight_Vector[i]);
		}else if(i < Y_sum + U_sum){
			YUVData[i] = uchar(U_Weight_Vector[i-Y_sum]);
		}else{
			YUVData[i] = uchar(V_Weight_Vector[i-Y_sum-U_sum]);
		}	
	}

	return YUVData;
}
uchar * Tools::YUV2RGB(uchar * YUVData, uint dataSize){

	uchar * BGR_Vector = new uchar[dataSize*2];
	uint Y_sum = 4 * (dataSize / 6), U_sum = dataSize / 6, V_sum = dataSize / 6;
	
	uchar * Y_Weight_r = new uchar[Y_sum];
	uchar * U_Weight_r = new uchar[U_sum];
	uchar * V_Weight_r = new uchar[V_sum];
	for(uint i = 0; i < dataSize; i++){
		if(i < Y_sum){
			Y_Weight_r[i] = YUVData[i];
		}else if(i < Y_sum+U_sum){
			U_Weight_r[i-Y_sum] = YUVData[i];
		}else{
			V_Weight_r[i-Y_sum-U_sum] = YUVData[i];
		}
	}


	for(uint i = 0; i < V_sum; i++){
		for(uint j = i * 4; j < i * 4 + 4; j++){

			BGR_Vector[3*j] = uchar(int(Y_Weight_r[j])+ 1.772*(int(U_Weight_r[i])-128));
	
			BGR_Vector[3*j+1] = uchar(int(Y_Weight_r[j])-0.34414*(int(U_Weight_r[i])-128)-0.71414*(int(V_Weight_r[i])-128));

			BGR_Vector[3*j+2] = uchar(int(Y_Weight_r[j])+1.402*(int(V_Weight_r[i])-128));
		}
	}
	return BGR_Vector;
}



std::vector<std::string> Tools::splitStr(const std::string &str,const std::string &pattern)
{
    std::vector<std::string> resVec;

	if ("" == str)
    {
        return resVec;
    }
    //方便截取最后一段数据
    std::string strs = str + pattern;
    
    size_t pos = strs.find(pattern);
    size_t size = strs.size();

    while (pos != std::string::npos)
    {
        std::string x = strs.substr(0,pos);
        resVec.push_back(x);
        strs = strs.substr(pos+1,size);
        pos = strs.find(pattern);
    }
    
    return resVec;
}


char Tools::bitset2char(std::bitset<8> bits)
{
	return (char)bitset<8>(bits.to_string()).to_ulong();
}

std::vector<uchar> Tools::Int2CharVector(uint data){
	std::vector<uchar> buf;
	bitset<32> data_bit(data);
	string data_bit_str = data_bit.to_string();
	for(int i = 0; i < 4; i++){
		//string key_bit_str_part = key_bit_str.substr(i*8, 8);
		//string value_bit_str_part = value_bit_str.substr(i*8, 8);
		bitset<8> data_bit_part(data_bit_str.substr(i*8, 8));
		buf.push_back(bitset2char(data_bit_part));
	}
	return buf;
}