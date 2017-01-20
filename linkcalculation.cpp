#include "linkcalculation.h"
#include <math.h>
#include <limits.h>

#include <QDebug>

#define M_PI        3.14159265358979323846
#define M_PI_2      1.57079632679489661923
#define M_PI_4      0.785398163397448309616

#define Radians(x)  ((x) / 180.0 * M_PI)
#define Degree(x)   ((x) / M_PI * 180.0)

const static float VelocitOfSpeed = 3e8;

// Bad bad bad code, should be reconstructed to guarantee fault tolerance

// The following formula from page
// http://wenku.baidu.com/view/515ae759be23482fb4da4c80.html

LinkCalculation::LinkCalculation(QObject *parent) : QObject(parent)
{
    // Basic parameters
    m_informationRate = INT_MIN;
    m_symbolRate = INT_MIN;
    m_FEC = INT_MIN;
    m_RS = INT_MIN;
     m_modulationFactor = INT_MIN;
    m_rolloffFactor = INT_MIN;
    m_protectionBandwidth = INT_MIN;
    m_uplinkFrequency = INT_MIN;
    m_downlinkFrequency = INT_MIN;
    m_EbN0GateValue = INT_MIN;
    m_antennaDiameter = INT_MIN;
    m_antennaEfficiency = INT_MIN;
    m_equivalentNoise = INT_MIN;
    m_noiseTemperature = INT_MIN;
    m_environmentTemperature = INT_MIN;
    m_feederLoss = INT_MIN;

    // Space loss
    m_earthRadius = INT_MIN;
    m_satelliteAltitude = INT_MIN;
    m_earthStationLongitude = INT_MIN;
    m_earthStationLatitude = INT_MIN;
    m_longitudeUnderSatellite = INT_MIN;
    m_uplinkRainLoss = INT_MIN;
    m_downlinkRainLoss = INT_MIN;

    // Uplink C/T
    m_IPBO = INT_MIN;
    m_satelliteGT = INT_MIN;
    m_SFD = INT_MIN;
    m_uplinkOccupiedBandwidth = INT_MIN;
    m_uplinkTransponderBandwidth = INT_MIN;
    m_uplinkRollbackCompensation = INT_MIN;
    m_uplinkIntermodulationInterference = INT_MIN;
    m_uplinkcrosspolarizationInterference = INT_MIN;
    m_uplinkSatelliteInterference = INT_MIN;

    // Downlink C/T
    m_OPBO = INT_MIN;
    m_satelliteEIRP = INT_MIN;
    m_downlinkOccupiedBandwidth = INT_MIN;
    m_downlinkTransponderBandwidth = INT_MIN;
    m_downlinkRollbackCompensation = INT_MIN;
    m_downlinkIntermodulationInterference = INT_MIN;
    m_downlinkcrosspolarizationInterference = INT_MIN;
    m_downlinkSatelliteInterference = INT_MIN;
}

float LinkCalculation::antennaGain(float antennaDiameter, float frequency, float efficiency)
{
    float lamda = VelocitOfSpeed / (frequency * 1e9);
    float A = M_PI * antennaDiameter * antennaDiameter / 4;
    float G = 4 * M_PI * A * efficiency / 100 / (lamda * lamda);

    return transToLogarithm(G);
}

float LinkCalculation::antennaGain()
{
    return antennaGain(m_antennaDiameter, m_downlinkFrequency, m_antennaEfficiency);
}

float LinkCalculation::receiverAntennaGOverT()
{
    return antennaGain() - transToLogarithm(systemEquivalentNoise());
}

float LinkCalculation::freeSpaceTransmissionLoss(float frequency, float distance)
{
    float loss = 92.45 + 20 * log10(distance) + 20 * log10(frequency);
    return loss;
}

float LinkCalculation::uplinkTransmissionLoss()
{
    return freeSpaceTransmissionLoss(m_uplinkFrequency, distanceBetweenStationAndSatellite());
}

float LinkCalculation::uplinkTotalLoss()
{
    return uplinkTransmissionLoss() + m_uplinkRainLoss;
}

float LinkCalculation::downlinkTransmissionLoss()
{
    return freeSpaceTransmissionLoss(m_downlinkFrequency, distanceBetweenStationAndSatellite());
}

float LinkCalculation::downlinkTotalLoss()
{
    return downlinkTransmissionLoss() + m_downlinkRainLoss;
}

float LinkCalculation::informationRate(float transmissionRate, float FEC, float RS)
{
    float R1 = transmissionRate * FEC * RS;
    return R1;
}

float LinkCalculation::transmissionRate(float informationRate, float FEC, float RS)
{
    float R2 = informationRate / (FEC * RS);
    return R2;
}

float LinkCalculation::symbolRate(float transmissionRate, int M)
{
    return transmissionRate / M;
}

float LinkCalculation::systemBandwidth(float symbolRate, float alpha, float Bs)
{
    float B = symbolRate * (1 + alpha) + Bs;
    return B;
}

float LinkCalculation::systemBandwidth()
{
    if (m_symbolRate == INT_MIN) {
        m_symbolRate = symbolRate(transmissionRate(m_informationRate, m_FEC, m_RS), m_modulationFactor);
    }
    else {
        m_informationRate = m_symbolRate * m_modulationFactor * m_FEC * m_RS;
    }

    return systemBandwidth(m_symbolRate, m_rolloffFactor, m_protectionBandwidth);
}

float LinkCalculation::pitchAngle(float latitudeOfEarthStation,
                                  float longitudeOfEarthStation,
                                  float longitudeUnderSatellite,
                                  float earthRadius,
                                  float satelliteAltitude)
{
    float cosTheta1 = cos(Radians(latitudeOfEarthStation));
    float cosPhi = cos(Radians(longitudeUnderSatellite - longitudeOfEarthStation));
    float ret = atan((cosTheta1 * cosPhi - earthRadius / (earthRadius + satelliteAltitude))
                     / sqrt(1 - cosTheta1 * cosTheta1 * cosPhi * cosPhi));

    return Degree(ret);
}

float LinkCalculation::azimuth(float latitudeOfEarthStation,
                               float longitudeOfEarthStation,
                               float longitudeUnderSatellite)
{
    float ret = tan(Radians(longitudeUnderSatellite - longitudeOfEarthStation)) / sin(latitudeOfEarthStation);

    return Degree(ret);
}

float LinkCalculation::distanceBetweenStationAndSatellite(float latitudeOfEarthStation,
                                                          float longitudeOfEarthStation,
                                                          float longitudeUnderSatellite,
                                                          float earthRadius,
                                                          float satelliteAltitude)
{
    float cosTheta1 = cos(Radians(latitudeOfEarthStation));
    float cosPhi = cos(Radians(longitudeUnderSatellite - longitudeOfEarthStation));
    float temp = earthRadius / (earthRadius + satelliteAltitude);
    float ret = (earthRadius + satelliteAltitude) * sqrt(1 + temp * temp - 2 * temp * cosTheta1 * cosPhi);

    return ret;
}

float LinkCalculation::distanceBetweenStationAndSatellite()
{
    return distanceBetweenStationAndSatellite(m_earthStationLatitude,
                                              m_earthStationLongitude,
                                              m_longitudeUnderSatellite,
                                              m_earthRadius,
                                              m_satelliteAltitude);
}

float LinkCalculation::systemEquivalentNoise(float Ter, float Ta, float T1, float Lf)
{
    float convertedLf = transToAntilogarithm(Lf);
    float ret = Ta + (convertedLf - 1) * T1 + convertedLf * Ter;

    return ret;
}

float LinkCalculation::systemEquivalentNoise()
{
    return systemEquivalentNoise(m_equivalentNoise, m_noiseTemperature, m_environmentTemperature, m_feederLoss);
}

float LinkCalculation::inputFallbackIPBOes(float IPBO, float forwarderBw, float occupiedBw)
{
    float ret = IPBO + transToLogarithm(forwarderBw / occupiedBw);

    return ret;
}

float LinkCalculation::IPBOes()
{
    if (m_uplinkTransponderBandwidth != INT_MIN && m_uplinkOccupiedBandwidth != INT_MIN)
        return inputFallbackIPBOes(m_IPBO, m_uplinkTransponderBandwidth, m_uplinkOccupiedBandwidth);
    else
        return m_IPBO;
}

float LinkCalculation::OPBOes()
{
    if (m_downlinkTransponderBandwidth != INT_MIN && m_downlinkOccupiedBandwidth != INT_MIN)
        return inputFallbackIPBOes(m_OPBO, m_downlinkTransponderBandwidth, m_downlinkOccupiedBandwidth);
    else
        return m_OPBO;
}

float LinkCalculation::uplinkCOverTWithoutInterference()
{
    float lamda = VelocitOfSpeed / (m_uplinkFrequency * 1e9);
    return m_SFD - IPBOes() - transToLogarithm(4 * M_PI / (lamda * lamda)) + m_satelliteGT;
}

float LinkCalculation::uplinkCOverNWithoutInterference()
{
    return uplinkCOverTWithoutInterference() + 228.6 - transToLogarithm(systemBandwidth()) - 60;
}

float LinkCalculation::uplinkCOverTWithInterference()
{
    return uplinkCOverNWithInterference() - 228.6 + transToLogarithm(systemBandwidth()) + 60;
}

float LinkCalculation::uplinkCOverNWithInterference()
{
    // Totally wrong, maybe
    if (m_uplinkIntermodulationInterference != INT_MIN
            && m_uplinkcrosspolarizationInterference != INT_MIN
            && m_uplinkSatelliteInterference != INT_MIN) {
        float A = transToAntilogarithm(uplinkCOverNWithoutInterference());
        float B = transToAntilogarithm(m_uplinkIntermodulationInterference);
        float C = transToAntilogarithm(m_uplinkcrosspolarizationInterference);
        float D = transToAntilogarithm(m_uplinkSatelliteInterference);
//        float A = uplinkCOverNWithoutInterference();
//        float B = m_uplinkIntermodulationInterference;
//        float C = m_uplinkcrosspolarizationInterference;
//        float D = m_uplinkSatelliteInterference;

        return transToLogarithm(1 / (1 / A + 1 / B + 1 / C + 1 / D));
    }
    else
        return uplinkCOverNWithoutInterference();
}

float LinkCalculation::downlinkCOverTWithoutInterference()
{
    return m_satelliteEIRP - OPBOes() - downlinkTransmissionLoss() + receiverAntennaGOverT();
}

float LinkCalculation::downlinkCOverNWithoutInterference()
{
    return downlinkCOverTWithoutInterference() + 228.6 - transToLogarithm(systemBandwidth() * 1e6);
}

float LinkCalculation::downlinkCOverTWithInterference()
{
    return downlinkCOverNWithInterference() - 228.6 + transToLogarithm(systemBandwidth() * 1e6);
}

float LinkCalculation::downlinkCOverNWithInterference()
{
    // Totally wrong, maybe
    if (m_downlinkIntermodulationInterference != INT_MIN
            && m_downlinkcrosspolarizationInterference != INT_MIN
            && m_downlinkSatelliteInterference != INT_MIN) {
        float A = transToAntilogarithm(downlinkCOverNWithoutInterference());
        float B = transToAntilogarithm(m_downlinkIntermodulationInterference);
        float C = transToAntilogarithm(m_downlinkcrosspolarizationInterference);
        float D = transToAntilogarithm(m_downlinkSatelliteInterference);
//        float A = uplinkCOverNWithoutInterference();
//        float B = m_uplinkIntermodulationInterference;
//        float C = m_uplinkcrosspolarizationInterference;
//        float D = m_uplinkSatelliteInterference;

        return transToLogarithm(1 / (1 / A + 1 / B + 1 / C + 1 / D));
    }
    else
        return downlinkCOverNWithoutInterference();
}

float LinkCalculation::linkTotalCOverN()
{
    float uplinkCOverN = transToAntilogarithm(uplinkCOverNWithInterference());
    float downlinkCOverN = transToAntilogarithm(downlinkCOverNWithInterference());
    return transToLogarithm(1 / (1 / uplinkCOverN + 1 / downlinkCOverN));
}

float LinkCalculation::linkTotalCOverT()
{
    return linkTotalCOverN() - 228.6 + transToLogarithm(systemBandwidth() * 1e6);
}

float LinkCalculation::EbOverN0()
{
    return linkTotalCOverN() - transToLogarithm(informationRate() * 1e6) + transToLogarithm(systemBandwidth() * 1e6);
}

float LinkCalculation::EbOverN0Margin()
{
    return EbOverN0() - m_EbN0GateValue;
}

float LinkCalculation::EIRPes(float IPBOes, float SFD, float Lu, float uploadFreq)
{
    float lamda = VelocitOfSpeed / (uploadFreq * 1e9);
    float ret = SFD - transToLogarithm(4 * M_PI / (lamda * lamda)) + Lu - IPBOes;

    return ret;
}

float LinkCalculation::informationRate()
{
    if (m_informationRate == INT_MIN && m_symbolRate != INT_MIN
            && m_modulationFactor != INT_MIN && m_FEC != INT_MIN && m_RS != INT_MIN)
        m_informationRate = m_symbolRate * m_modulationFactor * m_FEC * m_RS;
    return m_informationRate;
}

void LinkCalculation::setInformationRate(float informationRate)
{
    m_informationRate = informationRate;
}

float LinkCalculation::symbolRate()
{
    if (m_symbolRate == INT_MIN && m_informationRate != INT_MIN
            && m_modulationFactor != INT_MIN && m_FEC != INT_MIN && m_RS != INT_MIN)
        m_symbolRate = symbolRate(transmissionRate(m_informationRate, m_FEC, m_RS), m_modulationFactor);
    return m_symbolRate;
}

void LinkCalculation::setSymbolRate(float symbolRate)
{
    m_symbolRate = symbolRate;
}

float LinkCalculation::FEC() const
{
    return m_FEC;
}

void LinkCalculation::setFEC(float FEC)
{
    m_FEC = FEC;
}

float LinkCalculation::RS() const
{
    return m_RS;
}

void LinkCalculation::setRS(float RS)
{
    m_RS = RS;
}

int LinkCalculation::modulationFactor() const
{
    return m_modulationFactor;
}

void LinkCalculation::setModulationFactor(int modulationFactor)
{
    m_modulationFactor = modulationFactor;
}

float LinkCalculation::rolloffFactor() const
{
    return m_rolloffFactor;
}

void LinkCalculation::setRolloffFactor(float rolloffFactor)
{
    m_rolloffFactor = rolloffFactor;
}

float LinkCalculation::protectionBandwidth() const
{
    return m_protectionBandwidth;
}

void LinkCalculation::setProtectionBandwidth(float protectionBandwidth)
{
    m_protectionBandwidth = protectionBandwidth;
}

float LinkCalculation::uplinkFrequency() const
{
    return m_uplinkFrequency;
}

void LinkCalculation::setUplinkFrequency(float uplinkFrequency)
{
    m_uplinkFrequency = uplinkFrequency;
}

float LinkCalculation::downlinkFrequency() const
{
    return m_downlinkFrequency;
}

void LinkCalculation::setDownlinkFrequency(float downlinkFrequency)
{
    m_downlinkFrequency = downlinkFrequency;
}

float LinkCalculation::EbN0GateValue() const
{
    return m_EbN0GateValue;
}

void LinkCalculation::setEbN0GateValue(float EbN0GateValue)
{
    m_EbN0GateValue = EbN0GateValue;
}

float LinkCalculation::antennaDiameter()
{
    if (m_antennaDiameter == INT_MIN) {
        float totalCOverN = m_EbN0GateValue + transToLogarithm(informationRate())
                - transToLogarithm(systemBandwidth());
        float antilogarithmDownlinkCOverNWithInterference = 1 / ( 1 / transToAntilogarithm(totalCOverN)
                                                            + 1 / transToAntilogarithm(uplinkCOverNWithInterference()));
        float antilogarithmDownlinkCOverNWithoutInterference = 1 / ( 1 / antilogarithmDownlinkCOverNWithInterference
                                                                     - 1 / transToAntilogarithm(m_downlinkIntermodulationInterference)
                                                                     - 1 / transToAntilogarithm(m_downlinkcrosspolarizationInterference)
                                                                     - 1 / transToAntilogarithm(m_downlinkSatelliteInterference));
        float downlinkCOverNWithoutInterference = transToLogarithm(antilogarithmDownlinkCOverNWithoutInterference);
        float downlinkCOverTWithoutInterference = downlinkCOverNWithoutInterference - 228.6 + transToLogarithm(systemBandwidth() * 1e6);
        float earthStationGOverT = downlinkCOverTWithoutInterference - m_satelliteEIRP + OPBOes() + downlinkTransmissionLoss();
        float antennaGain = transToLogarithm(systemEquivalentNoise() * transToAntilogarithm(earthStationGOverT));
        float downlinkWaveLength = VelocitOfSpeed / (m_downlinkFrequency * 1e9);
        float antennaArea = antennaGain * (downlinkWaveLength * downlinkWaveLength) / (4 * M_PI * m_antennaEfficiency / 100.f);

        m_antennaDiameter = 2 * sqrt(antennaArea / M_PI);
    }

    return m_antennaDiameter;
}

void LinkCalculation::setAntennaDiameter(float antennaDiameter)
{
    m_antennaDiameter = antennaDiameter;
}

float LinkCalculation::antennaEfficiency() const
{
    return m_antennaEfficiency;
}

void LinkCalculation::setAntennaEfficiency(float antennaEfficiency)
{
    m_antennaEfficiency = antennaEfficiency;
}

float LinkCalculation::equivalentNoise() const
{
    return m_equivalentNoise;
}

void LinkCalculation::setEquivalentNoise(float equivalentNoise)
{
    m_equivalentNoise = equivalentNoise;
}

float LinkCalculation::noiseTemperature() const
{
    return m_noiseTemperature;
}

void LinkCalculation::setNoiseTemperature(float noiseTemperature)
{
    m_noiseTemperature = noiseTemperature;
}

float LinkCalculation::environmentTemperature() const
{
    return m_environmentTemperature;
}

void LinkCalculation::setEnvironmentTemperature(float environmentTemperature)
{
    m_environmentTemperature = environmentTemperature;
}

float LinkCalculation::feederLoss() const
{
    return m_feederLoss;
}

void LinkCalculation::setFeederLoss(float feederLoss)
{
    m_feederLoss = feederLoss;
}

float LinkCalculation::earthRadius() const
{
    return m_earthRadius;
}

void LinkCalculation::setEarthRadius(float earthRadius)
{
    m_earthRadius = earthRadius;
}

float LinkCalculation::satelliteAltitude() const
{
    return m_satelliteAltitude;
}

void LinkCalculation::setSatelliteAltitude(float satelliteAltitude)
{
    m_satelliteAltitude = satelliteAltitude;
}

float LinkCalculation::earthStationLongitude() const
{
    return m_earthStationLongitude;
}

void LinkCalculation::setEarthStationLongitude(float earthStationLongitude)
{
    m_earthStationLongitude = earthStationLongitude;
}

float LinkCalculation::earthStationLatitude() const
{
    return m_earthStationLatitude;
}

void LinkCalculation::setEarthStationLatitude(float earthStationLatitude)
{
    m_earthStationLatitude = earthStationLatitude;
}

float LinkCalculation::longitudeUnderSatellite() const
{
    return m_longitudeUnderSatellite;
}

void LinkCalculation::setLongitudeUnderSatellite(float longitudeUnderSatellite)
{
    m_longitudeUnderSatellite = longitudeUnderSatellite;
}

float LinkCalculation::uplinkRainLoss() const
{
    return m_uplinkRainLoss;
}

void LinkCalculation::setUplinkRainLoss(float uplinkRainLoss)
{
    m_uplinkRainLoss = uplinkRainLoss;
}

float LinkCalculation::downlinkRainLoss() const
{
    return m_downlinkRainLoss;
}

void LinkCalculation::setDownlinkRainLoss(float downlinkRainLoss)
{
    m_downlinkRainLoss = downlinkRainLoss;
}

float LinkCalculation::IPBO() const
{
    return m_IPBO;
}

void LinkCalculation::setIPBO(float IPBO)
{
    m_IPBO = IPBO;
}

float LinkCalculation::satelliteGT() const
{
    return m_satelliteGT;
}

void LinkCalculation::setSatelliteGT(float satelliteGT)
{
    m_satelliteGT = satelliteGT;
}

float LinkCalculation::SFD() const
{
    return m_SFD;
}

void LinkCalculation::setSFD(float SFD)
{
    m_SFD = SFD;
}

float LinkCalculation::uplinkOccupiedBandwidth() const
{
    return m_uplinkOccupiedBandwidth;
}

void LinkCalculation::setUplinkOccupiedBandwidth(float uplinkOccupiedBandwidth)
{
    m_uplinkOccupiedBandwidth = uplinkOccupiedBandwidth;
}

float LinkCalculation::uplinkTransponderBandwidth() const
{
    return m_uplinkTransponderBandwidth;
}

void LinkCalculation::setUplinkTransponderBandwidth(float uplinkTransponderBandwidth)
{
    m_uplinkTransponderBandwidth = uplinkTransponderBandwidth;
}

float LinkCalculation::uplinkRollbackCompensation() const
{
    return m_uplinkRollbackCompensation;
}

void LinkCalculation::setUplinkRollbackCompensation(float uplinkRollbackCompensation)
{
    m_uplinkRollbackCompensation = uplinkRollbackCompensation;
}

float LinkCalculation::uplinkIntermodulationInterference() const
{
    return m_uplinkIntermodulationInterference;
}

void LinkCalculation::setUplinkIntermodulationInterference(float uplinkIntermodulationInterference)
{
    m_uplinkIntermodulationInterference = uplinkIntermodulationInterference;
}

float LinkCalculation::uplinkcrosspolarizationInterference() const
{
    return m_uplinkcrosspolarizationInterference;
}

void LinkCalculation::setUplinkcrosspolarizationInterference(float uplinkcrosspolarizationInterference)
{
    m_uplinkcrosspolarizationInterference = uplinkcrosspolarizationInterference;
}

float LinkCalculation::uplinkSatelliteInterference() const
{
    return m_uplinkSatelliteInterference;
}

void LinkCalculation::setUplinkSatelliteInterference(float uplinkSatelliteInterference)
{
    m_uplinkSatelliteInterference = uplinkSatelliteInterference;
}

float LinkCalculation::OPBO() const
{
    return m_OPBO;
}

void LinkCalculation::setOPBO(float OPBO)
{
    m_OPBO = OPBO;
}

float LinkCalculation::satelliteEIRP() const
{
    return m_satelliteEIRP;
}

void LinkCalculation::setSatelliteEIRP(float satelliteEIRP)
{
    m_satelliteEIRP = satelliteEIRP;
}

float LinkCalculation::downlinkOccupiedBandwidth() const
{
    return m_downlinkOccupiedBandwidth;
}

void LinkCalculation::setDownlinkOccupiedBandwidth(float downlinkOccupiedBandwidth)
{
    m_downlinkOccupiedBandwidth = downlinkOccupiedBandwidth;
}

float LinkCalculation::downlinkTransponderBandwidth() const
{
    return m_downlinkTransponderBandwidth;
}

void LinkCalculation::setDownlinkTransponderBandwidth(float downlinkTransponderBandwidth)
{
    m_downlinkTransponderBandwidth = downlinkTransponderBandwidth;
}

float LinkCalculation::downlinkRollbackCompensation() const
{
    return m_downlinkRollbackCompensation;
}

void LinkCalculation::setDownlinkRollbackCompensation(float downlinkRollbackCompensation)
{
    m_downlinkRollbackCompensation = downlinkRollbackCompensation;
}

float LinkCalculation::downlinkIntermodulationInterference() const
{
    return m_downlinkIntermodulationInterference;
}

void LinkCalculation::setDownlinkIntermodulationInterference(float downlinkIntermodulationInterference)
{
    m_downlinkIntermodulationInterference = downlinkIntermodulationInterference;
}

float LinkCalculation::downlinkcrosspolarizationInterference() const
{
    return m_downlinkcrosspolarizationInterference;
}

void LinkCalculation::setDownlinkcrosspolarizationInterference(float downlinkcrosspolarizationInterference)
{
    m_downlinkcrosspolarizationInterference = downlinkcrosspolarizationInterference;
}

float LinkCalculation::downlinkSatelliteInterference() const
{
    return m_downlinkSatelliteInterference;
}

void LinkCalculation::setDownlinkSatelliteInterference(float downlinkSatelliteInterference)
{
    m_downlinkSatelliteInterference = downlinkSatelliteInterference;
}

QString LinkCalculation::FECString() const
{
    return m_FECString;
}

void LinkCalculation::setFECString(const QString &FECString)
{
    m_FECString = FECString;
}

QString LinkCalculation::RSString() const
{
    return m_RSString;
}

void LinkCalculation::setRSString(const QString &RSString)
{
    m_RSString = RSString;
}

inline float LinkCalculation::transToLogarithm(float antilogarithm)
{
    return 10 * log10(antilogarithm);
}

inline float LinkCalculation::transToAntilogarithm(float logarithm)
{
    return pow(10, logarithm / 10.f);
}

