#include "settingswindow.h"
#include <QBoxLayout>
#include <QGridLayout>
#include <QDoubleValidator>


SettingsWindow* settingsWindow = nullptr;

SettingsWindow::SettingsWindow(QWidget *parent) :
    QWidget(parent, Qt::Window)
{
    setWindowTitle("Settings");

    QBoxLayout *vboxLayout = new QVBoxLayout();
    QGridLayout *gridLayout = new QGridLayout();
    int row = 0;

    vboxLayout->addLayout(gridLayout);


    //Combのチェックボックス
    combCheckBox.reset(new QCheckBox(tr("Comb"), this));
    combCheckBox->setChecked(false); // 初期状態を「表示（ON）」にしておく

    // チェックボックスが切り替わったらシグナルを発信
    connect(combCheckBox.get(), &QCheckBox::toggled, this, [this](bool checked) {
        emit CombCheck(checked);
    });

    // レイアウトに追加 (例: row行目の0列目から3列分使って配置)
    gridLayout->addWidget(combCheckBox.get(), row, 0, 1, 3);
    row++;

    //曲率円のチェックボックス
    CofcurvatureCheckBox.reset(new QCheckBox(tr("circle"), this));
    CofcurvatureCheckBox->setChecked(false); // 初期状態を「表示（ON）」にしておく

    // チェックボックスが切り替わったらシグナルを発信
    connect(CofcurvatureCheckBox.get(), &QCheckBox::toggled, this, [this](bool checked) {
        emit CofCurvature(checked);
    });

    // レイアウトに追加 (例: row行目の0列目から3列分使って配置)
    gridLayout->addWidget(CofcurvatureCheckBox.get(), row, 0, 1, 3);
    row++;

    // K0スライダーの配置（-5.0 〜 5.0 を想定）
    k0Label.reset(new QLabel(tr("Curvature K0")));
    k0Slider.reset(new QSlider(Qt::Horizontal, this));
    k0Slider->setRange(-500, 500); // 0.01単位で -5.00〜5.00
    k0Slider->setValue(0);

    k0Edit.reset(new QLineEdit("0.00"));
    k0Edit->setValidator(new QDoubleValidator(-5.0, 5.0, 2, this));
    k0Edit->setFixedWidth(50);

    // K0スライダーが動いたらMainWindowへ通知
    connect(k0Slider.get(), &QSlider::valueChanged, this, [this](int value){
        float floatVal = value * 0.01f;
        k0Edit->setText(QString::number(floatVal, 'f', 2));
        emit curvatureK0Changed(floatVal); // 新しいシグナル
    });

    // Edit手入力時
    connect(k0Edit.get(), &QLineEdit::textEdited, this, [this](const QString &text) {
        float floatVal = text.toFloat();
        k0Slider->blockSignals(true);
        k0Slider->setValue(static_cast<int>(floatVal * 100));
        k0Slider->blockSignals(false);
        emit curvatureK0Changed(floatVal);
    });

    // レイアウトに追加（w0〜w3のさらに下の行に）
    gridLayout->addWidget(k0Label.get(), row, 0);
    gridLayout->addWidget(k0Slider.get(), row, 1);
    gridLayout->addWidget(k0Edit.get(), row, 2);
    row++;

    //k1slider
    k1Label.reset(new QLabel(tr("Curvature K1")));
    k1Slider.reset(new QSlider(Qt::Horizontal, this));
    k1Slider->setRange(-500, 500); // 0.01単位で -5.00〜5.00
    k1Slider->setValue(0);

    k1Edit.reset(new QLineEdit("0.00"));
    k1Edit->setValidator(new QDoubleValidator(-5.0, 5.0, 2, this));
    k1Edit->setFixedWidth(50);

    connect(k1Slider.get(), &QSlider::valueChanged, this, [this](int value){
        float floatVal = value * 0.01f;
        k1Edit->setText(QString::number(floatVal, 'f', 2));
        emit curvatureK1Changed(floatVal); // 新しいシグナル
    });

    connect(k1Edit.get(), &QLineEdit::textEdited, this, [this](const QString &text) {
        float floatVal = text.toFloat();
        k1Slider->blockSignals(true);
        k1Slider->setValue(static_cast<int>(floatVal * 100));
        k1Slider->blockSignals(false);
        emit curvatureK1Changed(floatVal);
    });

    gridLayout->addWidget(k1Label.get(), row, 0);
    gridLayout->addWidget(k1Slider.get(), row, 1);
    gridLayout->addWidget(k1Edit.get(), row, 2);
    row++;



    setLayout(vboxLayout);

    //ウェイトの変更をできるように

    // w0スライダーの配置
    w0Label.reset(new QLabel(tr("Weight 0")));
    w0Slider.reset(new QSlider(Qt::Horizontal, this));
    w0Slider->setRange(0, 20);  // 0.10(1) 〜 5.00(50) まで動かせるようにする
    w0Slider->setValue(10); // 初期値 1.00(10)、
    w0Edit.reset(new QLineEdit("1.00"));
    // 0.0〜2.0の範囲で、小数点以下2桁までしか入力できないように制限する
    w0Edit->setValidator(new QDoubleValidator(0.0, 2.0, 2, this));
    w0Edit->setFixedWidth(50);

    // スライダーが動いたら小数値にして返す
    connect(w0Slider.get(), &QSlider::valueChanged, this, [this](int value){
        float floatVal = value * 0.1f;
        w0Edit->setText(QString::number(floatVal, 'f', 2));
        emit weight0Changed(floatVal);
    });

    connect(w0Edit.get(), &QLineEdit::textEdited, this, [this](const QString &text) {
        float floatVal = text.toFloat();
        int sliderVal = static_cast<int>(floatVal * 100.0f);

        w0Slider->blockSignals(true);  // ループを防ぐためにスライダーの信号を止める
        w0Slider->setValue(sliderVal); // スライダーを動かす
        w0Slider->blockSignals(false); // 信号を再開

        emit weight0Changed(floatVal); // メイン画面にも変更を伝える
    });

    // 適切な row（行）にウィジェットを追加
    gridLayout->addWidget(w0Label.get(), row, 0);
    gridLayout->addWidget(w0Slider.get(), row, 1);
    gridLayout->addWidget(w0Edit.get(), row, 2);
    row++;


    // w1スライダーの配置
    w1Label.reset(new QLabel(tr("Weight 1")));
    w1Slider.reset(new QSlider(Qt::Horizontal, this));
    w1Slider->setRange(0, 20);
    w1Slider->setValue(10);
    w1Edit.reset(new QLineEdit("1.00"));
    // 0.0〜2.0の範囲で、小数点以下1桁までしか入力できないように制限する
    w1Edit->setValidator(new QDoubleValidator(0.0, 2.0, 2, this));
    w1Edit->setFixedWidth(50);

    connect(w1Slider.get(), &QSlider::valueChanged, this, [this](int value){
        float floatVal = value * 0.1f;
        w1Edit->setText(QString::number(floatVal, 'f', 2));
        emit weight1Changed(floatVal);
    });

    connect(w1Edit.get(), &QLineEdit::textEdited, this, [this](const QString &text) {
        float floatVal = text.toFloat();
        int sliderVal = static_cast<int>(floatVal * 100.0f);

        w1Slider->blockSignals(true);  // ループを防ぐためにスライダーの信号を止める
        w1Slider->setValue(sliderVal); // スライダーを動かす
        w1Slider->blockSignals(false); // 信号を再開

        emit weight1Changed(floatVal); // メイン画面にも変更を伝える
    });

    gridLayout->addWidget(w1Label.get(), row, 0);
    gridLayout->addWidget(w1Slider.get(), row, 1);
    gridLayout->addWidget(w1Edit.get(), row, 2);
    row++;

    // w2スライダーの配置
    w2Label.reset(new QLabel(tr("Weight 2")));
    w2Slider.reset(new QSlider(Qt::Horizontal, this));
    w2Slider->setRange(0, 20);
    w2Slider->setValue(10);
    w2Edit.reset(new QLineEdit("1.00"));
    // 0.0〜2.0の範囲で、小数点以下1桁までしか入力できないように制限する
    w2Edit->setValidator(new QDoubleValidator(0.0, 2.0, 2, this));
    w2Edit->setFixedWidth(50);


    connect(w2Slider.get(), &QSlider::valueChanged, this, [this](int value){
        float floatVal = value * 0.1f;
        w2Edit->setText(QString::number(floatVal, 'f', 2));
        emit weight2Changed(floatVal);
    });

    connect(w2Edit.get(), &QLineEdit::textEdited, this, [this](const QString &text) {
        float floatVal = text.toFloat();
        int sliderVal = static_cast<int>(floatVal * 100.0f);

        w2Slider->blockSignals(true);  // ループを防ぐためにスライダーの信号を止める
        w2Slider->setValue(sliderVal); // スライダーを動かす
        w2Slider->blockSignals(false); // 信号を再開

        emit weight2Changed(floatVal); // メイン画面にも変更を伝える
    });


    gridLayout->addWidget(w2Label.get(), row, 0);
    gridLayout->addWidget(w2Slider.get(), row, 1);
    gridLayout->addWidget(w2Edit.get(), row, 2);
    row++;

    // w3スライダーの配置
    w3Label.reset(new QLabel(tr("Weight 3")));
    w3Slider.reset(new QSlider(Qt::Horizontal, this));
    w3Slider->setRange(0, 20);
    w3Slider->setValue(10);
    w3Edit.reset(new QLineEdit("1.00"));
    // 0.0〜2.0の範囲で、小数点以下1桁までしか入力できないように制限する
    w3Edit->setValidator(new QDoubleValidator(0.0, 2.0, 2, this));
    w3Edit->setFixedWidth(50);


    connect(w3Slider.get(), &QSlider::valueChanged, this, [this](int value){
        float floatVal = value * 0.1f;
        w3Edit->setText(QString::number(floatVal, 'f', 2));
        emit weight3Changed(floatVal);
    });

    connect(w3Edit.get(), &QLineEdit::textEdited, this, [this](const QString &text) {
        float floatVal = text.toFloat();
        int sliderVal = static_cast<int>(floatVal * 100.0f);

        w3Slider->blockSignals(true);  // ループを防ぐためにスライダーの信号を止める
        w3Slider->setValue(sliderVal); // スライダーを動かす
        w3Slider->blockSignals(false); // 信号を再開

        emit weight3Changed(floatVal); // メイン画面にも変更を伝える
    });


    gridLayout->addWidget(w3Label.get(), row, 0);
    gridLayout->addWidget(w3Slider.get(), row, 1);
    gridLayout->addWidget(w3Edit.get(), row, 2);
    row++;


}
