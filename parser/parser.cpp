#include <QtWidgets>
#include <QDebug>
#include "parser.h"
#include <map>
#include <QThread>

/*!
    \brief конструктор
    \param scene_inp виджет, на котором будет происходить отрисовка
*/
Parser :: Parser(QGraphicsScene* scene_inp) {
    scene = scene_inp;
    current_pos.rx() = 0;
    current_pos.rx() = 0;
}


/*!
    \brief G0. Команда холостого хода
    \param params параметры команды
*/
int Parser :: g0_command(QStringList& params, int& str_num, QString& str) {
    int flag_x = 0;
    int flag_y = 0;
    QPointF new_pos = current_pos;
    for(int i = 0; i < params.length(); i++) {
        if(params.at(i).at(0) == "X") {
            if(!flag_x) {
                if(params.at(i).length() > 1) {
                    new_pos.rx() = params.at(i).mid(1).toFloat();
                }
                else {
                    qDebug() << "Wrong parameter " << params.at(i) << " at line " << str_num + 1 << " : " << str;
                    return 0;
                }
            }
            else {
                qDebug() << "Wrong parameter " << params.at(i) << " at line " << str_num + 1 << " : " << str;
                return 0;
            }
        }
        else if(params.at(i).at(0) == "Y") {
            if(!flag_x) {
                if(params.at(i).length() > 1) {
                    new_pos.ry() = -params.at(i).mid(1).toFloat();
                }
                else {
                    qDebug() << "Wrong parameter " << params.at(i) << " at line " << str_num + 1 << " : " << str;
                    return 0;
                }
            }
            else {
                qDebug() << "Wrong parameter " << params.at(i) << " at line " << str_num + 1 << " : " << str;
                return 0;
            }
        }
    }
    current_pos = new_pos;
    return 1;
}



/*!
    \brief G1. Команда линейной интерполяции
    \param params параметры команды
*/
int Parser :: g1_command(QStringList& params, int& str_num, QString& str) {
    int flag_x = 0;
    int flag_y = 0;
    QPointF new_pos = current_pos;
    for(int i = 0; i < params.length(); i++) {
        if(params.at(i).at(0) == "X") {
            
            if(!flag_x) {
                if(params.at(i).length() > 1) {
                    new_pos.rx() = params.at(i).mid(1).toFloat();
                }
                else {
                    qDebug() << "Wrong parameter " << params.at(i) << " at line " << str_num + 1 << " : " << str;
                    return 0;
                }
            }
            else {
                qDebug() << "Wrong parameter " << params.at(i) << " at line " << str_num + 1 << " : " << str;
                return 0;
            }
        }
        else if(params.at(i).at(0) == "Y") {
            if(!flag_x) {
                if(params.at(i).length() > 1) {
                    new_pos.ry() = -params.at(i).mid(1).toFloat();
                }
                else {
                    qDebug() << "Wrong parameter " << params.at(i) << " at line " << str_num + 1 << " : " << str;
                    return 0;
                }
            }
            else {
                qDebug() << "Wrong parameter " << params.at(i) << " at line " << str_num + 1 << " : " << str;
                return 0;
            }
        }
    }
    QPainterPath path;
    path.moveTo(current_pos);
    path.lineTo(new_pos);
    scene -> addPath(path, QPen(Qt::black));
    current_pos = new_pos;
    return 1;
}



/*!
    \brief G2 круговая интерполяция по часовой стрелке
    \param params параметры команды
*/
int Parser :: g2_command(QStringList& params, int& str_num, QString& str) {
    int flag_x = 0;
    int flag_y = 0;
    int flag_i = 0;
    int flag_j = 0;
    QPointF new_pos = current_pos;
    QPointF center = current_pos;
    for(int i = 0; i < params.length(); i++) {
        if(params.at(i).at(0) == "X") {
            if(!flag_x) {
                if(params.at(i).length() > 1) {
                    new_pos.rx() = params.at(i).mid(1).toFloat();
                }
                else {
                    qDebug() << "Wrong parameter " << params.at(i) << " at line " << str_num + 1 << " : " << str;
                    return 0;
                }
            }
            else {
                qDebug() << "Wrong parameter " << params.at(i) << " at line " << str_num + 1 << " : " << str;
                return 0;
            }
        }
        else if(params.at(i).at(0) == "Y") {
            if(!flag_x) {
                if(params.at(i).length() > 1) {
                    new_pos.ry() = -params.at(i).mid(1).toFloat();
                }
                else {
                    qDebug() << "Wrong parameter " << params.at(i) << " at line " << str_num + 1 << " : " << str;
                    return 0;
                }
            }
            else {
                qDebug() << "Wrong parameter " << params.at(i) << " at line " << str_num + 1 << " : " << str;
                return 0;
            }
        }
        else if(params.at(i).at(0) == "I") {
            if(!flag_x) {
                if(params.at(i).length() > 1) {
                    center.rx() += params.at(i).mid(1).toFloat();
                }
                else {
                    qDebug() << "Wrong parameter " << params.at(i) << " at line " << str_num + 1 << " : " << str;
                    return 0;
                }
            }
            else {
                qDebug() << "Wrong parameter " << params.at(i) << " at line " << str_num + 1 << " : " << str;
                return 0;
            }
        }
        else if(params.at(i).at(0) == "J") {
            if(!flag_x) {
                if(params.at(i).length() > 1) {
                    center.ry() -= params.at(i).mid(1).toFloat();
                }
                else {
                    qDebug() << "Wrong parameter " << params.at(i) << " at line " << str_num + 1 << " : " << str;
                    return 0;
                }
            }
            else {
                qDebug() << "Wrong parameter " << params.at(i) << " at line " << str_num + 1 << " : " << str;
                return 0;
            }
        }
    }

    qreal angle1 = calc_angle(current_pos, center);
    qreal angle2 = calc_angle(new_pos, center);
    qreal r = qSqrt(qPow(current_pos.rx() - center.rx(), 2) + qPow(current_pos.ry() - center.ry(), 2));
        
    QPainterPath path;
    path.moveTo(new_pos);
    qreal len;
    if(angle1 > angle2) {
        len = angle1 - angle2;
    }
    else {
        len = 360 - angle1 - angle2;
    }
    path.arcTo(center.rx() - r, center.ry() - r, 2*r, 2*r, angle2, len);
    scene -> addPath(path, QPen(Qt::black));
    current_pos = new_pos;
    return 1;
}




/*!
    \brief G3 круговая интерполяция против часовой стрелки
    \param params параметры команды
*/
int Parser :: g3_command(QStringList& params, int& str_num, QString& str) {
    int flag_x = 0;
    int flag_y = 0;
    int flag_i = 0;
    int flag_j = 0;
    QPointF new_pos = current_pos;
    QPointF center = current_pos;
    for(int i = 0; i < params.length(); i++) {
        if(params.at(i).at(0) == "X") {
            if(!flag_x) {
                if(params.at(i).length() > 1) {
                    new_pos.rx() = params.at(i).mid(1).toFloat();
                }
                else {
                    qDebug() << "Wrong parameter " << params.at(i) << " at line " << str_num + 1 << " : " << str;
                    return 0;
                }
            }
            else {
                qDebug() << "Wrong parameter " << params.at(i) << " at line " << str_num + 1 << " : " << str;
                return 0;
            }
        }
        else if(params.at(i).at(0) == "Y") {
            if(!flag_x) {
                if(params.at(i).length() > 1) {
                    new_pos.ry() = -params.at(i).mid(1).toFloat();
                }
                else {
                    qDebug() << "Wrong parameter " << params.at(i) << " at line " << str_num + 1 << " : " << str;
                    return 0;
                }
            }
            else {
                qDebug() << "Wrong parameter " << params.at(i) << " at line " << str_num + 1 << " : " << str;
                return 0;
            }
        }
        else if(params.at(i).at(0) == "I") {
            if(!flag_x) {
                if(params.at(i).length() > 1) {
                    center.rx() += params.at(i).mid(1).toFloat();
                }
                else {
                    qDebug() << "Wrong parameter " << params.at(i) << " at line " << str_num + 1 << " : " << str;
                    return 0;
                }
            }
            else {
                qDebug() << "Wrong parameter " << params.at(i) << " at line " << str_num + 1 << " : " << str;
                return 0;
            }
        }
        else if(params.at(i).at(0) == "J") {
            if(!flag_x) {
                if(params.at(i).length() > 1) {
                    center.ry() -= params.at(i).mid(1).toFloat();
                }
                else {
                    qDebug() << "Wrong parameter " << params.at(i) << " at line " << str_num + 1 << " : " << str;
                    return 0;
                }
            }
            else {
                qDebug() << "Wrong parameter " << params.at(i) << " at line " << str_num + 1 << " : " << str;
                return 0;
            }
        }
    }

    //раасчет угла начала
    //проекция начальной точки на линию параллельную x проходлящую через центр
    qreal angle1 = calc_angle(current_pos, center);
    qreal angle2 = calc_angle(new_pos, center);
    qreal r = qSqrt(qPow(current_pos.rx() - center.rx(), 2) + qPow(current_pos.ry() - center.ry(), 2));
    
    QPainterPath path;
    path.moveTo(current_pos);
    qreal len;
    qDebug() << new_pos;
    qDebug() << angle1 << angle2;
    if(angle1 < angle2) {
        len = angle2 - angle1;
    }
    else {
        len = 360 - angle2 - angle1;
    }
    qDebug() << len;
    path.arcTo(center.rx() - r, center.ry() - r, 2*r, 2*r, angle1, len);
    scene -> addPath(path, QPen(Qt::black));
    current_pos = new_pos;
    return 1;
}





qreal Parser :: calc_angle(QPointF& current_pos, QPointF& center){
    qreal angle;
    if(current_pos.rx() == center.rx() && current_pos.ry() < center.ry()){
        angle = 90;
        return angle;
    }
    else if(current_pos.rx() == center.rx() && current_pos.ry() > center.ry()) {
        angle = 270;
        return angle;
    }
    else if(current_pos.rx() > center.rx() && current_pos.ry() == center.ry()) {
        angle = 0;
        return angle;
    }
    else if (current_pos.rx() < center.rx() && current_pos.ry() == center.ry()) {
        angle = 180;
        return angle;
    }
    qreal tan1 = qAbs(current_pos.ry() - center.ry()) / qAbs(current_pos.rx() - center.rx());
    angle = qAtan(tan1) * 180 / M_PI;
    if(current_pos.rx() < center.rx() && current_pos.ry() < center.ry()) {
        angle = 180 - angle;
    }
    else if(current_pos.rx() < center.rx() && current_pos.ry() > center.ry()) {
        angle += 180;
    }
    else if(current_pos.rx() > center.rx() && current_pos.ry() > center.ry()) {
        angle = 360 - angle;
    }
    return angle;
}


/*!
    \brief маршрутизация команд
    \param current_command команда g-code
    \param params список параметров команды
*/
int Parser :: command_router(QString& current_command, QStringList& params, int& str_num, QString& str) {
    //если в строке не было команды, то взять команду из предыдущей строки
    if(current_command ==  "") {
        current_command = prev_command;
    }
    //обработать ошибку, если в самой первой строке нет команды еще надо
    if(current_command == "G0" || current_command == "G00") {
        g0_command(params, str_num, str);
        prev_command = "G0";
        return 1;
    }
    else if(current_command == "G1" || current_command == "G01") {
        g1_command(params, str_num, str);
        prev_command = "G1";
        return 1;
    }
    else if(current_command == "G3" || current_command == "G03") {
        g3_command(params, str_num, str);
        prev_command = "G3";
        return 1;
    }
    else if(current_command == "G2" || current_command == "G02") {
        g2_command(params, str_num, str);
        prev_command = "G2";
        return 1;
    }
    // qDebug() << "unknown command " << current_command << " at line " << str_num << " : " << str;
    return 1;
}



/*!
    \brief команда, считывающая цифры в строке и добавляющая их к команде G-Code
*/
int read_num(QString& current_command, QString& current_str, int& str_num, int& i, QString& str) {
    //счетчик точек. если точек больше, чем 1 - ошибка
    int point_counter = 0;
    //сохранить команду G + цифры после нее до первой не цифры
    for(int j = i+1; j <= current_str.length(); j++) {
        if(current_str.at(j) == "-" && j == i + 1) {
            current_command += current_str.at(j);
        }
        else if(current_str.at(j).isDigit()) {
            current_command += current_str.at(j);
        }
        else if(current_str.at(j) == ".") {
            int point_counter = 0;
            current_command += current_str.at(j);
        }
        else {
            j = current_str.length();
        }
    }
    //если больше чем 1 точка - ошибка
    if(point_counter > 1) {
        qDebug() << "Wrong parameter " << current_command << " at line " << str_num + 1 << " : " << str;
        return 0;
    }
    return 1;
}




/*!
    \brief парсер одной линии
*/
int Parser :: parse_line(QString& str, int& str_num) {
    QString current_str = str;
    current_str.replace(" ", "");
    QString current_command = "";
    QStringList params;
    //проходимся по каждому символу в строке
    for(int i = 0; i < current_str.length(); i++) {
        if(current_str.at(i) == 'G') {
            if(i != 0) {
                if(command_router(current_command, params, str_num, str) != 0) {
                    return 0;
                }
            }
            current_command = "G";
            params.clear();
            //считать цифры после 'G'
            if(read_num(current_command, current_str, str_num, i, str) == 0) {
                return 0;
            } 
        }
        else if(current_str.at(i) == 'M') {
            if(i != 0) {
                if(command_router(current_command, params, str_num, str) != 0) {
                    return 0;
                }
            }
            current_command = "";
            params.clear();
            //считать цифры после 'M'
            if(read_num(current_command, current_str, str_num, i, str) == 0) {
                return 0;
            }
            //команду M дальше не обрабатываем, т.к. не влияет на результат визуализации
        }
        //парметры, которые игнорируются, т.к. для визуализатора не важны
        else if(current_str.at(i) == "E" || 
                current_str.at(i) == "F" ||
                current_str.at(i) == "P" ||
                current_str.at(i) == "T" ||
                current_str.at(i) == "R" ||
                current_str.at(i) == "Z" ||
                current_str.at(i) == "S" ) {
            QString current_param = current_str.at(i);
            if(read_num(current_param, current_str, str_num, i, str) == 0) {
                return 0;
            }
        }
        else if(current_str.at(i) == "X" ||
                current_str.at(i) == "Y" ||
                current_str.at(i) == "I" ||
                current_str.at(i) == "J" ) {
            QString current_param = current_str.at(i);
            if(read_num(current_param, current_str, str_num, i, str) == 0) {
                return 0;
            }  
            params.append(current_param);
        }
    }
    if(command_router(current_command, params, str_num, str) == 0) {
        return 0;
    }
    
    return 1;
}


/*!
    \brief команда, содержащая один цикл прохода по строкам файла g-code
*/
int Parser :: parse() {
    for(int str_i = 0; str_i < contain.length(); str_i++) {
        QString current_str = contain.at(str_i);
        if(parse_line(current_str, str_i) == 0) {
            return 0;
        }
    }
    return 1;
}


/*!
    \brief команда открытия и чтения g-code файла
*/
bool Parser :: open(QString& gcode_file){
    QFile file(gcode_file);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        while (!file.atEnd()){
            QString contain_string = file.readAll();
            contain = contain_string.split("\n");
            file.close();
            
        }
        // debug
        // qDebug() << contain;
        return true;
      }
    else
    // debug
    qDebug()<< "file not open";
    return false;
}