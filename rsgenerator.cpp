#include "rsgenerator.h"

RSGenerator::RSGenerator (int i, int E, int dataLength)
{
    //根据GNU代码进行初始化
    this->i = i;//交织深度
    bitPerSmb = 8;
    numberOfElm = (1 << bitPerSmb) - 1;//255
    gfpoly = 0x187;
    fcr = 112;
    //prim = 11;
    nroots = E * 2;//校验符号数 nroot = 16    (255, 223)方式进行编码
                 //使用(255, 239)方式编码的时候把这里换成8？
    if (dataLength != numberOfElm - nroots)
    {
        printf("Error intput -- datalength");
    }
//    qDebug() << numberOfElm;
//    qDebug() << nroots;
}

Waveform RSGenerator::outputWithInputWfm(const Waveform &originWfm)
{
    //转换成unsigned char型的originWfm.data()，8合1的data，223或239个data，校验位
//    unsigned char *dataInput, *dataProccessed, *dataEncode,*primity;
    unsigned char *dataEncode = (unsigned char *)malloc(sizeof(unsigned char) * (numberOfElm - nroots));
    unsigned char *primity = (unsigned char *)malloc(sizeof(unsigned char) * nroots);
    //转换成常规基
    unsigned char *cdataEncode = (unsigned char *)malloc(sizeof(unsigned char) * (numberOfElm - nroots));
    unsigned char *cprimity = (unsigned char *)malloc(sizeof(unsigned char) * nroots);
    std::vector<unsigned char> dataInput;
    std::vector<unsigned char> dataProccessed;
    std::vector<unsigned char> dataOutput;//用于存放编码的输出，将所有的dataEncode和primity存入（255*blk_num个）
    std::vector<unsigned char> outputDataInBit;//用于存放将每字节dataOutput拆分成8字节的容器
    std::vector<float> outputDataInFloat;//用于存放将unsigned char类型的输出转换为float

    unsigned int byte_num = originWfm.length() % 8 == 0 ? originWfm.length() / 8 : originWfm.length() / 8 + 1;
//    int blk_num = byte_num % (numberOfElm - nroots) == 0 ? byte_num / (numberOfElm - nroots)
//                                                         : byte_num / (numberOfElm - nroots) + 1;

    //blk_num为输入数据能够形成的码块数量
    unsigned int blk_num;
//    if (byte_num % (numberOfElm - nroots) == 0)
//    {
//        blk_num = byte_num / (numberOfElm - nroots);
//    }
//    else
//    {
//        blk_num = byte_num / (numberOfElm - nroots) + 1;
//    }
    unsigned int paddingNum = 0;//填充的符号数（byte）
    while (byte_num % (numberOfElm - nroots - paddingNum) != 0)
    {
        paddingNum++;
    }
//    qDebug() << "padding number:" << paddingNum;
    blk_num = byte_num / (numberOfElm - nroots - paddingNum);//实际每个符号的有效数据字节数

//    dataInput = (unsigned char *)malloc(sizeof(unsigned char) * originWfm.length());//每byte装的是0或1（二进制）
//    dataProccessed = (unsigned char *)malloc(sizeof(unsigned char) * byte_num);//每byte装的是0-255
//    dataEncode = (unsigned char *)malloc(sizeof(unsigned char) * (numberOfElm - nroots));//用于进行编码、放到encodeRs中的，有223或239位
//    primity = (unsigned char *)malloc(sizeof(unsigned char) * nroots);//用于存放校验位

//    if (dataInput == NULL || dataProccessed == NULL || dataEncode == NULL || primity == NULL)
//    {
//        printf("Cannot allocate the memory");
//        exit(0);
//    }

    for (unsigned int i = 0; i < originWfm.length(); i++)     //将originWfm中的0,1的float型数据转化为unsigned char型
        dataInput.push_back((unsigned char)originWfm.data()[i]);

    for (unsigned int i = 0; i < byte_num; i++)//装bit的顺序有点疑问
    {
        unsigned char sum = 0;
        for (int j = 0; j < 8; j++)
        {
            if (i * 8 + j > originWfm.length() - 1)
                break;
            else
                sum += (dataInput[i * 8 + j] << (7 - j));//将在前面的bit装到byte的高位
        }
        //dataProccessed[i] = sum;
        dataProccessed.push_back(sum);
    }

//    free(dataInput);

    //进行初始化，准备开始编码

    initRS(0x187, 112, 11);
    genCCSDS();

//    for (int abc = 0; abc < 256; abc++)
//    {
//        if (abc % 16 == 0)
//            qDebug("\n");
//        qDebug("0x%02x,", Tal1tab[abc]);
//    }

    //进行编码
    //先把长串的输入dataProccessed拆分成223/239的块（！注意最后一块不满的话要补！）
    for (unsigned int i = 0; i < blk_num; i++)//将所有的待编码数据进行分块，每块223/239个byte符号
    {

        for (unsigned int j = 0; j < numberOfElm - nroots; j++)//拿出223/239个符号进行编码
        {
//            if (j + i * (numberOfElm - nroots) > byte_num - 1)//补0
//            {
//                dataEncode[j] = 0;
//                continue;
//            }
            if (j < paddingNum)
            {
                dataEncode[j] = 0;
            }
            else
                dataEncode[j] = dataProccessed[j - paddingNum + i * (numberOfElm - nroots)];
        }

        for (int a = 0; a < numberOfElm - nroots; a++)
        {
            cdataEncode[a] = Tal1tab[dataEncode[a]];
        }
        encodeRS(cdataEncode, cprimity);//编码

        //将校验位由常规基转换为对偶基
        for (unsigned int b = 0; b < nroots; b++)
        {
            primity[b] = Taltab[cprimity[b]];
        }

        //把编码的输出结果（dataEncode+primity）放到dataOutput中
        for (unsigned int j = 0; j < numberOfElm - nroots; j++)//把数据位放在了前面
        {
            dataOutput.push_back(dataEncode[j]);
        }
        for (unsigned int a = 0; a < nroots; a++)//把校验位放在了后面
        {
            dataOutput.push_back(primity[a]);
        }
    }

    //编码完成
//    free(dataProccessed);
    //将编码后的字节拆分成比特
    for (int i = 0; i < dataOutput.size(); i++)
    {
        for (int j = 0; j < 8; j++)
        {
            outputDataInBit.push_back((dataOutput[i] >> (7 - j)) & 0x01);
        }
    }//完成拆分

//    for (int a = 0; a < dataOutput.size(); a++)
//    qDebug() << outputDataInBit[a];

    //将unsigned char转换成float
    for (int i = 0; i < outputDataInBit.size(); i++)
    {
        outputDataInFloat.push_back(outputDataInBit[i]);
    }

    //delete dataOutput;  //这里为了语法注释了改了，注意！！！by 陆航

    return Waveform("", outputDataInFloat);

}

unsigned int RSGenerator::modn(unsigned int x)
{
    while (x >= numberOfElm)
    {
        x -= numberOfElm;
        x = (x >> bitPerSmb) + (x & numberOfElm);
    }

    return x;
}

//init_rs_char(8,0x187,112,11,32);这里把symsize去掉了 用bitPerSmb代替
void RSGenerator::initRS(unsigned int gfpoly,unsigned int fcr,unsigned int prime)
{
    int sr, root, iprim;    //sr用于生成GF域，相当于mask；iprim貌似与解调有关，相关代码用不上
    unsigned int i, j;

    index_of = (unsigned char *)malloc(sizeof(unsigned char) * (numberOfElm + 1));
    alpha_to = (unsigned char *)malloc(sizeof(unsigned char) * (numberOfElm + 1));
    genpoly = (unsigned char *)malloc(sizeof(unsigned char) * (nroots + 1));
    modnn_table = (int *)malloc(sizeof(int) * (2 << ((sizeof(unsigned char)) * 8)));

    index_of[0] = numberOfElm;
    alpha_to[numberOfElm] = 0;
    sr = 1;
    for (i = 0; i < numberOfElm; i++)
    {
        index_of[sr] = i;
        alpha_to[i] = sr;
        sr <<= 1;
        if (sr & (1 << bitPerSmb))
            sr ^= gfpoly;
        sr &= numberOfElm;
    }

    this->fcr = fcr;
    this->prim = prime;
    //this->nroots = nroots;

    // Find prim-th root of 1, used in decoding
    for (iprim = 1; (iprim % prime) != 0; iprim += numberOfElm)//源代码中这里prim为unsigned int
        ;
   // iprime = iprim / prime;   //这里为了语法注释了改了，注意！！！by 陆航

    genpoly[0] = 1;
    for(i = 0, root = fcr * prime; i < nroots; i++, root += prime)
    {
        genpoly[i + 1] = 1;
        // Multiply rs->genpoly[] by  @**(root + x)
        for (j = i; j > 0 ; j--)
        {
            if (genpoly[j] != 0)
            {
                genpoly[j] = genpoly[j - 1] ^ alpha_to[modn(index_of[genpoly[j]] + root)];
            }
            else
            {
                genpoly[j] = genpoly[j - 1];
            }
        }
        //rs->genpoly[0] can never be zero
        genpoly[0] = alpha_to[modn(index_of[genpoly[0]] + root)];
    }

//    for(int abc = 0; abc < 33; abc++)
//    {
//        qDebug() << "genpoly" << genpoly[abc];
//    }

    // convert rs->genpoly[] to index form for quicker encoding
    for (i = 0; i <= nroots; i++)
    {
        genpoly[i] = index_of[genpoly[i]];
    }

    // Form modnn lookup table
    modnn_table = (int *)malloc(sizeof(int)*(2<<((sizeof(unsigned char))*8)));

    for (i = 0; i < (2<<((sizeof(unsigned char))*8)); i++)
    {
        j = i;
        modnn_table[i] = modn(j);//程序里没有modnn    //这里为了语法注释了改了，注意！！！by 陆航
    }
}


//使用生成的alpha_to以及index_to还有生成多项式进行编码
void RSGenerator::encodeRS(unsigned char *data, unsigned char *bb)
{
    unsigned int i, j;
    unsigned char feedback;

    memset(bb, 0, nroots * sizeof(unsigned char));

    for (i = 0; i < numberOfElm - nroots; i++)
    {
        feedback = index_of[data[i] ^ bb[0]];
        if (feedback != numberOfElm)
        {
            for (j = 1; j < nroots; j++)
                bb[j] ^= alpha_to[modnn_table[feedback + genpoly[nroots - j]]];
        }

        //Shift
        memmove(&bb[0], &bb[1], sizeof(unsigned char) * (nroots - 1));
        if (feedback != numberOfElm)
            bb[nroots - 1] = alpha_to[modnn_table[feedback + genpoly[0]]];
        else
            bb[nroots - 1] = 0;
    }
}

void RSGenerator::genCCSDS()
{
    int i,j,k;

     for(i=0;i<256;i++){/* For each value of input */
       Taltab[i] = 0;
       for(j=0;j<8;j++) /* for each column of matrix */
         for(k=0;k<8;k++){ /* for each row of matrix */
       if(i & (1<<k))
          Taltab[i] ^= tal[7-k] & (1<<j);
         }
       Tal1tab[Taltab[i]] = i;
     }
}

