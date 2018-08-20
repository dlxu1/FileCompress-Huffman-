#include<iostream>
#include<fstream>
#include<string>
#include"huffmantree.h"
using namespace std;

typedef long long  longType;
struct CharInfo			    
{
	char _ch;		 //字符
	longType _count; //字符出现次数
	string _code;	 //字符的哈夫曼编码
	bool  operator!=(const CharInfo& a)
	{
		return _count != a._count; 
	}

	CharInfo operator+(const  CharInfo& a)
	{

		CharInfo tmp;
		tmp._count=_count+a._count;
		return tmp;
	}

	bool operator>(const CharInfo& a) const
	{
		return _count > a._count;
	}
};

struct ConfigInfo 	 
{
	char _ch;		  
	longType _count;
};

class FileCompress
{
	typedef HuffmanTreeNode<CharInfo> Node;
public:
	FileCompress()
	{
		for(int i=0;i<256;i++)
		{
			_hashInfos[i]._ch=i;
			_hashInfos[i]._count=0;
		}
	}
	void FileFileCompress(const char* file)
	{
		char ch;
		ifstream ifs(file);

		while(ifs.get(ch))
		{
			_hashInfos[(unsigned char)ch]._count++;
		}
		CharInfo invalid;
		invalid._count=0;
		HuffmanTree<CharInfo> tree(_hashInfos,256,invalid); //构建哈夫曼树
		GreateHuffmanCode(tree.GetRoot());	//生成编码

		string compressfile=file;
		compressfile+=".huffman";
		ofstream ofs(compressfile.c_str() ,ios::out | ios::binary);	//这里务必以二进制写的方式打开文件

		for (size_t i = 0; i < 256; ++i) //将出现次数大于零的字符以及字符次数下入压缩文件，为后期解压准备
		{   
			if (_hashInfos[i]._count>0)
			{
				ConfigInfo info;
				info._ch= _hashInfos[i]._ch;						
				info._count=_hashInfos[i]._count;
				ofs.write((char*)&info, sizeof(ConfigInfo));
			}
		}

		ConfigInfo over;
		over._count = 0;
		ofs.write((char*)&over, sizeof(ConfigInfo));//用于标记末尾

		char value=0;
		int pos=0;
		ifs.clear();
		ifs.seekg(0);
		while(ifs.get(ch))	  //将编码以二进制形式写入压缩文件
		{
			string& code=_hashInfos[(unsigned char)ch]._code;
			for(size_t i=0;i<code.size();i++)
			{
				if(code[i]=='0')
					value &=(~(1 << pos));
				else if(code[i]=='1')
					value |=(1 << pos);
				else
					assert(false);
				++pos;							

				if(pos==8)
				{
					ofs.put(value);
					pos=0;
					value=0;
				}
			}
		}
		if(pos>0)  // 将最后不足八位的的二进制写入压缩文件中
			ofs.put(value);
	}

	void FileUncompress(const char* file) //输入需要解压的文件
	{

		ifstream ifs(file,ios::in | ios::binary); //务必以二进制读的方式打开文件
		string uncompressfile=file;
		size_t pos =uncompressfile.rfind (".");
		if(pos!=string::npos)
			uncompressfile.erase(pos);
		uncompressfile+=".unhuffman";
		ofstream ofs(uncompressfile.c_str());  


		while (1)	//从压缩文件中读取字符及出现次数
		{
			ConfigInfo info;
			ifs.read((char*)&info, sizeof(ConfigInfo));
			if (info._count > 0)
			{
				_hashInfos[(unsigned char)info._ch]._count = info._count;
			}
			else
				break;
		}

		CharInfo invalid;
		invalid._count = 0;
		HuffmanTree<CharInfo> tree(_hashInfos, 256, invalid); // 创建哈夫曼树


		Node* root = tree.GetRoot();
		longType filecount = root->_w._count;	//哈夫曼树根节点记录了总的叶子节点数，即字符总数  
		Node* cur = root;
		char ch;
		while (ifs.read(&ch,1))			  //通过哈夫曼编码解压文件 
		{
			for (size_t i = 0; i < 8; ++i)
			{
				if (ch & (1 << i))
					cur = cur->_right;
				else
					cur = cur->_left;
				if (cur->_left == NULL&&cur->_right == NULL)
				{
					ofs.put(cur->_w._ch);
					cur = root;
					if (--filecount == 0)
					{
						break;
					}
				}
			}
		}
	}

	void GreateHuffmanCode(Node* root)	 
	{
		if (root == NULL)
			return;
		if (root->_left == NULL && root->_right == NULL)
		{
			_hashInfos[(unsigned char)root->_w._ch]._code = root->_w._code;
			return;
		}
		if (root->_left != NULL)
			root->_left->_w._code = root->_w._code + '0';
		if (root->_right != NULL)
			root->_right->_w._code = root->_w._code + '1';

		GreateHuffmanCode(root->_left);
		GreateHuffmanCode(root->_right);
	}
private:
	CharInfo _hashInfos[256];
};

void testFileCompress()
{

	FileCompress test;
	test.FileFileCompress("input.txt");
	FileCompress test1;
	test1.FileUncompress("input.txt.huffman");
}