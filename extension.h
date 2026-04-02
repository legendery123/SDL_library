#ifndef EXTENSION_H
#define EXTENSION_H
#include <SDL2/SDL.h>
#include <stdio.h>

typedef struct  {
    unsigned char r;
    unsigned char g;
    unsigned char b;
}Color;

typedef union {
    unsigned char raw;

    struct {
        unsigned char visible  : 1;
        unsigned char pressed  : 1;
        unsigned char hover    : 1;
        unsigned char dragging : 1;
        unsigned char unused   : 4;
    };
} UIFlags;

typedef struct UIElement UIElement;

struct UIElement {
    int posX, posY;
    int w, h;
    UIFlags flags;
    void (*draw)(UIElement*, SDL_Renderer*);
    void (*click)(UIElement*, int mouseX, int mouseY);
    void (*move)(UIElement*, int , int);
};
typedef struct {
    UIElement base;
    Color color_on;
    Color color_off;
    void (*onclick)(void*);
    void *param;
}Button;

typedef struct {
    UIElement base;
    unsigned char toggled;
    Color color_on;
    Color color_off;
    Color color_field;
    void (*onclick)(void*);
    void *param;
}Toggle;

typedef struct {
    UIElement base;
    float min ,max,value;
    Color color_track;        // grau
    Color color_fill;         // cyan
    Color color_fill_off;      // hellbalau
    // unsigned char text[20];
    void (*onclick)(void*);
    void *param;
}Slider;

typedef struct {
    UIElement **elements;
    int count;
    unsigned char visible;
    int posX, posY;
    int w, h;
} Screen;
void drawScreen(Screen *s, SDL_Renderer *renderer);
void addElement(Screen *s, UIElement *e);
void initScreen(Screen *s, int x, int y, int w, int h);

Button* createButton(int x, int y, int w, int h,Color on, Color off,void (*fn)(void*), void *param);
void drawButton(UIElement *e, SDL_Renderer *renderer);
void clickButton(UIElement *e, int mouseX, int mouseY);


//
void handleMouseDown(Screen *s ,int x , int y);
void handleMouseUp(Screen *s ,int x , int y);
void handleMouseMove(Screen *s ,int x , int y);

Toggle* createToggle(int x, int y, int w, int h,Color on, Color off,void (*fn)(void*), void *param);
void drawToggle(UIElement *e, SDL_Renderer *renderer);
void clickToggle(UIElement *e, int mouseX, int mouseY);

Slider* createSlider(int x, int y, int w, int h,Color track, Color fill,Color fill_of,float min, float max, float value,void (*fn)(void*), void *param);
void drawSlider(UIElement *e, SDL_Renderer *renderer);
void moveSlider(UIElement *e, int mouseX, int mouseY);

void freeScreen(Screen *s);
#endif
