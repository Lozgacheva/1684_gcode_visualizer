#include "scene.h"

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
