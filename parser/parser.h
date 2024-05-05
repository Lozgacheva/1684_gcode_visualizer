class Parser {
private:
    QGraphicsScene* scene;
    QStringList contain;

    QPointF current_pos;
    QString prev_com;

    QString prev_command;

    int x_temp;
    int y_temp;

    qreal coef;
public:
    Parser(QGraphicsScene* scene);
    bool open(QString&);
    int parse(qreal coef_inp);
    int parse_line(QString&, int&);
    int command_router(QString&, QStringList&, int&, QString&);

    int g0_command(QStringList&, int&, QString&);
    int g1_command(QStringList&, int&, QString&);
    int g2_command(QStringList&, int&, QString&);
    int g3_command(QStringList&, int&, QString&);
    int g4_command(QStringList);
    qreal calc_angle(QPointF&, QPointF&);
};