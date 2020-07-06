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
	for(int i = 0; i < 20; i++){
		YUVdata_n.push_back(to_string(i%10));
	}
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

	// int j = 0;
	// for(auto iter = encode_map.begin(); iter != encode_map.end(); iter++){
	// 	cout<<iter->first<<"\t"<<iter->second<<endl;
	// 	// if(j == 300)
	// 	// 	break;
	// 	// j++;
	// }

	cout<<"symbol:"<<symbol<<endl;

	// cout<<"encode_output:"<<endl;
	// for(auto iter = lzw_encode_output.begin(); iter != lzw_encode_output.end(); iter++)
	// 	cout<<*iter<<"\t";
	// cout<<endl;

}


void LZWcompress::LZW_decode(){
	decode_init();
	uint symbol = 256;
	// for(auto iter = lzw_encode_output.begin(); iter != lzw_encode_output.end(); iter++){
	// 	cout<<*iter<<"\t";
	// }
	cout<<"encode_size:"<<lzw_encode_output.size()<<endl;

	int previous_symbol = -1, current_symbol = -1;
	string previous_char = "", current_char = "", p_and_c = "";

	lzw_decode_output.push_back(decode_map[lzw_encode_output[0]]);
	previous_symbol = lzw_encode_output[0];


	for(auto iter = (lzw_encode_output.begin()+1); iter != lzw_encode_output.end(); iter++){
		current_symbol = *iter;
		// cout<<"come here"<<endl;
		// cout<<"current_symbol:"<<current_symbol<<endl;
		// cout<<"previous_symbol:"<<previous_symbol<<endl;
		if(decode_map.find(current_symbol) != decode_map.end()){
			//symbol在字典中
			//cout<<"come here22"<<endl;
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
			//cout<<"come here33"<<endl;
			previous_char = decode_map[previous_symbol];
			
			//cout<<"decode_map:"<<decode_map[previous_symbol]<<endl;
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

	// cout<<"decode_map:"<<endl;
	// for(auto iter = decode_map.begin(); iter != decode_map.end(); iter++){
	// 	cout<<iter->first<<"\t"<<iter->second<<endl;
	// }


	cout<<"decode_size:"<<lzw_decode_output.size()<<endl;
	cout<<"origin:"<<YUVdata_n.size()<<endl;
	// for(auto iter = lzw_decode_output.begin(); iter != (lzw_decode_output.end()); iter++)
	// 	cout<<*iter<<"\t";
	// cout<<endl<<"***********"<<endl;
	// for(auto iter = YUVdata_n.begin(); iter != (YUVdata_n.end()); iter++)
	// 	cout<<*iter<<"\t";
	// for(auto iter = lzw_decode_output.begin(); iter != lzw_decode_output.end(); iter++)
	// 	cout<<*iter<<"\t";
	// cout<<endl;
}



std::vector<uint> get_lzw_encode(){
	return lzw_decode_output;
}