## 基于BMP位图的图像压缩 (ImageCompression)

#### 项目语言
C++(std=C++11)

#### 操作系统
windos10　ubuntu16.04　均测试通过

#### 文件要求

- 目前只支持BMP24位的图像格式．
- 提供两种压缩方式
  - 无损压缩：LZW压缩(生成文件后缀名为.lzw)
  - 有损压缩：RGB转YUV + LZW压缩(生成文件后缀名为.ylf)

#### 算法流程

#### 项目结构

![](images/structure.png)

#### 读取和写入图像二进制常用的函数

- Char2Hex ：将单个字符转为16进制字符串

```cpp
string Char2Hex(uchar c){
	const std::string hex = "0123456789ABCDEF";
	string ret;
	ret.push_back(hex[(c >> 4) & 0xf]); //取二进制高四位
	ret.push_back(hex[c & 0xf]);        //取二进制低四位
	return ret;
}
```

- splitStr ：c++划分字符串

```cpp
std::vector<std::string> splitStr(const std::string &str,const std::string &pattern)
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
```

- bitset2char ：8位bitset类型转为char

```cpp
char bitset2char(std::bitset<8> bits)
{
	return (char)bitset<8>(bits.to_string()).to_ulong();
}
```

- Int2CharVector ：32位整数转4字节char型

```cpp
std::vector<uchar> Int2CharVector(uint data){
	std::vector<uchar> buf;
	bitset<32> data_bit(data);
	string data_bit_str = data_bit.to_string();
	for(int i = 0; i < 4; i++){
		bitset<8> data_bit_part(data_bit_str.substr(i*8, 8));
		buf.push_back(bitset2char(data_bit_part));
	}
	return buf;
}
```

#### 项目进度记录

##### 2020/7/5更新

代码风格从RGB转为YUV之后开始发生转变，原因是要开始使用STL提供的容器如vector, map等来替代c++原生的new动态数组．


##### 2020/7/6更新
将YUV格式数据经过LZW编码，实现空间压缩的效果．
最终生成图片的后缀 .ylh -- YUV+LZW+Huffman


##### 2020/7/7更新
完成lzw编码后的数据转换为霍夫曼编码
霍夫曼编码文件存储格式: bmp图像信息头　+ 权值表大小信息　+ 权值表　＋　霍夫曼编码结果

##### 2020/7/8更新
霍夫曼解码部分出现问题，在读取权值表之后生成霍夫曼树时，由于数据出现的频率有重复，因此构成的
霍夫曼树不唯一，导致编码和解码的结果不一样．
解决方案应该是　把霍夫曼树生成的数据对应的编码代替权值表，但是要将bit序列转为char型保存在文件中后，再读取复原
时会因为长度的原因会丢失编码表的信息，所以也不可取．
解决方案二，在保存霍夫曼树生成的编码表时，可以将bit序列的长度也保存，这样就不会因为读取而丢失信息．
最终，因为个人时间原因，我没有再去深究这个问题，即放弃使用霍夫曼编码．只采用了(1)RGB转YUV，和(2)LZW编码算法
编码YUV数据这两个步骤来实现bmp图像的压缩．