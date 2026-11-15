#pragma once
#include <QDebug>
#include <QString>

template <int maxDeg>

class BinomialCoeff {

protected:
    double coeffv[maxDeg+1][maxDeg+1];

public:
    BinomialCoeff(){
        ComputeCoefficients();
    }

    double coeff(int i, int j) {
        Q_ASSERT(i <= maxDeg && j <= maxDeg);
        return coeffv[i][j];
    }

protected:
    void ComputeCoefficients() {
        coeffv[0][0] = 1.0;
        coeffv[1][0] = coeffv[1][1] = 1.0;
        for(int i = 2; i < (maxDeg+1); i++) {
            coeffv[i][0] = 1.0;
            coeffv[i][i] = 1.0;
            for(int j = 1; j < i; j++) {
                coeffv[i][j] = coeffv[i-1][j-1] + coeffv[i-1][j];
            }
        }
//#define SHOW_BINOMIAL_COEFFICIENT_FOR_DEBUG
#ifdef SHOW_BINOMIAL_COEFFICIENT_FOR_DEBUG
        int maxDegreeBinomail = 11;
        for(int i = 0; i <= maxDegreeBinomail; i++) {
            QString str;
            for(int j = 0; j <= maxDegreeBinomail; j++) {
                if ( j <= i ) str += QString("%1").arg(coeffv[i][j]);
                else str += QString("0");
                if ( j != maxDegreeBinomail ) str += QString(",");
            }
            qDebug() << str;
        }
        qDebug() << "";
        int maxKnDegreeBinomial = 4 * maxDegreeBinomail -7;
        qDebug() << "maxKnDegree = " << maxKnDegreeBinomial;
        for(int i = 0; i <= maxDegreeBinomail; i++) {
            QString str;
            int dg = (4 * i - 7) < 0 ? 0 : (4 *i -7);
            for(int j = 0; j <= maxKnDegreeBinomial; j++) {
                if ( j <= i ) str += QString("%1").arg(coeffv[dg][j]);
                else str += QString("0");
                if ( j != maxKnDegreeBinomial ) str += QString(",");
            }
            qDebug() << str;
        }
#endif
    }
};

const int maxDegree = 100;
extern BinomialCoeff<maxDegree> biCoeff;
