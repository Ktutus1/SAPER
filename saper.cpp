#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <stdlib.h>
#include <locale.h>

int main() {

    srand(time(0));
    SetConsoleCP(1251);          // Установка кодовой страницы ввода
    SetConsoleOutputCP(1251);    // Установка кодовой страницы вывода
    setlocale(LC_ALL, "Russian");

    int vk_matrix[3][3] = {
        {0x51, 0x57, 0x45},  // Q, W, E
        {0x41, 0x53, 0x44},  // A, S, D
        {0x5A, 0x58, 0x43}   // Z, X, C
    };

    char char_matrix[3][3] = {
        {'q', 'w', 'e'},
        {'a', 's', 'd'},
        {'z', 'x', 'c'}
    };

    // Чтение статистики из файла
    int wins = 0, losses = 0;
    FILE* stats_file = fopen("stats.txt", "r");
    if (stats_file != NULL) {
        fscanf(stats_file, "%d %d", &wins, &losses);
        fclose(stats_file);
    }

    int bomb_i = rand() % 3;
    int bomb_j = rand() % 3;
    int bomb_vk = vk_matrix[bomb_i][bomb_j];

    int pressed[3][3] = { {0} };
    int count = 0;

    printf("Играть на q w e \n");
    printf("          a s d \n");
    printf("          z x c \n");
    printf("Если вы слышите 1 короткий звук - значит рядом бомба\n\n");

    while (1) {

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (GetAsyncKeyState(vk_matrix[i][j]) & 0x8000) {

                    while (GetAsyncKeyState(vk_matrix[i][j]) & 0x8000) {

                    }

                    if (count == 0 && vk_matrix[i][j] == bomb_vk) {
                        while (vk_matrix[i][j] == bomb_vk) {
                            bomb_i = rand() % 3;
                            bomb_j = rand() % 3;
                            bomb_vk = vk_matrix[bomb_i][bomb_j];
                        }
                    }

                    if (vk_matrix[i][j] == bomb_vk) {
                        Beep(1000, 1500);
                        printf("ВЗРЫВ\n");


                        losses++;


                        stats_file = fopen("stats.txt", "w");
                        if (stats_file != NULL) {
                            fprintf(stats_file, "%d %d", wins, losses);
                            fclose(stats_file);
                        }

                        printf("ВЫ ПРОИГРАЛИ! побед:%d поражений:%d \n", wins, losses);
                        system("pause");
                        return 0;
                    }

                    if (!pressed[i][j]) {
                        printf("нажато:%c\n", char_matrix[i][j]);


                        if (j > 0 && vk_matrix[i][j - 1] == bomb_vk) {
                            Beep(800, 200);
                        }
                        // Справа
                        else if (j < 2 && vk_matrix[i][j + 1] == bomb_vk) {
                            Beep(800, 200);
                        }

                        else if (i > 0 && vk_matrix[i - 1][j] == bomb_vk) {
                            Beep(800, 200);
                        }

                        else if (i < 2 && vk_matrix[i + 1][j] == bomb_vk) {
                            Beep(800, 200);
                        }

                        pressed[i][j] = 1;
                        count++;
                        printf("Вы нажали %d/8 \n", count);
                    }
                    else
                    {

                        if (j > 0 && vk_matrix[i][j - 1] == bomb_vk) {
                            Beep(800, 200);
                        }
                        else if (j < 2 && vk_matrix[i][j + 1] == bomb_vk) {
                            Beep(800, 200);
                        }
                        else if (i > 0 && vk_matrix[i - 1][j] == bomb_vk) {
                            Beep(800, 200);
                        }
                        else if (i < 2 && vk_matrix[i + 1][j] == bomb_vk) {
                            Beep(800, 200);
                        }
                    }

                    break;
                }
            }
        }

        if (count == 8) {
            Beep(1000, 300);
            Beep(1000, 300);
            Beep(1000, 300);


            wins++;


            stats_file = fopen("stats.txt", "w");
            if (stats_file != NULL) {
                fprintf(stats_file, "%d %d", wins, losses);
                fclose(stats_file);
            }

            printf("ВЫ ПОБЕДИЛИ!!! побед:%d поражений:%d \n", wins, losses);
            system("pause");
            return 0;
        }
    }

    return 0;
}