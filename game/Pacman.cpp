#pragma GCC optimize ("-O3")

#include "libgame.h"
#include "graphics.h"
#include "binary.h"
#include "controls.h"
#define GAME 1
#define B BLUE
#define WALLNUM 203
const uint8_t wall_lines[] PROGMEM = {
    B, B, B,
    B, B, B,
    B, B, B
};

#undef B
#define B YELLOW
const uint8_t pacman_lines[] PROGMEM = {
    B, B, B,
    B, B, B,
    B, B, B
};

#undef B
const game_sprite pacman PROGMEM = {
    // ШИРИНА, ВЫСОТА, ДАННЫЕ
    3, 3, pacman_lines
};
const game_sprite wall PROGMEM = {
    // ШИРИНА, ВЫСОТА, ДАННЫЕ
    3, 3, wall_lines
};
/* Встроенные цвета:
 *
 *  BLACK - Чёрный
 *  BLUE - Синий
 *  GREEN - Зелёный
 *  RED - Красный
 *  CYAN - Циановый
 *  PURPLE - Фиолетовый
 *  BROWN - Коричневый
 *  WHITE - Белый
 *
 *  Для использования 64-х цветной палитры, укажите в game.ino COLOR_6BIT = 1
 *
 * */

/* Кнопки:
 *
 * НА КОРПУСЕ:
 * BUTTON_SW, BUTTON_NW, BUTTON_SE, BUTTON_NE
 *
 * NW              NE
 *  SW            SE
 *
 *
 * НА ДЖОЙСТИКЕ:
 * BUTTON_UP, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_DOWN, BUTTON_SELECT, BUTTON_START, BUTTON_A, BUTTON_B
 *
 *      UP
 *  LEFT+RIGHT     SELECT  START      B  A
 *     DOWN
 *
 * */

/* Спрайты
 *
 * максимальная высота - 16 пикселей

 определение спрайта


   x     x
    x   x
     x x
  xxxxxxxxx
 xxxxxxxxxxx
xxx  xxx  xxx
 xxxxxxxxxxx
  xxxxxxxxx
    x x x
   x     x


 --------------------------------

const uint8_t YourSprite_lines[] PROGMEM = {
    B00010000, B01000000,
    B00001000, B10000000,
    B00000101, B00000000,
    B00111111, B11100000,
    B01111111, B11110000,
    B11100111, B00111000,
    B01111111, B11110000,
    B00111111, B11100000,
    B00001010, B10000000,
    B00010000, B01000000
};

const game_sprite YourSprite PROGMEM = {
    // ШИРИНА, ВЫСОТА, ДАННЫЕ
    13, 10, YourSprite_lines
};

*/

/* Функции отрисовки
 *
 * game_draw_pixel(x, y, color) - Красит точку (x, y) в цвет color
 * game_draw_char(char, x, y, color) - Выводит символ char с левым верхним углом в точке (x, y) с цветом color
 * game_draw_text(string, x, y, color) - Выводит строку string с левым верхним углом в точке (x, y) с цветом color
 * game_draw_sprite(sprite, x, y, color) - Выводит спрайт sprite с левым верхним углом в точке (x, y) с цветом color
 *
 * */

/* Функции ввода
 *
 * game_is_button_pressed(button) - Нажата ли кнопка? Например: game_is_button_pressed(BUTTON_START)
 * game_is_any_button_pressed(mask) - Нажата ли хотя бы одна кнопка? Например: game_is_any_button_pressed(BITMASK(BUTTON_SW) | BITMASK(BUTTON_DOWN))
 *
 * */

struct PacmanData
{

    int8_t pos[2];//player position
    int8_t speed[2];//player speed
    int8_t enemies[4][2];//enemies position
    int8_t doof[4][2];//position of POWEEER
    int8_t state; //gamestate

    /* Объявляйте ваши переменные здесь */
    /* Чтобы потом обращаться к ним, пишите data->ПЕРЕМЕННАЯ */
};
static PacmanData* data; /* Эта переменная - указатель на структуру, которая содержит ваши переменные */

int8_t walls[][2] = {{0,0},{1,0},{2,0},{3,0},{4,0},{5,0},{6,0},{7,0},{8,0},{9,0},{10,0},{11,0},{12,0},{13,0},{14,0},{15,0},{16,0},{17,0},{18,0},{0,1},{9,1},{18,1},{0,2},{2,2},{3,2},{5,2},{6,2},{7,2},{9,2},{11,2},{12,2},{13,2},{15,2},{16,2},{18,2},{0,3},{18,3},{0,4},{2,4},{3,4},{5,4},{7,4},{8,4},{9,4},{10,4},{11,4},{13,4},{15,4},{16,4},{18,4},{0,5},{5,5},{9,5},{13,5},{18,5},{0,6},{1,6},{2,6},{3,6},{5,6},{6,6},{7,6},{9,6},{11,6},{12,6},{13,6},{15,6},{16,6},{17,6},{18,6},{0,7},{1,7},{2,7},{3,7},{5,7},{13,7},{15,7},{16,7},{17,7},{18,7},{0,8},{1,8},{2,8},{3,8},{5,8},{7,8},{8,8},{9,8},{10,8},{11,8},{13,8},{15,8},{16,8},{17,8},{18,8},{0,9},{1,9},{2,9},{3,9},{7,9},{8,9},{9,9},{10,9},{11,9},{15,9},{16,9},{17,9},{18,9},{0,10},{1,10},{2,10},{3,10},{5,10},{7,10},{8,10},{9,10},{10,10},{11,10},{13,10},{15,10},{16,10},{17,10},{18,10},{0,11},{1,11},{2,11},{3,11},{5,11},{13,11},{15,11},{16,11},{17,11},{18,11},{0,12},{1,12},{2,12},{3,12},{5,12},{6,12},{7,12},{9,12},{11,12},{12,12},{13,12},{15,12},{16,12},{17,12},{18,12},{0,13},{5,13},{9,13},{13,13},{18,13},{0,14},{2,14},{3,14},{5,14},{7,14},{8,14},{9,14},{10,14},{11,14},{13,14},{15,14},{16,14},{18,14},{0,15},{18,15},{0,16},{2,16},{3,16},{5,16},{6,16},{7,16},{9,16},{11,16},{12,16},{13,16},{15,16},{16,16},{18,16},{0,17},{9,17},{18,17},{0,18},{1,18},{2,18},{3,18},{4,18},{5,18},{6,18},{7,18},{8,18},{9,18},{10,18},{11,18},{12,18},{13,18},{14,18},{15,18},{16,18},{17,18},{18,18}};
bool a;
static void Pacman_prepare()
{
    data->pos[0] = data->pos[1] = 3;
    data->state = 1;
    data->speed[0] = 1;
    /* Здесь код, который будет исполнятся один раз */
    /* Здесь нужно инициализировать переменные */
}

static void Pacman_render()
{

    /* Здесь код, который будет вывзваться для отрисовки кадра */
    /* Он не должен менять состояние игры, для этого есть функция update */
    for(int i = 0; i < WALLNUM; i++)
    {
        game_draw_color_sprite(&wall, walls[i][0]*3, walls[i][1]*3);
    }
    game_draw_color_sprite(&pacman, data->pos[0], data->pos[1]);
    /* Здесь (и только здесь) нужно вызывать функции game_draw_??? */
}

static void Pacman_update(unsigned long delta)
{

    switch(data->state)
    {
    case GAME:
    for(int i = 0; i < WALLNUM; i++)
    {
        if( (walls[i][0] - data->speed[0]) * 3 == data->pos[0] && (walls[i][1] - data->speed[1]) * 3 == data->pos[1])
            data->speed[0] = data -> speed[1] = 0;
    }
    if(!(data->pos[0]%3 || data->pos[1]%3)){
        if( game_is_button_pressed(BUTTON_LEFT))
        {
            for(int i = 0; i < WALLNUM; i++)
            {
                if( (walls[i][0] + 1) * 3 == data->pos[0] && (walls[i][1]) * 3 == data->pos[1])
                    goto exit;
            }
            data->speed[0] = -1;
            data->speed[1] = 0;
            break;
        }
        if( game_is_button_pressed(BUTTON_RIGHT))
        {
            for(int i = 0; i < WALLNUM; i++)
            {
                if( (walls[i][0] - 1) * 3 == data->pos[0] && (walls[i][1]) * 3 == data->pos[1]) {
                    goto exit;

                }
            }
            data->speed[0] = 1;
            data->speed[1] = 0;

        }
        if( game_is_button_pressed(BUTTON_DOWN))
        {
            for(int i = 0; i < WALLNUM; i++)
            {
                if( (walls[i][0]) * 3 == data->pos[0] && (walls[i][1]-1) * 3 == data->pos[1])
                    goto exit;
            }
            data->speed[0] = 0;
            data->speed[1] = 1;
        }
        if( game_is_button_pressed(BUTTON_UP))
        {
            for(int i = 0; i < WALLNUM; i++)
            {
                if( (walls[i][0]) * 3 == data->pos[0] && (walls[i][1]+1) * 3 == data->pos[1])
                    goto exit;
            }
            data->speed[0] = 0;
            data->speed[1] = -1;

        }
      }

exit:
        a = !a;
        if(a)
        {
        data->pos[0] += data->speed[0];
        data->pos[1] += data->speed[1];
      }
    }
    /* Здесь код, который будет выполняться в цикле */
    /* Переменная delta содержит количество миллисекунд с последнего вызова */

    /* Здесь можно работать с кнопками и обновлять переменные */
}

const game_instance Pacman PROGMEM = {
    "Pacman",         /* Название игры.
                           Используется как имя записи в EEPROM.
                           Не больше 10 символов */
    Pacman_prepare,
    Pacman_render,
    Pacman_update,
    sizeof(PacmanData),
    (void**)(&data)
};


/* Не забудьте зарегистрировать игру в application.cpp, libgame.h */
/* Также нужно добавить файл с игрой в qt_emulator/CMakeLists.txt */