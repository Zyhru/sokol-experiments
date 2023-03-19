@echo off

echo Loading...
echo Twitter: https://twitter.com/z7hru
echo Github: https://github.com/zyhru

@REM sokol-shdc -i cube.glsl -o cube-glsl.h -l glsl330
@REM gcc -o game main.c -I\thirdparty -lopengl32 -lgdi32 
@REM game.exe

sokol-shdc -i quad.glsl -o quad-glsl.h -l glsl330
gcc -o texture loadtex.c -I\thirdparty -lopengl32 -lgdi32 
texture.exe
