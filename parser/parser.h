/*!
\brief Класс парсера gcode файла
*/
class Parser {
private:
    /*!
    \brief хранит содержимое gcode файла
    */
    QStringList contain;
    /*!
    \brief итоговый путь, который добавляется на сцену для визуализации
    */
    QPainterPath main_path;
    /*!
    \brief текущая позиция
    */
    QPointF current_pos;
    /*!
    \brief предыдущая команда. Используется, если строчка начинается с параметров, а не команды
    */
    QString prev_command;
    /*!
    \brief коээфициент увеличения изображения
    */
    qreal coef;
public:
    /*!
        \brief конструктор
        в конструкторе инициализируются нулями точки current_pos - текующей позиции
    */
    Parser();

    /*!
        \brief команда открытия и чтения g-code файла
        \param gcode_file путь gcode файла
        \return 1 - если файл успешно открыт, 0 - иначе 
    */
    int open(QString&);

    /*!
        \brief команда, содержащая один цикл прохода по строкам файла g-code
        \param coef_inp коэффициент для увеличения изображения
        \return 1 - если не возникло ошибок, 0 - иначе
    */
    int parse(qreal coef_inp);

    /*!
        \brief парсер одной линии
        \param str строка, которая парсится
        \param str_num номер строки
        \return 1 - если не возникло ошибок, 0 - иначе
    */
    int parse_line(QString&, int&);

    /*!
        \brief маршрутизация команд
        \param current_command команда g-code
        \param params список параметров команды
        \param str_num номер строки для вывода ошибки
        \param str исходная строка для вывода ошибки
        \return 1 - если не возникло ошибок, 0 - иначе
    */
    virtual int command_router(QString&, QStringList&, int&, QString&);

    /*!
        \brief команда, считывающая цифры в "слове" и добавляющая их к команде G-Code или параметру команды
        \param current_command текущая команда, к которой добавляются цифры
        \param str_num номер строки для вывода ошибки
        \param i индекс символа в исходной строке без пробелов
        \param str исходная строка для вывода ошибки
        \return 1 - если не возникло ошибок, 0 - иначе
    */
    int read_num(QString& current_command, QString& current_str, int& str_num, int& i, QString& str);

    /*!
        \brief G0. Команда холостого хода
        \param params параметры команды (X, Y)
        \param str_num номер строки для вывода ошибки
        \param str исходная строка для вывода ошибки
        \return 1 - если не возникло ошибок, 0 - иначе
        Команда холостого хода считывает параметры из params и меняет curren_pos (текущую позицию), на new_pos (новую позицию из params)
    */
    int g0_command(QStringList&, int&, QString&);

    /*!
        \brief G1. Команда линейной интерполяции
        \param params параметры команды (X, Y)
        \param str_num номер строки для вывода ошибки
        \param str исходная строка для вывода ошибки
        \return 1 - если не возникло ошибок, 0 - иначе
        Команда линейной интерполяции считывает параметры из params, добавляет к main_path (итоговый путь) линию от current_pos (текущей позции) до new_pos (новой позиции из params); Меняет current_pos на new_pos 
    */
    int g1_command(QStringList&, int&, QString&);

    /*!
        \brief G2. круговая интерполяция по часовой стрелке
        \param params параметры команды (X, Y, I, J)
        \param str_num номер строки для вывода ошибки
        \param str исходная строка для вывода ошибки
        \return 1 - если не возникло ошибок, 0 - иначе
        Команда круговой интерполяции по часовой стрелке считывает параметры из params; Считает углы между current_pos (текущей позицией) и осью X и между new_pos (новой позицией из params) и осью X; Добавляет в main_path (итоговый путь) соответствующую дугу
    */
    int g2_command(QStringList&, int&, QString&);

    /*!
        \brief G3 круговая интерполяция против часовой стрелки
        \param params параметры команды (X, Y, I, J)
        \param str_num номер строки для вывода ошибки
        \param str исходная строка для вывода ошибки
        \return 1 - если не возникло ошибок, 0 - иначе
        Команда круговой интерполяции против часовой стрелке считывает параметры из params; Считает углы между current_pos (текущей позицией) и осью X и между new_pos (новой позицией из params) и осью X; Добавляет в main_path (итоговый путь) соответствующую дугу
    */
    int g3_command(QStringList&, int&, QString&);

    /*!
        \brief команда подсчета угла для круговой интерполяции
        \param current_pos текущая позиция инструмента
        \param center центр круга
        \return угол между осью x и cuurent_pos по часовой стрелке
    */
    qreal calc_angle(QPointF&, QPointF&);
    
    /*!
        \brief функция вывода ошибки
        \param c символ, на котором произошла ошибка
        \param str_num номер строки, на которой произошла ошибка
        \param str исходная строка, на которой произошла ошибка
        \return 0
        выводит на консоль "Something wrong with: ", символ на котором произошла ошибка, строку и ее номер из файла gcode
    */
    int error_param(const QString& c, int& str_num, QString& str);
    
    /*!
        \brief метод получения итогового объекта класса QPainterPath
        \return main_path - итоговый объект класса QPainterPath
    */
    QPainterPath get_path();
};