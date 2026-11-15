#include "curvatureWindow.h"
#include <QPainter>
#include <cmath>

CurvatureWindow::CurvatureWindow(QWidget *parent) : QWidget(parent, Qt::Window) {
    // ウィンドウのタイトルと初期サイズだけ設定しておく
    setWindowTitle(tr("Curvature Plot"));
    resize(500, 500);
}

CurvatureWindow::~CurvatureWindow() {
}


void CurvatureWindow::updatePlotData(const std::vector<float>& curvatureX, const std::vector<float>& curvatureY, float k0max, float k1min) {
    m_curvatureX = curvatureX;
    m_curvatureY = curvatureY;

    m_K0max = k0max;
    m_K1min = k1min;
    m_hasLimits = true;
    update(); // データが来たら、画面を再描画（paintEventを呼び出す）する
}

void CurvatureWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 線をなめらかにする設定
    painter.fillRect(rect(), Qt::white);          // 背景を白で塗りつぶす

    // 1. グラフを描くエリアの「余白（マージン）」を決める
    //  t=0を左端にするため、marginLeftを小さくする（ラベル表示用だけ残す）
    int marginLeft   = 10; // 60 -> 10 に変更
    int marginRight  = 10; // 40 -> 10 に変更
    int marginTop    = 30; // 40 -> 30 に変更（ラベル用）
    int marginBottom = 30; // 60 -> 30 に変更（ラベル用）

    int graphWidth  = width() - marginLeft - marginRight;
    int graphHeight = height() - marginTop - marginBottom;

    // 2. 原点 (0, 0) の画面上の位置を決める
    // X軸はt=0なので左寄り。Y軸は下寄りに原点（曲率が負になることが多いため）
    int originX = marginLeft; //  t=0 の位置
    int originY = marginTop + graphHeight * 0.5; // 原点位置は下寄り

    // 3. 軸を描く（メイン画面に合わせて、X軸＝赤、Y軸＝緑）
    QPen axisPen;
    axisPen.setWidth(2);

    // X軸（赤）
    axisPen.setColor(Qt::red);
    painter.setPen(axisPen);
    // 軸をグラフ表示範囲だけにする（余白あり）
    painter.drawLine(marginLeft, originY, width() - marginRight, originY);

    // Y軸（緑）
    axisPen.setColor(Qt::green);
    painter.setPen(axisPen);
    // 軸をグラフ表示範囲だけにする（余白あり）
    painter.drawLine(marginLeft, marginTop, marginLeft, height() - marginBottom);

    // X軸、Y軸のラベルを追加
    QPen textPen(Qt::darkGray);
    painter.setPen(textPen);
    // X軸ラベル "t"
    painter.drawText(width() - marginRight - 10, originY + 15, "t");
    // Y軸ラベル "Curvature (K)"
    painter.drawText(marginLeft - 5, marginTop - 10, "Curvature (K)");

    // 4. データが空なら戻る
    if (m_curvatureX.empty() || m_curvatureY.empty() || m_curvatureX.size() != m_curvatureY.size()) {
        return;
    }

    // 5. スケール計算
    float maxX = 1.0f;
    float maxK = 0.0f;

    for (float c : m_curvatureY) {
        if (std::isnan(c) || std::isinf(c)) continue;
        if (std::abs(c) > maxK) maxK = std::abs(c);
    }

    // ▼ 追加：K0max と K1min の値も最大値チェックに含める（線が画面外に消えないようにする）
    if (m_hasLimits) {
        if (std::abs(m_K0max) > maxK) maxK = std::abs(m_K0max);
        if (std::abs(m_K1min) > maxK) maxK = std::abs(m_K1min);
    }

    float maxY = maxK * 1.5f;
    if (maxY < 0.1f) maxY = 0.1f;

    double scaleX = graphWidth / maxX;
    double scaleY = (graphHeight * 0.5) / maxY;

    if (m_hasLimits) {
        QPen limitPen;
        limitPen.setStyle(Qt::DashLine); // 点線・破線にする
        limitPen.setWidth(1);

        // K0max の線（青色で描画）
        limitPen.setColor(Qt::blue);
        painter.setPen(limitPen);
        double y0max = originY - m_K0max * scaleY;
        painter.drawLine(marginLeft, y0max, width() - marginRight, y0max);
        painter.drawText(marginLeft + 10, y0max - 5, "K0max"); // ラベルも添える

        // K1min の線（オレンジ色で描画）
        limitPen.setColor(QColor(255, 165, 0));
        painter.setPen(limitPen);
        double y1min = originY - m_K1min * scaleY;
        painter.drawLine(marginLeft, y1min, width() - marginRight, y1min);
        painter.drawText(marginLeft + 10, y1min - 5, "K1min"); // ラベルも添える
    }

    // 6. 点の変換
    QPolygonF points;
    for (size_t i = 0; i < m_curvatureX.size(); ++i) {
        float currentK = m_curvatureY[i];

        if (std::isnan(currentK) || std::isinf(currentK)) {
            continue;
        }

        double screenX = originX + m_curvatureX[i] * scaleX;
        double screenY = originY - currentK * scaleY;
        points.append(QPointF(screenX, screenY));
    }

    // 7. 描画
    QPen plotPen(Qt::black, 2);
    painter.setPen(plotPen);
    painter.drawPolyline(points);
}
