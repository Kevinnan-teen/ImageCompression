/**
* @version:1.0
* @author kevinnan.org.cn  2018212114@mail.hfut.edu.cn
* @date 2020/7/6
* @Content:对LZW压缩后的数据进行霍夫曼编码，实现进一步的压缩
*/


#ifndef IMAGECOMPRESSION_HUFFMANCOMPRESSION_H
#define IMAGECOMPRESSION_HUFFMANCOMPRESSION_H

#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <unordered_map> 
#include <bitset>
#include "tools.h"
using namespace std;
typedef unsigned int uint;
typedef unsigned char uchar;

class HuffmanCompression{
private:
	struct HuffmanNode
	{
		uint data;
		uint weight;
		HuffmanNode * left;
		HuffmanNode * right;
		HuffmanNode(uint m_data, uint m_weight) : data(m_data), weight(m_weight), left(nullptr), right(nullptr){}
	};

	struct cmp
	{
		bool operator()(HuffmanNode* h1, HuffmanNode* h2){
			return h1->weight >= h2->weight;
		}
	};
	priority_queue<HuffmanNode*, vector<HuffmanNode*>, cmp> nodeQueue;
	HuffmanNode * rootNode;


	std::unordered_map<uint, uint> weightMap; 	//权重表(需保存到编码文件中,通过权重表可以生成霍夫曼树)
	std::vector<uint> rawData;	//编码原数据

	uint encode_sum_size;		//压缩后总bytes
	uint bit_sum_length;		//霍夫曼编总长度(01序列)

	unordered_map<uint, uint> decodeWeightMap;	//文件解码后的权值表
	//std::vector<uint> v;
	string decode_list;							//解码01序列


public:
	unordered_map<uint, uint> getWeightMap(){
		return weightMap;
	}

	unordered_map<uint, uint> getDecodeWeightMap(){
		return decodeWeightMap;
	}


	HuffmanCompression(std::vector<uint> data);

	HuffmanCompression();

	~HuffmanCompression();

	HuffmanNode * getRoot(){
		return rootNode;
	}

	uint get_encode_sum_size(){
		return encode_sum_size;
	}

	void build_Huffman_Tree();

	void traverse_tree(HuffmanNode* root, string str, unordered_map<uint, string>& huffman_code);

	uchar* Huffman_encode();

	void Huffman_decode(HuffmanNode* root, string& decodeListStr, vector<uint>& recoverData);

	//编码原始数据
	void generate_encode_data(std::vector<uint>& rawData, 
							  std::unordered_map<uint, string>& huffman_code, 
							  std::vector<uchar>& outputData);

	//将权重表转为char
	void generate_encode_weightMap(std::vector<uchar>& outputWeightMapKey,
								   std::vector<uchar>& outputWeightMapValue);

	//生成信息头
	void generate_ylfInfo(const vector<uchar>& outputData,
					  					  const vector<uchar>& outputWeightMapKey,
					                      const vector<uchar>& outputWeightMapValue,
					  					  vector<uchar>& ylfInfo);

	std::vector<uint> huffman_decode_init(std::vector<uchar>& data, uint& weightMapKeySize, 
							 uint weightMapValueSize, uint dataSize, uint bit_sum_length);
};

#endif