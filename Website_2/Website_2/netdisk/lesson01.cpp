// lesson01.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//CString  char*p  new  内存是连续
int a[10] = { 3, 4, 6, 1, 20 };

struct stNode
{
	int a;
	float b;
	stNode* pNext;
};

stNode node_array[10];  //十个stNode元素存储是连续的
//不连续的内存保存十个元素
//stNode* node_array_ptr[10];
//node_array_ptr[0] = new stNode;




//假如要保存很多数据，如果用数组，大小限制
//cString   //保存字符串类型
//int      //封装备一个装 int类型容器
//类。。。。
//容器分为多种（vector,list,map..）不同的容器,每个容器都可以存放任何类型的数据，
//数据（元素，结点）之间会有一些关系，跟据这些关系的不同或者其他的一些程序要的要求我们会选择不同的容器
//数据的关系：数据结构
/*四种结构：
1.集合结构：元素之间关系是属性同一个集合，极松散的一种结构
2.线性结构：元素之间存在着一对一的关系
3.树型结构：元素之间存在着一对多的关系
4.图形结构：元素之间存在着多对多的关系，也称网状结构

物理结构：
前一个结点与下一个结点之间内存是连续的：顺序存储结构
前一个结点与下一个结点之间的内存不一定是连续的，是随机的

算法：不用太在意，只要知道一些简单的算法
复杂的没有必要都知道，有需要的时候查下资料

对于不同的算法，比较时间复杂度是没有意义

*/
//最小次数、最大次数
int temsArray[10];//已经是从小到大的排序
//认为基本操作被执行了0次
//(num-1)*(num-1)  num^2 -2*num+1  num的最大数量级  2次方
//O(f(n))     O(n^2)
void sort(int num, int* a)
{
	for (int i = 0; i < num - 1;i++)
	{
		for (int j = i + 1; j < num;j++)
		{
			if (a[i]>a[j])
			{
				int temp = a[i];
				a[i] = a[j];
				a[j] = temp;
			}
		}
	}
}

//num次  O(n)
void addValue1(int num, int* a)
{
	for (int i = 0; i < num;++i)
	{
		a[i]++;
	}
}

//i = 1;
//while (i < n)
//	i = i * 2;//i = i+2    
//log2n
//计算次数 100 100 1000000   一年
//O(1)  确定次数的算法
int testFunc()
{
	int sum = 0;
	for (int i = 1; i <= 100;i++)
	{
		sum += i;
	}
	return sum;
}



int _tmain(int argc, _TCHAR* argv[])
{
	stNode* pHead = new stNode;//有了第一个结点
	stNode* pCur = pHead;
	pCur->a = -1;
	for (int i = 0; i < 9;i++)
	{
		pCur->pNext = new stNode;
		pCur = pCur->pNext;
		pCur->pNext = NULL;
		pCur->a = i;
	}
	pCur = pHead;
	while (pCur)
	{
		stNode* pTemp = pCur;
		pCur = pTemp->pNext;
		delete pTemp;
	}
	
	return 0;
}

