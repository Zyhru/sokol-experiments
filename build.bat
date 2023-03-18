@echo off

echo Twitter: https://twitter.com/z7hru
echo Github: https://github.com/zyhru

gcc -o game main.c -I\thirdparty -lopengl32 -lgdi32 
game.exe
