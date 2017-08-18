#pragma GCC optimize ("-O3")

#include "libgame.h"
#include "sprite.h"
#include "binary.h"
#include "string"
#include <stdlib.h>

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
 */
const uint8_t YourSprite_lines[] PROGMEM = {
    B11110000, 
	B11110000,
	B11110000,
	B11110000,
	B11110000,
	B11110000,
	B11110000, 
	B11110000,
	B11110000,
	B11110000,
	B11110000,
	B11110000
};

const game_sprite YourSprite PROGMEM = {
    // ШИРИНА, ВЫСОТА, КОЛИЧЕСТВО БАЙТ НА СТРОКУ, ДАННЫЕ
    3, 12, 1, YourSprite_lines
};
const uint8_t BallData[] PROGMEM = {
    B01100000, //   xxxx  
    B11110000, // xxxxxxxx
    B11110000, //  xxxxxxxx
    B01100000  //    xxxx
};

const game_sprite Ball PROGMEM = {
    // ШИРИНА, ВЫСОТА, КОЛИЧЕСТВО БАЙТ НА СТРОКУ, ДАННЫЕ
     4, 4, 1, BallData
};



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

struct BreakOutData
{
	int Board1Y, Board2Y, flag, k, ballX, ballY, speedy, speedx, lvlcount;
	bool ft;
    /* Объявляйте ваши переменные здесь */
    /* Чтобы потом обращаться к ним, пишите data->ПЕРЕМЕННАЯ */
};
static BreakOutData* data; /* Эта переменная - указатель на структуру, которая содержит ваши переменные */

static void BreakOut_prepare()
{
	game_set_ups(10);
	data->lvlcount = 0;
    data->ballX = 30;
	data->ballY = 30;
    data->speedy = 1;
	data->speedx = 1;
	data->Board1Y = 26;
	data->Board2Y = 26;
	data->ft = true;

	//while(!game_is_button_pressed(BUTTON_DOWN));
    /* Здесь код, который будет исполнятся один раз */
    /* Здесь нужно инициализировать переменные */
}

static void BreakOut_render()
{
    /* Здесь код, который будет вывзваться для отрисовки кадра */
    /* Он не должен менять состояние игры, для этого есть функция update */
    game_draw_sprite(&YourSprite,0,data->Board2Y,GREEN);
    game_draw_sprite(&Ball,data->ballX,data->ballY,RED);
	game_draw_sprite(&YourSprite,61,data->Board1Y,BLUE);
	switch(data->lvlcount)
	{
	case 2:
		game_draw_text((uint8_t*)"GO GO", 18, 26, WHITE);
		game_set_ups(25);
		break;
	case 4: //10
		game_draw_text((uint8_t*)"NORMA", 18, 26, GREEN);
		game_set_ups(35);
		break;
	case 6: //20
		game_draw_text((uint8_t*)"HARD", 18, 26, BROWN);
		game_set_ups(45);
		break;
	case 8:
		game_draw_text((uint8_t*)"HARDCORE", 12, 26, RED);
		game_set_ups(55);
		break;
	case 10:
		for(int i = 0; i < 1000; i++)
		game_draw_text((uint8_t*)"SUICIDE", 12, 26, RED);
		for(int i = 0; i < 1000; i++)
		game_draw_text((uint8_t*)"SUICIDE", 12, 26, BROWN);
		game_set_ups(65);
		break;
	}
    /* Здесь (и только здесь) нужно вызывать функции game_draw_??? */
}

static void BreakOut_update(unsigned long delta)
{
    /* Здесь код, который будет выполняться в цикле */
    /* Переменная delta содержит количество миллисекунд с последнего вызова */
    if(game_is_button_pressed(BUTTON_DOWN) && data->Board1Y < 52)
    {
      data->Board1Y = (data->Board1Y + 1);
      }
      if(game_is_button_pressed(BUTTON_UP) && data->Board1Y > 0)
    {
      data->Board1Y = (data->Board1Y - 1);
      
      }

	  if(game_is_button_pressed(BUTTON_SW) && data->Board2Y < 52)
    {
      data->Board2Y = (data->Board2Y + 1);
      }
      if(game_is_button_pressed(BUTTON_NW) && data->Board2Y > 0)
    {
      data->Board2Y = (data->Board2Y - 1);
      
      }

	  if((data->ballY >= (data->Board1Y - 4)) && (data->ballY <= (data->Board1Y + 16)) && 
		  ( (data->ballX == 57) )) {

		  data->speedx = -data->speedx;
			data->lvlcount++;
	  }

	  if((data->ballY >= (data->Board2Y - 6)) && 
		  (data->ballY <= (data->Board2Y + 16)) && 
		  ( (data->ballX == 3) ) ) {

		  data->speedx = -data->speedx;
	  }
	  
	  if( (data->ballY == 0) || (data->ballY == 60) ) data->speedy = -data->speedy;

	  if(1){
      data->ballX += data->speedx;
      
      data->ballY += data->speedy;
	  }
	  data->ft = !(data->ft);
    /* Здесь можно работать с кнопками и обновлять переменные */
}


game_instance BreakOut = {
    "BreakOut",         /* Имя, отображаемое в меню */
    BreakOut_prepare,
    BreakOut_render,
    BreakOut_update,
    sizeof(BreakOutData),
    (void**)(&data)
};

/* Не забудьте зарегистрировать игру в games.h */