#include <QApplication>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGestureEvent>
#include <QLayout>
#include <QPushButton>
#include <QRandomGenerator>
#include <QWheelEvent>

#include "parser.h"

// Создаем пользовательский класс, наследующий QGraphicsView
class GraphicsView : public QGraphicsView {
public:
    GraphicsView(QWidget *parent = nullptr) : QGraphicsView(parent) {
        setRenderHint(QPainter::Antialiasing); // Включаем сглаживание для более качественного отображения
        grabGesture(Qt::PinchGesture); // Обрабатываем жесты масштабирования
        grabGesture(Qt::PanGesture); // Обрабатываем жесты перетаскивания
    }

    // Переопределяем событие прокрутки колеса мыши
    void wheelEvent(QWheelEvent *event) override {
        qreal scaleFactor = 1.15;
        if (event->angleDelta().y() < 0)
            scaleFactor = 1.0 / scaleFactor;
        scale(scaleFactor, scaleFactor); // Масштабирование при помощи колеса мыши
    }

    // Переопределяем обработку событий
    bool event(QEvent *event) override {
        if (event->type() == QEvent::Gesture) {
            return gestureEvent(static_cast<QGestureEvent*>(event));
        }
        return QGraphicsView::event(event);
    }

    // Обрабатываем жесты
    bool gestureEvent(QGestureEvent *event) {
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
};

// Функция для генерации случайных кривых
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

#include <sys/resource.h>

// Функция для установки лимита по памяти
void setMemoryLimit(rlim_t maxMemory) {
    struct rlimit rl;
    rl.rlim_cur = maxMemory; // Устанавливаем текущий лимит
    rl.rlim_max = maxMemory; // Устанавливаем максимальный лимит

    if (setrlimit(RLIMIT_AS, &rl) != 0) {
        qDebug() << "Failed to set memory limit!"; // Выводим сообщение об ошибке, если не удалось установить лимит
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // setMemoryLimit(400 * 1024 * 1024); // Устанавливаем лимит по памяти (400 МБ)

    QGraphicsScene scene;
    scene.setSceneRect(
            -9000, -7000, 18000, 14000); // Устанавливаем размер сцены и ее начальные координаты

    // Добавляем 1000 случайных кривых на сцену для более плотного заполнения
    /*for (int i = 0; i < 500; ++i) {
        QPainterPath path = generateRandomPath();
        scene.addPath(
            path, QPen(Qt::black)); // Добавляем кривую на сцену с черным контуром
    } */

    QString gcode_file = "files/Orcs.gc"; // Имя файла с G-кодом

    Parser parser(&scene); // Создаем объект парсера
    if (parser.open(gcode_file)) {
        parser.parse(); // Парсим файл, если он успешно открыт
    }

    GraphicsView view;
    view.setScene(&scene); // Устанавливаем сцену для отображения
    // Устанавливаем режим обновления вида только при изменении охвата сцены для улучшения производительности
    view.setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    // Создаем кнопки увеличения и уменьшения масштаба
    QPushButton zoomInButton("+");
    QPushButton zoomOutButton("-");

    // Связываем кнопки с соответствующими слотами
    QObject::connect(&zoomInButton, &QPushButton::clicked, [&view]() {
        view.scale(1.2, 1.2); // Увеличиваем масштаб вида
    });

    QObject::connect(&zoomOutButton, &QPushButton::clicked, [&view]() {
        view.scale(1 / 1.2, 1 / 1.2); // Уменьшаем масштаб вида
    });

    // Создаем вертикальный layout для размещения кнопок и виджета QGraphicsView
    QVBoxLayout layout;
    layout.addWidget(&view);
    layout.addWidget(&zoomInButton);
    layout.addWidget(&zoomOutButton);

    // Создаем основной виджет и устанавливаем layout
    QWidget widget;
    widget.setLayout(&layout);
    widget.resize(600, 800); // Устанавливаем размер основного виджета
    widget.show(); // Показываем виджет

    return app.exec(); // Запускаем основной цикл приложения
}
