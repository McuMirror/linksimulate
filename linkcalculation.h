#ifndef LINKCALCULATION_H
#define LINKCALCULATION_H

#include <QObject>

class LinkCalculation : public QObject
{
    Q_OBJECT
public:
    explicit LinkCalculation(QObject *parent = 0);

    // return in dB
    float antennaGain(float antennaDiameter,                    // meter
                      float frequency,                          // GHz
                      float efficiency);                        // %
    float antennaGain();                                        // Downlink frequency
    float receiverAntennaGOverT();                              // dB

    // return in dB
    float freeSpaceTransmissionLoss(float frequency,            // GHz
                                    float distance);            // km
    float uplinkTransmissionLoss();
    float uplinkTotalLoss();
    float downlinkTransmissionLoss();
    float downlinkTotalLoss();

    // return in Mbps
    float informationRate(float transmissionRate,               // Mbps
                          float FEC,                            // Forward error correction rate
                          float RS);                            // RS correction rate

    // return in Mbps
    float transmissionRate(float informationRate,               // Mbps
                           float FEC,                           // Forward error correction rate
                           float RS);                           // RS correction rate

    // return in Baud
    float symbolRate(float transmissionRate,                    // Mbps
                     int M);                                    // Modulation factor

    // return in MHz
    float systemBandwidth(float symbolRate,                     // Mbps
                          float alpha,                          // Roll-off factor
                          float Bs);                            // Protection bandwidth, MHz
    float systemBandwidth();

    // return in degree
    float pitchAngle(float latitudeOfEarthStation,              // degree, north
                     float longitudeOfEarthStation,             // degree, east
                     float longitudeUnderSatellite,             // degree, east
                     float earthRadius,                         // km
                     float satelliteAltitude);                  // km

    // return in degree
    float azimuth(float latitudeOfEarthStation,                 // degree, north
                  float longitudeOfEarthStation,                // degree, east
                  float longitudeUnderSatellite);               // km


    // return in km
    float distanceBetweenStationAndSatellite(float latitudeOfEarthStation,              // degree, north
                                             float longitudeOfEarthStation,             // degree, east
                                             float longitudeUnderSatellite,             // degree, east
                                             float earthRadius,                         // km
                                             float satelliteAltitude);                  // km
    float distanceBetweenStationAndSatellite();

    // return in K
    float systemEquivalentNoise(float Ter,                      // K
                                float Ta,                       // K
                                float T1,                       // K
                                float Lf);                      // dB
    float systemEquivalentNoise();

    // return in dB
    float inputFallbackIPBOes(float IPBO,                       // dB
                              float forwarderBw,                // MHz
                              float occupiedBw);                // MHz
    float IPBOes();
    float OPBOes();

    float uplinkCOverTWithoutInterference();                    // return in dB/K
    float uplinkCOverNWithoutInterference();                    // return in dB
    float uplinkCOverTWithInterference();                       // return in dB/K
    float uplinkCOverNWithInterference();                       // return in dB

    float downlinkCOverTWithoutInterference();                  // return in dB/K
    float downlinkCOverNWithoutInterference();                  // return in dB
    float downlinkCOverTWithInterference();                     // return in dB/K
    float downlinkCOverNWithInterference();                     // return in dB

    float linkTotalCOverN();                                    // return in dB
    float linkTotalCOverT();                                    // return in dB/K
    float EbOverN0();                                           // return in dB
    float EbOverN0Margin();                                     // return in dB

    // return in dBW
    float EIRPes(float IPBOes,                            // dB
                 float SFD,                               // dBW/m^2, less than 0
                 float Lu,                                // dB
                 float uploadFreq);                       // GHz

    float informationRate();
    void setInformationRate(float informationRate);

    float symbolRate();
    void setSymbolRate(float symbolRate);

    float FEC() const;
    void setFEC(float FEC);

    float RS() const;
    void setRS(float RS);

    int modulationFactor() const;
    void setModulationFactor(int modulationFactor);

    float rolloffFactor() const;
    void setRolloffFactor(float rolloffFactor);

    float protectionBandwidth() const;
    void setProtectionBandwidth(float protectionBandwidth);

    float uplinkFrequency() const;
    void setUplinkFrequency(float uplinkFrequency);

    float downlinkFrequency() const;
    void setDownlinkFrequency(float downlinkFrequency);

    float EbN0GateValue() const;
    void setEbN0GateValue(float EbN0GateValue);

    float antennaDiameter();
    void setAntennaDiameter(float antennaDiameter);

    float antennaEfficiency() const;
    void setAntennaEfficiency(float antennaEfficiency);

    float equivalentNoise() const;
    void setEquivalentNoise(float equivalentNoise);

    float noiseTemperature() const;
    void setNoiseTemperature(float noiseTemperature);

    float environmentTemperature() const;
    void setEnvironmentTemperature(float environmentTemperature);

    float feederLoss() const;
    void setFeederLoss(float feederLoss);

    float earthRadius() const;
    void setEarthRadius(float earthRadius);

    float satelliteAltitude() const;
    void setSatelliteAltitude(float satelliteAltitude);

    float earthStationLongitude() const;
    void setEarthStationLongitude(float earthStationLongitude);

    float earthStationLatitude() const;
    void setEarthStationLatitude(float earthStationLatitude);

    float longitudeUnderSatellite() const;
    void setLongitudeUnderSatellite(float longitudeUnderSatellite);

    float uplinkRainLoss() const;
    void setUplinkRainLoss(float uplinkRainLoss);

    float downlinkRainLoss() const;
    void setDownlinkRainLoss(float downlinkRainLoss);

    float IPBO() const;
    void setIPBO(float IPBO);

    float satelliteGT() const;
    void setSatelliteGT(float satelliteGT);

    float SFD() const;
    void setSFD(float SFD);

    float uplinkOccupiedBandwidth() const;
    void setUplinkOccupiedBandwidth(float uplinkOccupiedBandwidth);

    float uplinkTransponderBandwidth() const;
    void setUplinkTransponderBandwidth(float uplinkTransponderBandwidth);

    float uplinkRollbackCompensation() const;
    void setUplinkRollbackCompensation(float uplinkRollbackCompensation);

    float uplinkIntermodulationInterference() const;
    void setUplinkIntermodulationInterference(float uplinkIntermodulationInterference);

    float uplinkcrosspolarizationInterference() const;
    void setUplinkcrosspolarizationInterference(float uplinkcrosspolarizationInterference);

    float uplinkSatelliteInterference() const;
    void setUplinkSatelliteInterference(float uplinkSatelliteInterference);

    float OPBO() const;
    void setOPBO(float OPBO);

    float satelliteEIRP() const;
    void setSatelliteEIRP(float satelliteEIRP);

    float downlinkOccupiedBandwidth() const;
    void setDownlinkOccupiedBandwidth(float downlinkOccupiedBandwidth);

    float downlinkTransponderBandwidth() const;
    void setDownlinkTransponderBandwidth(float downlinkTransponderBandwidth);

    float downlinkRollbackCompensation() const;
    void setDownlinkRollbackCompensation(float downlinkRollbackCompensation);

    float downlinkIntermodulationInterference() const;
    void setDownlinkIntermodulationInterference(float downlinkIntermodulationInterference);

    float downlinkcrosspolarizationInterference() const;
    void setDownlinkcrosspolarizationInterference(float downlinkcrosspolarizationInterference);

    float downlinkSatelliteInterference() const;
    void setDownlinkSatelliteInterference(float downlinkSatelliteInterference);

    QString FECString() const;
    void setFECString(const QString &FECString);

    QString RSString() const;
    void setRSString(const QString &RSString);

private:
    float transToLogarithm(float antilogarithm);
    float transToAntilogarithm(float logarithm);
    // Basic parameters
    float               m_informationRate;                      // Mbps
    float               m_symbolRate;                           // MBaud
    float               m_FEC;                                  // (0, 1)
    QString             m_FECString;
    float               m_RS;                                   // (0, 1)
    QString             m_RSString;
    int                 m_modulationFactor;
    float               m_rolloffFactor;
    float               m_protectionBandwidth;                  // MHz
    float               m_uplinkFrequency;                      // GHz
    float               m_downlinkFrequency;                    // GHz
    float               m_EbN0GateValue;                        // dB
    float               m_antennaDiameter;                      // m
    float               m_antennaEfficiency;                    // %
    float               m_equivalentNoise;                      // K
    float               m_noiseTemperature;                     // K
    float               m_environmentTemperature;               // K
    float               m_feederLoss;                           // dB

    // Space loss
    float               m_earthRadius;                          // km
    float               m_satelliteAltitude;                    // km
    float               m_earthStationLongitude;                // east
    float               m_earthStationLatitude;                 // north
    float               m_longitudeUnderSatellite;              // east
    float               m_uplinkRainLoss;                       // dB
    float               m_downlinkRainLoss;                     // dB

    // Uplink C/T
    float               m_IPBO;                                 // dB
    float               m_satelliteGT;                          // dB/K
    float               m_SFD;                                  // < 0
    float               m_uplinkOccupiedBandwidth;              // MHz
    float               m_uplinkTransponderBandwidth;           // MHz
    float               m_uplinkRollbackCompensation;           // dB
    float               m_uplinkIntermodulationInterference;    // dB/K
    float               m_uplinkcrosspolarizationInterference;  // dB/K
    float               m_uplinkSatelliteInterference;          // dB/K

    // Downlink C/T
    float               m_OPBO;                                   // dB
    float               m_satelliteEIRP;                          // dBW
    float               m_downlinkOccupiedBandwidth;              // MHz
    float               m_downlinkTransponderBandwidth;           // MHz
    float               m_downlinkRollbackCompensation;           // dB
    float               m_downlinkIntermodulationInterference;    // dB/K
    float               m_downlinkcrosspolarizationInterference;  // dB/K
    float               m_downlinkSatelliteInterference;          // dB/K

signals:

public slots:
};

#endif // LINKCALCULATION_H
