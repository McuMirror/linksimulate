#ifndef SCRAMBLER_H
#define SCRAMBLER_H

#include <QObject>

class Waveform;

class Scrambler : public QObject
{
    Q_OBJECT
public:
    explicit Scrambler(QObject *parent = 0);
    Scrambler(char32_t eigenpolynomial, size_t order, char32_t initialPhase);
    Waveform scramblingCodeWithInput(const Waveform &wfm);

    char32_t eigenpolynomial() const;
    void setEigenpolynomial(char32_t eigenpolynomial);

    size_t order() const;
    void setOrder(const size_t &order);

    char32_t initialPhase() const;
    void setInitialPhase(char32_t initialPhase);

signals:

public slots:

private:
    char32_t        m_eigenpolynomial;      // Eigenpolynomial represents the feedback logic used,
                                            //  Cn to C1, and n = m_Order.
    size_t          m_order;                // Order means the number of registers,
                                            //  and the max number is 32.
    char32_t        m_initialPhase;         // Scrambler code initial phase, length equals order.
};

#endif // SCRAMBLER_H
