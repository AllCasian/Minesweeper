#include <string.h>
#include <stdio.h>
#include "minesweeper.h"

struct Command{
    char type;
    char col;
    int row;
}Command;

void print(int h, int w, char t[h][w]){

    ///marginea de sus
    int k = 0;
    for(int i = 0; i < w; i ++){
        if(i == 0){
            printf("   A");
        }
        else if('A' + i > 90){
                printf("%c", 'a' + k);
                k ++;
        }
        else{
            printf("%c", 'A' + i);
        }
    }
    printf("\n");

    ///marginea din stanga si dreapta + VALORILE
    for(int i = 1; i <= h; i ++){
        printf("%d", i);
        if(i > 9){
            printf(" ");
        }
        else{
            printf("  ");
        }
        for(int j = 1; j <= w; j ++){

            ///AICI SE INTRODUC VALORILE
            printf("%c", t[i][j]);

        }
        printf(" ");
        printf("%d\n", i);
    }

    ///marginea de jos
    k = 0;
    for(int i = 0; i < w; i ++){
        if(i == 0){
            printf("   A");
        }
        else if('A' + i > 90){
                printf("%c", 'a' + k);
                k ++;
        }
        else{
            printf("%c", 'A' + i);
        }
    }
    printf("\n\n");
}

void create(int nr_bombs, char cols[], int rows[], int h, int w, char t[h][w]){
    ///transformarea coloanelor din litere in cifre
    int cols1[nr_bombs];
    for(int i = 0; i < nr_bombs; i ++){
        if(cols[i] < 91){
            cols1[i] = cols[i] - 65 + 1;
        }
        else if(cols[i] > 96){
            cols1[i] = cols[i] - 72 + 2;
        }
    }
    //for(int i = 0; i < nr_bombs; i ++){
    //    printf("%d ", cols1[i]);
    //}

    ///pozitionarea bombelor
    int n = 0; //pt parcurgere cols si rows
    for(int i = 1; i <= h; i ++){
        for(int j = 1; j <= w; j ++){
            if(rows[n] == i && cols1[n] == j){
                t[i][j] = 'X';
                n ++;
            }
            else{
                t[i][j] = '0';
            }
        }
    }

    ///pozitionarea numerelor cu vecinatatea cu bombe
    for(int i = 1; i <= h; i ++){
        for(int j = 1; j <= w; j ++){
            if(t[i][j] == '0'){
                int counter = 0; //pt numararea bombelor
                if(t[i - 1][j - 1] == 'X'){
                    counter ++;
                }
                if(t[i - 1][j] == 'X'){
                    counter ++;
                }
                if(t[i - 1][j + 1] == 'X'){
                    counter ++;
                }
                if(t[i][j - 1] == 'X'){
                    counter ++;
                }
                if(t[i][j + 1] == 'X'){
                    counter ++;
                }
                if(t[i + 1][j + 1] == 'X'){
                    counter ++;
                }
                if(t[i + 1][j] == 'X'){
                    counter ++;
                }
                if(t[i + 1][j - 1] == 'X'){
                    counter ++;
                }

                if(counter == 0 && t[i][j] == '0'){
                    t[i][j] = '.';
                }
                else {
                    t[i][j] = counter + '0';
                }

            }
        }
    }
}

char** init_state(int h, int w){
    char **s = malloc(h * sizeof(char*));
    for(int i = 1; i <= h; i ++){
        s[i] = malloc(w * sizeof(char));
    }

    for(int i = 1; i <= h; i ++){
        for(int j = 1; j <= w; j ++){
            s[i][j] = '?';
        }
    }

    return s;
}

int discover(int i, int j, int h, int w, char t[h][w], char** s){
    if(i < 1 || i > h || j < 1 || j > w){
        return -2;
    }
    if(t[i][j] == 'X'){
        return -1;
    }
    if(s[i][j] != '?'){
        return 0;
    }
    if(isdigit(t[i][j])){
        s[i][j] = '.';
        return 1;
    }
    s[i][j] = '.';
    int x;
    for(int dx = -1; dx <= 1; dx ++){
        for(int dy = -1; dy <= 1; dy ++){
            x = discover(i + dx, j + dy, h, w, t, s);
        }
    }
    return 2;
}

int taking_commands(char *str, struct Command *command){
    int cnt = 0;

    char sep[]=" ";
    char * p = strtok(str , sep);

    while(p != NULL){
        if(p[0] == '!'){
            command[cnt].type = '!';
            command[cnt].col = p[1];
            if(p[3] >= '0' && p[3] <= '9') {
                command[cnt].row = 0;
                for(int i = 2; i < 4; i ++){
                    command[cnt].row = command[cnt].row * 10 + atoi(p + i);
                }
                command[cnt].row = command[cnt].row / 10;
            }
            else{
                command[cnt].row = atoi(p + 2);
            }
        }
        else{
            command[cnt].type = '.';
            command[cnt].col = p[0];
            if(p[2] >= '0' && p[2] <= '9'){
                command[cnt].row = 0;
                for(int i = 1; i < 3; i ++){
                    command[cnt].row = command[cnt].row * 10 + atoi(p + i);
                }
                command[cnt].row = command[cnt].row / 10;
            }
            else{
                command[cnt].row = atoi(p + 1);
            }
        }
        cnt ++;
        p = strtok(NULL , sep);
    }
    return cnt;
}

void mark(int i, int j, int h, int w, char** s){
    if(s[i][j] == '?'){
        s[i][j] = '!';
    }
    else if(s[i][j] == '!'){
        s[i][j] = '?';
    }
    else{
        s[i][j] = '.';
    }
}

void player_view(int h, int w, char t[h][w], char** s){ ///la fel ca print() doar ca aici afisam ce trebuie sa vada jucatorul
    char matrix[100][100];

    ///marginea de sus
    int k = 0;
    for(int i = 0; i < w; i ++){
        if(i == 0){
            printf("   A");
        }
        else if('A' + i > 90){
                printf("%c", 'a' + k);
                k ++;
        }
        else{
            printf("%c", 'A' + i);
        }
    }
    printf("\n");

    ///marginea din stanga si dreapta + VALORILE
    for(int i = 1; i <= h; i ++){
        printf("%d", i);
        if(i > 9){
            printf(" ");
        }
        else{
            printf("  ");
        }
        for(int j = 1; j <= w; j ++){

            ///AICI SE INTRODUC VALORILE
            if(s[i][j] == '?'){
                matrix[i][j] = s[i][j];
                printf("%c", matrix[i][j]);
            }
            else if(isdigit(t[i][j]) && s[i][j] != '!'){
                matrix[i][j] = t[i][j];
                printf("%c", matrix[i][j]);
            }
            else if(t[i][j] == '.' && s[i][j] != '!'){
                matrix[i][j] = t[i][j];
                printf("%c", matrix[i][j]);
            }
            else if(s[i][j] == '!'){
                matrix[i][j] = '!';
                printf("%c", matrix[i][j]);
            }

        }
        printf(" ");
        printf("%d\n", i);
    }

    ///marginea de jos
    k = 0;
    for(int i = 0; i < w; i ++){
        if(i == 0){
            printf("   A");
        }
        else if('A' + i > 90){
                printf("%c", 'a' + k);
                k ++;
        }
        else{
            printf("%c", 'A' + i);
        }
    }
    printf("\n\n");
}

