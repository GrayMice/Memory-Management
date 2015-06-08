#include "memsys.h"
#include <cstdlib>
#include <iostream>
using namespace mainsys;
using namespace std;

MemPackage::MemPackage(unsigned int request) :TotalInfo(request + SIZE_BLOCK_M)	//有点小问题
{
	headSpace = rootTree = (MemBlock*)malloc(getTS());
	if (headSpace == nullptr)
	{
		cerr << "内存不足" << endl;
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
		//在空闲内存管理树中遍历结点
	}
}

MainSys::MainSys() :TotalInfo(PACKAGE_SIZE)
{
	headPackage = latestPackage = (MemPackage*)malloc(sizeof(MemPackage));
	if (headPackage == nullptr)
	{
		cerr << "内存不足" << endl;
		exit(EXIT_FAILURE);
	}
}

void MainSys::addPackageNode(unsigned int packageSize = PACKAGE_SIZE)
{
	MemPackage *pn = (MemPackage*)malloc(packageSize);
	if (pn == nullptr)
	{
		cerr << "内存不足" << endl;
		exit(EXIT_FAILURE);
	}
	pn->pre = latestPackage;
	pn->next = nullptr;
	latestPackage->next = pn;
	latestPackage = pn;
}

void MainSys::deletePackageNode(MemPackage *packageNode)
{
	if (packageNode->pre == nullptr)	//第一个结点一直保留
		return;
	else
	{
		if (packageNode->next == nullptr)	//最后一个结点
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