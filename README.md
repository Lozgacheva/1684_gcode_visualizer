Использование парсера в main:
```c
Parser parser;
if(parser.open(gcode_file)) {
    parser.parse(20);
}
scene.addPath(parser.get_path(), QPen(Qt::black));
```


Использование парсера для имитации отслеживания процесса фрезирования:
```c
Parser_process parser_process;
if(parser_process.open(gcode_file)) {
    parser_process.parse(20);
}

QGraphicsEllipseItem m_actor(0, 0, 10, 10);

m_actor.setBrush(QBrush(QColor(255, 0, 0)));
window.scene.addItem(&m_actor);

QTimeLine *timer = new QTimeLine(550000, &window.view);
QGraphicsItemAnimation *animation = new QGraphicsItemAnimation(timer);
timer->setEasingCurve(QEasingCurve::Linear);
animation->setItem(&m_actor);
animation->setTimeLine(timer);
QPointF coef(5, 5);
QPainterPath path_process = parser_process.get_path();
for(qreal j = 0; j <= 1; j += 0.0001) {
    animation->setPosAt(j, path_process.pointAtPercent(j) - coef);
}
timer->start();
```