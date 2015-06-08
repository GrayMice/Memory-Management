#include "memsys.h"
#include <cstdlib>
#include <iostream>
using namespace mainsys;
using namespace std;

MemPackage::MemPackage(unsigned int request) :TotalInfo(request)	//�е�С����
{
	bool *p = (bool*)malloc(request + SIZE_BLOCK_M + 1);	//��������������ڴ��ͷ��Ϣ�����ڴ��Ĺ������
	if (p == nullptr)	//�ڴ�����ʧ��ʱ�����ش�����Ϣ���˳�����
	{
		cerr << "�ڴ治�㣬�޷������µ��ڴ��" << endl;
		exit(EXIT_FAILURE);
	}

	headSpace = rootTree = (MemBlock*)p;	//������ͷ��ַ�Ϳ����ڴ�������ָ���һ���ڴ��
	
	next = pre = nullptr;	//�ڴ����Ϣ�ĳ�ʼ��
	headSpace->left = headSpace->right = headSpace->pre = nullptr;	
	headSpace->parent = headSpace;
	headSpace->totalSize = request;
	headSpace->usedSize = 0;
	p = (bool*)headSpace + SIZE_BLOCK_M + 1;	//�����ڴ��Ĺ������Ϊ1
	*p = 1;
}

MemBlock* MemPackage::divideBlock(MemBlock *target)
{
	bool *p;	//���ָ��
	MemBlock *secondBlock;
	p = (bool*)target;
	p = p + SIZE_BLOCK_M + target->usedSize + 1;	//���ָ���target����ƶ����ڴ浥Ԫ
	secondBlock = (MemBlock*)p;
	 
	secondBlock->pre = target;	//��ʼ���¿����Ϣ
	secondBlock->left = secondBlock->parent = secondBlock->right = nullptr;
	secondBlock->totalSize = target->totalSize - target->usedSize - SIZE_BLOCK_M - 1;	//�ָ�����¿���ܹ���ռ�
	secondBlock->usedSize = 0;
	p = (bool*)secondBlock + SIZE_BLOCK_M + 1;	//�����ڴ��Ĺ������Ϊ1
	*p = 1;
	return secondBlock;
}

MemBlock* MemPackage::mergeBlock(MemBlock *target)
{
	MemBlock *newBlock, *tmp;

	newBlock = target; tmp = mergeBlock(target, false);	//ֻҪ����Ŀ��ǿյģ������ϲ�
	while (tmp != nullptr)
	{
		newBlock = tmp;
		tmp = mergeBlock(newBlock, false);
	}

	tmp = mergeBlock(newBlock, true);	//ֻҪǰ��Ŀ��ǿյģ�����ǰ�ϲ�
	while (tmp != nullptr)
	{
		newBlock = tmp;
		tmp = mergeBlock(newBlock, true);
	}

	return newBlock;	//���غϲ��õĿ�
}

MemBlock* MemPackage::mergeBlock(MemBlock *target, bool direction)
{
	if (direction)	//��ǰ�ϲ�
	{
		MemBlock *pl;
		if (target->pre != nullptr)	//����鲻�ǵ�һ����
		{
			pl = target->pre;
			if (pl->usedSize == 0)
				pl->totalSize = pl->totalSize + target->totalSize + SIZE_BLOCK_M - 1;	//�¿����ռ�Ĵ�С
			return pl;
		}
		else
			return nullptr;	//�޷�����ǰ�ϲ�ʱ�����ؿ�ָ��
	}
	else	//���ϲ�
	{
		MemBlock *pn;
		bool *endp, *p;
		endp = (bool*)headSpace + getTS();
		p = (bool*)target;
		p = p + target->totalSize + SIZE_BLOCK_M + 1;	//�ƶ���������ĩβ
		if (p != endp)	//���ָ��pû���Ƶ����������
		{
			p++;	//���ָ���Ƶ���һ����
			pn = (MemBlock*)p;
			if (pn->usedSize == 0)
				target->totalSize = target->totalSize + pn->totalSize + SIZE_BLOCK_M + 1;
			return target;
		}
		else
			return nullptr;	//�޷������ϲ�ʱ�����ؿ�ָ��
	}
}

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
	if (headPackage == nullptr)	//�ڴ治��ʱ���޷���ʼ����ϵͳ
	{
		cerr << "�ڴ治�㣬�޷���ʼ����ϵͳ" << endl;
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

void MainSys::deletePackageNode(MemPackage *packageNode)	//ɾ�������
{
	if (packageNode->pre == nullptr)	//��һ�����һֱ����
		return;
	else
	{
		if (packageNode->next == nullptr)	//���һ�����ʱ
		{
			MemPackage *pl = packageNode->pre;	//ǰһ���ڴ�����
			pl->next = nullptr;	//���ڴ��������βɾȥ
			free(packageNode);	//�ͷŸý��
		}
		else	//������м�ʱ
		{
			MemPackage *pl, *pn;
			pl = packageNode->pre;	//ǰһ���ڴ�����
			pn = packageNode->next;	//��һ���ڴ�����
			pl->next = pn;	//��packageNode���ڴ��������ɾ��
			pn->pre = pl;
			free(packageNode);	//�ͷŸý��
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

void* 