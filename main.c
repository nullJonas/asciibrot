#include <stdio.h>
#include <math.h>
#include <ncurses.h>

// Essas constantes devem ser trocadas por variáveis mais tarde
#define XSCALE 4.7 / COLS
#define XOFFSET 2.9
#define YSCALE 2.6 / LINES
#define YOFFSET 1.3
#define ITERATIONS 96

int mandelbrot(double x, double y){
    int t;
    double a = 0.0; // Parte real
    double b = 0.0; // Parte imaginária
    double z;       // Magnitude do número a + bi ao quadrado

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

int supersample(double x, double y){
    int centro = mandelbrot(x,y);
    if(centro){
        /* Hexagonal supersampling: */
        double a = XSCALE / 6;
        double b = (3*YSCALE - XSCALE) / 6;

        int mean = (int)round((centro + mandelbrot(x + 2*a, y)   + mandelbrot(x + a, y + b)
                                      + mandelbrot(x - a, y + b) + mandelbrot(x - 2*a, y)
                                      + mandelbrot(x - a, y - b) + mandelbrot(x + a, y - b))/7);
        return mean;
    }
    return 0;
}

int color_map(int t){
    int n = (1 + t * 5 / 16) % 30;
    return n;
}

void init_color_pairs(int codes[30]){
    int i;
    for(i = 0; i < 30; i++){
        init_pair(i,codes[i],COLOR_BLACK);
    }
}

int main(){
    initscr();   // Inicia a tela do ncurses
    noecho();    // Não mostra as teclas apertadas na tela
    cbreak();    // Desliga o line buffering
    curs_set(0); // Esconde o cursor

    // Inicia o conjunto de cores
    start_color();
    int color_codes[30] = {21,27,33,39,45,51,50,49,48,47,46,82,118,154,190,226,
                           220,214,208,202,196,197,198,199,200,201,165,129,93,57};
    init_color_pairs(color_codes);

    // Desenhando o conjunto de mandelbrot na tela
    int i, j, t, c;
    double x, y;
    for(i = 0; i < LINES; i++){
        for(j = 0; j < COLS; j++){

            // Ajustando a posição e a escala da imagem
            x = j * XSCALE - XOFFSET;
            y = YOFFSET - i * YSCALE;

            // Checando se o número da posição está no conjunto
            t = supersample(x, y);
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