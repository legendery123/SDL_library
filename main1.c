#include <stdlib.h>
#include <stdio.h>
#include "extension.h"

void buttonCallback(void *param) {
    Button *b = (Button*)param;
    printf("Button geklickt!\n");
}

void sliderCallback(void *param) {
    Slider *s = (Slider*)param;
    printf("Slider value: %.2f\n", s->value);
}

void toggleCallback(void *param) {
    Toggle *t = (Toggle*)param;
    printf("Toggle: %s\n", t->base.flags.pressed ? "an" : "aus");
}

int main(){
    SDL_Window *window = SDL_CreateWindow(
        "UI Test",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Screen *screen = malloc(sizeof(Screen));
    initScreen(screen);

    // Button
    Button *b1 = createButton(
        100, 100, 200, 60,
        (Color){0, 200, 0},
        (Color){0, 100, 0},
        buttonCallback,
        NULL
    );
    b1->param = b1;
    addElement(screen, (UIElement*)b1);
    // Toggle
    Toggle *t1 = createToggle(
    100, 320, 100, 50,
    (Color){0, 200, 0},   // on
    (Color){150, 0, 0},   // off
    toggleCallback,
    NULL
    );
    t1->param = t1;
    addElement(screen, (UIElement*)t1);
    // Slider
    Slider *s1 = createSlider(
        100, 220, 300, 40,
        (Color){80, 80, 80},    // track
        (Color){0, 180, 255},   // fill
        (Color){0, 80, 180},    // fill_off (falls gedrückt)
        0, 100, 50,             // min, max, startwert
        sliderCallback,
        NULL
    );
    s1->param = s1;
    addElement(screen, (UIElement*)s1);

    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                handleMouseDown(screen, x, y);
            }
            if (event.type == SDL_MOUSEBUTTONUP) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                handleMouseUp(screen, x, y);
            }
            if (event.type == SDL_MOUSEMOTION) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                handleMouseMove(screen, x, y);
            }
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);
        drawScreen(screen, renderer);
        SDL_RenderPresent(renderer);
    }
    freeScreen(screen);

    return 0;
}