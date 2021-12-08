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
// Step2:构建Huffman树(构建n叉Huffman)
// Step3:根据Huffman树得到具体编码(以string形式)存储至Map中
//      func2(Map*m)
//      构建Huffman树并存储,将有效结点的Huffman编码存储
//
// Step4:根据符号串及压缩映射表，读原文件并构建压缩文件，Map信息存储至压缩头部.
//      func3(Map*m,File*fp1,FILE*fp2)
//      读原文件构建新文件
#ifndef HUFFMANCOMPRESS_COMPRESS_H
#define HUFFMANCOMPRESS_COMPRESS_H

#endif //HUFFMANCOMPRESS_COMPRESS_H