#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#define WIDTH  40
#define HEIGHT 20
#define MAX_LEN (WIDTH * HEIGHT)

typedef struct { int x, y; } Point;

Point zhang_wei[MAX_LEN]; /* 蛇 */
int   snake_len = 3;
Point lao_ba;             /* 食物 */
int   dir_x = 1, dir_y = 0;
int   score = 0;
int   game_over = 0;

static void gotoxy(int x, int y) {
    COORD c = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

static void hide_cursor(void) {
    CONSOLE_CURSOR_INFO ci;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
    ci.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
}

static void spawn_food(void) {
    int ok = 0;
    while (!ok) {
        lao_ba.x = 1 + rand() % (WIDTH - 2);
        lao_ba.y = 1 + rand() % (HEIGHT - 2);
        ok = 1;
        int i;
		for ( i = 0; i < snake_len; i++)
            if (zhang_wei[i].x == lao_ba.x && zhang_wei[i].y == lao_ba.y)
                ok = 0;
    }
}

static void draw(void) {
    gotoxy(0, 0);
    int y;
    for ( y = 0; y < HEIGHT; y++) {
        int x;
		for ( x = 0; x < WIDTH; x++) {
            char c = ' ';
            if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1)
                c = '#';
            else if (x == lao_ba.x && y == lao_ba.y)
                c = '@';
            else {
                int i;
				for ( i = 0; i < snake_len; i++) {
                    if (zhang_wei[i].x == x && zhang_wei[i].y == y) {
                        c = (i == 0) ? 'O' : 'o';
                        break;
                    }
                }
            }
            putchar(c);
        }
        putchar('\n');
    }
    printf("score: %d  (wasd/方向键移动, q 退出)\n", score);
}

static void step(void) {
    Point next = zhang_wei[0];
    next.x += dir_x;
    next.y += dir_y;

    /* 撞墙 */
    if (next.x <= 0 || next.x >= WIDTH - 1 || next.y <= 0 || next.y >= HEIGHT - 1) {
        game_over = 1;
        return;
    }
    /* 撞自己 */
    int i;
	for ( i = 0; i < snake_len - 1; i++)
        if (zhang_wei[i].x == next.x && zhang_wei[i].y == next.y) {
            game_over = 1;
            return;
        }

    
	for (i = snake_len; i > 0; i--)
        zhang_wei[i] = zhang_wei[i - 1];
    zhang_wei[0] = next;

    if (next.x == lao_ba.x && next.y == lao_ba.y) {
        snake_len++;
        score++;
        printf("遥遥领先\n");   /* 每次吃到食物打印一句 */
        spawn_food();
    }
}

int main(void) {
    SetConsoleOutputCP(CP_UTF8);
    srand((unsigned)GetTickCount());
    hide_cursor();

    zhang_wei[0] = (Point){ WIDTH / 2, HEIGHT / 2 };
    zhang_wei[1] = (Point){ WIDTH / 2 - 1, HEIGHT / 2 };
    zhang_wei[2] = (Point){ WIDTH / 2 - 2, HEIGHT / 2 };
    spawn_food();

    while (!game_over) {
        draw();
        if (_kbhit()) {
            int ch = _getch();
            if (ch == 224) ch = _getch();      /* 方向键 */
            switch (ch) {
                case 'w': case 'W': case 72: if (dir_y == 0) { dir_x = 0;  dir_y = -1; } break;
                case 's': case 'S': case 80: if (dir_y == 0) { dir_x = 0;  dir_y =  1; } break;
                case 'a': case 'A': case 75: if (dir_x == 0) { dir_x = -1; dir_y =  0; } break;
                case 'd': case 'D': case 77: if (dir_x == 0) { dir_x =  1; dir_y =  0; } break;
                case 'q': case 'Q': game_over = 1; continue;
            }
        }
        step();
        Sleep(120);
    }

    gotoxy(0, HEIGHT + 1);
    printf("Game over! score: %d\n", score);
    return 0;
}
