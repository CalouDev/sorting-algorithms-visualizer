<h1 align="center">Sorting Algorithms Visualizer v1</h1>

## Project Description

This project is a tool for visualizing the most popular comparison-based sorting algorithms. It displays the evolution of the array in real-time, with a customizable delay between each step.

The latest version is stable but there are still sound problems that I might fix.

![image](https://github.com/user-attachments/assets/5f78d44f-6328-4cb0-986a-ad14a4476445)

## Instructions

### Compilation  
bin/app.exe was made using `gcc` to compile the project:  
```sh
gcc src/*.c -o bin/app -I include -L lib -lmingw32 -lSDL3 -lSDL3_ttf -mwindows -O2 -Wall -Wextra -Werror -Wpedantic
```
### Execution
```sh
./bin/app
```

If you don't want to compile by yourself, you can just download the repo, a compiled .exe is already available in ./bin/


