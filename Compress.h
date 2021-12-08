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

#endif //HUFFMANCOMPRESS_COMPRESS_H