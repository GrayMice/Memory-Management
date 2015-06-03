#include "memsys.h"
#include <iostream>
using namespace sys;

unsigned int Info::getSize(unsigned int size)const{return size;}
void Info::setSize(unsigned int &size,unsigned int value){size=value;}
Info::Info(unsigned int &size,unsigned int value){setSize(size,value);}

unsigned int TotalInfo::getTS()const{return getSize(totalSize);}
void TotalInfo::resetTS(){setSize(totalSize,0);}
void TotalInfo::addTS(unsigned int increment){setSize(totalSize,totalSize+increment);}
TotalInfo::TotalInfo(unsigned int value):Info(totalSize,value){}

unsigned int UsedInfo::getUS()const{return getSize(usedSize);}
void UsedInfo::resetUS(){setSize(usedSize,0);}
void UsedInfo::addUS(unsigned int increment){setSize(usedSize,usedSize+increment);}
UsedInfo::UsedInfo(unsigned int value):Info(usedSize,value){}

void* Sys::allocate(unsigned int request){
    void *space=NULL;
    if(present==NULL){
        
    }
    return space;
}