#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QApplication>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGestureEvent>
#include <QLayout>
#include <QPushButton>
#include <QRandomGenerator>
#include <QWheelEvent>
#include <QPainterPath>
#include <sys/resource.h>

#include "../parser/parser.h" // надо путь до парсера прописать

// Класс GraphicsView, наследующий QGraphicsView
class GraphicsView : public QGraphicsView {
    Q_OBJECT

public:
    GraphicsView(QWidget *parent = nullptr);

protected:
    void wheelEvent(QWheelEvent *event) override;
    bool event(QEvent *event) override;

private:
    bool gestureEvent(QGestureEvent *event);
};

// Функция для генерации случайных кривых
QPainterPath generateRandomPath();

// Функция для установки лимита по памяти
void setMemoryLimit(rlim_t maxMemory);

#endif // GRAPHICSVIEW_H
