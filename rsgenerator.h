#ifndef RSGENERATOR_H
#define RSGENERATOR_H
#include "waveform.h"
#include "encoder.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <vector>
#include <QDebug>

class RSGenerator : public Encoder
{
public:
    RSGenerator(int i, int nroot, int dataLength);                    //使用GNU代码中规定的参数，参数nroot为8或者16，分别对应(255, 239)和(255, 223)
//    RSGenerator2(int a);
    Waveform outputWithInputWfm(const Waveform &originWfm);

private:
    unsigned int bitPerSmb;                     //每符号的bit数m
    unsigned int numberOfElm;                   //每符号的元素数量n
    unsigned char *alpha_to;                    //GF元素（二进制 指数形式）
    unsigned char *index_of;                    //GF元素（转换为十进制）
//    std::vector<unsigned char> genpoly;       //生成多项式
    unsigned int gfpoly;
    unsigned char *genpoly;
    unsigned int nroots;                        //Number of generator roots = number of parity symbols
    unsigned char fcr;                          //First consecutive root, index form
    unsigned char prim;                         //Primitive element, index form
    unsigned char iprim;                        //prim-th root of 1, index form
    int *modnn_table;                           //modn lookup table, 512 entries
    int i;                                      //i为交织深度
    unsigned char Taltab[256], Tal1tab[256];    //Taltab----常规基转换成对偶基
                                                //Tal1tab----对偶基转换成常规基
    unsigned char tal[8] = { 0x8d, 0xef, 0xec, 0x86, 0xfa, 0x99, 0xaf, 0x7b };


    unsigned int modn(unsigned int x);        //进行模255运算
    void initRS(unsigned int gfpoly, unsigned int fcr, unsigned int prime);  //生成alpha_to以及index_of
    //对成员变量进行初始化
//    std::vector<unsigned char>  generateGF(unsigned char genpoly[]);    //生成GF域元素（删掉了，功能放到initRS中）
//    std::vector<unsigned char>  generateIndex(unsigned int n);
    void encodeRS(unsigned char *data, unsigned char *bb);                            //进行RS编码
    void genCCSDS();
};

#endif // RSGENERATOR2_H
