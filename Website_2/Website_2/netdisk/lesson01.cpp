// lesson01.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

//CString  char*p  new  �ڴ�������
int a[10] = { 3, 4, 6, 1, 20 };

struct stNode
{
	int a;
	float b;
	stNode* pNext;
};

stNode node_array[10];  //ʮ��stNodeԪ�ش洢��������
//���������ڴ汣��ʮ��Ԫ��
//stNode* node_array_ptr[10];
//node_array_ptr[0] = new stNode;




//����Ҫ����ܶ����ݣ���������飬��С����
//cString   //�����ַ�������
//int      //��װ��һ��װ int��������
//�ࡣ������
//������Ϊ���֣�vector,list,map..����ͬ������,ÿ�����������Դ���κ����͵����ݣ�
//���ݣ�Ԫ�أ���㣩֮�����һЩ��ϵ��������Щ��ϵ�Ĳ�ͬ����������һЩ����Ҫ��Ҫ�����ǻ�ѡ��ͬ������
//���ݵĹ�ϵ�����ݽṹ
/*���ֽṹ��
1.���Ͻṹ��Ԫ��֮���ϵ������ͬһ�����ϣ�����ɢ��һ�ֽṹ
2.���Խṹ��Ԫ��֮�������һ��һ�Ĺ�ϵ
3.���ͽṹ��Ԫ��֮�������һ�Զ�Ĺ�ϵ
4.ͼ�νṹ��Ԫ��֮������Ŷ�Զ�Ĺ�ϵ��Ҳ����״�ṹ

����ṹ��
ǰһ���������һ�����֮���ڴ��������ģ�˳��洢�ṹ
ǰһ���������һ�����֮����ڴ治һ���������ģ��������

�㷨������̫���⣬ֻҪ֪��һЩ�򵥵��㷨
���ӵ�û�б�Ҫ��֪��������Ҫ��ʱ���������

���ڲ�ͬ���㷨���Ƚ�ʱ�临�Ӷ���û������

*/
//��С������������
int temsArray[10];//�Ѿ��Ǵ�С���������
//��Ϊ����������ִ����0��
//(num-1)*(num-1)  num^2 -2*num+1  num�����������  2�η�
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

//num��  O(n)
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
//������� 100 100 1000000   һ��
//O(1)  ȷ���������㷨
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
	stNode* pHead = new stNode;//���˵�һ�����
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

