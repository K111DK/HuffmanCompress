#include "BasicStruct.h"
#include "utils.h"
#include "Compress.h"
int main() {
//char *a="a";
//char *b="b";
//char *c="c";
//char *d="abc";
//char *f= BiChConverse(d,1);
//    printf("%s--hex:%x\n",f,*f);
//    f= BiChConverse(f,0);
//    printf("%s--hex:%x\n",f,*f);
CompressInfo *CInfo;
HuffmanTree *HT;
    CInfo=MapConstruct(5,10,"C:\\Users\\35802\\Desktop\\HuffmanCompress\\实验08_信号处理及有限状态机.pdf");
    MapPrint(CInfo);
    int i=0;   for(i=0;i<CInfo->UnitNum;++i){
        printf("%s--%lld\n",CInfo->UnitSet[i].unit,CInfo->UnitSet[i].appearNum);
    }
//    HT=HuffmanTreeConstruct(CInfo);
    return 0;
}
//00100101--1314
//01010000--4438
//01000110--1417
//00101101--1436
//00101110--1291
//00110111--1735

//00110000--1748
//01100010--1322831745
//@--1591