/*!
\brief Класс парсера gcode файла для имитации визуализации процесса фрезирования. Наследуется от класса Parser, заменяя команду G0 на G1
*/
class Parser_process : public Parser {
private:
    QStringList contain;
    QPainterPath main_path;

    QPointF current_pos;

    QString prev_command;

    qreal coef;
public:
    int command_router(QString&, QStringList&, int&, QString&) override; 
};