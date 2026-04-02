#include "extension.h"
void initScreen(Screen *s, int x, int y, int w, int h) {
    // Initialisiere die dynamische Liste der Elemente
    s->elements = NULL;
    s->count = 0;      
    
    // Setze die Container-Eigenschaften
    s->posX = x;
    s->posY = y;
    s->w = w;
    s->h = h;
    
    // Standardmäßig auf sichtbar setzen (1 = true)
    s->visible = 1;
}
void addElement(Screen *s, UIElement *e) {
    s->elements = realloc(s->elements, (s->count + 1) * sizeof(UIElement*));
    s->elements[s->count++] = e;
}

void freeScreen(Screen *s) {
    if (s->elements == NULL) return;

    for (int i = 0; i < s->count; i++) {
        // Zuerst das einzelne Element (Button, Slider, etc.) freigeben
        free(s->elements[i]);
    }

    // Danach das dynamische Array freigeben, das durch realloc gewachsen ist
    free(s->elements);

    // Zur Sicherheit alles auf NULL/0 setzen
    s->elements = NULL;
    s->count = 0;
}

void handleMouseUp(Screen *s, int mouseX, int mouseY) {
    for (int i = 0; i < s->count; i++) {
        UIElement *e = s->elements[i];
        e->flags.dragging = 0;         
        if (!e->flags.visible) continue;
        if (e->click) {
            e->click(e, mouseX, mouseY);
        }
    }
}

void handleMouseDown(Screen *s, int mouseX, int mouseY) {
    for (int i = 0; i < s->count; i++) {
        UIElement *e = s->elements[i];

        if (!e->flags.visible) continue;
        e->flags.dragging = 1;         
        if (e->click) {
            e->click(e, mouseX, mouseY);
        }
    }
}

void handleMouseMove(Screen *s, int mouseX, int mouseY) {
    for (int i = 0; i < s->count; i++) {
        UIElement *e = s->elements[i];

        if (!e->flags.visible && !e->flags.dragging) continue;        
        if (e->move) {
            e->move(e, mouseX, mouseY);
        }
    }
}

void drawScreen(Screen *s, SDL_Renderer *renderer) {
    for (int i = 0; i < s->count; i++) {
        UIElement *e = s->elements[i];

        if (!e->flags.visible) continue;

        if (e->draw) {
            e->draw(e, renderer);
        }
    }
}
Button* createButton(
    int x, int y, int w, int h,
    Color on, Color off,
    void (*fn)(void*), void *param
) {
    Button *b = malloc(sizeof(Button));

    b->base.posX = x;
    b->base.posY = y;
    b->base.w = w;
    b->base.h = h;
    b->base.flags.raw = 1;

    b->base.draw = drawButton;
    b->base.click = clickButton;
    b->base.move = NULL;

    b->color_on = on;
    b->color_off = off;

    b->onclick = fn;
    b->param = param;

    return b;
}

void drawButton(UIElement *e, SDL_Renderer *renderer) {
    Button *b = (Button*)e;
    Color c;
     c = e->flags.pressed == 1? b->color_on :b->color_off;
    SDL_SetRenderDrawColor(renderer,c.r,c.g,c.b, 255);

    SDL_Rect rect = {e->posX, e->posY, e->w, e->h};
    SDL_RenderFillRect(renderer, &rect);
}

void clickButton(UIElement *e, int mouseX, int mouseY) {
    Button *b = (Button*)e;
    e->flags.pressed = 0;
    if (mouseX >= e->posX && mouseX <= e->posX + e->w &&
        mouseY >= e->posY && mouseY <= e->posY + e->h) {
        e->flags.pressed = (e->flags.dragging == 1) ? 1:0;
        if (b->onclick&&e->flags.dragging ==1) {
            b->onclick(b->param);
        }
    }
}

Toggle* createToggle(
    int x, int y, int w, int h,
    Color on, Color off,
    void (*fn)(void*), void *param
) {
    Toggle *t = malloc(sizeof(Toggle));

    t->base.posX = x;
    t->base.posY = y;
    t->base.w = w;
    t->base.h = h;
    t->base.flags.raw = 1;

    t->base.draw = drawToggle;
    t->base.click = clickToggle;
    t->base.move = NULL;

    t->color_on = on;
    t->color_off = off;

    t->onclick = fn;
    t->param = param;

    return t;
}

void clickToggle(UIElement *e, int mouseX, int mouseY) {
    Toggle *t = (Toggle*)e;
    if (mouseX >= e->posX && mouseX <= e->posX + e->w &&
        mouseY >= e->posY && mouseY <= e->posY + e->h) {
        e->flags.pressed = (e->flags.dragging ==1)?!e->flags.pressed:e->flags.pressed;
        if (t->onclick&&e->flags.dragging ==1) {
            t->onclick(t->param);
        }
    }
}
void drawToggle(UIElement *e, SDL_Renderer *renderer) {
    Toggle *t = (Toggle*)e;
    Color c;
     c = e->flags.pressed == 1? t->color_on :t->color_off;
    SDL_SetRenderDrawColor(renderer,c.r,c.g,c.b, 255);

    SDL_Rect rect = {e->posX, e->posY, e->w, e->h};
    SDL_RenderFillRect(renderer, &rect);
}

Slider* createSlider(
    int x, int y, int w, int h,
    Color track, Color fill, Color fill_off,
   float min, float max, float value,
    void (*fn)(void*), void *param
) {
    Slider *s = malloc(sizeof(Slider));

    s->base.posX = x;
    s->base.posY = y;
    s->base.w = w;
    s->base.h = h;
    s->base.flags.raw = 1;

    s->base.draw = drawSlider;
    s->base.click = NULL;
    s->base.move = moveSlider;

    s->color_track = track;
    s->color_fill = fill;
    s->color_fill_off = fill_off;
    s->min = min;
    s->max = max;
    s->value = value;
    s->onclick = fn;
    s->param = param;

    return s;
}

void moveSlider(UIElement *e, int mouseX, int mouseY) {
    Slider *s = (Slider*)e;
    if (e->flags.dragging ==0){
        e->flags.pressed =  0;
    }
    if (mouseX >= e->posX && mouseX <= e->posX + e->w &&
        mouseY >= e->posY && mouseY <= e->posY + e->h) {
            if (e->flags.dragging ==1){
                e->flags.pressed =  1;
            }
    }
    if (s->onclick&&e->flags.pressed ==  1 ) {
        float t = (float)(mouseX - e->posX) / (float)e->w;
        if (t < 0.0f) t = 0.0f;
        if (t > 1.0f) t = 1.0f;
        s->value = s->min + t * (s->max - s->min);
            s->onclick(s->param);
    }
}
void drawSlider(UIElement *e, SDL_Renderer *renderer) {
    Slider *s = (Slider*)e;

    // Track (Hintergrund)
    SDL_SetRenderDrawColor(renderer, s->color_track.r, s->color_track.g, s->color_track.b, 255);
    SDL_Rect track = {e->posX, e->posY, e->w, e->h};
    SDL_RenderFillRect(renderer, &track);

    // Fill-Breite aus value berechnen
    float t = (float)(s->value - s->min) / (float)(s->max - s->min);
    int fill_w = (int)(t * (e->w-2));

    // Fill (Fortschritt)
    Color c = e->flags.pressed == 1 ? s->color_fill : s->color_fill_off;
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255);
    SDL_Rect fill = {e->posX+1, e->posY+1, fill_w, e->h-2};
    SDL_RenderFillRect(renderer, &fill);
}


