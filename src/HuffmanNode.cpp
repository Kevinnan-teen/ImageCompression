/**
* @version:1.0
* @author kevinnan.org.cn  2018212114@mail.hfut.edu.cn
* @date 2020/7/6
* @Content:构建霍夫曼树的节点
*/

#include "HuffmanNode.h"

HuffmanNode::HuffmanNode(map<uint, int> data, HuffmanNode * left, HuffmanNode * right){
	this->data = data;
}