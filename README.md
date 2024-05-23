Использование парсера в main:
```c
Parser parser;
if(parser.open(gcode_file)) {
    parser.parse(20);
}
scene.addPath(parser.get_path(), QPen(Qt::black));
```