#include "BasicStruct.h"
#include "utils.h"
#include "Compress.h"
int main() {
CompressInfo *Map;
HuffmanTree *HF;
    Map=MapConstruct(1,2,"bee1.mp4");
    HF= HuffmanTreeConstruct(Map);
//    HuffCodeConstruct(HF,Map);
//    CompressFileGen(Map,HF,"bee1.mp4","bee1.hfzip");
    return 0;
}
