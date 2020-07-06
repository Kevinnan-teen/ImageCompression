/**
* @version:1.0
* @author kevinnan.org.cn  2018212114@mail.hfut.edu.cn
* @date 2020/7/6
* @Content:构建霍夫曼树的节点
*/


#ifndef IMAGECOMPRESSION_HUFFMANNODE_H
#define IMAGECOMPRESSION_HUFFMANNODE_H

#include <map>
using namespace std;

class HuffmanNode{
private:
	map<uint, int> data;
	HuffmanNode * left;
	HuffmanNode * right;

public:
	HuffmanNode(map<uint, int> data, HuffmanNode * left, HuffmanNode * right);
};

#endif