g++ -C ./src/*.cpp -o ./bin/tetris -Wextra -I./include -L./lib -lsfml-window -lsfml-system -lsfml-graphics
cd bin
tetris.exe
pause