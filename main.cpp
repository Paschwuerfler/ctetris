
#include "main.h"
#include "treiber.h"


const bool SerialActive = true;

const int sizeM = 12; //size of the matrix
const int sizeplayfieldx = 7;
const int sizeplayfieldy = 12;

const int playfieldposx = 0;
const int playfieldposy = 0;

const int maxpiecesizex = 2;
const int maxpiecesizey = 4;


const bool lp[maxpiecesizex][maxpiecesizey] = {  //the l piece
        { 1, 0, 0, 0 }, { 1, 1, 1, 0 }, };

const bool lpm[maxpiecesizex][maxpiecesizey] = { { 0, 0, 1, 0 }, //the mirrored l piece
                                                 { 1, 1, 1, 0 }, };

int playfield[sizeplayfieldx][sizeplayfieldy];
int playfieldold[sizeplayfieldx][sizeplayfieldy];

int a[sizeM][sizeM];

bool dets = false;

int posx = 0;  //position and rotation of the piece
int posy = 0;
int rotation = 0;

int posxold = 0;  //position and rotation of the piece
int posyold = 0;
int rotationold = 0;

int main() {
    setup();
    while(true) {
        loop();
    }
}


void setup() {

    // put your setup code here, to run once:
    sbegin(9600);
    sprintln('1');
    posx = 3;
    posy = 3;
    transfer();

}

void loop() {
    // put your main code here, to run repeatedly:
    minit();
    mprint();
    delay(1000);
    draw(lp,0); //undraw old piece
    draw(lp,1); //draw new piece
    transfer();
    mprint();
    rotation ++;
    sprint('l');





}

void minit() {  //blanks the matrix (initializer)
    for (int i = 0; i <= sizeM; i++) {
        for (int o = 0; o <= sizeM; o++) {
            a[i][o] = 0;
        }
    }
}

void playfieldinit() {  //blanks the matrix (initializer)
    for (int i = 0; i <= sizeplayfieldx; i++) {
        for (int o = 0; o <= sizeplayfieldy; o++) {
            playfield[i][o] = 0;
        }
    }
    memcpy(playfield, playfieldold, sizeof(playfield));
}

void mprint() {  // does the actual drawing
    for (int i = 0; i <= sizeM; i++) {
        for (int o = 0; o <= sizeM; o++) {
            sprint(a[i][o]);

        }
        sprintln(' ');
    }
    sprintln('-');
}

void transfer() { //transfers playfield onto canvas, updates pos & rot
    posxold = posx;
    posyold = posy;
    rotationold = rotation;
    memcpy(playfieldold, playfield, sizeof(playfield));
    for (int i = 0; i <= sizeplayfieldx; i++) {
        for (int o = 0; o <= sizeplayfieldy; o++) {
            a[playfieldposx + i][playfieldposy + o] = playfield[i][o];
        }
    }
}



bool mput(int x, int y, int color, bool clearcheck) { //'collision' check //only for playfield
    if (clearcheck) {
        if (a[x][y] != 0 && x == 0 && y == 0) {
            return false;
        }
        playfield[x][y] = color;
        return true;
    }
}


bool draw(const bool b[2][4],int color) {  //handels piece drawing
switch (rotation) {

case 0: //no rot
for (int i = 0; i <= maxpiecesizex; i++) {
for (int o = 0; o <= maxpiecesizey; o++) {
if (b[i][o]) {
if (mput(posx + i, posy + o, color, true) == false) {
undoplayfield();
return false;
}
}

}
}
return true;

break;

case 1:  //one clockwise
for (int i = 0; i <= maxpiecesizex; i++) {
for (int o = 0; o <= maxpiecesizey; o++) {
if (b[i][o]) {
if (mput(posx + o, posy - i, color, true) == false) {
undoplayfield();
return false;
}
}
}
}
return true;

break;

case 2: //upside down
for (int i = 0; i <= maxpiecesizex; i++) {
for (int o = 0; o <= maxpiecesizey; o++) {
if (b[i][o]) {
if (mput(posx - i, posy - o, color, true) == false) {
undoplayfield();
return false;
}
}
}
}
return true;

break;

case 3: //one counterclockwise
for (int i = 0; i <= maxpiecesizex; i++) {
for (int o = 0; o <= maxpiecesizey; o++) {
if (b[i][o]) {
if (mput(posx + i, posy - o, color, true) == false) {
undoplayfield();
return false;
}
}
}
}
return true;

break;
default:
sprintln('r');
rotation = 0;
return false;

}
}

void undoplayfield() {
    posx = posxold;
    posy = posyold;
    rotation = rotationold;
    memcpy(playfield, playfieldold, sizeof(playfield));
    sprintln('u');
}
