#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <stdlib.h>
#include <locale.h>

// Функция для подсчета мин вокруг клетки
int countMinesAround(int i, int j, int vk_matrix[3][3], int bomb_vk[], int bomb_count) {
    int mines_count = 0;

    // Проверяем все 4 направления (слева, справа, сверху, снизу)

    // Слева
    if (j > 0) {
        for (int b = 0; b < bomb_count; b++) {
            if (vk_matrix[i][j - 1] == bomb_vk[b]) {
                mines_count++;
                break;
            }
        }
    }

    // Справа
    if (j < 2) {
        for (int b = 0; b < bomb_count; b++) {
            if (vk_matrix[i][j + 1] == bomb_vk[b]) {
                mines_count++;
                break;
            }
        }
    }

    // Сверху
    if (i > 0) {
        for (int b = 0; b < bomb_count; b++) {
            if (vk_matrix[i - 1][j] == bomb_vk[b]) {
                mines_count++;
                break;
            }
        }
    }

    // Снизу
    if (i < 2) {
        for (int b = 0; b < bomb_count; b++) {
            if (vk_matrix[i + 1][j] == bomb_vk[b]) {
                mines_count++;
                break;
            }
        }
    }

    return mines_count;
}

// Функция для запуска игры с заданным количеством мин
void game(int num_mines) {
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

    // Создание массива для мин
    int bomb_vk[9]; // Максимум 9 мин
    int bomb_count = num_mines;

    // Генерация уникальных мин
    for (int b = 0; b < bomb_count; b++) {
        int new_bomb;
        int unique;

        do {
            unique = 1;
            int bomb_i = rand() % 3;
            int bomb_j = rand() % 3;
            new_bomb = vk_matrix[bomb_i][bomb_j];

            // Проверяем, не повторяется ли мина
            for (int i = 0; i < b; i++) {
                if (bomb_vk[i] == new_bomb) {
                    unique = 0;
                    break;
                }
            }
        } while (!unique);

        bomb_vk[b] = new_bomb;
    }

    int pressed[3][3] = { {0} };
    int count = 0;

    system("cls");
    printf("Играть на q w e \n");
    printf("          a s d \n");
    printf("          z x c \n");
    printf("Если вы слышите 1 короткий звук - значит рядом мина\n");
    printf("Количество коротких звуков = количество мин рядом\n");
    printf("Количество мин: %d\n\n", bomb_count);

    while (1) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (GetAsyncKeyState(vk_matrix[i][j]) & 0x8000) {
                    while (GetAsyncKeyState(vk_matrix[i][j]) & 0x8000) {
                        // Ждем отпускания клавиши
                    }

                    // Если это первое нажатие и нажата мина, переставляем все мины
                    if (count == 0) {
                        int is_bomb = 0;
                        for (int b = 0; b < bomb_count; b++) {
                            if (vk_matrix[i][j] == bomb_vk[b]) {
                                is_bomb = 1;
                                break;
                            }
                        }

                        if (is_bomb) {
                            // Перегенерируем все мины
                            for (int b = 0; b < bomb_count; b++) {
                                int new_bomb;
                                int unique;

                                do {
                                    unique = 1;
                                    int bomb_i = rand() % 3;
                                    int bomb_j = rand() % 3;
                                    new_bomb = vk_matrix[bomb_i][bomb_j];

                                    // Проверяем, не повторяется ли мина и не нажата ли она
                                    for (int k = 0; k < b; k++) {
                                        if (bomb_vk[k] == new_bomb) {
                                            unique = 0;
                                            break;
                                        }
                                    }

                                    // Проверяем, не нажата ли эта клавиша
                                    if (new_bomb == vk_matrix[i][j]) {
                                        unique = 0;
                                    }
                                } while (!unique);

                                bomb_vk[b] = new_bomb;
                            }
                        }
                    }

                    // Проверяем, не мина ли это
                    int is_bomb = 0;
                    for (int b = 0; b < bomb_count; b++) {
                        if (vk_matrix[i][j] == bomb_vk[b]) {
                            is_bomb = 1;
                            break;
                        }
                    }

                    if (is_bomb) {
                        Beep(1000, 1500);
                        printf("ВЗРЫВ\n");

                        losses++;

                        // Сохраняем статистику
                        stats_file = fopen("stats.txt", "w");
                        if (stats_file != NULL) {
                            fprintf(stats_file, "%d %d", wins, losses);
                            fclose(stats_file);
                        }

                        printf("ВЫ ПРОИГРАЛИ! побед:%d поражений:%d \n", wins, losses);
                        system("pause");
                        return;
                    }

                    if (!pressed[i][j]) {
                        // Подсчитываем количество мин вокруг
                        int mines_around = countMinesAround(i, j, vk_matrix, bomb_vk, bomb_count);

                        // Пищим столько раз, сколько мин вокруг
                        if (mines_around > 0) {
                            for (int k = 0; k < mines_around; k++) {
                                Beep(800, 200);
                                Sleep(100); // Пауза между звуками
                            }
                        }

                        pressed[i][j] = 1;
                        count++;
                    }
                    else {
                        // Если клетка уже нажата, все равно показываем сколько мин вокруг
                        int mines_around = countMinesAround(i, j, vk_matrix, bomb_vk, bomb_count);

                        // Пищим столько раз, сколько мин вокруг
                        if (mines_around > 0) {
                            for (int k = 0; k < mines_around; k++) {
                                Beep(800, 200);
                                Sleep(100); // Пауза между звуками
                            }
                        }
                    }

                    system("cls");
                    printf("Играть на q w e \n");
                    printf("          a s d \n");
                    printf("          z x c \n");
                    printf("Если вы слышите 1 короткий звук - значит рядом мина\n");
                    printf("Количество коротких звуков = количество мин рядом\n");
                    printf("Количество мин: %d\n\n", bomb_count);

                    for (int i = 0; i < 3; i++) {
                        for (int j = 0; j < 3; j++) {
                            if (pressed[i][j]) {
                                printf("%c| ", char_matrix[i][j]);
                            }
                            else {
                                printf("*| ");
                            }
                        }
                        printf("\n");
                    }
                    break;
                }
            }
        }

        if (count == 9 - bomb_count) {
           
            Beep(1000, 300);
            
            Beep(1200, 300);

            Beep(1000, 500);

            wins++;

            stats_file = fopen("stats.txt", "w");
            if (stats_file != NULL) {
                fprintf(stats_file, "%d %d", wins, losses);
                fclose(stats_file);
            }

            printf("ВЫ ПОБЕДИЛИ!!! побед:%d поражений:%d \n", wins, losses);

            // Показываем где были мины
            printf("Мины были на: ");
            for (int b = 0; b < bomb_count; b++) {
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        if (vk_matrix[i][j] == bomb_vk[b]) {
                            printf("%c ", char_matrix[i][j]);
                        }
                    }
                }
            }
            printf("\n");

            system("pause");
            return;
        }
    }
}

int main() {
    srand(time(0));
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");

    int level;

    printf("Выберите уровень сложности(количество мин)\n");
    printf("1 - одна мина\n");
    printf("2 - две мины\n");
    printf("3 - три мины\n");
    scanf("%d", &level);

    switch (level) {
    case 1:
        game(1);
        break;
    case 2:
        game(2);
        break;
    case 3:
        game(3);
        break;
    default:
        printf("Сделайте выбор правильно!\n");
        system("pause");
        return 1;
    }

    return 0;
}
