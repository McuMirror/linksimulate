#include "waveformmanager.h"
#include "mat.h"
#include "mex.h"
#include "matrix.h"
#include <exception>

#pragma comment(lib, "libmat.lib")
#pragma comment(lib, "libmx.lib")
#pragma comment(lib, "libmex.lib")
#pragma comment(lib, "libeng.lib")

#define BUFSIZE 256

WaveformManager * WaveformManager::m_instance = new WaveformManager;

WaveformManager::WaveformManager(QObject *parent)
    : QObject(parent)
{

}

WaveformList WaveformManager::loadFromMAT(std::string &filePath)
{
    MATFile *pMF = NULL;
    mxArray *pMxArray = NULL;

    pMF = matOpen(filePath.c_str(), "r");
    if(pMF == NULL) {
        matClose(pMF);
        throw std::exception("Error - An error occurs when open MAT-file.");
    }

    char **dir = NULL;
    int num = 0;
    dir = matGetDir(pMF, &num);
    if(dir == NULL && num < 0) {
        mxFree(dir);
        matClose(pMF);
        throw std::exception("Error - There are no arrays in the MAT-file.");
    }

    WaveformList list("");
    for(size_t i = 0; i < num; i++) {
        pMxArray = matGetVariable(pMF, dir[i]);
        double *initData = (double *)mxGetData(pMxArray);
        size_t M = mxGetM(pMxArray);
        size_t N = mxGetN(pMxArray);
        size_t length = M * N;
        float *data = new float[length]();
        for(size_t row = 0; row < M; row++) {
            for(size_t col = 0; col < N; col++) {
                data[row * M + col] = (float)(initData[row * M + col]);
            }
        }

        list.addWaveform(dir[i], length, data);
        mxFree(initData);
        delete [] data;
    }
    mxFree(dir);
    matClose(pMF);

    return list;
}

void WaveformManager::saveToMAT(const WaveformList &list, std::string &filePath)
{
    MATFile *pMF = NULL;
    mxArray *pMxArray = NULL;
    int status;
    char str[BUFSIZE];

    pMF = matOpen(filePath.c_str(), "w");
    if(pMF == NULL) {
        throw std::exception("Error - MAT-file create failed.");
    }

    for(WfmList::const_iterator iter = list.list().begin()
        ; iter != list.list().end()
        ; iter++) {
        WaveformPtr wfmPtr = *iter;
        Waveform wfm = *(wfmPtr.get());
        size_t row = 1;
        size_t col = wfm.length();
        pMxArray = mxCreateDoubleMatrix(row, col, mxREAL);
        if(pMF == NULL) {
            sprintf_s(str, BUFSIZE, "Error - %s : out of memory on line %d.", __FILE__, __LINE__);
            throw std::exception(str);
        }

        status = matPutVariable(pMF, wfm.name().c_str(), pMxArray);
        if (status != 0) {
            sprintf_s(str, BUFSIZE, "Error - %s :  error using matPutVariable on line %d.", __FILE__, __LINE__);
            throw std::exception(str);
        }

        size_t dataSize = row * col;
        double *data = new double[dataSize];
        for(size_t i = 0; i < dataSize; i++) {
            double value = *(wfm.data() + i);
            data[i] = value;
        }
        memcpy((void *)(mxGetPr(pMxArray)), (void *)data, dataSize * sizeof(double));

        /*
         * Ooops! we need to copy data before writing the array.  (Well,
         * ok, this was really intentional.) This demonstrates that
         * matPutVariable will overwrite an existing array in a MAT-file.
         */
        memcpy((void *)(mxGetPr(pMxArray)), (void *)data, dataSize * sizeof(double));
        status = matPutVariable(pMF, wfm.name().c_str(), pMxArray);
        if (status != 0) {
            sprintf_s(str, BUFSIZE, "Error - %s :  error using matPutVariable on line %d.", __FILE__, __LINE__);
            throw std::exception(str);
        }

        /* clean up */
        mxDestroyArray(pMxArray);
    }

    if (matClose(pMF) != 0) {
        throw std::exception("Error - MAT-file colse failed.");
    }
}

void WaveformManager::addWaveform(const Waveform &wfm)
{
    if(m_sharedWaveforms.find(wfm.name()) == m_sharedWaveforms.end()) {
        Waveform *temp = new Waveform(wfm);
        WaveformPtr pWfm = std::make_shared<Waveform>(*temp);
        m_sharedWaveforms[temp->name()] = pWfm;

        emit dataChanged();
    }
    else
        throw std::exception("Error - The waveform you find exists.");
}

void WaveformManager::removeWaveformWithName(std::string name)
{
    if(m_sharedWaveforms.find(name) != m_sharedWaveforms.end())
        m_sharedWaveforms.erase(name);
}

const Waveform &WaveformManager::waveformWithName(std::string name)
{
    WaveformMap::iterator iter = m_sharedWaveforms.find(name);

    if(iter == m_sharedWaveforms.end())
        throw std::exception("Error - The waveform you find does not exist.");

    return *((*iter).second);
}
