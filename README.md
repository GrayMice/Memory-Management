# 单线程动态内存池管理系统需求&设计说明
快速高效的内存分配解决方案，《C++程序设计实践》结课设计，designed by BISTU.

## 概述

### 编写目的
为“单线程动态内存池管理系统”的开发提供：
* 软件总体要求，作为用户和软件开发人员之间朴素了解的基础；
* 初步设计、性能分析和对用户的影响，作为软件人员进行概要设计和编码的基础；
* 软件综合测试的依据。

### 参考资料
* 《内存池设计与实现》http://blog.csdn.net/shawngucas/article/details/6574863
* 《平衡二叉树》百度百科“平衡二叉树”
* 《浅谈C中的malloc和free》http://bbs.bccn.net/thread-82212-1-1.html
* 《浅析malloc()的几种实现方式》http://lionwq.spaces.eepw.com.cn/articles/article/item/18555#

### 术语和缩写词
* 内存管理：软件运行时对计算机内存资源的分配和使用的技术。
* 单线程：内存管理系统仅为唯一的一个线程提供内存管理。
* 动态内存管理：内存管理系统在初始化时所创建的内存池的大小固定。
* 内存池：对内存管理系统从操作系统中获取的所有内存空间进行管理的管理器。
* 内存包：管理系统一次从操作系统中获取的内存空间。
* 内存块：内存管理系统对内存空间进行管理的最小单元。
* 空闲状态：若某个内存块所管理的内存空间没有被分配使用，则此内存块处于空闲状态。
* 占用状态：若某个内存块所管理的内存空间存在被分配使用的部分，则此内存块处于占用状态。
* 空闲内存管理树：管理处于空闲状态的内存块的平衡二叉树。
* 空闲内存块搜索：在空闲内存管理树中对需求空间进行查找，找到最小的不小于需求空间的内存块的过程。
* “地址-内存块”索引树：保存被管理的内存地址到内存块信息地址的映射的二叉搜索树。
* “地址-内存块”搜索：在“地址-内存块”索引树中由内存地址信息查找相应的内存块地址的过程。
* 内存块逻辑相邻：若两个内存块所管理的内存空间在逻辑地址上相邻，则称这两个内存块逻辑相邻。
* 内存回收：由被分配的内存地址，经“地址-内存块”搜索得到内存块信息，并进行内存块合并，最后将内存块添加至空闲内存管理树的过程。
* 内存块合并：将两个逻辑相邻的空闲内存块合并的过程。
* 内存块分割：将某个处于占用状态的内存块按一定规则分割为两个逻辑相邻的空闲内存块与占用内存块的过程。

## 软件总体概要
### 软件总体说明
### 软件开发目的
为某单一线程的软件提供快速高效的内存管理，提升软件运行效率，减少内存的浪费与零碎化。在单线程软件的编写过程中，用户可通过内存申请函数从内存管理系统中获取所需要的内存空间；当软件的内存需求结束的时候，用户可通过手工或自动的方式将获取到的内存释放回内存管理系统中。籍以此方式，内存的申请与释放由进程与操作系统之间交互改为进程与内存管理系统交互，减少进程与系统内存栈的交互次数，提升软件运行效率。

### 适用范围
单线程软件。

### 主要功能要求
* 内存池初始化
* 内存分配
* 内存回收
* 内存状态监控

### 现有系统的方法和处理过程
### 待开发系统的方法和处理过程
#### 改进
#### 影响
### 假设和约束

## 详细需求&设计
### 性能需求
#### 精度
#### 时间特性
#### 灵活性
### 结构设计
#### 内存池
#### 内存包
#### 内存块
#### 内存结构“继承派生”图示
#### 空闲内存管理树
#### “地址-内存块”索引树
### 功能设计
#### 内存池初始化
#### 内存分配
#### 内存回收
#### 内存状态监控
### 故障处理

## 环境
### 设备环境
### 支持软件环境
### 接口
