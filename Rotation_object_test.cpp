#include "RotationLib.h"

int main() {
    srand(time(NULL));
    txCreateWindow(1200, 800);
    txClear();
    ro_t test;
    string fname1 = "fish2.1.txt";
    string fname2 = "fish1.2.txt";
    HDC background = txLoadImage("background.bmp" );
    vector <pair<ro_t, double>> flock(20);
    for (auto &k : flock) {
        if(rand() % 2)
            k.first.get_from_file(fname1.c_str(), 10, 541, 221);
        else
            k.first.get_from_file(fname2.c_str(), 10, 431, 126);
    }
    for (auto &k : flock)
        k.first.set_xy(rand() % 1200, rand() % 800);
    for (auto &k : flock)
        k.second = (rand() % 100 ) / 10 + 5;
    txBegin();
    while(!GetAsyncKeyState(VK_ESCAPE)){
        txSetFillColor(TX_WHITE);
        txClear();
        txBitBlt(0, 0, background);
        for (auto &k : flock) {
            k.first.rotate_by_mouse(RINVERSE_Y);
            if (GetAsyncKeyState(VK_LBUTTON)){
                double mx = txMouseX() , my = txMouseY() ;
                POINT xy = k.first.returnXY();
                double dx = mx - xy.x, dy = my - xy.y;
                double r = sqrt(dx * dx + dy * dy) + 1;
                double ddx = k.second * dx / r;
                double ddy = k.second * dy / r;
                k.first.set_xy(xy.x + ddx, xy.y + ddy);
            }
        }
        for (auto &k : flock)
            k.first.draw();
        txSleep(10);
    }
    txEnd();
    txDeleteDC(background);
}
