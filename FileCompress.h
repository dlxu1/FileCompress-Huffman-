#include<iostream>
#include<fstream>
#include<string>
#include"huffmantree.h"
using namespace std;

typedef long long  longType;
struct CharInfo			    
{
	char _ch;		 //�ַ�
	longType _count; //�ַ����ִ���
	string _code;	 //�ַ��Ĺ���������
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
		HuffmanTree<CharInfo> tree(_hashInfos,256,invalid); //������������
		GreateHuffmanCode(tree.GetRoot());	//���ɱ���

		string compressfile=file;
		compressfile+=".huffman";
		ofstream ofs(compressfile.c_str() ,ios::out | ios::binary);	//��������Զ�����д�ķ�ʽ���ļ�

		for (size_t i = 0; i < 256; ++i) //�����ִ�����������ַ��Լ��ַ���������ѹ���ļ���Ϊ���ڽ�ѹ׼��
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
		ofs.write((char*)&over, sizeof(ConfigInfo));//���ڱ��ĩβ

		char value=0;
		int pos=0;
		ifs.clear();
		ifs.seekg(0);
		while(ifs.get(ch))	  //�������Զ�������ʽд��ѹ���ļ�
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
		if(pos>0)  // ��������λ�ĵĶ�����д��ѹ���ļ���
			ofs.put(value);
	}

	void FileUncompress(const char* file) //������Ҫ��ѹ���ļ�
	{

		ifstream ifs(file,ios::in | ios::binary); //����Զ����ƶ��ķ�ʽ���ļ�
		string uncompressfile=file;
		size_t pos =uncompressfile.rfind (".");
		if(pos!=string::npos)
			uncompressfile.erase(pos);
		uncompressfile+=".unhuffman";
		ofstream ofs(uncompressfile.c_str());  


		while (1)	//��ѹ���ļ��ж�ȡ�ַ������ִ���
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
		HuffmanTree<CharInfo> tree(_hashInfos, 256, invalid); // ������������


		Node* root = tree.GetRoot();
		longType filecount = root->_w._count;	//�����������ڵ��¼���ܵ�Ҷ�ӽڵ��������ַ�����  
		Node* cur = root;
		char ch;
		while (ifs.read(&ch,1))			  //ͨ�������������ѹ�ļ� 
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