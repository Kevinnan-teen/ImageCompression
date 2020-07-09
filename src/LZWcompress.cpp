/**
* @version:1.0
* @author kevinnan.org.cn  2018212114@mail.hfut.edu.cn
* @date 2020/7/5
* @Content:将YUV数据通过LZW空间压缩算法进一步压缩
*/

#include "LZWcompress.h"

LZWcompress::LZWcompress(uchar * YUVdata, uint size){
	for(uint i = 0; i < size; i++){
		YUVdata_n.push_back(std::to_string(int(YUVdata[i])));
	}
}

LZWcompress::LZWcompress(){
// 	for(int i = 0; i < 20; i++){
// 		YUVdata_n.push_back(to_string(i%10));
// 	}
}

void LZWcompress::display(){
	for(int i = 0; i < 16; i++){
		std::cout<<YUVdata_n[i]<<"\t";
	}
	cout<<endl;
}

void LZWcompress::encode_init(){
	for(int i = 0; i < 256; i++){
		encode_map.insert(std::pair<string, uint>(std::to_string(i), i));
	}

	// for(int i = 0; i < 256; i++){
	// 	if(encode_map.find(to_string(i)) == encode_map.end())
	// 		cout<<"lack!"<<endl;
	// }

	// for(auto iter = encode_map.begin(); iter != encode_map.end(); iter++){
	// 	cout<<iter->first<<"\t"<<iter->second<<endl;
	// }
	
}


void LZWcompress::decode_init(){
	for(int i = 0; i < 256; i++){
		decode_map.insert(std::pair<uint, string>(i, std::to_string(i)));
	}
}


void LZWcompress::LZW_encode(){
	encode_init();
	uint symbol = 256;
	string previous_char = "", current_char = "", p_and_c = "";
	for(auto yuvIter = YUVdata_n.begin(); yuvIter != YUVdata_n.end(); yuvIter++){
		current_char = *yuvIter;
		if(previous_char != ""){
			p_and_c = previous_char + "-" + current_char;
		}else{
			p_and_c = current_char;
		}
		//cout<<"previous_:"<<previous_char<<"\tcurrent:"<<current_char<<"\tp_and_c"<<p_and_c<<endl;
		if (encode_map.find(p_and_c) != encode_map.end()){
			//p_and_c在字典里
			previous_char = p_and_c;
		}else{
			//p_and_c不在字典里
			lzw_encode_output.push_back(encode_map[previous_char]);
			encode_map.insert(pair<string, uint>(p_and_c, symbol));
			symbol += 1;
			previous_char = current_char;
		}
	}
	lzw_encode_output.push_back(encode_map[p_and_c]);

	cout<<"symbol:"<<symbol<<endl;


}


void LZWcompress::LZW_decode(){
	decode_init();
	uint symbol = 256;

	cout<<"encode_size:"<<lzw_encode_output.size()<<endl;

	int previous_symbol = -1, current_symbol = -1;
	string previous_char = "", current_char = "", p_and_c = "";

	lzw_decode_output.push_back(decode_map[lzw_encode_output[0]]);
	previous_symbol = lzw_encode_output[0];


	for(auto iter = (lzw_encode_output.begin()+1); iter != lzw_encode_output.end(); iter++){
		current_symbol = *iter;
		if(decode_map.find(current_symbol) != decode_map.end()){
			//symbol在字典中
			auto decode_vector = Tools::splitStr(decode_map[current_symbol], "-");
			for(auto iter = decode_vector.begin(); iter != decode_vector.end(); iter++)
				lzw_decode_output.push_back(*iter);
			previous_char = decode_map[previous_symbol];
			current_char = decode_vector[0];
			if(previous_char != "")
				p_and_c = previous_char + "-" + current_char;
			else
				p_and_c = current_char;
			decode_map.insert(pair<int, string>(symbol, p_and_c));
			symbol += 1;
			
		}else{
			//symbol不在字典中
			previous_char = decode_map[previous_symbol];

			current_char = Tools::splitStr(decode_map[previous_symbol], "-")[0];
			if(previous_char != "")
				p_and_c = previous_char + "-" + current_char;
			else
				p_and_c = current_char;
			decode_map.insert(pair<int, string>(symbol, p_and_c));
			auto decode_vector_2 = Tools::splitStr(p_and_c, "-");
			for(auto iter = decode_vector_2.begin(); iter != decode_vector_2.end(); iter++)
				lzw_decode_output.push_back(*iter);

			symbol += 1;
			
		}
		previous_symbol = current_symbol;
	}
}



uchar* LZWcompress::get_lzw_encode(){
	
	uchar* lzw_encode_data = new uchar[lzw_encode_output.size()*4+4];
	int k = 0;
	//保存lzw压缩文件大小
	for(int i = 0; i < 4; i++){
		std::vector<uchar> char_vector = Tools::Int2CharVector(get_encode_size());
		lzw_encode_data[k++] = char_vector[i];
	}
	for(auto iter = lzw_encode_output.begin(); iter != lzw_encode_output.end(); iter++){
		std::vector<uchar> char_vector = Tools::Int2CharVector(*iter);
		for(int i = 0; i < 4; i++){
			lzw_encode_data[k++] = char_vector[i];
		}
	}
	return lzw_encode_data;
}


uchar* LZWcompress::get_lzw_decode(){
	uchar* lzw_decode_data = new uchar[lzw_decode_output.size()];
	for(int i = 0; i < lzw_decode_output.size(); i++){
		lzw_decode_data[i] = uchar(stoi(lzw_decode_output[i]));
	}
	cout<<"lzw_decode_data:"<<lzw_decode_output.size()<<endl;
	return lzw_decode_data;
}


uint LZWcompress::get_encode_size(){
	return lzw_encode_output.size() * 4;
}

uint LZWcompress::get_decode_size(){
	return lzw_decode_output.size();
}


void LZWcompress::set_encode_data(uchar* lzw_encode_data, uint encode_size){
	for(int i = 0; i < encode_size; i+=4){
		string str = "";
		for(int j = 0; j < 4; j++){
			str += Tools::Char2Hex(lzw_encode_data[i+j]);
		}
		lzw_encode_output.push_back(stoi(str, nullptr, 16));
	}
	cout<<lzw_encode_output.size()<<endl;
}


