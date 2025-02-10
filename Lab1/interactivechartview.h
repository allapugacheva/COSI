#ifndef INTERACTIVECHARTVIEW_H
#define INTERACTIVECHARTVIEW_H

#include <QChartView>
#include <QChart>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QPointF>
#include <QRectF>
#include <QValueAxis>

class InteractiveChartView : public QChartView {
    Q_OBJECT
public:
    explicit InteractiveChartView(QChart* chart, QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    bool m_isDragging;
    QPoint m_lastMousePos;

public slots:
    void resetView();
};

#endif // INTERACTIVECHARTVIEW_H
