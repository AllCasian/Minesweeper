#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "minesweeper.h"

struct Command{
    char type;
    char col;
    int row;
}Command;

int main()
{
    printf("#######  MINESWEEPER  #######\n\n");

    int h = 20;
    int w = 26;
    int nr_bombs = 8;
    char cols[] = {'B', 'D', 'A', 'E', 'S', 'N', 'V', 'Z'};
    int rows[] = {1, 3, 5, 7, 7, 12, 17, 20};
    //char t[h][w] = {{'.', '2', 'X'}, {'.', '2', 'X'}, {'.', '1', '1'}};
    char t[h][w];
    char str[1000];
    struct Command command[100];
    char **s = init_state(h, w);


    create(nr_bombs, cols, rows, h, w, t);
    print(h, w, t);

    int ok = 1;
    while(ok){
        system("cls");
        player_view(h, w, t, s);
        fgets(str, 1000, stdin);
        int x = taking_commands(str, command);
        int b[1000] = {0};
        for(int i = 0; i < x; i ++){
            b[i] = (int)(command[i].col) - 65; // transformarea coloanei din litera in cifra (A - 1, B - 2, ...)
            b[i] = b[i] - 31;
            if(b[i] >= -31 && b[i] <= -6){
                b[i] = b[i] + 32;
            }
            //printf("%d", b);
        }
        for(int i = 0; i < x; i ++){
            if(command[i].type == '!'){
                mark(command[i].row, b[i], h, w, s);
            }
            else{
                int a = discover(command[i].row, b[i], h, w, t, s);

                if(a == -1){
                    ok = 0;
                    system("cls");
                    player_view(h, w, t, s);
                    break;
                }
            }
        }

        int cnt = 0;
        for(int i = 1; i <= h; i ++){
            for(int j = 1; j <= w; j ++){
                if(s[i][j] == '?' || s[i][j] == '!'){
                    cnt++;
                }
            }
        }
        if(cnt == nr_bombs){
            ok = -1;
            system("cls");
            player_view(h, w, t, s);
            break;
        }

    }

    if(ok == 0){
        printf("Game Over!");
    }
    else if(ok == -1){
        printf("You Win!");
    }

    return 0;
}
