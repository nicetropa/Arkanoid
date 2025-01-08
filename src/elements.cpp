#include "../lib/elements.hpp"

Cell::Cell(int posx, int posy): posx_{posx}, posy_{posy} {}

Cell::~Cell() {}

void Cell::cellBreak() {
    /*Methode permettant de changer l'etat d'une brique a cassee*/
    if (will_break) {broken_=true;}
    else if(color!=GOLD)will_break++;
}

bool Cell::isBroken(){
    /*Methode permettant de savoir si une brique est cassee*/
    return broken_;
}

int Cell::getPosX() {
    /*Methode permettant la recuperation de la position en x de la brique*/
    return posx_;
}

int Cell::getPosY() {
    /*Methode permettant la recuperation de la position en y de la brique*/
    return posy_;
}

int Cell::getLen() {
    /*Methode permettant la recuperation de la hauteur de la brique*/
    return length;
}

int Cell::getWide() {
    /*Methode permettant la recuperation de la largeur de la brique*/
    return wide;
}

void Cell::reset() {
    /*Methode permettant la reinitilisation de la brique*/
    if (color!=NOCOLOR)broken_=false;
    if (color==SILVER||color==GOLD)will_break=0;
}

void Cell::setColor(char newcolor) {
    /*Methode permettant de donner une couleur a la brique*/
    switch (newcolor)
    {
    case 'W':
        color = WHITE;
        color_map = {255,255,255};
        break;

    case 'O':
        color = ORANGE;
        color_map = {255,165,0};
        break;
    case 'C':
        color = CYAN;
        color_map = {0,255,255};
        break;
    case 'G':
        color = GREEN;
        color_map = {0,255,0};
        break;
    case 'R':
        color = RED;
        color_map = {255,0,0};
        break;
    case 'B':
        color = BLUE;
        color_map = {0,0,255};
        break;
    case 'M':
        color = MAGENTA;
        color_map = {255,0,255};
        break;
    case 'Y':
        color = YELLOW;
        color_map = {255,255,0};
        break;
    case 'S':
        color = SILVER;
        color_map = {170,170,170};
        will_break = 0;
        break;
    case 'A':
        color=GOLD;
        will_break = 0;
        color_map = {212,175,55};
        break;
    default:
        break;
    }
}

std::vector<int> Cell::getColor() {
    /*getter de la plage de couleur de la brique*/
    return color_map;
}

int Cell::getPoints() {
    /*getter du nombre de point donne par la brique*/
    return color;
}

unsigned short Cell::getState() {
    /*getter de l'etat de la brique (si elle est sur le point de casser)*/
    return will_break;
}

char Cell::getBonus() {
    /*getter du bonus stocke dans la brique*/
    return bonus;
}

void Cell::setBonus(char new_bonus) {
    /*setter du bonus stocke dans la brique*/
    switch (new_bonus)
    {
    case 'l':
        bonus=LASER;
        break;
    case 'b':
        bonus=BIGGER;
        break;
    case 'c':
        bonus=CATCH;
        break;
    case 's':
        bonus=SLOW;
        break;
    case 't':
        bonus=TRIPLE;
        break;
    case 'h':
        bonus=HEAL;
        break;
    default:
        break;
    }
}

Blocs::Blocs(){
    std::ifstream file;
    std::string levelInfo;
    std::string tmp;
    unsigned short cellx=0;
    unsigned short celly=0;
    unsigned short indexTmp=0;
    unsigned short firstDone = 0;
    file.open("data/levels/1");
    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        exit(1);
    }
    getline(file,levelInfo);
    while (indexTmp<levelInfo.size()&&(indexTmp == 0 || levelInfo[indexTmp - 1] != '[')) {
        if (levelInfo[indexTmp]==','||levelInfo[indexTmp]=='[') {
            sizes[firstDone] = std::stoi(tmp);
            tmp="";
            firstDone++;
        }
        else {
            tmp+=levelInfo[indexTmp];
        }
        indexTmp++;
    }
    for (unsigned short int i=0;i<sizes[0];++i) {
        for (unsigned short int j=0;j<sizes[1];++j) {
            cellMatrice[i][j] = new Cell(25+j*45,60+i*25);
        }
    }
    while (indexTmp<levelInfo.size()&&(indexTmp == 0 || levelInfo[indexTmp - 1] != ']')) {
        if (levelInfo[indexTmp]!=';'&&levelInfo[indexTmp]!=']') {
            if (isupper(levelInfo[indexTmp])){
                if (levelInfo[indexTmp]!='N') {
                    cellMatrice[celly][cellx]->setColor(levelInfo[indexTmp]);
                    if (cellx+1>=sizes[1]) {
                        cellx=0;
                        celly++;
                    }
                    else cellx++;
                }
                else {
                    cellMatrice[celly][cellx]->cellBreak();
                    if (cellx+1>=sizes[1]) {
                        cellx=0;
                        celly++;
                    }
                    else cellx++;
                }
            }
            else {
                cellMatrice[celly][cellx]->setBonus(levelInfo[indexTmp]);
            }
        }
        indexTmp++;
    }
    max_score = std::stoi(std::string()+levelInfo[indexTmp]+levelInfo[indexTmp+1]+levelInfo[indexTmp+2]+levelInfo[indexTmp+3]);
    file.close();
}

Blocs::~Blocs() {
    for (unsigned short int i=0;i<sizes[0];++i) {
        for (unsigned short int j=0;j<sizes[1];++j) {
            delete cellMatrice[i][j];
            cellMatrice[i][j] = nullptr;
        }
    }
}

unsigned short Blocs::getLevelSize(unsigned short level, unsigned short index) {
    /*Methode permettant l'obtention de la taille de la grille du niveau*/
    std::ifstream file;
    std::string levelInfo;
    std::string tmp;
    std::string path = "data/levels/";
    unsigned short indexTmp=0;
    unsigned short firstDone = 0;
    unsigned short sizesLL [2] = {};
    path += std::to_string(level);
    file.open(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        exit(1);
    }
    getline(file,levelInfo);
    while (indexTmp<levelInfo.size()&&(indexTmp == 0 || levelInfo[indexTmp - 1] != '[')) {
        if (levelInfo[indexTmp]==','||levelInfo[indexTmp]=='[') {
            sizesLL[firstDone] = std::stoi(tmp);
            tmp="";
            firstDone++;
        }
        else {
            tmp+=levelInfo[indexTmp];
        }
        indexTmp++;
    }
    return sizesLL[index];
}

void Blocs::unloadLastLevel(unsigned short level, short next) {
    /*Methode permettant le dechargement du niveau*/
    std::cout << level+(-1*next) << std::endl;
    unsigned short lastSizes[2]={getLevelSize(level+(-1*next),0),getLevelSize(level+(-1*next),1)};
    for (unsigned short int i=0;i<lastSizes[0];++i) {
        for (unsigned short int j=0;j<lastSizes[1];++j) {
            delete cellMatrice[i][j];
        }
    }

}

void Blocs::loadLevel(unsigned short level, short next) {
    /*Methode permettant le chargement d'un niveau*/
    std::ifstream file;
    std::string levelInfo;
    std::string tmp;
    std::string path = "data/levels/";
    unsigned short cellx=0;
    unsigned short celly=0;
    unsigned short indexTmp=0;
    unsigned short firstDone = 0;
    path += std::to_string(level);
    unloadLastLevel(level,next);
    file.open(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        exit(1);
    }
    getline(file,levelInfo);
    while (indexTmp<levelInfo.size()&&(indexTmp == 0 || levelInfo[indexTmp - 1] != '[')) {
        if (levelInfo[indexTmp]==','||levelInfo[indexTmp]=='[') {
            sizes[firstDone] = std::stoi(tmp);
            tmp="";
            firstDone++;
        }
        else {
            tmp+=levelInfo[indexTmp];
        }
        indexTmp++;
    }
    for (unsigned short int i=0;i<sizes[0];++i) {
        for (unsigned short int j=0;j<sizes[1];++j) {
            cellMatrice[i][j] = new Cell(25+j*45,60+i*25);
        }
    }
    while (indexTmp<levelInfo.size()&&(indexTmp == 0 || levelInfo[indexTmp - 1] != ']')) {
        if (levelInfo[indexTmp]!=';'&&levelInfo[indexTmp]!=']') {
            if (isupper(levelInfo[indexTmp])){
                if (levelInfo[indexTmp]!='N') {
                    cellMatrice[celly][cellx]->setColor(levelInfo[indexTmp]);
                    if (cellx+1>=sizes[1]) {
                        cellx=0;
                        celly++;
                    }
                    else cellx++;
                }
                else {
                    cellMatrice[celly][cellx]->cellBreak();
                    if (cellx+1>=sizes[1]) {
                        cellx=0;
                        celly++;
                    }
                    else cellx++;
                }
            }
            else {
                cellMatrice[celly][cellx]->setBonus(levelInfo[indexTmp]);
            }
        }
        indexTmp++;
    }
    max_score = std::stoi(std::string()+levelInfo[indexTmp]+levelInfo[indexTmp+1]+levelInfo[indexTmp+2]+levelInfo[indexTmp+3]);
    file.close();
}

unsigned short* Blocs::getMatriceSizes() {
    /*getter de la taille de la matrice de jeu*/
    return sizes;
}

Cell* Blocs::getCell(int posx, int posy) {
    /*getter renvoyant la brique correspondant a la position entre en parametre(position dans la matrice de jeu)*/
    return cellMatrice[posy][posx];
}

void Blocs::resetCells() {
    /*Methode permettant la reinitialisation de toutes les briques de la matrice de jeu*/
    for (unsigned short int i=0;i<sizes[0];++i) {
        for (unsigned short int j=0;j<sizes[1];++j) {
            cellMatrice[i][j]->reset();
        }
    }
}

unsigned Blocs::getMaxScore() {
    /*getter du score maximal du niveau*/
    return max_score;
}  

Racket::Racket(){};

Racket::~Racket(){};

float Racket::getPosX() {
    /*getter de la position de la raquette en x*/
    return posx_;
}

float Racket::getPosY() {
    /*getter de la postion de la raquette en y*/
    return posy_;
}

float* Racket::getlenSide() {
    /*getter des dimensions de la raquette*/
    return lenSide_;
}

int Racket::getBaseLen() {
    /*getter de la longueur de base de la raquette*/
    return baseLen_;
}

void Racket::moveLeft() {
    /*methode permettant le deplacement de la raquette vers la gauche*/
    if (posx_>0+lenSide_[0]) posx_-=20;
}

void Racket::moveRight() {
    /*methode permettant le deplacement de la raquette vers la droite*/
    if (posx_<635-lenSide_[0]) posx_+=20;
}

void Racket::moveTo(int posx) {
    /*methode permettant le deplacement de la raquette vers la position en x entree en parametre*/
    posx_=posx;
}

void Racket::setLenSide(int new_len) {
    /*methode permettant de changer la taille de la raquette par la taille entree en parametre*/
    lenSide_[0]=new_len;
}

Ball::Ball(){}

Ball::~Ball(){}

float Ball::getPosX() {
    /*getter de la position en x de la balle*/
    return posx_;
}

float Ball::getPosY() {
    /*getter de la position en y de la balle*/
    return posy_;
}

void Ball::setPosX(float newPosX) {
    /*setter de la position en x de la balle*/
    posx_=newPosX;
}

void Ball::setPosY(float newPosY) {
    /*setter de la position en y de la balle*/
    posy_=newPosY;
}

float Ball::getRadius() {
    /*getter du rayon de la balle*/
    return radius_;
}

void Ball::ballReset() {
    /*methode permettant de reinitialiser les attributs de la balle*/
    setPosX(310);
    setPosY(600);
    setDecompSpeed(0,0);
    setDecompSpeed(0,1);
}

void Ball::move() {
    /*methode permettant le deplacement de la balle*/
    if (0<posx_-radius_ && posx_+radius_<635){
        posx_+= getDecompSpeed()[0];
    }
    else{
        setDecompSpeed(-getDecompSpeed()[0],0);
        posx_+= getDecompSpeed()[0];
    }
    if (0<posy_-radius_){
        posy_+= getDecompSpeed()[1];
    }
    else{
        setDecompSpeed(-getDecompSpeed()[1],1);
        posy_+= getDecompSpeed()[1];
    }
}

float* Ball::getDecompSpeed() {
    /*getter de la vitesse decomposee de la balle*/
    return decompSpeed_;
}

void Ball::setDecompSpeed(float newSpeed, int index) {
    /*setter de la vitesse decomposee de la balle*/
    decompSpeed_[index] = newSpeed;
}

float Ball::getSpeed() {
    /*getter de la vitesse de la balle*/
    return speed_;
}

void Ball::setSpeed(float newspeed) {
    /*setter de la vitesse de la balle*/
    speed_=newspeed;
}

float Ball::getBaseSpeed() {
    /*getter de la vitesse de base de la balle*/
    return base_speed_;
}

Bonus::Bonus(int x,int y) {
    setPosX(x);
    setPosY(y);
}

char Bonus::getBonus() {
    /*getter du bonus stocke dans la capsule de bonus*/
    return bonus;
}

void Bonus::setBonus(char new_bonus) {
    /*setter du bonus stocke dans la capsule de bonus*/
    switch (new_bonus)
    {
    case 'l':
        bonus=LASER;
        color = {255,0,0};
        break;
    case 'b':
        bonus=BIGGER;
        color = {0,0,255};
        break;
    case 'c':
        bonus=CATCH;
        color = {0,255,0};
        break;
    case 's':
        bonus=SLOW;
        color = {255,128,0};
        break;
    case 't':
        bonus=TRIPLE;
        color = {43,255,255};
        break;
    case 'h':
        bonus=HEAL;
        color = {128,128,128};
        break;
    default:
        break;
    }
}

std::vector<int> Bonus::getColor() {
    /*getter de la plage de couleur de la capsule de bonus*/
    return color;
}

void Bonus::setColor(int r, int g, int b) {
    /*setter de la plage de couleur de la capsule de bonus*/
    color = {r,g,b};
}