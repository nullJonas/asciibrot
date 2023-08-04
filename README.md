![screenshot_8](https://github.com/nullJonas/asciibrot/blob/main/screenshots/screenshot_8.png)
# asciibrot
Esse é um projeto que fiz enquanto aprendia o básico de C, é um explorador do conjunto de Mandelbrot feito apenas com caracteres ASCII coloridos no terminal.

### Como compilar:
(É preciso ter a biblioteca ncurses já instalada)

Compile o programa com:

```gcc code/main.c -o asciibrot -lm -lncurses -ltinfo```

Atualmente o programa não funciona no Windows pois ele depende da biblioteca ncurses.

### Como executar:
Em um terminal basta executar, na pasta do repositório:

```./asciibrot```

### Controles:
W, A, S e D para movimentar a câmera;

Q e E para girar a câmera;

I e O para controlar o zoom;

ESC para sair.

A "resolução" é controlada diretamente pelo tamanho da fonte no terminal.
