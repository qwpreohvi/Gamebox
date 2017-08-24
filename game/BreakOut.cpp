#pragma GCC optimize ("-O3")
#include <stdint.h>
#include "storage.h"
#include "libgame.h"
#include "graphics.h"
#include "binary.h"
//#include "Arduino.h"
#include <stdlib.h>
#include "controls.h"
#include "iostream"

const uint8_t pauseLines[] PROGMEM = {
    B01111000,B11100100,B01001111,B01111101,B11100000,
    B01000101,B00010100,B01010000,B01000001,B00010000,
    B01111001,B11110100,B01011111,B01111101,B00010000,
    B01000001,B00010100,B01000001,B01000001,B00010000,
    B01000001,B00010011,B10011110,B01111101,B11100000,
};

const game_sprite pausef PROGMEM = {
    36, 5, 5, pauseLines
};

const uint8_t BoardData[] PROGMEM = {
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

const game_sprite Board PROGMEM = {
    3, 12, 1, BoardData
};
const game_sprite BoardMin PROGMEM = {
    3, 8, 1, BoardData
};
const game_sprite BoardMax PROGMEM = {
    3, 16, 1, BoardData
};
const uint8_t BallData[] PROGMEM = {
    B01100000,  
    B11110000, 
    B11110000, 
    B01100000  
};

const game_sprite Ball PROGMEM = {
    4, 4, 1, BallData
};

const game_sprite Bonus PROGMEM = {
    2, 2, 1, BallData
};




struct BreakOutData {
    short int Board1Y, Board2Y, k, 
        ballX, ballY, 
        speedy, speedx, 
        bonusx, bonusy,
        i, lvlcount, 
        Board1Lenght, Board2Lenght, paused;

    bool IsBot, menu, Bcatch, flag;
    int time;

};
static BreakOutData* data;

static void BreakOut_prepare() {

    game_set_ups(10);
    data->bonusx = 24;
    data->bonusy = -4;
    data->Board1Lenght = data->Board2Lenght = 12;
    data->lvlcount = 0;
    data->i  = 65;
    data->ballX = data->ballY = 30;
    data->Board1Y = data->Board2Y  = 26;
    data->IsBot = true;
    data->menu = true;
    data->time = 1;
    if(rand() % 2) data->speedx = 1;
    else data->speedx = -1;
    if(rand() % 2) data->speedy = 1;
    else data->speedy = -1;


}

static void BreakOut_render() {
     if(data->menu) {
        game_draw_text((const unsigned char*)"survival-A",2,2,GREEN);
        game_draw_text((const unsigned char*)"2player-B",6,12,GREEN);
        for(int i = 0; i <= 64; i++)
            game_draw_pixel(i,20,WHITE);
        game_draw_text((const unsigned char*)"help:",16,22,BLUE);
        for(int i = 0; i <= 64; i++)
            game_draw_pixel(i,30,WHITE);
        game_draw_text((const unsigned char*)"pause-",1,30,BLUE);
        game_draw_text((const unsigned char*)"{select}",17,38,WHITE);
        game_draw_text((const unsigned char*)"restart-",1,48,BLUE);
        game_draw_text((const unsigned char*)"{start}",20,56,WHITE);
        return;
    }
    if(data->paused) {
        game_draw_sprite(&pausef,14,29,RED);
    }

    
    if( (data->ballX > 57) || (data->ballX < 3)) {
        if(data->ballX > 57)
            if(!data->IsBot)
            game_draw_text((const unsigned char*)"P2 WINS",12,18,GREEN);
            else {
            game_draw_text((const unsigned char*)"BOT WINS.",6,18,GREEN);
            game_draw_text((const unsigned char*)"AGAIN.",6,28,GREEN);
            }
        else
            game_draw_text((const unsigned char*)"P1 WINS",12,18,BLUE);
    }
   
    game_draw_sprite(&Board,0,data->Board2Y,GREEN);
    game_draw_sprite(&Ball,data->ballX,data->ballY,RED);
    game_draw_sprite(&Board,61,data->Board1Y,BLUE);

    if(data->IsBot)
        game_draw_digits(data->lvlcount, 3, 16, 0, WHITE);
    switch(data->lvlcount){
    case 2:
        game_draw_text((uint8_t*)"GO GO", 18, 26, WHITE);
        game_set_ups(25);
        break;

    case 20:
        game_draw_text((uint8_t*)"NORMA", 18, 26, GREEN);
        game_set_ups(37);
        break;

    case 30: //20
        game_draw_text((uint8_t*)"HARD", 18, 26, YELLOW);
        game_set_ups(49);
        break;

    case 40:
        game_draw_text((uint8_t*)"HARDCORE", 12, 26, RED);
        game_set_ups(61);
        break;

    case 50:
        game_draw_text((uint8_t*)"SUICIDE", 12, 26, data->i);
        game_set_ups(73);
        break;

    }
    
}

static void BreakOut_update(unsigned long delta) {
    if (game_is_button_pressed(BUTTON_SELECT)) {
        data->paused = !data->paused;
        return;
    }
    if(data->paused) {
        return;
    }
    if(data->menu) {
        if(game_is_button_pressed(BUTTON_A)){
            data->IsBot = true;
            data->menu = false;
        }
        if(game_is_button_pressed(BUTTON_B))
        {
            data->IsBot = false;
            data->menu = false;
        }
        return;
    } 
    if (game_is_button_pressed(BUTTON_SELECT))
    {
        data->paused = !data->paused;
        for(int i = 0; i < 1000;i++);
        return;
    }
 
    if( (data->ballX > 60) || 
        (data->ballX < 3))
    {
        game_set_ups(2);
        if(data->ballX > 60)
        {
            if(game_is_button_pressed(BUTTON_START))
                BreakOut_prepare();
        }
        else{
            if(game_is_button_pressed(BUTTON_START))
                BreakOut_prepare();
        }
    }

    if(game_is_button_pressed(BUTTON_DOWN) && data->Board1Y < 52)
        data->Board1Y = (data->Board1Y + 1);

    if(game_is_button_pressed(BUTTON_UP) && data->Board1Y > 0)
        data->Board1Y = (data->Board1Y - 1);
    if(!data->IsBot){
        if(game_is_button_pressed(BUTTON_A) && data->Board2Y < 52)
            data->Board2Y = (data->Board2Y + 1);

        if(game_is_button_pressed(BUTTON_B) && data->Board2Y > 0)
            data->Board2Y = (data->Board2Y - 1);
    }
    else if(data->ballY <= 52){
        data->Board2Y = data->ballY;

    } //&& data->time
        

    if((data->ballY >= data->Board1Y - 3) && 
        (data->ballY <= data->Board1Y + 15) && 
        (data->ballX == 57)) 
    {
        std::cout << "BOUNCE";
        data->speedx = -data->speedx;
         data->lvlcount++;
    }

    if((data->ballY >= data->Board2Y - 3) && 
        (data->ballY <= data->Board2Y + 15) && 
        ( data->ballX == 3) ) {
            std::cout << "BOUNCE";
            data->speedx = -data->speedx;
           
    }

    if( (data->ballY == 0) || (data->ballY == 60) ) 
        data->speedy = -data->speedy;

    if(data->ballX < 64 && data->ballX > -4){
        data->ballX += data->speedx;
        data->ballY += data->speedy;
    }

    if(data->lvlcount > 40 && (data->lvlcount % 4 == 0)) { 
        data->i++;
        game_set_ups(data->i);

    }
    data->i = data->i == 0 ? 48 : 0;
}
game_instance BreakOut = {
    "BreakOut",
    BreakOut_prepare,
    BreakOut_render,
    BreakOut_update,
    sizeof(BreakOutData),
    (void**)(&data)
};