![screenshot_8](https://github.com/nullJonas/asciibrot/blob/main/screenshots/screenshot_8.png)
# asciibrot
Esse é um projeto que fiz enquanto aprendia o básico de C, é um explorador do conjunto de Mandelbrot feito apenas com caracteres ASCII coloridos no terminal.

### Controles:
W, A, S e D para movimentar a câmera;

Q e E para girar a câmera;

I e O para controlar o zoom;

ESC para sair.

### Como executar:
Em um terminal basta executar, na pasta do repositório:

```./asciibrot```

### Como compilar:
Em uma distribuição linux, primeiro instale a biblioteca ncurses com:

```sudo apt-get install libncurses5-dev libncursesw5-dev```

Em seguida, compile com:

```gcc code/main.c -o asciibrot -lm -lncurses```

Atualmente o programa não funciona no Windows pois ele depende da biblioteca ncurses.