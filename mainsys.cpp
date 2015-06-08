#include "memsys.h"
#include <cstdlib>
#include <iostream>
using namespace mainsys;
using namespace std;

MemPackage::MemPackage(unsigned int request) :TotalInfo(request + SIZE_BLOCK_M)	//�е�С����
{
	headSpace = rootTree = (MemBlock*)malloc(sizeof(MemBlock));	//������ͷ��ַ�Ϳ����ڴ�������ָ���һ���ڴ��
	if (headSpace == nullptr)
	{
		cerr << "�ڴ治��" << endl;
		exit(EXIT_FAILURE);
	}
	next = pre = nullptr;

	headSpace->left = headSpace->right = headSpace->pre = nullptr;	//�ڴ����Ϣ�ĳ�ʼ��
	headSpace->parent = headSpace;
	headSpace->totalSize = request;
	headSpace->usedSize = 0;
}

MemPackage

void* MemPackage::getBlock(unsigned int request)
{
	void *user = nullptr;
	if (request > getTS() - getUS())	//�������δ����ռ�ʱ�����ؿ�ָ��
		return user;	
	else
	{
		//�ڿ����ڴ�������б������
	}
}

MainSys::MainSys() :TotalInfo(PACKAGE_SIZE)
{
	headPackage = latestPackage = (MemPackage*)malloc(sizeof(MemPackage));
	if (headPackage == nullptr)
	{
		cerr << "�ڴ治��" << endl;
		exit(EXIT_FAILURE);
	}
}

void MainSys::addPackageNode(unsigned int packageSize)
{
	MemPackage *pn = (MemPackage*)malloc(packageSize);
	if (pn == nullptr)
	{
		cerr << "�ڴ治��" << endl;
		exit(EXIT_FAILURE);
	}
	pn->pre = latestPackage;
	pn->next = nullptr;
	latestPackage->next = pn;
	latestPackage = pn;
}

void MainSys::deletePackageNode(MemPackage *packageNode)
{
	if (packageNode->pre == nullptr)	//��һ�����һֱ����
		return;
	else
	{
		if (packageNode->next == nullptr)	//���һ�����ʱ
		{
			MemPackage *pl = packageNode->pre;
			pl->next = nullptr;
			free(packageNode);
		}
		else	//������м�ʱ
		{
			MemPackage *pl, *pn;
			pl = packageNode->pre;
			pn = packageNode->next;
			pl->next = pn;
			pn->pre = pl;
			free(packageNode);
		}
	}
}

void* MainSys::getBlock(unsigned int request)
{
	void *user;
	if (request > getTS() - getUS())	//������ڿɷ���ռ�ʱ������ڴ��
	{
		addPackageNode(request + SIZE_BLOCK_M);
		user = latestPackage->getBlock(request);
	}
	else	//����С�ڵ��ڿɷ���ռ�ʱ����˳�����ÿ���ڴ��
	{
		MemPackage *p = headPackage;
		while (p->next != nullptr)	//�������һ�����
		{
			if (request <= p->getTS() - p->getUS())	//����һ�����ÿռ����������ڴ��
			{
				user = p->getBlock(request);	//�����ڴ�����ڴ���亯��
				break;
			}
			else
				continue;	//���ڴ������Ҫ��
		}
		if (user == nullptr)	//���δ�ܳɹ��������Ҫ����ڴ�飬���½�һ���ڴ��
		{
			addPackageNode(request + SIZE_BLOCK_M);
			user = latestPackage->getBlock(request);
		}
	}
}