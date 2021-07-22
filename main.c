#include <stdio.h>
#include <ncurses.h>

#define XSCALE 4.7 / COLS
#define XOFFSET 2.9
#define YSCALE 2.6 / LINES
#define YOFFSET 1.3
#define ITERATIONS 96

int mandelbrot(double x, double y){
    int t;
    double a = 0.0; // Parte real
    double b = 0.0; // Parte imaginária
    double z;       // Magnitude do número a + bi

    double a_sqr, b_sqr, a_new, b_new;

    for (int t = 0; t < ITERATIONS; t++)
    {   
        // Checando se o número saiu do conjunto
        a_sqr = a * a;
        b_sqr = b * b;
        z = a_sqr + b_sqr;
        if(z > 4.0){
            return t; // Número de iterações necessárias para ele sair
        }

        // Atualizando valores
        a_new = a_sqr - b_sqr + x;
        b_new = 2 * a * b + y;
        a = a_new;
        b = b_new;
    }
    return 0;
}
//TODO: Implementar um mapeamento de cores bonitinho
int color_map(int t){
    int n = t / 16 * 5;
    return n;
}

void init_color_pairs(){
    int fg, i;
    for(i = 0; i < 256; i++){
        init_pair(i,6*i+2,COLOR_BLACK);
    }
}

int main(){
    initscr();   // Inicia a tela do ncurses
    noecho();    // Não mostra as teclas apertadas na tela
    cbreak();    // Desliga o line buffering
    curs_set(0); // Esconde o cursor
    start_color();
    init_color_pairs();

    // Desenhando o conjunto de mandelbrot na tela
    int i, j, t, c;
    double x, y;
    for(i = 0; i < LINES; i++){
        for(j = 0; j < COLS; j++){

            // Ajustando a posição e a escala da imagem
            x = j * XSCALE - XOFFSET;
            y = YOFFSET - i * YSCALE;

            // Checando se o número da posição está no conjunto
            t = mandelbrot(x, y);
            if(t){
                // Pintando o caractere nessa posição
                c = color_map(t);
                attron(COLOR_PAIR(c));
                mvaddch(i, j, '@');
                attroff(COLOR_PAIR(c));
            }
        }
    }
    refresh(); // Mostra o frame na tela
    

    // Espera o usuário pressionar um botão para sair
    getch();
    endwin();
}