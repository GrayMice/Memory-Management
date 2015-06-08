#include "memsys.h"
#include <iostream>
using namespace sys;

unsigned int Info::getSize(unsigned int size)const{return size;}
void Info::setSize(unsigned int &size,unsigned int value){size=value;}
Info::Info(unsigned int &size,unsigned int value){setSize(size,value);}

unsigned int TotalInfo::getTS()const{return getSize(totalSize);}	//获得总管理空间大小
void TotalInfo::resetTS(){setSize(totalSize,0);}
void TotalInfo::addTS(unsigned int increment){setSize(totalSize,totalSize+increment);}
TotalInfo::TotalInfo(unsigned int value):Info(totalSize,value){}	//value是总管理空间的值，默认是0。 usedSize告诉程序要赋值的值是哪一个

unsigned int UsedInfo::getUS()const{return getSize(usedSize);}	//获得已使用空间大小
void UsedInfo::resetUS(){setSize(usedSize,0);}
void UsedInfo::addUS(unsigned int increment){setSize(usedSize,usedSize+increment);}
UsedInfo::UsedInfo(unsigned int value):Info(usedSize,value){}	//value是已使用空间的值，默认是0。 usedSize告诉程序要赋值的值是哪一个

void* Sys::allocate(unsigned int request){
    void *space=nullptr;
    if(present==nullptr){
        
    }
    return space;
}