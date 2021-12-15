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
    CInfo=MapConstruct(1,10,"C:\\Users\\35802\\Desktop\\HuffmanCompress\\实验08_信号处理及有限状态机.pdf");

    int i=0;   for(i=0;i<CInfo->UnitNum;++i){
        printf("%s--%lld\n",CInfo->UnitSet[i].unit,CInfo->UnitSet[i].appearNum);
    }
    MapPrint(CInfo);
    HT=HuffmanTreeConstruct(CInfo);
    HuffNodePrint(HT->Head,CInfo,0);
    return 0;
}
