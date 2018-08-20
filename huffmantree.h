#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__
#include<iostream>
#include<queue>
#include<assert.h>

using std::queue;

template<class W>
struct HuffmanTreeNode
{
	HuffmanTreeNode<W>* _left;
	HuffmanTreeNode<W>* _right;
	HuffmanTreeNode<W>* _parent;
	W _w;
	HuffmanTreeNode(const W& w)
		:_w(w)
		, _left(NULL)
		, _right(NULL)
	{}
};

template <class W>
class HuffmanTree
{
	typedef HuffmanTreeNode<W> Node; 
public:
	HuffmanTree()
		:_root(NULL)
	{}
	struct NodeCompare
	{
		bool operator()(const Node* l,const Node * r)  
		{
			return l->_w > r->_w;
		}
	};

	HuffmanTree(W* a, size_t n,const W& invalid)
	{
		assert(a);
		priority_queue<Node*,vector<Node*>,NodeCompare> minHeap;
		for (size_t i  = 0; i < n; ++i)
		{
			if (a[i] != invalid)
			{
				minHeap.push(new Node(a[i]));
			}
		}

		while (minHeap.size()>1)	//自底向上构建哈夫曼树，每次选择权值最小的节点构建，并将父节点入栈
		{
			Node* left = minHeap.top();
			minHeap.pop();
			Node* right = minHeap.top();
			minHeap.pop();
			Node* parent = new Node(left->_w + right->_w);
			parent->_left = left;
			parent->_right = right;

			minHeap.push(parent);
		}
		_root = minHeap.top();
	}

	Node* GetRoot()	   // 此处返回Root，方便文件压缩处使用
	{
		return _root;
	}
	~HuffmanTree()
	{
		Dsetory(_root);
		_root=NULL;
	}

	void Dsetory(Node* root)
	{
		if(root==NULL)
			return;
		Dsetory(root->_left);
		Dsetory(root->_right);
		delete root;
	}
private:
	Node* _root;
};
#endif //HUFFMAN_H__