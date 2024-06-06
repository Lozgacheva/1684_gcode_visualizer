#include "scene.h"

// Реализация конструктора GraphicsView
GraphicsView::GraphicsView(QWidget *parent) : QGraphicsView(parent) {
    setRenderHint(QPainter::Antialiasing); // Включаем сглаживание для более качественного отображения
    grabGesture(Qt::PinchGesture); // Обрабатываем жесты масштабирования
    grabGesture(Qt::PanGesture); // Обрабатываем жесты перетаскивания
}

// Реализация события прокрутки колеса мыши
void GraphicsView::wheelEvent(QWheelEvent *event) {
    qreal scaleFactor = 1.15;
    if (event->angleDelta().y() < 0)
        scaleFactor = 1.0 / scaleFactor;
    scale(scaleFactor, scaleFactor); // Масштабирование при помощи колеса мыши
}

// Реализация обработки событий
bool GraphicsView::event(QEvent *event) {
    if (event->type() == QEvent::Gesture) {
        return gestureEvent(static_cast<QGestureEvent*>(event));
    }
    return QGraphicsView::event(event);
}

// Реализация обработки жестов
bool GraphicsView::gestureEvent(QGestureEvent *event) {
    if (QGesture *pinch = event->gesture(Qt::PinchGesture)) {
        QPinchGesture *pinchGesture = static_cast<QPinchGesture*>(pinch);
        qreal factor = pinchGesture->scaleFactor();
        scale(factor, factor); // Масштабирование при помощи жеста сжатия
    }

    if (QGesture *pan = event->gesture(Qt::PanGesture)) {
        QPanGesture *panGesture = static_cast<QPanGesture*>(pan);
        QPointF delta = panGesture->delta();
        translate(delta.x(), delta.y()); // Перемещение сцены при помощи жеста панорамирования
    }

    return true;
}

// Реализация функции для генерации случайных кривых
QPainterPath generateRandomPath() {
    QPainterPath path;
    // Генерируем случайное количество точек для кривой в диапазоне от 1 до 2
    int numPoints = QRandomGenerator::global()->bounded(1, 2);
    // Генерируем начальную точку кривой в пределах большой области
    QPointF startPoint(QRandomGenerator::global()->bounded(-800, 800),
                       QRandomGenerator::global()->bounded(-600, 600));
    path.moveTo(startPoint); // Устанавливаем начальную точку кривой
    for (int i = 0; i < numPoints; ++i) {
        // Генерируем случайные конечную точку и две управляющие точки для кубической кривой Безье
        QPointF endPoint(QRandomGenerator::global()->bounded(-800, 800),
                         QRandomGenerator::global()->bounded(-600, 600));
        QPointF controlPoint1(QRandomGenerator::global()->bounded(-800, 800),
                              QRandomGenerator::global()->bounded(-600, 600));
        QPointF controlPoint2(QRandomGenerator::global()->bounded(-800, 800),
                              QRandomGenerator::global()->bounded(-600, 600));
        path.cubicTo(controlPoint1, controlPoint2, endPoint); // Добавляем кривую на сцену
    }
    return path;
}

// Реализация функции для установки лимита по памяти
void setMemoryLimit(rlim_t maxMemory) {
    struct rlimit rl;
    rl.rlim_cur = maxMemory; // Устанавливаем текущий лимит
    rl.rlim_max = maxMemory; // Устанавливаем максимальный лимит

    if (setrlimit(RLIMIT_AS, &rl) != 0) {
        qDebug() << "Failed to set memory limit!"; // Выводим сообщение об ошибке, если не удалось установить лимит
    }
}
