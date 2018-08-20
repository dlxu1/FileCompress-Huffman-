#ifndef __HEAP_H__
#define __HEAP_H__
#include<vector>
#include<assert.h>
using std::vector;	 

template<class T>
struct Less
{
	bool operator()(const T& left,const  T& right)
	{
		return left<right;
	}
};
template<class T>
struct Greater
{
	bool operator()(const T& left, const T& right)
	{
		return left > right;
	}
};

template<class T, class Compare=Less<T> >
class Heap
{
public:
	Heap()
	{}
	Heap(T data[] ,int size)
	{
		for(int i=0;i<size;i++)
			_heap.push_back(data[i]);

		int j=(size-2)/2;	  //���һ�����ڵ���㹫ʽ

		while(j>=0)
		{
			Adjust_down(j);
			j--;
		}
	}
	void push(T data)
	{
		_heap.push_back(data);
		Adjust_up(_heap.size()-1);
	}
	T top()
	{
		assert(this->size()>0);
		return _heap[0];
	}
	void swap(T& a,T& b)
	{
		T tmp=a;
		a=b;
		b=tmp;
	}
	void pop()
	{
		assert(this->size()>0);
		swap(_heap[_heap.size()-1] , _heap[0]);	  //�����һ��Ԫ����Ѷ�����
		_heap.pop_back();						 //�������һ��Ԫ��
			Adjust_down(0);						 //���µ���
	}
	size_t size()	const 
	{
		return _heap.size();
	}
	void Adjust_down(int parent)
	{
		Compare com;
		size_t  child= parent*2+1;	  //���ݸ��ڵ��������
		while(child<_heap.size())
		{								   
			if(child+1 < _heap.size() && com(_heap[child+1],_heap[child]))
				child++;
			if( com(_heap[child],_heap[parent]) )
			{
				swap(_heap[child],_heap[parent]);
				parent= child;
				child=parent*2+1;
			}
			else
				break;
		}
	}
	void Adjust_up(int child)
	{
		Compare com;
		while(child)
		{
			int parent=(child-1)/2;		//���ݺ��ӽڵ���㸸��
			if(com(_heap[child],_heap[parent]))
			{
				swap(_heap[child],_heap[parent]);
				child=parent;
			}
			else
				break;
		}
	}
private:
	Heap(Heap& t);
	Heap& operator=(const Heap& data);
	vector<T> _heap;
};

#endif //__HEAP_H__