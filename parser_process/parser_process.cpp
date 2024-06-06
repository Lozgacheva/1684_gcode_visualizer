#include <QtWidgets>
#include <QDebug>
#include "../parser/parser.h"
#include "parser_process.h"

int Parser_process :: command_router(QString& current_command, QStringList& params, int& str_num, QString& str) {
    //если в строке не было команды, то взять команду из предыдущей строки
    if(current_command ==  "") {
        current_command = prev_command;
    }
    //обработать ошибку, если в самой первой строке нет команды еще надо
    if(current_command == "G0" || current_command == "G00") {
        g1_command(params, str_num, str);
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
    else {
        
    }
    // qDebug() << "unknown command " << current_command << " at line " << str_num << " : " << str;
    return 1;
}