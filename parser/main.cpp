#include <QApplication>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLayout>
#include <QPushButton>
#include <QRandomGenerator>
#include <QWheelEvent>

#include "parser.h"

// Создаем пользовательский класс, наследующий QGraphicsView
class GraphicsView : public QGraphicsView {
public:
  GraphicsView(QWidget *parent = nullptr) : QGraphicsView(parent) {
    // setRenderHint(QPainter::Antialiasing); // Включаем сглаживание для более
                                           // качественного отображения
  }

  void wheelEvent(QWheelEvent *event) override {
      qreal scaleFactor = 1.15;
      if (event->angleDelta().y() < 0)
        scaleFactor = 1.0 / scaleFactor;
      scale(scaleFactor, scaleFactor); // Масштабирование при помощи колеса мыши
  }
};

// Генерирует случайные кривые
QPainterPath generateRandomPath() {
  QPainterPath path;
  // Генерируем случайное количество точек для кривой в диапазоне от 1 до 4
  int numPoints = QRandomGenerator::global()->bounded(1, 2);
  // Генерируем начальную точку кривой в пределах большой области
  QPointF startPoint(QRandomGenerator::global()->bounded(-800, 800),
                     QRandomGenerator::global()->bounded(-600, 600));
  path.moveTo(startPoint); // Устанавливаем начальную точку кривой
  for (int i = 0; i < numPoints; ++i) {
    // Генерируем случайные конечную точку и две управляющие точки для
    QPointF endPoint(QRandomGenerator::global()->bounded(-800, 800),
                     QRandomGenerator::global()->bounded(-600, 600));
    QPointF controlPoint1(QRandomGenerator::global()->bounded(-800, 800),
                          QRandomGenerator::global()->bounded(-600, 600));
    QPointF controlPoint2(QRandomGenerator::global()->bounded(-800, 800),
                          QRandomGenerator::global()->bounded(-600, 600));
    path.cubicTo(controlPoint1, controlPoint2,
                 endPoint); // Добавляем кривую на сцену
  }
  return path;
}


#include <sys/resource.h>

void setMemoryLimit(rlim_t maxMemory) {
    struct rlimit rl;
    rl.rlim_cur = maxMemory; // Устанавливаем текущий лимит
    rl.rlim_max = maxMemory; // Устанавливаем максимальный лимит

    if (setrlimit(RLIMIT_AS, &rl) != 0) {
        qDebug() << "Failed to set memory limit!";
    }
}


int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

   // setMemoryLimit(400 * 1024 * 1024);

  QGraphicsScene scene;
  scene.setSceneRect(
      -9000, -7000, 18000,
      14000); // Устанавливаем размер сцены и ее начальные координаты

  // Добавляем 1000 случайных кривых на сцену для более плотного заполнения
  /*for (int i = 0; i < 500; ++i) {
    QPainterPath path = generateRandomPath();
    scene.addPath(
        path, QPen(Qt::black)); // Добавляем кривую на сцену с черным контуром
  } */

  QString gcode_file = "files/Orcs.gc";

  Parser parser(&scene);
  if(parser.open(gcode_file)) {
      parser.parse();
  }

  GraphicsView view;
  view.setScene(&scene);
  // Устанавливаем режим обновления вида только при изменении охвата сцены для
  // улучшения производительности
  view.setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

  // Создаем кнопки увеличения и уменьшения масштаба
  QPushButton zoomInButton("+");
  QPushButton zoomOutButton("-");

  // Связываем кнопки с соответствующими слотами
  QObject::connect(&zoomInButton, &QPushButton::clicked, [&view](){
      view.scale(1.2, 1.2); // Увеличиваем масштаб вида
  });

  QObject::connect(&zoomOutButton, &QPushButton::clicked, [&view](){
      view.scale(1/1.2, 1/1.2); // Уменьшаем масштаб вида
  });

  // Создаем вертикальный layout для размещения кнопок и виджета QGraphicsView
  QVBoxLayout layout;
  layout.addWidget(&view);
  layout.addWidget(&zoomInButton);
  layout.addWidget(&zoomOutButton);

  // Создаем основной виджет и устанавливаем layout
  QWidget widget;
  widget.setLayout(&layout);
  widget.show();

    return app.exec();
}
