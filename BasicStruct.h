//
// Created by 35802 on 2021/12/8.
//
//TO DO
//结构体:压缩信息表
//{
//
// 文件后缀名
//
// 压缩信息
// 基本符号单元大小(以0.5字节（4bit)为粒度）
// Huffman树叉数
// 文件总字节数
//
//
// 压缩字符映射表
// 1.原字符
//   字符对应的Huffman编码(二进制串)的字符形式
//   出现次数
// ..........
//
//
//
//
// }
#ifndef HUFFMANCOMPRESS_BASICSTRUCT_H
#define HUFFMANCOMPRESS_BASICSTRUCT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string.h>
#define MaxUnitSize 40





typedef struct CompressNode{
    char *unit;//单元翻译过来的二进制码字符串
    char *HuffCode;//huffman编码的二进制形式
    int appearNum;//该单元出现次数
}CompressNode;

typedef struct CompressInfo{
    double BasicUnitSize;//基本单元的大小
    int HuffBranch;//(huffman树叉数)
    int TotalCharNum;//文件大小(包含多少基本单元)
    int UnitNum;//单元种类数
    char *name;//文件名
    char  *Extension;//后缀名
    CompressNode*UnitSet;//符号单元
}CompressInfo;



typedef struct HuffmanNode{
    int num;
    int value;
    int code;
    struct HuffmanNode*Child;
}HuffmanNode;
typedef struct HuffmanTree{
    HuffmanNode *Head;
    int deep;
    int branch;
}HuffmanTree;




typedef struct DynamicArray{
    HuffmanNode**Array;
    int PreviousSize;
}DynamicArray;
DynamicArray* Init(int initSize){
    DynamicArray*Darray=(DynamicArray*) malloc(sizeof (DynamicArray));
    Darray->PreviousSize=initSize;
    Darray->Array=(HuffmanNode**) malloc(sizeof(HuffmanNode*)*initSize);
}

void AssertArray(DynamicArray*Darray,int num,HuffmanNode*node){
    if(num>=Darray->PreviousSize){
        Darray->PreviousSize=num+1;
        Darray->Array= realloc(Darray->Array,(num+1)*sizeof (HuffmanNode*));
    }
    Darray->Array[num]=node;
}
HuffmanNode* ReadArray(DynamicArray*Darray,int num){
    if(num>=Darray->PreviousSize){
        return NULL;
    }else{
        return Darray->Array[num];
    }
}
void DelectArray(DynamicArray*Darray,int num){
    if(num>=Darray->PreviousSize){
        return;
    }else{
        Darray->Array[num]=NULL;
    }
}
#endif //HUFFMANCOMPRESS_BASICSTRUCT_H