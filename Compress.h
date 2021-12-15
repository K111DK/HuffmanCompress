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
#include "utils.h"
CompressInfo *MapConstruct(double basicUnitSize,int BranchSize,char*originPath);
HuffmanTree *HuffmanTreeConstruct(CompressInfo*CInfo);
void CompressUnitInsert(char InsertUnit[MaxUnitSize],CompressInfo*CInfo);
void HuffCodeConstruct(HuffmanTree*HTree,CompressInfo*CInfo);
void HuffmanTreePrint(HuffmanTree*HTree);
void CompressFileGen(CompressInfo*CInfo,HuffmanTree*HTree,char*originPath,char*targetPath);
char*HuffmanCode(HuffmanNode*node,int num,int branch);
CompressInfo *HeadInfoWread(FILE*fp);
void HeadInfoWrite(FILE*fp,CompressInfo*Info);
void WriteString(FILE*fp,char*string);
char*GetEle(char *input,int mode,CompressInfo*CInfo);
char*ReadString(FILE*fp,double basicUnitSize);

CompressInfo *MapConstruct(double basicUnitSize,int BranchSize,char*originPath){
    FILE *fp;
    char *p;
    printf("Map constructing 1\n");
    fp= fopen(originPath,"rb+");//打开目标文件
    if(fp==NULL){
        printf("fail to open\n");
        exit(0);
    }
    CompressNode *head;
    CompressInfo *CInfo=(CompressInfo*) malloc(sizeof (CompressInfo));

    CInfo->BasicUnitSize=basicUnitSize;
    CInfo->HuffBranch=BranchSize;
    CInfo->TotalCharNum=0;
    CInfo->UnitNum=0;
    CInfo->Extension=(char*) malloc(sizeof (char)*20);
    CInfo->name=(char *) malloc(sizeof (char )*80);
    //记录后缀名及文件名
    int i,j,z;
    i= strlen(originPath);//从文件最末往前遍历直至'.'，目的是截取后缀名
    for(j=i-1;originPath[j]!='.';--j){}
    CInfo->Extension=StringCombina(&originPath[j],NULL);//截取文件名
    for(z=j;z>=0&&originPath[z]!='\\';--z){}
    ++z;
    CInfo->name=StringCombina(&originPath[z],NULL);
    CInfo->name=StringCut(CInfo->name,j-1-z);
    //if(basicUnitSize-(double)((int)basicUnitSize)!=0)



    char *temp;
    char *temp1;
    printf("Map constructing2\n"); //构建字符记录表
    i=0;
    while(!feof(fp)){//未到文件末时
        temp= ReadString(fp,basicUnitSize);//读字符串，长度为BasicNum*2字节，保证整字节
        if(!feof(fp)) {//未到文件末，证明读到的不是尾串
            char*new;
            new = BiChConverse(temp, 1, (int) (basicUnitSize * 2));//basicUnitSize*2*8
//            temp= realloc(temp,1);
//            free(temp);
            temp=new;
        }else{//已到文件末，证明读到的是尾串，需要特殊处理
            char*new;
            char*old;
            old=temp;
            new = StringCombina(temp,NULL);
            temp = BiChConverse(new, 1, temp[(int)(basicUnitSize*2)]);
        }
        if(!feof(fp)){//对于非文末串
            temp1= StringCut(temp,(int)(basicUnitSize*8-1));//将两个基本单元二进制字符串中间切半
            CompressUnitInsert(temp1,CInfo);//记入记录表
            ++CInfo->TotalCharNum;//总单元数++
            CompressUnitInsert(temp,CInfo);//记入记录表
            ++CInfo->TotalCharNum;
//            free(temp);//释放单元
//            free(temp1);//
        }else{//对于文末串
            if(CInfo->TotalCharNum!=0){//开辟空间
                CInfo->UnitSet= realloc(CInfo->UnitSet,CInfo->UnitNum+1);
                if(!CInfo->UnitSet){
                    exit(114514);
                }
            }else{
                CInfo->UnitSet=(CompressNode*) malloc(sizeof (CompressNode));
            }
            CInfo->UnitSet[CInfo->UnitNum].unit=StringCombina(temp,NULL);
            ++CInfo->UnitNum;
            ++CInfo->TotalCharNum;
//            free(temp);
        }
    }
    printf("Map construct done!\n");
    fclose(fp);
    return CInfo;
}

void CompressUnitInsert(char *InsertUnit,CompressInfo*CInfo){//将基本符号单元插入
    int i=0;
    if(!InsertUnit){
        return;
    }
    if(CInfo->TotalCharNum==0){//表中无单元
        CInfo->UnitSet= (CompressNode *)malloc(sizeof (CompressNode));
        CInfo->UnitSet[0].unit=StringCombina(InsertUnit,NULL);
        CInfo->UnitSet->appearNum=1;
        ++CInfo->UnitNum;
        return;
    }
    else{//单元集中已存在
        CompressNode *node=CInfo->UnitSet;
        for(i=0;i<CInfo->UnitNum;++i){
            if(strcmp(node->unit,InsertUnit)==0){//若在字库中匹配到
                    ++node->appearNum;//该单元+1
//                    if(strcmp(node->unit,"01100010")==0){
//
//                    }
                return;
            }
            if(i==CInfo->UnitNum){
                break;
            }
            ++node;
        }//不存在
        CInfo->UnitSet=realloc(CInfo->UnitSet,(CInfo->UnitNum+3)*sizeof (CompressNode));
            if(!CInfo->UnitSet){
                exit(0);
            }
        CInfo->UnitSet[CInfo->UnitNum].unit=StringCombina(InsertUnit,NULL);
        CInfo->UnitSet[CInfo->UnitNum].appearNum=1;
        ++CInfo->UnitNum;
        return;
    }
}
char* ReadString(FILE*fp,double basicUnitSize){//读取basic*2单元
    char c;
    int i=0;
    char *chSet=(char*) malloc((int)(basicUnitSize*2+1)*sizeof(char));//尾部+'\0'
    *chSet='\0';
    int previousNum=0;
    c=fgetc(fp);
    while((!feof(fp))&&previousNum<(int)(basicUnitSize*2)){
        chSet[previousNum]=c;
        previousNum++;
        c=fgetc(fp);
    }//取满单元
    if(!feof(fp)){//非文件尾，加\0
        chSet[previousNum]='\0';
        return chSet;
    }else{//否
            chSet[previousNum]='\0';
            chSet= realloc(chSet,(int)(basicUnitSize*2+4)*sizeof(char));
            if(!chSet){
                exit(11451415);
            }
            chSet[(int)(basicUnitSize*2)]=previousNum;//最后一个字符的下标+1
            return chSet;
    }
}

//HuffmanTree *HuffmanTreeConstruct(CompressInfo*CInfo){
//    int minSet[CInfo->HuffBranch];//?exit
//    int nodeData[CInfo->UnitNum][2];//存放单元数据
//    HuffmanNode *node;
//    DynamicArray *Darray;//动态指针数组，存放结点指针
//    Darray= Init(CInfo->UnitNum);
//    int i=0;
//    printf("Tree constructing\n");
//    for(i=0;i<CInfo->UnitNum;++i){
//        nodeData[i][0]=i;//编号
//        nodeData[i][1]=CInfo->UnitSet[i].appearNum;//权值
//        node=(HuffmanNode*) malloc(sizeof (HuffmanNode));
//        node->num=i;
//        node->value=CInfo->UnitSet[i].appearNum;
//        AssertArray(Darray,i,node);//Darray存放HuffmanTree临时结点
//    }
//    HuffmanTree *head=(HuffmanTree*) malloc(sizeof (HuffmanTree));
//    head->branch=CInfo->HuffBranch;
//    head->Head=(HuffmanNode*) malloc(sizeof (HuffmanNode));
//    //构造
//    //int nodeData[UnitNum][2] 0编号 1权值
//    //HuffNode *node[UnitNum]
//    //sort
//    //创建新node，权值为所选node之和 编号为 ele=-1;
//    //
//    //
//    //
//    int exten=0;
//    while(nodeData[CInfo->UnitNum-1][1]!=0&&nodeData[CInfo->UnitNum-2][1]==0){
//            int j=0;
//            int flag=0;
//            int temp;
//            int start;
//            ++exten;
//            for(i=0;i<CInfo->UnitNum;++i){//将所有结点按权值从小到大排序
//                for(flag=j=i;j<CInfo->UnitNum;++j){
//                    if(nodeData[flag][1]>nodeData[j][1]){
//                        flag=j;
//                    }
//                }
//                if(flag!=i){
//                    temp=nodeData[flag][1];
//                    nodeData[flag][1]=nodeData[i][1];
//                    nodeData[i][1]=temp;
//                    temp=nodeData[flag][0];
//                    nodeData[flag][0]=nodeData[i][0];
//                    nodeData[i][0]=temp;
//                }
//            }
//        node=(HuffmanNode*) malloc(sizeof (HuffmanNode));//新开结点
//        node->Child=(HuffmanNode*) malloc(sizeof (HuffmanNode)*CInfo->HuffBranch);
//        HuffmanNode *node1;
//        node1=node->Child;
//        for(i=0;nodeData[i][1]!=0;++i){}//找到最小的非0单元位置
//        if(CInfo->UnitNum-1-i<CInfo->HuffBranch){//若所有非0单元不足n个，往回取0单元不足
//            j=i-(CInfo->HuffBranch-CInfo->UnitNum+1+i);
//        }else{
//            j=i;
//        }
//        for(start=j;start<=j+15&&node1!=NULL;start++,++node1){
//            node->value+=nodeData[start][1];
//            if(nodeData[start][1]!=0) {//将单元从动态数组中删除
//                node1= ReadArray(Darray,nodeData[start][0]);
//                DelectArray(Darray, nodeData[start][0]);
//            } else{
//                node1=NULL;
//            }
//            nodeData[start][1]=0;
//            nodeData[start][0]=0;
//        }
//        nodeData[start][0]=CInfo->UnitNum+exten;
//        node->num=CInfo->UnitNum+exten;
//        nodeData[start][1]=node->value;
//        AssertArray(Darray,CInfo->UnitNum+exten,node);//合成新结点并存入
//    }
//    head->Head= ReadArray(Darray,nodeData[CInfo->UnitNum-1][0]);
//    printf("Tree constructing done!\n");
//    return head;
//}

//char*HuffmanCode(HuffmanNode*node,int num,int branch){
//    int i=0;
//    char *a;
//    char*b;
//    for(i=0;i<branch;++i){
//        if((node->Child+i)==NULL){
//            continue;
//        }
//        else{
//            if((node->Child+i)->num==num){
//                a=(char *) malloc(sizeof (char ));
//                *a=i+48;
//                char end='\0';
//                return StringCombina(a,&end);
//            } else if((b=HuffmanCode(node->Child+i,num,branch))!=NULL){
//                a=(char *) malloc(sizeof (char ));
//                *a=i+48;
//                return StringCombina(a,b);
//            }else{
//                continue;
//            }
//        }
//    }
//    return NULL;
//}

//void CompressFileGen(CompressInfo*CInfo,HuffmanTree*HTree,char*originPath,char*targetPath){
//    FILE *fpin,*fpout;
//    int i=0;
//    fpin= fopen(originPath,"rb+");
//    char*TargetPath= StringCombina(targetPath,StringCombina(CInfo->name,CInfo->Extension));
//    fpout= fopen(TargetPath,"wb+");
//    HeadInfoWrite(fpout,CInfo);
//    double basicUnitSize=CInfo->BasicUnitSize;
//    char temp[(int)basicUnitSize*2];
//    char temp1[(int)basicUnitSize+1];
//    char temp2[(int)basicUnitSize+1];
//    char*p=temp;
//    while(!feof(fpin)){
//        strcpy(temp, ReadString(fpin,basicUnitSize));
//        if(!feof(fpin)){
//            strcpy(temp1,temp);
//            for(i=(int)basicUnitSize+1;i<=2*basicUnitSize-1;++i){
//                temp2[i-(int)basicUnitSize-1]=temp[i];
//            }
//            WriteString(fpout,BiChar(DemicalToBinary(GetEle(temp1,1,CInfo),CInfo->HuffBranch)));
//            WriteString(fpout,BiChar(DemicalToBinary(GetEle(temp2,1,CInfo),CInfo->HuffBranch)));
//        }else{
//            WriteString(fpout,BiChar(DemicalToBinary(GetEle(temp,1,CInfo),CInfo->HuffBranch)));
//        }
//    }
//    fclose(fpin);
//    fclose(fpout);
//}

//
//CompressInfo *HeadInfoRead(FILE*fp){
//    CompressInfo *Info;
//    fread(Info,sizeof (CompressInfo),1,fp);
//    Info->UnitSet=(CompressNode*) malloc(sizeof (CompressNode)*Info->UnitNum);
//    CompressNode *node=Info->UnitSet;
//    int size=Info->UnitNum;
//    int i=0;
//    for(i=0;i<size;++i){
//        fread(node,sizeof(CompressNode),1,fp);
//        node++;
//    }
//    return Info;
//}
//void HeadInfoWrite(FILE*fp,CompressInfo*Info){
//    fwrite(Info,sizeof (CompressInfo),1,fp);
//    CompressNode *node=Info->UnitSet;
//    int size=Info->UnitNum;
//    int i=0;
//    for(i=0;i<size;++i){
//        fwrite(node,sizeof(CompressNode),1,fp);
//        node++;
//    }
//}
////
//
//void WriteString(FILE*fp,char*string){
//    int len= strlen(string);
//    if(len<=0){
//        exit(0);
//    }
//    fwrite(string,sizeof (char ),len,fp);
//}
//
//char*GetEle(char*input,int mode,CompressInfo*CInfo){//1为压缩映射 2为解压映射
//        int size=CInfo->UnitNum-1;
//        CompressNode *node=CInfo->UnitSet;
//        int i;
//        for(i=0;i<=size;++i){
//            if(mode==1){
//                if(strcmp(input,node->unit)==0){
//                    return node->HuffCode;
//            }
//            }else{
//                if(strcmp(input,node->HuffCode)==0){
//                    return node->unit;
//                }
//            }
//            node++;
//        }
//}
void MapPrint(CompressInfo*CInfo){
    printf("total char num:%d\n",CInfo->TotalCharNum);
    printf("total unitset num:%d\n",CInfo->UnitNum);
    printf("file name %s\n",CInfo->name);
    printf("file extension %s\n",CInfo->Extension);
}
#endif //HUFFMANCOMPRESS_COMPRESS_H
//change
//取2n个字节
//翻译为二进制编码的string形式
//切半存储
//编号
//构建Huffman树
//解压
//构建Huffman树
//读文件((int)log2n)+1个字节，转化为二进制串
//每次读((int)log2n)+1位数，利用huffman树进行寻找,构建原二进制串
//当原二进制串大于等于4字节时，存储并
//Stringcombi
//StringCut