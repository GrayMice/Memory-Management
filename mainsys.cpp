#include "memsys.h"
#include <cstdlib>
#include <iostream>
using namespace mainsys;
using namespace std;

MemPackage::MemPackage(unsigned int request) :TotalInfo(request + SIZE_BLOCK_M)	//�е�С����
{
	headSpace = rootTree = (MemBlock*)malloc(getTS());
	if (headSpace == nullptr)
	{
		cerr << "�ڴ治��" << endl;
		exit(EXIT_FAILURE);
	}
	next = pre = nullptr;
}

void* MemPackage::getBlock(unsigned int request)
{
	void *user = nullptr;
	if (request > getTS() || request > getUS())
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

void MainSys::addPackageNode(unsigned int packageSize = PACKAGE_SIZE)
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
		if (packageNode->next == nullptr)	//���һ�����
		{
			MemPackage *pl = packageNode->pre;
			pl->next = nullptr;
			free(packageNode);
		}
		else
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