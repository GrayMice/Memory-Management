#include "memsys.h"
#include <cstdlib>
#include <iostream>
using namespace mainsys;
using namespace std;

MemPackage::MemPackage(unsigned int request) :TotalInfo(request + SIZE_BLOCK_M)	//有点小问题
{
	headSpace = rootTree = (MemBlock*)malloc(sizeof(MemBlock));	//管理域头地址和空闲内存搜索树指向第一个内存块
	if (headSpace == nullptr)
	{
		cerr << "内存不足" << endl;
		exit(EXIT_FAILURE);
	}
	next = pre = nullptr;

	headSpace->left = headSpace->right = headSpace->pre = nullptr;	//内存块信息的初始化
	headSpace->parent = headSpace;
	headSpace->totalSize = request;
	headSpace->usedSize = 0;
}

MemPackage

void* MemPackage::getBlock(unsigned int request)
{
	void *user = nullptr;
	if (request > getTS() - getUS())	//需求大于未分配空间时，返回空指针
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

void MainSys::addPackageNode(unsigned int packageSize)
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
		if (packageNode->next == nullptr)	//最后一个结点时
		{
			MemPackage *pl = packageNode->pre;
			pl->next = nullptr;
			free(packageNode);
		}
		else	//结点在中间时
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
	if (request > getTS() - getUS())	//需求大于可分配空间时，添加内存包
	{
		addPackageNode(request + SIZE_BLOCK_M);
		user = latestPackage->getBlock(request);
	}
	else	//需求小于等于可分配空间时，按顺序遍历每个内存包
	{
		MemPackage *p = headPackage;
		while (p->next != nullptr)	//不是最后一个结点
		{
			if (request <= p->getTS() - p->getUS())	//发现一个可用空间大于需求的内存包
			{
				user = p->getBlock(request);	//调用内存包的内存分配函数
				break;
			}
			else
				continue;	//该内存包不合要求
		}
		if (user == nullptr)	//最后未能成功分配符合要求的内存块，则新建一个内存包
		{
			addPackageNode(request + SIZE_BLOCK_M);
			user = latestPackage->getBlock(request);
		}
	}
}