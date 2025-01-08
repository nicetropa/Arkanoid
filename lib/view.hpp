#ifndef VIEW_HPP
#define VIEW_HPP

#include "elements.hpp"
#include "linkLogGame.hpp"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include <memory>

class View {

    private:

    ALLEGRO_TIMER* timer = nullptr;
    ALLEGRO_EVENT_QUEUE* queue = nullptr;
    ALLEGRO_DISPLAY* disp = nullptr;
    ALLEGRO_FONT* font = nullptr;
    ALLEGRO_FONT* fontLW = nullptr;
    ALLEGRO_FONT* bonusFont = nullptr;
    ALLEGRO_BITMAP* background = nullptr;
    bool backgroundLoaded = false;
    bool redraw = true;
    bool playing = true;
    ALLEGRO_EVENT event;
    std::unique_ptr<Linker> linker;
    std::string scorestr = "SCORE: ";
    std::string lifestr = "LIFE: ";
    std::string pbstr = "HIGHSCORE: ";
    int posMouseX;
    int posMouseY;
    void must_init(bool, const char*);
    void must_init(void*, const char*);
    void drawBlocs();
    void drawBonus();

    public:

    View();
    virtual ~View();
    virtual int loadBackground();
    virtual void displaying();
};


#endif