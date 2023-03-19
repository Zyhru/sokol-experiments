@echo off

echo Loading...
echo Twitter: https://twitter.com/z7hru
echo Github: https://github.com/zyhru

sokol-shdc -i cube.glsl -o cube-glsl.h -l glsl330
gcc -o game main.c -I\thirdparty -lopengl32 -lgdi32 
game.exe
