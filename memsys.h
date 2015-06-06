#ifndef __MemoryManagement__memsys__
#define __MemoryManagement__memsys__

#define PACKAGE_SIZE (1024*1024)
#define SIZE_LEVEL 40
#define SIZE_RANGE 10
#define SIZE_BLOCK_S (sizeof(SubBlock)+sizeof(bool*))
#define SIZE_BLOCK_M (sizeof(MemBlock)+sizeof(bool*))
namespace sys{ //系统全局变量空间
    class Info{ //基本信息虚基类
    public:
        unsigned int getSize(unsigned int size)const; //获取空间大小
        void setSize(unsigned int &size,unsigned int value); //设置空间大小
        Info(unsigned int &size,unsigned int value=0);
    };
    
    class TotalInfo:virtual public Info{ //总大小基类
    private:
        unsigned int totalSize; //空间总大小
    public:
        unsigned int getTS() const;
        void resetTS();
        void addTS(unsigned int increment);
        TotalInfo(unsigned int value=0);
    };
    
    class UsedInfo:virtual public Info{  //已使用大小基类
    private:
        unsigned int usedSize; //已使用大小
    public:
        unsigned int getUS() const;
        void resetUS();
        void addUS(unsigned int increment);
        UsedInfo(unsigned int value=0);
    };
    
    template <typename BlockType> //空间管理虚基类模版，BlockType：空间种类
    class BlockManagement{
    public:
        virtual void *getBlock(unsigned int request)=0; //分配request大小的空间
        virtual void callbackBlock(BlockType *target)=0; //回收地址为target的空间
    };
}

namespace mainsys{ //主系统变量空间
    struct MemBlock{ //主系统内存块
        unsigned int totalSize,usedSize; //内存块管理空间总大小、已使用大小
        MemBlock *pre; //前一个逻辑相邻的内存块的首地址
        MemBlock *parent,*left,*right; //管理树关系：父节点首地址、左子结点首地址、右子节点首地址
    };
    
    class MemPackage: public sys::TotalInfo, public sys::UsedInfo, public sys::BlockManagement<MemBlock>{ //主系统内存包
    private:
        MemBlock *headSpace,rootTree; //管理域首地址、管理树根节点首地址
        MemBlock *divideBlock(MemBlock *target); //工具：分割首地址为target的内存块，第一块管理空间大小为usedSize，第二块管理空间大小为(target->totalSize)-(target->usedSize)
        MemBlock *mergeBlock(MemBlock *target); //工具：合并首地址为target的内存块前后相邻的空闲内存块
        MemBlock *mergeBlock(MemBlock *target,bool direction); //工具：合并首地址为target的内存块，若direction为true则向前合并，若为false则向后合并
        void insertBlock(MemBlock *block); //工具：向管理树中添加首地址为block的节点
        void deleteBlock(MemBlock *block); //工具：从管理树中删除首地址为block的节点
    public:
        MemPackage *next,**pre; //内存包的下一个节点地址、上一个节点地址
        void *getBlock(unsigned int request);
        void callBlock(MemBlock *target);
    };
    
    class MainSys: public sys::TotalInfo, public sys::UsedInfo, public sys::BlockManagement<void>{ //主管理系统类
    private:
        MemPackage *latestPackage; //索引表最后一项的地址
        MemPackage *headPackage; //索引表第一项的地址
    public:
        void addPackageNode(unsigned int packageSize=PACKAGE_SIZE); //内部接口：添加包节点（默认大小为PACKAGE_SIZE）
        void deletePackageNode(MemPackage *packageNode); //内部接口：删除包节点
        void *getBlock(unsigned int request);
        void callbackBlock(void *target);
    };
}

namespace subsys{ //辅助系统变量空间
    struct SubBlock{ //辅助系统内存块
        unsigned int totalSize; //管理空间大小
        SubBlock *next; //指向下一个内存块
    };
    class SubSys: public sys::TotalInfo, public sys::UsedInfo,public sys::BlockManagement<void>{ //辅助管理系统类
    private:
        SubBlock *bolckList[SIZE_LEVEL+1]; //散列表表头
    public:
        void *getBlock(unsigned int request);
        void callbackBlock(void *target);
    };
}

namespace sys {
    class Sys: public TotalInfo, public UsedInfo{ //系统全局类
    private:
        mainsys::MainSys mainSys; //主系统对象
        subsys::SubSys subSys; //辅助系统对象
    public:
        static Sys *present; //指向当前系统位置
        static void *allocate(unsigned int request); //外部接口：在管理系统中分配request大小的内存
        static void free(void *target); //外部接口：将地址为target的内存释放回管理系统
        static void remove(); //外部接口：删除系统
    };
}

#endif /* defined(__MemoryManagement__memsys__) */
