#pragma once

#include <QWidget>
#include <QtGui>
#include <QLabel>
#include <QSlider>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <memory>
#include <QDoubleSpinBox>

class SettingsWindow: public QWidget
{
    Q_OBJECT
protected:
    //std::unique_ptr<QWidget> centralWidget;
public:
    std::unique_ptr<QLabel> k0Label;
    std::unique_ptr<QSlider> k0Slider;
    std::unique_ptr<QLineEdit> k0Edit;
    std::unique_ptr<QLabel> k1Label;
    std::unique_ptr<QSlider> k1Slider;
    std::unique_ptr<QLineEdit> k1Edit;
    std::unique_ptr<QLabel> w1Label;    //w1ラベルの追加
    std::unique_ptr<QSlider> w1Slider;  //w1スライダーの追加
    std::unique_ptr<QLineEdit> w1Edit;  //Editの追加
    std::unique_ptr<QLabel> w0Label;    //w0
    std::unique_ptr<QSlider> w0Slider;
    std::unique_ptr<QLineEdit> w0Edit;
    std::unique_ptr<QLabel> w2Label;    //w2
    std::unique_ptr<QSlider> w2Slider;
    std::unique_ptr<QLineEdit> w2Edit;
    std::unique_ptr<QLabel> w3Label;    //w3
    std::unique_ptr<QSlider> w3Slider;
    std::unique_ptr<QLineEdit> w3Edit;
    std::unique_ptr<QCheckBox> combCheckBox;
    std::unique_ptr<QCheckBox> CofcurvatureCheckBox;

    SettingsWindow(QWidget *parent = 0);

signals:
    void weight0Changed(float val);
    void weight1Changed(float val);
    void weight2Changed(float val);
    void weight3Changed(float val);
    void curvatureK0Changed(float val);
    void curvatureK1Changed(float val);

    void CombCheck(bool visible);
    void CofCurvature(bool visible);

public slots:
    void alphaChanged() {

    }
    void alphaLineEditChanged() {

    }
    void updateW0(float w0) {
        w0Slider->blockSignals(true); // ループ防止！
        w0Slider->setValue(static_cast<int>(w0 * 10)); // w0スライダーのスケールに合わせる
        w0Edit->setText(QString::number(w0, 'f', 1));
        w0Slider->blockSignals(false);
    }

    void updateW3(float w3) {
        w3Slider->blockSignals(true); // ループ防止！
        w3Slider->setValue(static_cast<int>(w3 * 10));
        w3Edit->setText(QString::number(w3, 'f', 1));
        w3Slider->blockSignals(false);
    }


    void updateK0(float k0) {
        k0Slider->blockSignals(true); // ループ防止！
        k0Slider->setValue(static_cast<int>(k0 * 100));
        k0Edit->setText(QString::number(k0, 'f', 2));
        k0Slider->blockSignals(false);
    }

    void updateK1(float k1) {
        k1Slider->blockSignals(true); // ループ防止！
        k1Slider->setValue(static_cast<int>(k1 * 100));
        k1Edit->setText(QString::number(k1, 'f', 2));
        k1Slider->blockSignals(false);
    }

};

