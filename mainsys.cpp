#include "memsys.h"
#include <cstdlib>
#include <iostream>
using namespace mainsys;
using namespace std;

MemPackage::MemPackage(unsigned int request) :TotalInfo(request)	//有点小问题
{
	bool *p = (bool*)malloc(request + SIZE_BLOCK_M + 1);	//需求，满足需求的内存块头信息，该内存块的归属标记
	if (p == nullptr)	//内存申请失败时，返回错误信息并退出程序
	{
		cerr << "内存不足，无法创建新的内存包" << endl;
		exit(EXIT_FAILURE);
	}

	headSpace = rootTree = (MemBlock*)p;	//管理域头地址和空闲内存搜索树指向第一个内存块
	
	next = pre = nullptr;	//内存块信息的初始化
	headSpace->left = headSpace->right = headSpace->pre = nullptr;	
	headSpace->parent = headSpace;
	headSpace->totalSize = request;
	headSpace->usedSize = 0;
	p = (bool*)headSpace + SIZE_BLOCK_M + 1;	//设置内存块的归属标记为1
	*p = 1;
}

MemBlock* MemPackage::divideBlock(MemBlock *target)
{
	bool *p;	//标记指针
	MemBlock *secondBlock;
	p = (bool*)target;
	p = p + SIZE_BLOCK_M + target->usedSize + 1;	//标记指针从target向后移动的内存单元
	secondBlock = (MemBlock*)p;
	 
	secondBlock->pre = target;	//初始化新块的信息
	secondBlock->left = secondBlock->parent = secondBlock->right = nullptr;
	secondBlock->totalSize = target->totalSize - target->usedSize - SIZE_BLOCK_M - 1;	//分割出的新块的总管理空间
	secondBlock->usedSize = 0;
	p = (bool*)secondBlock + SIZE_BLOCK_M + 1;	//设置内存块的归属标记为1
	*p = 1;
	return secondBlock;
}

MemBlock* MemPackage::mergeBlock(MemBlock *target)
{
	MemBlock *newBlock, *tmp;

	newBlock = target; tmp = mergeBlock(target, false);	//只要后面的块是空的，就向后合并
	while (tmp != nullptr)
	{
		newBlock = tmp;
		tmp = mergeBlock(newBlock, false);
	}

	tmp = mergeBlock(newBlock, true);	//只要前面的块是空的，就向前合并
	while (tmp != nullptr)
	{
		newBlock = tmp;
		tmp = mergeBlock(newBlock, true);
	}

	return newBlock;	//返回合并好的块
}

MemBlock* MemPackage::mergeBlock(MemBlock *target, bool direction)
{
	if (direction)	//向前合并
	{
		MemBlock *pl;
		if (target->pre != nullptr)	//这个块不是第一个块
		{
			pl = target->pre;
			if (pl->usedSize == 0)
				pl->totalSize = pl->totalSize + target->totalSize + SIZE_BLOCK_M - 1;	//新块管理空间的大小
			return pl;
		}
		else
			return nullptr;	//无法再向前合并时，返回空指针
	}
	else	//向后合并
	{
		MemBlock *pn;
		bool *endp, *p;
		endp = (bool*)headSpace + getTS();
		p = (bool*)target;
		p = p + target->totalSize + SIZE_BLOCK_M + 1;	//移动到这个块的末尾
		if (p != endp)	//标记指针p没有移到管理域最后
		{
			p++;	//标记指针移到下一个块
			pn = (MemBlock*)p;
			if (pn->usedSize == 0)
				target->totalSize = target->totalSize + pn->totalSize + SIZE_BLOCK_M + 1;
			return target;
		}
		else
			return nullptr;	//无法再向后合并时，返回空指针
	}
}

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
	if (headPackage == nullptr)	//内存不足时，无法初始化主系统
	{
		cerr << "内存不足，无法初始化主系统" << endl;
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

void MainSys::deletePackageNode(MemPackage *packageNode)	//删除包结点
{
	if (packageNode->pre == nullptr)	//第一个结点一直保留
		return;
	else
	{
		if (packageNode->next == nullptr)	//最后一个结点时
		{
			MemPackage *pl = packageNode->pre;	//前一个内存包结点
			pl->next = nullptr;	//将内存包从链表尾删去
			free(packageNode);	//释放该结点
		}
		else	//结点在中间时
		{
			MemPackage *pl, *pn;
			pl = packageNode->pre;	//前一个内存包结点
			pn = packageNode->next;	//后一个内存包结点
			pl->next = pn;	//将packageNode从内存包链表中删除
			pn->pre = pl;
			free(packageNode);	//释放该结点
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

void* 