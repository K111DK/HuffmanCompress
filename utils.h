//
// Created by 35802 on 2021/12/9.
//
#ifndef BASICSTRUCT_H_UTILS_H
#define BASICSTRUCT_H_UTILS_H
#include "BasicStruct.h"
char*DemicalToBinary(char*demical,int branch);
char*BiChar(char*Binary);
char*BinaryAdd(char*add1,char *add2);


char*DemicalToBinary(char *demical,int Branch){
    int branch=log2(Branch)+1;
    int unitSize=branch+1;
    char *deString= StringCombina(NULL,demical);
    int size= strlen(demical)-2;
    int u;
    char *temp;
    char*out=(char*) malloc(((strlen(demical)-1)*branch+1)*sizeof(char ));
    char*unit=(char*)malloc(unitSize*sizeof(char));
    int i=0;
    for(i=0;i<unitSize;++i){
        unit[i]='0';
        if(i==unitSize-2){
            unit[i]='1';
        }
        if(i==unitSize-1){
            unit[i]='\0';
        }
    }
    int j=0;
    for(j=0;j< strlen(demical)-1;++j){
            u=demical[j]-48;
            int z=0;
            for(z=0;z<u;++z){
                if(z==0){
                    temp= unit;
                }
                temp=BinaryAdd(temp, unit);
            }
            for(z=0;z<unitSize;++z){
                out[j*branch+z]=temp[z];
            }
    }
    return out;
}


char*BiChar(char*Binary){
int outLen= strlen(Binary)/8+1;
    char*outChar=(char *) malloc(sizeof (char)*outLen);
    outChar[outLen-1]='\0';
    int j=0;
    int i=j=0;
    char code=0x0;
    char unit=0x01;
    for(i=0;i<outLen-1;++i){
        code=0x0;
        unit=0x01;
        for(j=(i+1)*8-1;j>=i*8;--j){
            code=code|(Binary[j]==0?code:unit);
            unit=unit<<1;
        }
        outChar[i]=code;
    }
    return outChar;
}

char*BinaryAdd(char*add1,char *add2){//二进制字符串加法
    int l1=0,l2=0;
    l1= strlen(add1);
    l2= strlen(add2);
    int cin=0;
    int a1=0,a2=0;
    char *out=(char*) malloc(sizeof(char)*l1);
    out[l1-1]='\0';
    if(l1==l2){
        int i=0;
        for(i= l1-1-1;i>=0;--i){
            a1=add1[i]-48;
            a2=add2[i]-48;
            switch (a1+a2+cin) {
                case 0: out[i]='0';
                        cin=0;break;
                case 1:out[i]='1';
                        cin=0;break;
                case 2:out[i]='0';
                        cin=1;break;
                case 3:out[i]='1';
                        cin=1;break;
            }
        }
        return out;
    }else{
        exit(0);
    }
}


char*BiChConverse(char*inputS,int mode){//字符和字符对应的二进制串转换，1为字符转二进制串，0为反过来
    char ch;
    char *out1="\0";
    char *out="\0";
    char *BiCode="\0";
    char a[2]="1\0";
    a[0]=0x00;
    char unit=0x01;
    if(!inputS){
        return NULL;
    }
    if(mode){
        int i=0;
        while(inputS[i]!='\0'){
            unit=0x01;
            out1="\0";
            while(unit!=0x00){
                if((inputS[i]&unit)==unit){//证明该位为1
                    out1=StringCombina("1",out1);
                }else{
                    out1=StringCombina("0",out1);
                }
                unit=unit<<1;
            }
            out=StringCombina(out,out1);
            ++i;
        }
        return out;
    } else{
        int len= strlen(inputS);
        if(len%8!=0){
            return NULL;
        }else{
            BiCode=StringCombina(BiCode,inputS);
            while(*BiCode!='\0'){
                char* a1;
                int i=0;
                a[0]=0x00;
                unit=0x01;
                a1=StringCut(BiCode,7);
                for(i=7;i>=0;--i){
                    if(a1[i]=='1'){
                        a[0]=a[0]|unit;
                    }
                    unit=unit<<1;
                }
                out=StringCombina(out,a);
            }
            return out;
        }
    }
}
#endif //BASICSTRUCT_H_UTILS_H
