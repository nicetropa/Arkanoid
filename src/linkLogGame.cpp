#include "../lib/linkLogGame.hpp"

Linker::Linker(){
    getPBfromFile(1);
}

Linker::~Linker() {
    for (unsigned short i=0;i<bonus_vect_.size();++i) {
        delete bonus_vect_.at(i);
    }
    delete ball_;
    delete racket_;
    delete blocs_;
}

unsigned short int Linker::getScore() {/*getter du score du joueur*/return score_;}

void Linker::checkInteractionRB() {
    /*methode permettant les interaction entre la raquette et la balle*/
    if ((racket_->getPosX()-racket_->getlenSide()[0]<ball_->getPosX()+ball_->getRadius()) && (ball_->getPosX()-ball_->getRadius()<racket_->getPosX()+racket_->getlenSide()[0]) && (racket_->getPosY()+racket_->getlenSide()[1]>ball_->getPosY()+ball_->getRadius()) && (racket_->getPosY()-racket_->getlenSide()[1]<ball_->getPosY()+ball_->getRadius())){
        float newAngleD = 30+120*(1-((ball_->getPosX()-racket_->getPosX()+racket_->getlenSide()[0])/(racket_->getlenSide()[0]*2)));
        float newAngleR = (newAngleD*(M_PI/180));
        if (catch_ball) {
            if (timer_catch!=-1&&time(NULL)-timer_catch>=3) {
                ball_->setDecompSpeed(ball_->getSpeed(),1);
                timer_catch=-1;
            }
            else {
                if (timer_catch==-1) timer_catch=time(NULL);
                ball_->setDecompSpeed(0,0);
                ball_->setDecompSpeed(0,1);
                ball_->setPosX(racket_->getPosX());
            }
        }
        else {
            ball_->setDecompSpeed(ball_->getSpeed()*cos(newAngleR),0);
            ball_->setDecompSpeed(ball_->getSpeed()*sin(newAngleR),1);
        }
    }
}

void Linker::enlargementBonus() {
    /*methode permettant l'application du bonus d'elargissement de raquette*/
    racket_->setLenSide(racket_->getBaseLen()+20);
}

void Linker::unenlargementBonus() {
    /*methode permettant de desappliquer le bonus d'elargissement de raquette*/
    racket_->setLenSide(racket_->getBaseLen());
}

void Linker::healBonus() {
    /*methode permettant l'ajout de vie accorde par le bonus heal*/
    ++life_;
}

void Linker::catchBonus() {
    /*methode permettant l'application du bonus permettant d'attraper la balle*/
    catch_ball=true;
}

void Linker::unCatchBonus() {
    /*methode permettant de desappliquer le bonus permettant d'attraper la balle*/
    catch_ball=false;
}

void Linker::slowBonus() {
    /*methode permettant le ralentissement de la balle accorde par le bonus slow*/
    if (ball_->getSpeed()+1<0) ball_->setSpeed((ball_->getSpeed()+1));
}

void Linker::unSlowBonus() {
    /*methode permettant la desapplication du bonus slow*/
    ball_->setSpeed(ball_->getBaseSpeed());
}

bool Linker::startMovingBall() {
    /*methode permettant de donner une vitesse à la balle lorsqu'elle est a l'arret*/
    if (ball_->getDecompSpeed()[1]==0){
        ball_->setDecompSpeed(ball_->getSpeed(),1);
        resetTimerCatch();
        return true;
    }
    return false;
}

int Linker::shootLaser() {
    /*methode permettant de tirer des laser accorde par le bonus laser*/
    if (act_bonus!=LASER) return -1;
    if (timer_laser==-1) {timer_laser=time(NULL);}
    else if (time(NULL)-timer_laser<=3){return -1;}
    for (unsigned short i=blocs_->getLevelSize(level_,0)-1;i!=0;i--) {
        unsigned short j=blocs_->getLevelSize(level_,1)-1;
        for (;j!=0;j--) {
            if (!(blocs_->getCell(j,i)->isBroken())&&(blocs_->getCell(j,i)->getPosX()-blocs_->getCell(j,i)->getWide()<=racket_->getPosX())&&(racket_->getPosX()<=blocs_->getCell(j,i)->getPosX()+blocs_->getCell(j,i)->getWide())&&(blocs_->getCell(j,i)->getPoints()!=GOLD)) {
                blocs_->getCell(j,i)->cellBreak();
                timer_laser=time(NULL);
                break;
            }
        }
        if (j!=0) return blocs_->getCell(j,i)->getPosY()+blocs_->getCell(j,i)->getLen();
    }
    return 0;
}

void Linker::handleBonus() {
    /*methode permettant de gerer quel bonus appliquer*/
    switch (act_bonus)
    {
    case BIGGER:
        enlargementBonus();
        break;
    case HEAL:
        healBonus();
        break;
    case CATCH:
        catchBonus();
        break;
    case SLOW:
        slowBonus();
        break;
    default:
        break;
    }
}

void Linker::unHandleBonus() {
    /*methode permettant de gerer quel bonus desappliquer*/
    switch (prev_bonus)
    {
    case BIGGER:
        unenlargementBonus();
        break;
    case CATCH:
        unCatchBonus();
        break;
    case SLOW:
        unSlowBonus();
        break;
    default:
        break;
    }
}

char Linker::getActBonus() {/*getter du bonus actuel*/return act_bonus;}

void Linker::checkInteractionRBonus() {
    /*methode permettant les interaction entre la raquette et les bonus*/
    for (unsigned short i=0;i<bonus_vect_.size();++i) {
        if ((racket_->getPosX()-racket_->getlenSide()[0]<bonus_vect_.at(i)->getPosX()+bonus_vect_.at(i)->getRadius()) && (bonus_vect_.at(i)->getPosX()-bonus_vect_.at(i)->getRadius()<racket_->getPosX()+racket_->getlenSide()[0]) && (racket_->getPosY()+racket_->getlenSide()[1]>bonus_vect_.at(i)->getPosY()+bonus_vect_.at(i)->getRadius()) && (racket_->getPosY()-racket_->getlenSide()[1]<bonus_vect_.at(i)->getPosY()+bonus_vect_.at(i)->getRadius())) {
            prev_bonus=act_bonus;
            act_bonus=bonus_vect_.at(i)->getBonus();
            if (prev_bonus!=act_bonus) unHandleBonus();
            handleBonus();
            delete bonus_vect_.at(i);
            bonus_vect_.erase(bonus_vect_.begin()+i);
            --i;
        }
    }
}

void Linker::checkInteractionBB() {
    /*methode permettant les interaction entre la balle et les briques*/
    for (unsigned short int i=0;i<blocs_->getMatriceSizes()[1];i++) {
        for (unsigned short int j=0;j<blocs_->getMatriceSizes()[0];j++) {
            if (!blocs_->getCell(i,j)->isBroken()) {
                checkInteractionCB(i,j);
            }
        }
    }
    collision_done=false;
}

void Linker::checkInteractionCB(int cellPosX, int cellPosY) {
    /*methode permettant les interactions entre la balle et une brique*/
    int pos1x = (blocs_->getCell(cellPosX,cellPosY)->getPosX())-blocs_->getCell(cellPosX,cellPosY)->getWide();
    int pos2x = (blocs_->getCell(cellPosX,cellPosY)->getPosX())+blocs_->getCell(cellPosX,cellPosY)->getWide();
    int pos1y = (blocs_->getCell(cellPosX,cellPosY)->getPosY())-blocs_->getCell(cellPosX,cellPosY)->getLen();
    int pos2y = (blocs_->getCell(cellPosX,cellPosY)->getPosY())+blocs_->getCell(cellPosX,cellPosY)->getLen();
    
    int distTop = ball_->getPosY()-pos1y;
    int distBot = pos2y-ball_->getPosY();
    int distLeft = ball_->getPosX()-pos1x;
    int distRight = pos2x-ball_->getPosX();
    
    if (!collision_done&&(pos1x<(ball_->getPosX()+ball_->getRadius()) && pos2x>(ball_->getPosX()-ball_->getRadius()))&&((ball_->getPosY()+ball_->getRadius()>pos1y)&&(ball_->getPosY()-ball_->getRadius()<pos2y))) {
        collision_done=true;
        float smallestDist = std::numeric_limits<float>::max();
        side collisionSide;

        if (ball_->getDecompSpeed()[1] > 0 && distTop < smallestDist) {
            smallestDist = distTop;
            collisionSide = TOP;
        }

        if (ball_->getDecompSpeed()[1] < 0 && distBot < smallestDist) {
            smallestDist = distBot;
            collisionSide = BOT;
        }

        if (ball_->getDecompSpeed()[0] > 0 && distLeft < smallestDist) {
            smallestDist = distLeft;
            collisionSide = LEFT;
        }

        if (ball_->getDecompSpeed()[0] < 0 && distRight < smallestDist) {
            smallestDist = distRight;
            collisionSide = RIGHT;
        }
        
        if (collisionSide==TOP) {
            ball_->setPosY(pos1y - ball_->getRadius());
            ball_->setDecompSpeed(-ball_->getDecompSpeed()[1],1);
        }
        else if (collisionSide==BOT) {
            ball_->setPosY(pos2y + ball_->getRadius());
            ball_->setDecompSpeed(-ball_->getDecompSpeed()[1],1);
        }
        else if (collisionSide==LEFT){
            ball_->setPosX(pos1x - ball_->getRadius());
            ball_->setDecompSpeed(-ball_->getDecompSpeed()[0],0);
        }
        else if (collisionSide==RIGHT){
            ball_->setPosX(pos2x + ball_->getRadius());
            ball_->setDecompSpeed(-ball_->getDecompSpeed()[0],0);
        }
                
        blocs_->getCell(cellPosX,cellPosY)->cellBreak();
        score_ += blocs_->getCell(cellPosX,cellPosY)->getPoints();
        
        if (blocs_->getCell(cellPosX,cellPosY)->getBonus()!='n') {
            Bonus* b = new Bonus(blocs_->getCell(cellPosX,cellPosY)->getPosX(),blocs_->getCell(cellPosX,cellPosY)->getPosY());
            b->setBonus(blocs_->getCell(cellPosX,cellPosY)->getBonus());
            b->setDecompSpeed(2,1);
            bonus_vect_.push_back(b);
        }
    }
}

unsigned Linker::getMaxScore() {/*getter du score maximal possible du niveau*/return blocs_->getMaxScore();}

int Linker::checkLoosedBall() {
    /*methode permettant de savoir si la balle est tombe*/
    return (ball_->getPosY()-ball_->getRadius())>=FALL_POS;
}

void Linker::resetBall() {
    /*methode permettant de reinitialiser les attributs de la balle et de decrementer le nombre de vie lorsque la balle est perdu*/
    if (score_) life_--;
    if (life_) ball_->ballReset();
}

unsigned short Linker::getLife() {
    /*getter de la vie du joueur*/
    return life_;
}

void Linker::checkSpeed() {
    /*methode permettant de verifier la vitesse de la balle et d'accelerer la balle si elle est plus lente que la vietesse de base(utilise pour le bonus de deceleration)*/
    if (ball_->getBaseSpeed()!=ball_->getSpeed()) {
        if (timer_speed==-1) timer_speed=time(NULL);
        else if (time(NULL)-timer_speed>=20){
            timer_speed=time(NULL);
            ball_->setSpeed(ball_->getSpeed()-1);
        }
    }
}

void Linker::moveBall() {
    /*methode permettant le mouvement de la balle et des bonus*/
    checkSpeed();
    ball_->move();
    if (checkLoosedBall()&&life_&&(score_<getMaxScore())) {
        resetBall();
    }
    for (Bonus* b : bonus_vect_) b->move();
}

void Linker::moveRacketLeft() {
    /*methode permettant le deplacement de la raquette vers la gauche*/
    if (life_) {
        racket_->moveLeft();
    }
}

void Linker::moveRacketRight() {
    /*methode permettant le deplacement de la raquette vers la droite*/
    if (life_) {
        racket_->moveRight();
    }
}

void Linker::resetLife() {
    /*methode permettant de reinitialiser la vie du joueur*/
    life_=BASE_LIFE;
}

Blocs* Linker::getBlocs() {
    /*methode permettant de recuperer la matrice de jeu*/
    return blocs_;
}

Ball* Linker::getBall() {
    /*methode permettant de recuperer la balle*/
    return ball_;
}

Racket* Linker::getRacket() {
    /*methode permettant de recuperer la raquette*/
    return racket_;
}

void Linker::resetBlocs() {
    /*methode permettant de reinitialiser la matrice de jeu*/
    blocs_->resetCells();
}

void Linker::setScore(int newscore) {
    /*setter du score*/
    score_ = newscore;
}

void Linker::resetScore() {
    /*methode permettant de reinitialiser le score*/
    score_=0;
}

void Linker::moveRacketTo(int posx) {
    /*methode permettant le deplacement de la raquette vers la position en x entree en parametre*/
    if (life_) racket_->moveTo(posx);
}

void Linker::loadLevel(unsigned short level,short next) {
    /*methode permettant de charger le niveau entre en parametre (le deuxieme parametre permet de charger le precedent niveau)*/
    level_ = level;
    getPBfromFile(level);
    blocs_->loadLevel(level,next);
}

unsigned short Linker::getLevel() {
    /*getter du niveau actuel*/
    return level_;
}

void Linker::nextLevel() {
    /*methode permettant de passer au niveau suivant*/
    if (level_+1<=MAX_LEVEL) {
        setLevel(level_+1);
        loadLevel(level_,1);
    }
}

void Linker::prevLevel() {
    /*methode permettant de passer au niveau precedent*/
    if (level_-1>=FIRST_LEVEL) {
        setLevel(level_-1);
        loadLevel(level_,-1);
    }
}

void Linker::setLevel(unsigned short newlevel) {
    /*setter du niveau*/
    level_=newlevel;
}

unsigned short Linker::getMaxLevel() {
    /*getter du niveau maximal disponible*/
    return MAX_LEVEL;
}

unsigned short Linker::getPB() {
    /*getter du meilleur score*/
    return pb_;
}

void Linker::getPBfromFile(unsigned short level) {
    /*methode permettant de recuperer le meilleur score depuis le fichier*/
    std::ifstream file;
    std::string pbscore;
    std::string path = "data/PBScores/"+std::to_string(level);
    file.open(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        exit(1);
    }
    getline(file,pbscore);
    file.close();
    pb_=std::stoi(pbscore);
}

void Linker::setPB(unsigned short level, unsigned new_pb) {
    /*methode permettant de sauvegarder le meilleur score dans le fichier*/
    std::ofstream file;
    std::string path = "data/PBScores/"+std::to_string(level);
    file.open(path, std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        exit(1);
    }
    file << new_pb;
    file.close();
}

void Linker::updatePB(unsigned short new_pb) {
    /*methode permettant de mettre a jour le meilleur score lorsque celui ci est depasse par le joueur*/
    pb_=new_pb;
}

void Linker::resetPB(unsigned short level) {
    /*methode permettant de reinitialiser le meilleur score*/
    updatePB(0);
    setPB(level,0);
}

int Linker::getNbrBonus() {
    /*methode permettant la recuperation du nombre de bonus en jeu*/
    return bonus_vect_.size();
}

Bonus* Linker::getBonus(unsigned short i) {
    /*methode permettant la recuperation de la capsule de bonus depuis le vecteur stockant les bonus a la position entree en parametre*/
    if (i>=bonus_vect_.size()) {
        throw std::out_of_range("getBonus index out of range");
    }
    return bonus_vect_.at(i);
}

void Linker::checkBonusPos() {
    /*methode permettant de savoir si les capsules de bonus sont tombe*/
    for (unsigned short i=0;i<bonus_vect_.size();++i) {
        if (bonus_vect_.at(i)->getPosY()>FALL_POS) {
            delete bonus_vect_.at(i);
            bonus_vect_.erase(bonus_vect_.begin()+i);
            --i;
        }
    }
}

void Linker::resetTimerCatch() {
    /*methode permettant de reinitialiser le timer du bonus permettant d'attraper la balle*/
    timer_catch=-1;
    }