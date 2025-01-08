#include "../lib/view.hpp"


void View::must_init(bool test, const char *description) {
    /*Methode permettant la verification de l'initialisation*/
    if (test) {
        return;
    }
    std::cerr << "couldn't initialize " << description << '\n';
    exit(1);
}

void View::must_init(void *test, const char *description) {
    /*Methode permettant la verification de l'initialisation*/
    must_init(test != nullptr, description);
}

View::View(){
    linker = std::make_unique<Linker>();
    must_init(al_init(), "Allegro");
    must_init(al_install_keyboard(), "Keybord");
    must_init(al_init_primitives_addon(), "Primitives");
    must_init(al_init_font_addon(), "Font");
    must_init(al_init_ttf_addon(), "TTF");
    must_init(al_install_mouse(), "Mouse");
    timer = al_create_timer(1.0/60.0);
    queue = al_create_event_queue();
    disp = al_create_display(635, 800);
    font = al_load_ttf_font("assets/OpenSans-ExtraBold.ttf", 20, 0);
    fontLW = al_load_ttf_font("assets/OpenSans-ExtraBold.ttf", 40, 0);
    bonusFont = al_load_ttf_font("assets/OpenSans-ExtraBold.ttf", 17, 0);
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue,al_get_mouse_event_source());
}

View::~View() {
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    if (backgroundLoaded) {
        al_destroy_bitmap(background);
    }
}

int View::loadBackground(){
    /*Methode permettant le chargement de l'arriere plan*/
    ALLEGRO_BITMAP *tmpBackground, *prevTarget;
    must_init(al_init_image_addon(), "Add-on image");
    if (!(tmpBackground = al_load_bitmap("assets/background.png"))) {
        std::cerr << "couldn't load background" << std::endl;
        return 1;
    }
    backgroundLoaded = true;
    background = al_create_bitmap(635,800);
    prevTarget = al_get_target_bitmap();
    al_set_target_bitmap(background);
    al_draw_scaled_bitmap(tmpBackground,0,0,al_get_bitmap_width(tmpBackground),al_get_bitmap_height(tmpBackground),0,0,635,800,0);
    al_set_target_bitmap(prevTarget);
    al_destroy_bitmap(tmpBackground);
    return 0;
}

void View::drawBlocs() {
    /*Methode permettant l'affichage des briques*/
    for (unsigned short int i=0;i<linker->getBlocs()->getMatriceSizes()[0];++i) {
        for (unsigned short int j=0;j<linker->getBlocs()->getMatriceSizes()[1];++j) {
            if (!linker->getBlocs()->getCell(j,i)->isBroken()){
                std::vector<int> color_map = linker->getBlocs()->getCell(j,i)->getColor();
                int pos1x = (linker->getBlocs()->getCell(j,i)->getPosX())-linker->getBlocs()->getCell(j,i)->getWide();
                int pos2x = (linker->getBlocs()->getCell(j,i)->getPosX())+linker->getBlocs()->getCell(j,i)->getWide();
                int pos1y = (linker->getBlocs()->getCell(j,i)->getPosY())-linker->getBlocs()->getCell(j,i)->getLen();
                int pos2y = (linker->getBlocs()->getCell(j,i)->getPosY())+linker->getBlocs()->getCell(j,i)->getLen();
                al_draw_filled_rectangle(pos1x,pos1y,pos2x,pos2y,al_map_rgb(color_map.at(0),color_map.at(1),color_map.at(2)));
                if (linker->getBlocs()->getCell(j,i)->getState() && linker->getBlocs()->getCell(j,i)->getPoints()==200) {
                    al_draw_rectangle(pos1x,pos1y,pos2x,pos2y,al_map_rgb(color_map.at(0)-150,color_map.at(1)-150,color_map.at(2)-150),5);
                }
                if (linker->getBlocs()->getCell(j,i)->getBonus()!='n') {
                    al_draw_text(font,al_map_rgb(50,50,50),linker->getBlocs()->getCell(j,i)->getPosX()-5,linker->getBlocs()->getCell(j,i)->getPosY()-15,0,(std::string()+(linker->getBlocs()->getCell(j,i)->getBonus())).c_str());
                }
            }
        }
    }
}

void View::drawBonus() {
    /*Methode permettant l'affichage des bonus*/
    for (unsigned short i=0;i<linker->getNbrBonus();++i) {
        al_draw_filled_circle(linker->getBonus(i)->getPosX(),linker->getBonus(i)->getPosY(),linker->getBonus(i)->getRadius(),al_map_rgb(linker->getBonus(i)->getColor().at(0),linker->getBonus(i)->getColor().at(1),linker->getBonus(i)->getColor().at(2)));
        al_draw_text(bonusFont,al_map_rgb(50,50,50),linker->getBonus(i)->getPosX()-4,linker->getBonus(i)->getPosY()-14,0,(std::string()+linker->getBonus(i)->getBonus()).c_str());
        if (linker->getBonus(i)->getBonus()=='b') {
            al_draw_circle(linker->getBonus(i)->getPosX(),linker->getBonus(i)->getPosY(),linker->getBonus(i)->getRadius()+1,al_map_rgb(0,0,0),1);
        }
    }
}

void View::displaying() {
    /*Methode permettant l'affichage du jeu*/
    int cellPosY=-1;
    al_start_timer(timer);
    while (playing)
    {
        al_wait_for_event(queue, &event);
        if (event.type == ALLEGRO_EVENT_TIMER) redraw = true;
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;


        if (event.type == ALLEGRO_EVENT_KEY_CHAR && linker->getScore()<linker->getMaxScore()) {
            const char*namekey = al_keycode_to_name(event.keyboard.keycode);
            std::cout << "DEBUG KEY PRESSED:" <<*namekey <<std::endl;
            switch (*namekey)
            {
            case 'd':
                linker->moveRacketRight();
                break;
            case 'p':
                linker->moveRacketRight();
                break;
            case 'q':
                linker->moveRacketLeft();
                break;
            case 'a':
                linker->moveRacketLeft();
                break;
            case 's':
                if (linker->startMovingBall()) break;
                cellPosY = linker->shootLaser();
                break;
            case 'x':
                if (!linker->getLife()) {
                    playing = false;
                }
                break;
            case 'r':
                linker->resetLife();
                linker->resetBlocs();
                linker->resetScore();
                linker->resetBall();
                break;
            case 'K':
                linker->setScore(linker->getMaxScore()-1);
                break;
            case 'n':
                linker->resetScore();
                linker->resetBall();
                linker->resetLife();
                linker->nextLevel();
                break;
            case 'b':
                linker->resetScore();
                linker->resetBall();
                linker->resetLife();
                linker->prevLevel();
                break;
            case 'o':
                linker->resetPB(linker->getLevel());
                break;
            default:
                break;
            }
        }
        else if (linker->getScore()>=linker->getMaxScore()) {
            linker->setPB(linker->getLevel(),linker->getMaxScore());
            if (linker->getMaxLevel()==linker->getLevel()&&event.type == ALLEGRO_EVENT_KEY_DOWN) {
                linker->resetLife();
                linker->resetScore();
                linker->resetBall();
                linker->loadLevel(1,-linker->getMaxLevel()+1);
            }
            else if (linker->getMaxLevel()>linker->getLevel()) {
                linker->resetScore();
                linker->resetBall();
                linker->resetLife();
                linker->nextLevel();
            }
        }
        else if (event.type == ALLEGRO_EVENT_MOUSE_AXES&&linker->getScore()<linker->getMaxScore()) {
            posMouseX = event.mouse.x;
            linker->moveRacketTo(posMouseX);
        }
        if (!linker->getLife()&&(linker->getScore()>=linker->getPB())) {
                linker->setPB(linker->getLevel(),linker->getScore());
        }

        if (redraw && al_is_event_queue_empty(queue)) {
            al_clear_to_color(al_map_rgb(0,0,0));
            if (backgroundLoaded) {
                al_draw_bitmap(background, 0, 0, 0);
            }
            if (linker->getScore()<linker->getMaxScore()) {
                linker->moveBall();
                linker->checkInteractionRB();
                linker->checkInteractionBB();
                linker->checkBonusPos();
                linker->checkInteractionRBonus();
                al_draw_filled_circle(linker->getBall()->getPosX(),linker->getBall()->getPosY(),linker->getBall()->getRadius(),al_map_rgb(255,255,255));
            }
            if (linker->getScore()>linker->getPB()) linker->updatePB(linker->getScore());

            int rpos1x = (linker->getRacket()->getPosX())-(linker->getRacket()->getlenSide()[0]);
            int rpos1y = (linker->getRacket()->getPosY())-(linker->getRacket()->getlenSide()[1]);
            int rpos2x = (linker->getRacket()->getPosX())+(linker->getRacket()->getlenSide()[0]);
            int rpos2y = (linker->getRacket()->getPosY())+(linker->getRacket()->getlenSide()[1]);
            al_draw_text(font,al_map_rgb(255,255,255), 0, 0, 0, (scorestr+std::to_string(linker->getScore())).c_str());
            al_draw_text(font,al_map_rgb(255,255,255),140,0,0,(lifestr+std::to_string(linker->getLife())).c_str());
            al_draw_text(font,al_map_rgb(255,255,255),240,0,0,(pbstr+std::to_string(linker->getPB())).c_str());
            drawBlocs();
            drawBonus();
            al_draw_filled_rectangle(rpos1x,rpos1y,rpos2x,rpos2y,al_map_rgb(0,0,0));
            if (cellPosY!=-1){
                al_draw_line(linker->getRacket()->getPosX(),linker->getRacket()->getPosY()-linker->getRacket()->getlenSide()[1],linker->getRacket()->getPosX(),cellPosY,al_map_rgb(255,0,0),2);
                cellPosY=-1;
                }
            if (linker->getScore()>=linker->getMaxScore()&&linker->getMaxLevel()==linker->getLevel()) {
                al_draw_filled_rectangle(0,0,635,800,al_map_rgba(0,0,0,150));
                al_draw_text(fontLW, al_map_rgb(255,255,255),210,200,0,"YOU WIN");
                al_draw_text(font,al_map_rgb(255,255,255),250,250,0,(scorestr+std::to_string(linker->getScore())).c_str());
                al_draw_text(font,al_map_rgb(255,255,255),190,300,0,"Press any key to restart");
            }
            if (!linker->getLife()) {
                al_draw_filled_rectangle(0,0,635,800,al_map_rgba(0,0,0,150));
                al_draw_text(fontLW, al_map_rgb(255,255,255),210,200,0,"YOU LOOSE");
                al_draw_text(font,al_map_rgb(255,255,255),280,250,0,(scorestr+std::to_string(linker->getScore())).c_str());
                al_draw_text(font,al_map_rgb(255,255,255),220,300,0,"Press 'R' to restart");
                al_draw_text(font,al_map_rgb(255,255,255),220,350,0,"Press 'X' to quit");
            }
            al_flip_display();

            redraw = false;
        }
    }
    

}