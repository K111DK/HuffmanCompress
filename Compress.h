//
// Created by 35802 on 2021/12/8.
//
//
//
// TO DO
// Step1:读取文件,按n%1=0?分情况进行读取操作
//      func1(FILE*fp,n(基本符号单元大小),Map*m(压缩符号映射表))
//      对所有基本单元进行统计,未满单元将在尾部补充字符
//
//      是否n%1==0
//      是:直接按n字节读取为1个单元
//      否:一次读取2n个字节,以前n-1字节+(第n+1字节与0xF0按位与+0X0F)为1个单元
//         (第n+1字节与0x0F按位与+0xF0)+剩下的n-1字节为另一个单元
//      在Map中查找，若原Map中存在，将其出现次数+1；
//                 否则在原Map中加入该新元素，其出现次数++；
//                 文件字节数+=2n
//      若读取至EOF时未满
//                 文件字节数+=c(剩余字节数,未满2n）
//                 直接将其作为1个单元（尾单元）加入MAP中
//
// Step2:构建Huffman树(构建n叉Huffman)
//      Huffman*func(Map*m)
//      HuffmanNode{
//      HufmanNode*Child[n]
//      int Element;//-1为补充字节, 否则为单元在Map中的编号
//      int value;//权值 补充字节权值为-1
//      }
//      HuffmanTree{
//      HuffmanNode*node;//头结点
//      int d;//深度
//      int n;//叉数
//      }
//
// Step3:根据Huffman树得到具体编码(以string形式)存储至Map中
//      func2(Map*m)
//      构建Huffman树并存储,将有效结点的Huffman编码存储
//
//
// Step4:根据符号串及压缩映射表，读原文件并构建压缩文件，Map信息存储至压缩头部.
//      func3(Map*m,File*fp1,FILE*fp2)
//      读原文件构建新文件
//
//
#ifndef HUFFMANCOMPRESS_COMPRESS_H
#define HUFFMANCOMPRESS_COMPRESS_H
#include "BasicStruct.h"
CompressInfo *MapConstruct(int basicUnitSize,int BranchSize,char*originPath);
HuffmanTree *HuffmanTreeConstruct(CompressInfo*CInfo);
void CompressUnitInsert(char InsertUnit[MaxUnitSize],CompressInfo*CInfo);
void HuffCodeConstruct(HuffmanTree*HTree,CompressInfo*CInfo);
void HuffmanTreePrint(HuffmanTree*HTree);
void CompressFileGen(CompressInfo*CInfo,HuffmanTree*HTree,char*originPath,char*targetPath);


void CompressUnitInsert(char InsertUnit[MaxUnitSize],CompressInfo*CInfo){//将基本符号单元插入
    if(CInfo->TotalCharNum==1){
        CInfo->UnitSet= (CompressNode *)malloc(sizeof (CompressNode));
        CInfo->UnitSet->appearNum=1;
        ++CInfo->UnitNum;
        strcpy(CInfo->UnitSet->unit,InsertUnit);
        return;
    }
    else{//单元集中已存在
        for(i=0;i<=CInfo->TotalCharNum-2;++i){
            if(strcmp(InsertUnit,CInfo->UnitSet[i])==0){
                    CompressNode *node;
                    node=CInfo->UnitSet[i];
                    ++node->appearNum;
                return;
            }
        }//不存在
        CInfo->UnitSet=realloc(CInfo->UnitSet,CInfo->TotalCharNum);
        strcpy(InsertUnit,CInfo->UnitSet[CInfo->TotalCharNum-1].unit);
        CInfo->UnitSet[CInfo->TotalCharNum-1].appearNum=1;
        ++CInfo->UnitNum;
        return;
    }
}


CompressInfo *MapConstruct(int basicUnitSize,int BranchSize,char*originPath){
    FILE *fp;
    char *p;
    fp= fopen(originPath,"rb+");
    CompressInfo *CInfo=(CompressInfo*) malloc(sizeof (CompressInfo));
    CInfo->BasicUnitSize=basicUnitSize;
    CInfo->HuffBranch=BranchSize;
    CInfo->TotalCharNum=0;

    //记录后缀名
    int i=j=z=0;
    for(p=originPath;*p!='\0';++p,++i){}
    for(j=i;originPath[i]!='.';--j){}
    for(z=j+1;z!=i;z++){
        strcat(CInfo->Extension,originPath[z]);
    }
    //开始文件记录
    char ch;
    char [basicUnitSize*2]chSet;
    int previousNum=0;
    while((ch=fgetc(fp)&&feof(fp)==0)){
        strcat(chSet,&ch);
        previousNum++;
        ++CInfo->TotalCharNum;
        if(previousNum==basicUnitSize*2-1){
            char unit1[MaxUnitSize];
            char unit2[MapUnitSize];
            char temp;
            if(basicUnitSize%1!=0){
            for(i=0;i<=basicUnitSize-2;i++){//当基本单元非整字节时(n.5字节）
                strcat(unit1,&chSet[i]);//(将前n个字节存入
            }
                temp=0xF0;//对于第n+1个字节，取前半段(4bit)后半段记0,存入
                strcat(unit1,&(chSet[i]&temp));
                temp=0x0F;//将后半段保留，前半段记0,存入第二个单元
                strcat(unit2,&(chSet[i]&temp));
                for(++i;i<basicUnitSize*2;++i){//将第二个单元的剩下n个字符存入
                    strcat(unit2,&chSet[i]);
                }
            }else{//为整字节
                for (i=0;i<basicUnitSize:++i){//存入前半
                    strcat(unit1,&chSet[i]);
                }
                for(;i<basicUnitSize*2;++i){//存入后半段
                    strcat(unit2,&chSet[i]);
                }
            }
            //记录单元
            CompressUnitInsert(unit1,CInfo);
            CompressUnitInsert(unit2,CInfo);
            for(i=0;i<basicUnitSize*2-1;++i){//原文件字符流截取清0
                chSet[i]='\0';
            }
                previousNum=0;
        }
    }
    if(previousNum!=0){//证明文件尾不满一个基本单元
        CompressUnitInsert(chSet,CInfo);//直接插入作为1个单元
    }
    fclose(fp);
}

HuffmanTree *HuffmanTreeConstruct(CompressInfo*CInfo){
    int BinarySize= log2(CInfo->HuffBranch)+1;
    int minSet[CInfo->HuffBranch];//?
    int nodeData[CInfo->UnitNum][2];
    HuffmanNode *node;
    DynamicArray *Darray;
    Darray= Init(CInfo->UnitNum);
    int i=0;
    for(i=0;i<CInfo->UnitNum;++i){
        nodeData[i][0]=i;//编号
        nodeData[i][1]=CInfo->UnitSet[i].appearNum;//权值
        node=(HuffmanNode*) malloc(sizeof (HuffmanNode));
        node->num=i;
        node->value=CInfo->UnitSet[i].appearNum;
        AssertArray(Darray,i,node);
    }
    HuffmanTree *head=(HuffmanTree*) malloc(sizeof (HuffmanTree));
    head->branch=CInfo->HuffBranch;
    head->Head=(HuffmanNode*) malloc(sizeof (HuffmanNode));
    //构造
    //int nodeData[UnitNum][2] 0编号 1权值
    //HuffNode *node[UnitNum]
    //sort
    //创建新node，权值为所选node之和 编号为 ele=-1;
    //
    //
    //
    int exten=0;
    while(nodeData[CInfo->UnitNum-1][1]!=0&&nodeData[CInfo->UnitNum-2][1]==0){
            int j=flag=0;
            int temp;
            int start;
            ++exten;
            for(i=0;i<CInfo->UnitNum;++i){
                for(flag=j=i;j<CInfo->UnitNum;++j){
                    if(nodeData[flag][1]>nodeData[j][1]){
                        flag=j;
                    }
                }
                if(flag!=i){
                    temp=nodeData[flag][1];
                    nodeData[flag][1]=nodeData[i][1];
                    nodeData[i][1]=temp;
                    temp=nodeData[flag][0];
                    nodeData[flag][0]=nodeData[i][0];
                    nodeData[i][0]=temp;
                }
            }
        node=(HuffmanNode*) malloc(sizeof (HuffmanNode));
        for(i=0;nodeData[i][1]!=0;++i){}
        if(CInfo->UnitNum-1-i<CInfo->HuffBranch){
            j=i-(CInfo->HuffBranch-CInfo->UnitNum+1+i);
        }else{
            j=i;
        }
        int start;
        for(start=j;start<=j+15;start++){
            node->value+=nodeData[start][1];
            nodeData[start][1]=0;
            nodeData[start][0]=0;
            DelectArray(Darray,nodeData[start][0]);
        }
        nodeData[start][0]=CInfo->UnitNum+exten;
        node->num=CInfo->UnitNum+exten;
        nodeData[start][1]=node->value;
        AssertArray(Darray,CInfo->UnitNum+exten;node);
    }
    head->Head= ReadArray(Darray,nodeData[CInfo->UnitNum-1][0]);
    return head;
}





#endif //HUFFMANCOMPRESS_COMPRESS_H