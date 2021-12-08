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
#define MaxUnitSize 40
typedef struct CompressNode{
    char [MaxUnitSize]unit;
    char [MaxUnitSize*8]BinaryCode;
    int appearNum;
}CompressNode;
typedef

#endif //HUFFMANCOMPRESS_BASICSTRUCT_H