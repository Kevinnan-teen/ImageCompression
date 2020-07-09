/**
* @version:1.0
* @author kevinnan.org.cn  2018212114@mail.hfut.edu.cn
* @date 2020/7/6
* @Content:对LZW压缩后的数据进行霍夫曼编码，实现进一步的压缩
*/

#include "HuffmanCompression.h"

HuffmanCompression::HuffmanCompression(std::vector<uint> data){
	for(auto iter = data.begin(); iter != data.end(); iter++){
		if(weightMap.find(*iter) != weightMap.end())
			weightMap[*iter] += 1;
		else
			weightMap.insert(pair<uint, uint>(*iter, 1));
		rawData.push_back(*iter);
	}

	cout<<"rawData_size:"<<data.size()<<endl;
	
	for(auto iter = weightMap.begin(); iter != weightMap.end(); iter++){
		nodeQueue.push(new HuffmanNode(iter->first, iter->second));
	}
	
	cout<<nodeQueue.top()->data<<"\t"<<nodeQueue.top()->weight<<endl;
	cout<<"nodeQueue_size:"<<nodeQueue.size()<<endl;
}


HuffmanCompression::HuffmanCompression(){
	// std::vector<uint> data;
	// data.push_back(1);
	// data.push_back(10);
	// data.push_back(2);
	// data.push_back(1);
	// data.push_back(2);
	// data.push_back(2);
	// data.push_back(10);
	// data.push_back(1);
	// data.push_back(1);
	// data.push_back(100);
	// data.push_back(11);
	// data.push_back(100);
	// data.push_back(199);
	// data.push_back(10);
	// data.push_back(20);
	// data.push_back(1000);


	// rawData = data;

	// for(auto iter = data.begin(); iter != data.end(); iter++){
	// 	if(weightMap.find(*iter) != weightMap.end())
	// 		weightMap[*iter] += 1;
	// 	else
	// 		weightMap.insert(pair<uint, uint>(*iter, 1));
	// }

	
	// for(auto iter = weightMap.begin(); iter != weightMap.end(); iter++){
	// 	nodeQueue.push(new HuffmanNode(iter->first, iter->second));
	// }

	// cout<<"nodeQueue:"<<nodeQueue.size()<<endl;

}


void HuffmanCompression::build_Huffman_Tree(){
	while(nodeQueue.size() >= 2){
		HuffmanNode * leftNode = nodeQueue.top();
		nodeQueue.pop();
		HuffmanNode * rightNode = nodeQueue.top();
		nodeQueue.pop();
		HuffmanNode * newNode = new HuffmanNode(0, leftNode->weight + rightNode->weight);
		newNode->left = leftNode;
		newNode->right = rightNode;
		nodeQueue.push(newNode);
		//cout<<newNode->data<<"\t"<<newNode->weight<<endl;
	}
	rootNode = nodeQueue.top();
	//cout<<rootNode->data<<"\t"<<rootNode->weight<<endl;
}


void HuffmanCompression::traverse_tree(HuffmanNode* root, string str, unordered_map<uint, string>& huffman_code){
	if (root != nullptr){
		if (root->right == nullptr && root->left == nullptr){
			huffman_code.insert(pair<uint, string>(root->data, str));
			str = str.substr(0, str.length()-1);
			return;
		}
		str += "0";
		traverse_tree(root->left, str, huffman_code);
		str = str.substr(0, str.length()-1);
		str += "1";
		traverse_tree(root->right, str, huffman_code);
	}
}

uchar* HuffmanCompression::Huffman_encode(){
	build_Huffman_Tree();
	std::unordered_map<uint, string> huffman_code;
	traverse_tree(rootNode, "", huffman_code);

	cout<<"huffman_code_size:"<<huffman_code.size()<<endl;


	//存入压缩文件ylh的数据
	vector<uchar> outputData;
	vector<uchar> outputWeightMapKey;
	vector<uchar> outputWeightMapValue;
	vector<uchar> ylfInfo;		//压缩文件ylf信息头


	generate_encode_data(rawData, huffman_code, outputData);

	generate_encode_weightMap(outputWeightMapKey, outputWeightMapValue);

	generate_ylfInfo(outputData, outputWeightMapKey, outputWeightMapValue, ylfInfo); 

	//压缩后总bytes
	encode_sum_size = ylfInfo.size()+outputWeightMapKey.size()+outputWeightMapValue.size()+outputData.size();

	//压缩数据
	uchar * huffman_encode_data = new uchar[encode_sum_size];
	uint i = 0;
	for(auto iter = ylfInfo.begin(); iter != ylfInfo.end(); iter++){
		huffman_encode_data[i++] = *iter;
	}
	for(auto iter = outputWeightMapKey.begin(); iter != outputWeightMapKey.end(); iter++){
		huffman_encode_data[i++] = *iter;
	}
	for(auto iter = outputWeightMapValue.begin(); iter != outputWeightMapValue.end(); iter++){
		huffman_encode_data[i++] = *iter;
	}
	for(auto iter = outputData.begin(); iter != outputData.end(); iter++){
		huffman_encode_data[i++] = *iter;
	}



	cout<<"key_size:"<<outputWeightMapKey.size()<<"\t"<<"value_size:"<<outputWeightMapValue.size()<<"\t"<<
														"outputData_size:"<<outputData.size()<<endl;

	cout<<"sum_size:"<<encode_sum_size<<endl;
	cout<<"origin_size:"<<rawData.size()*4<<endl;

	// cout<<"code_result:"<<endl;
	// // for(auto iter = huffman_code.begin(); iter != huffman_code.end(); iter++)
	// // 	cout<<iter->first<<"\t"<<iter->second<<endl;
	// cout<<"huffman_code_size:"<<huffman_code.size()<<endl;
	return huffman_encode_data;
}


void HuffmanCompression::generate_encode_data(std::vector<uint>& rawData, 
											  std::unordered_map<uint, string>& huffman_code, 
											  std::vector<uchar>& outputData){
	cout<<"霍夫曼编码前数据大小:"<<rawData.size()<<endl;
	cout<<"编码结果:"<<endl;
	string encode_str = "";
	//string encode_str_2 = "";
	bit_sum_length = 0;
	for(auto iter = rawData.begin(); iter != rawData.end(); iter++){
		//cout<<huffman_code.find(*iter)->first<<"\t"<<huffman_code.find(*iter)->second<<endl;
		encode_str += huffman_code.find(*iter)->second;
		bit_sum_length += huffman_code.find(*iter)->second.length();
		//cout<<huffman_code.find(*iter)->second.length()<<endl;	
		//encode_str_2 = encode_str;
		while(encode_str.length() >= 8){
			bitset<8> bits(encode_str.substr(0, 8));
			encode_str = encode_str.substr(8);
			//cout<<bits<<endl;
			outputData.push_back(Tools::bitset2char(bits));
		}

		//cout<<bit_sum_length<<endl;
		//cout<<j++<<endl;
	}
	cout<<huffman_code.find(*rawData.rbegin())->second<<"\t"<<*rawData.rbegin()<<endl;
	cout<<"bit_sum_length:"<<bit_sum_length<<endl;
	cout<<"left_str:"<<encode_str<<endl;
	if(encode_str.length() < 8 && encode_str.length() > 0){
		bitset<8> bits(encode_str);
		outputData.push_back(Tools::bitset2char(bits));
	}
	cout<<"outputData_size:"<<outputData.size()<<endl;
	//cout<<"encode_str:"<<encode_str_2<<"\t"<<encode_str_2.length()<<endl;
}


void HuffmanCompression::generate_encode_weightMap(std::vector<uchar>& outputWeightMapKey,
												   std::vector<uchar>& outputWeightMapValue){
	//cout<<"weightMap_output:"<<endl;
	cout<<"origin_key:"<<endl;
	for(auto iter = weightMap.begin(); iter != weightMap.end(); iter++){
		std::vector<uchar> key_char_vector = Tools::Int2CharVector(iter->first);
		std::vector<uchar> value_char_vector = Tools::Int2CharVector(iter->second);
		for(int i = 0; i < 4; i++){
			outputWeightMapKey.push_back(key_char_vector[i]);
			outputWeightMapValue.push_back(value_char_vector[i]);
		}
		//cout<<iter->first<<"\t"<<iter->second<<"\t";
	}
	

	//读取weightMap部分
	cout<<endl<<"decode_key:"<<endl;
	for(int i = 0; i < outputWeightMapKey.size(); i+=4){
		string key_str = "";
		string value_str = "";
		for(int j = 0; j < 4; j++){
			key_str += Tools::Char2Hex(outputWeightMapKey[i+j]);
			value_str += Tools::Char2Hex(outputWeightMapValue[i+j]);
		}
		//cout<<stoi(key_str, nullptr, 16)<<"\t"<<stoi(value_str, nullptr, 16)<<"\t";
	}
}


void HuffmanCompression::generate_ylfInfo(const vector<uchar>& outputData,
					  const vector<uchar>& outputWeightMapKey,
					  const vector<uchar>& outputWeightMapValue,
					  vector<uchar>& ylfInfo){
	std::vector<uchar> v = Tools::Int2CharVector(outputWeightMapKey.size());
	std::vector<uchar> v2 = Tools::Int2CharVector(outputWeightMapValue.size());
	std::vector<uchar> v3 = Tools::Int2CharVector(outputData.size());
	std::vector<uchar> v4 = Tools::Int2CharVector(bit_sum_length);
	for(int i = 0; i < 4; i++){
		ylfInfo.push_back(v[i]);
	}
	for(int i = 0; i < 4; i++){
		ylfInfo.push_back(v2[i]);
	}
	for(int i = 0; i < 4; i++){
		ylfInfo.push_back(v3[i]);
	}
	for(int i = 0; i < 4; i++){
		ylfInfo.push_back(v4[i]);
	}



	cout<<"decode_ylfInfo:"<<endl;
	for(int i = 0; i < ylfInfo.size(); i+=4){
		string info = "";
		for(int j = 0; j < 4; j++){
			info += Tools::Char2Hex(ylfInfo[i+j]);
		}
		cout<<stoi(info, nullptr, 16)<<"\t";
	}
}


void HuffmanCompression::Huffman_decode(HuffmanNode* root, string& decodeListStr, vector<uint>& recoverData){
	// int length = decodeListStr.length();
	// for(uint i = 0; i < length; i++){
	// 	if(root->left == nullptr && root->right == nullptr){
	// 		recoverData.push_back(root->data);
	// 		root = rootNode;
	// 	}
	// 	uint byteOff = i / 8;
	// 	uint bitOffInByte = i % 8;
	// 	if ((rawDataPtr[byteOff] >> bitOffInByte) & 1){
			
	// 	}
	// }
	// while(decodeListStr.length()){
	// 	if(root->left == nullptr && root->right == nullptr){
	// 		recoverData.push_back(root->data);
	// 		root = rootNode;
	// 		return;
	// 	}
	// 	if(decodeListStr[0] == '0'){
	// 		root = root->left;
	// 		decodeListStr = decodeListStr.substr(1);
	// 		//cout<<decodeListStr.length()<<endl;
	// 		Huffman_decode(root, decodeListStr, recoverData);
	// 	}else{
	// 		root = root->right;
	// 		decodeListStr = decodeListStr.substr(1);
	// 		//cout<<decodeListStr.length()<<endl;
	// 		Huffman_decode(root, decodeListStr, recoverData);
	// 	}
	// }
	while(decodeListStr.length()){
		if(root->left == nullptr && root->right == nullptr){
			recoverData.push_back(root->data);
			root = rootNode;
		}
		if(decodeListStr[0] == '0'){
			root = root->left;
			decodeListStr = decodeListStr.substr(1);
		}else{
			root = root->right;
			decodeListStr = decodeListStr.substr(1);
		}
		// if(decodeListStr.length()%50000==0)
		// 	cout<<decodeListStr.length()<<endl;
	}
}


std::vector<uint> HuffmanCompression::huffman_decode_init(std::vector<uchar>& data, uint& weightMapKeySize, 
							 				 uint weightMapValueSize, uint dataSize, uint bit_sum_length){
	for(int i = 0; i < weightMapKeySize; i+=4){
		string key_str = "";
		string value_str = "";
		for(int j = 0; j < 4; j++){
			key_str += Tools::Char2Hex(data[i+j]);
			value_str += Tools::Char2Hex(data[weightMapKeySize+i+j]);
		}
		decodeWeightMap.insert(pair<uint, uint>(stoi(key_str, nullptr, 16), 
												stoi(value_str, nullptr, 16)));


	}
	cout<<"decodeWeightMap:"<<decodeWeightMap.size()<<endl;

	for(auto iter = decodeWeightMap.begin(); iter != decodeWeightMap.end(); iter++){
		nodeQueue.push(new HuffmanNode(iter->first, iter->second));
	}	

	while(!nodeQueue.empty()){
		cout<<nodeQueue.top()->data<<"\t"<<nodeQueue.top()->weight<<endl;
		nodeQueue.pop();
	}

	

	cout<<"nodeQueue_size:"<<nodeQueue.size()<<endl;

	build_Huffman_Tree();

	decode_list = "";

	int j = weightMapKeySize+weightMapValueSize;
	cout<<"list_size:"<<(bit_sum_length/8)<<endl;
	for(int i = weightMapKeySize+weightMapValueSize; 
			i < weightMapKeySize+weightMapValueSize+bit_sum_length/8; i++){
		bitset<8> bits(stoi(Tools::Char2Hex(data[i]), nullptr, 16));
		decode_list += bits.to_string();
		if(j > weightMapKeySize+weightMapValueSize+bit_sum_length/8 - 5){
			cout<<bits.to_string()<<"\t";
		}
		j++;
	}
	if(bit_sum_length < (dataSize*8)){
		bitset<8> bits(stoi(Tools::Char2Hex(data[weightMapKeySize+weightMapValueSize+dataSize-1]), nullptr, 16));
		decode_list += bits.to_string().substr((dataSize*8)-bit_sum_length);
	}

	cout<<"解码序列长度:"<<decode_list.length()<<endl;
	cout<<decode_list.substr(decode_list.length()-50)<<endl;

	std::vector<uint> recoverData;
	//Huffman_decode(rootNode, decode_list, recoverData);
	return recoverData;
}

