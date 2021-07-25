#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <ncurses.h>

#define ITERATIONS 128

// Constantes baseadas no tamanho do terminal
#define XSCALE 4.7 / COLS
#define XOFFSET 2.35
#define YSCALE 2.6 / LINES
#define YOFFSET 1.3

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

// No final eu acabei não usando essa função, mas vou deixar ela aqui
int supersample(double x, double y){
    /* Hexagonal supersampling: */
    int centro = mandelbrot(x,y);
    if(centro){
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
    if(t){
    int n = 1 + (t / 3) % 30;
    return n;
    }
    return 31;
}

void init_color_pairs(int codes[30]){
    int i;
    for(i = 1; i < 31; i++){
        init_pair(i,codes[i - 1],16);
    }
    init_pair(31,16,16);
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

    // Inicia os caracteres aleatórios
    int size = LINES*COLS;
    char ascii[size];
    srand(time(0));
    for(int i = 0;i < size;i++){
        ascii[i] = 33 + rand()%93;
    }

    // Inicia variáveis:
    int i, j, t, c;
    double x, y, x_new, y_new;
    double x_scale = XSCALE;
    double y_scale = YSCALE;
    double x_speed, y_speed;
    // Posição do centro da tela em relação a x = 0, y = 0
    double rposx = 0;
    double rposy = 0;
    // Seno e cosseno do ângulo de rotação da tela em relação ao eixo-x
    double sin_theta = 0;
    double cos_theta = 1;
    double sin_theta_new;
    // Fator de zoom da tela
    double zoom = 1;
    // Tecla atualmente pressionada
    char key;
    
    while(key != '\e'){
        // Ajusta a velocidade do movimento ao nível de zoom
        y_speed = y_scale * 2 * zoom;
        x_speed = x_scale * 4 * zoom;

        // Interpreta a tecla pressionada
        switch (key){
            case 'w':
                rposx -= x_speed * sin_theta;
                rposy += y_speed * cos_theta;
                break;
            case 's':
                rposx += x_speed * sin_theta;
                rposy -= y_speed * cos_theta;
                break;
            case 'd':
                rposx += x_speed * cos_theta;
                rposy += y_speed * sin_theta;
                break;
            case 'a':
                rposx -= x_speed * cos_theta;
                rposy -= y_speed * sin_theta;
                break;
            case 'e':
                sin_theta_new = sin_theta*0.999688 - cos_theta*0.024997;
                cos_theta = cos_theta*0.999688 + sin_theta*0.024997;
                sin_theta = sin_theta_new;
                break;
            case 'q':
                sin_theta_new = sin_theta*0.999688 + cos_theta*0.024997;
                cos_theta = cos_theta*0.999688 - sin_theta*0.024997;
                sin_theta = sin_theta_new;
                break;
            case 'i':
                zoom /= 1.05;
                break;
            case 'o':
                zoom *= 1.05;
                break;
            default:
                break;
        }

        // Desenha o frame atual
        int count = 0;
        for(i = 0; i < LINES; i++){
            for(j = 0; j < COLS; j++){
                // Ajustando a posição do pixel
                x = j * x_scale - XOFFSET;
                y = YOFFSET - i * y_scale;

                // Ajustando as dimensões do pixel
                x *= zoom;
                y *= zoom;

                // Ajustando a rotação do pixel
                x_new = x*cos_theta - y*sin_theta;
                y_new = y*cos_theta + x*sin_theta;
                x = x_new + rposx;
                y = y_new + rposy;

                // Checando se o número da posição está no conjunto
                t = mandelbrot(x, y);
                //t = supersample(x, y);

                // Pintando o caractere nessa posição
                c = color_map(t);
                attron(COLOR_PAIR(c));
                mvaddch(i, j, ascii[count]);
                attroff(COLOR_PAIR(c));
                count++;
            }
        }
        refresh(); // Mostra o frame na tela
        flushinp();
        key = getch(); // Espera a próxima tecla ser apertada
        clear();
    }
    
    // Encerra o programa
    endwin();
}