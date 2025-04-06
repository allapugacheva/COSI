#include "interactivechartview.h"

InteractiveChartView::InteractiveChartView(QChart* chart, QWidget* parent) : QChartView(chart, parent), m_isDragging(false) {}

void InteractiveChartView::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_isDragging = true;
        m_lastMousePos = event->pos();
    } else if (event->button() == Qt::RightButton)
        resetView();
    QChartView::mousePressEvent(event);
}

void InteractiveChartView::mouseMoveEvent(QMouseEvent* event) {
    if (m_isDragging) {
        QPointF delta = chart()->mapToValue(m_lastMousePos) - chart()->mapToValue(event->pos());

        QValueAxis* axisX = qobject_cast<QValueAxis*>(chart()->axes(Qt::Horizontal).first());
        QValueAxis* axisY = qobject_cast<QValueAxis*>(chart()->axes(Qt::Vertical).first());

        if (axisX && axisY) {
            axisX->setRange(axisX->min() + delta.x(), axisX->max() + delta.x());
            axisY->setRange(axisY->min() + delta.y(), axisY->max() + delta.y());
        }

        m_lastMousePos = event->pos();
    }
    QChartView::mouseMoveEvent(event);
}

void InteractiveChartView::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
    }
    QChartView::mouseReleaseEvent(event);
}

void InteractiveChartView::wheelEvent(QWheelEvent* event) {
    QPoint angleDelta = event->angleDelta();

    if (!angleDelta.isNull()) {
        qreal factor = (angleDelta.y() > 0) ? 1.1 : 0.9;
        chart()->zoom(factor);
    }
    QChartView::wheelEvent(event);
}

void InteractiveChartView::resetView() {

    chart()->zoomReset();
    chart()->zoom(0.95);
}
