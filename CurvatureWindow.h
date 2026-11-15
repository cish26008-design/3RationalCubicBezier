#pragma once
#include <QWidget>
#include <vector>

class CurvatureWindow : public QWidget {
    Q_OBJECT
public:
    explicit CurvatureWindow(QWidget *parent = nullptr);
    ~CurvatureWindow();

    // メインウィンドウから曲率データを受け取るための関数
    void updatePlotData(const std::vector<float>& arcLengths, const std::vector<float>& curvatures);

protected:
    // ここで実際の描画処理を行う（QPainterを使う場合など）
    void paintEvent(QPaintEvent *event) override;

private:
    std::vector<float> m_curvatureX;
    std::vector<float> m_curvatureY;
    float m_K0max = 0.0f;
    float m_K1min = 0.0f;
    bool m_hasLimits = false;

public:
    void updatePlotData(const std::vector<float>& curvatureX, const std::vector<float>& curvatureY, float k0max = 0.0f, float k1min = 0.0f);
};
