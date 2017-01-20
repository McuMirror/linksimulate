#include "ldpcgenerator.h"
#include "waveform.h"
#include <vector>
#include <QFile>
#include <QDataStream>
#include <stdio.h>

#define LDPC_K			7154
#define LDPC_N			8176
#define BLK_LENGTH      8160        // bit
#define INFO_LENGTH     7136        // bit

LDPCGenerator::LDPCGenerator()
{

}

Waveform LDPCGenerator::outputWithInputWfm(const Waveform &inputWfm,int a)
{
    FILE *fp = fopen("glen.txt", "rb");
    if(fp == NULL)
    {
        printf("can not open the glen file!\n");
        return inputWfm;
    }
    int glen;
    fread((void *)&glen, sizeof(int), 1, fp);
    fclose(fp);

    unsigned char *g = (unsigned char *)calloc(glen, sizeof(unsigned char));
    if (NULL == g)
    {
        printf("can not alloc memory for g!\n");
        return inputWfm;
    }
    fp = fopen("g.txt", "rb");
    if(fp == NULL)
    {
        printf("can not open the file!\n");
        return inputWfm;
    }
    fread(g, sizeof(unsigned char), glen, fp);
    fclose(fp);

    int blk_num = inputWfm.length() % LDPC_K == 0 ? inputWfm.length() / LDPC_K
                                                  : inputWfm.length() / LDPC_K + 1;

    std::vector<unsigned char> info_bit(LDPC_K * blk_num, 0);
    for (size_t i = 0, infoIndex = 0; i < info_bit.size() && infoIndex < inputWfm.length(); i++) {
        if (i % LDPC_K < 18)
            continue;
        else
            info_bit[i] = (unsigned char)inputWfm.data()[infoIndex++];
    }

    std::vector<unsigned char> encoded_bit(LDPC_N * blk_num, 0);
    for (size_t i = 0; i < blk_num; i++) {
        for (size_t j = 0; j < LDPC_K; j++) {
            encoded_bit[i * LDPC_N + j] = info_bit[i * LDPC_K + j];
        }
    }

    for (size_t blkIndex = 0; blkIndex < blk_num; blkIndex++) {
        for (size_t i = 0; i < LDPC_N - LDPC_K; i++) {
            encoded_bit[blkIndex * LDPC_N + LDPC_K + i] = 0;
            for (size_t ii = 0; ii < LDPC_K; ii++) {
                encoded_bit[blkIndex * LDPC_N + LDPC_K + i] ^= (info_bit[blkIndex * LDPC_K + ii] * g[(i) * LDPC_K + ii]);
            }
        }
    }

    for (size_t blkIndex = 0; blkIndex < blk_num; blkIndex++) {
        for(size_t i = 0; i < LDPC_N - 18; i++) {
            encoded_bit[blkIndex * LDPC_N + i] = encoded_bit[blkIndex * LDPC_N + i + 18];
        }

        encoded_bit[blkIndex * LDPC_N + 8158] = 0;
        encoded_bit[blkIndex * LDPC_N + 8159] = 0;
    }

    std::vector<float> data(encoded_bit.size(), 0.f);
    for (size_t i = 0; i < encoded_bit.size(); i++) {
        data[i] = encoded_bit[i];
    }

    return Waveform("", data);
}
