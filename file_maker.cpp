#include <TXLib.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

enum ObjectType {CIRCLE, POLYGON, LINE};

class polygon {
private:
    vector<POINT> v;
    COLORREF fillcolor = TX_NULL;
    COLORREF outlinecolor = TX_NULL;
public:
    void Vreturn (POINT *p) {
        for (unsigned int i = 0; i < v.size(); i++)
            p[i] = v[i];

    }
    void push_back(POINT p) {
        v.push_back(p);
    }
    void push_back(LONG X, LONG Y) {
        POINT p = {X, Y};
        push_back(p);
    }
    void print(ofstream &output) {
        output << v.size() << "\n";
        for (auto k : v)
            output << k.x << " " << k.y << "\n";
        output << GetRValue(fillcolor) << " " << GetGValue(fillcolor) << " " << GetBValue(fillcolor) << "\n";
    }
    void set_fill_color(COLORREF fc) {
        fillcolor = fc;
    }
    void set_outline_color(COLORREF fc) {
        outlinecolor = fc;
    }
    void draw() {
        POINT p[v.size()];
        Vreturn(p);
        txSetFillColor(fillcolor);
        txSetColor(outlinecolor);
        txPolygon(p, v.size());
    }

};

class circle {
private:
    double x;
    double y;
    double r;
    COLORREF fillcolor;
    COLORREF outlinecolor;
public:
    void set(double X, double Y, double R) {
        x = X;
        y = Y;
        r = R;
    }
    void print(ofstream &output) {
        output << "1\n";
        output << x << " " << y << " " << r << "\n";
        output << GetRValue(fillcolor) << " " << GetGValue(fillcolor) << " " << GetBValue(fillcolor) << "\n";
    }
    void set_fill_color(COLORREF fc) {
        fillcolor = fc;
    }
    void set_outline_color(COLORREF fc) {
        outlinecolor = fc;
    }
    void draw() {
        txSetFillColor(fillcolor);
        txSetColor(outlinecolor);
        txCircle(x, y, r);
    }

};

class line {
private:
    double x1;
    double y1;
    double x2;
    double y2;
    double r;
    COLORREF outlinecolor;
public:
    void set_outline_color(COLORREF fc) {
        outlinecolor = fc;
    }

    void set(double X1, double Y1, double X2, double Y2, double R) {
        x1 = X1;
        y1 = Y1;
        x2 = X2;
        y2 = Y2;
        r = R;
    }

    void set(POINT p1, POINT p2, double R) {
        x1 = p1.x;
        y1 = p1.y;
        x2 = p2.x;
        y2 = p2.y;
        r = R;
    }
    void print(ofstream &output) {
        output << "2\n";
        output << x1 << " " << y1 << " ";
        output << x2 << " " << y2 << " " << r << "\n";
        output << GetRValue(outlinecolor) << " " << GetGValue(outlinecolor) << " " << GetBValue(outlinecolor) << "\n";
    }

    void draw() {
        txSetColor(outlinecolor, r);
        txLine(x1, y1, x2, y2);
    }

};

class Object {
private:
    vector<line> l;
    vector<polygon> p;
    vector<circle> c;
    vector<string> coments;
    vector<ObjectType> order;
public:
    void push_back(circle C, string coment = "") {
        order.push_back(CIRCLE);
        c.push_back(C);
        coments.push_back(coment);
    }
    void push_back(line L, string coment = "") {
        order.push_back(LINE);
        l.push_back(L);
        coments.push_back(coment);
    }
    void push_back(polygon P, string coment = "") {
        order.push_back(POLYGON);
        p.push_back(P);
        coments.push_back(coment);
    }
    void get_from_file(string name) {
         ifstream input;
         input.open(name);
         int n;
         while (input >> n) {
            if (n == 1) {
                double x, y, r;
                int red, green, blue;
                input >> x >> y >> r;
                input >> red >> green >> blue;
                circle k;
                k.set(x, y, r);
                k.set_fill_color(RGB(red, green, blue));
                k.set_outline_color(TX_BLACK);
                push_back(k);
            }
            else if (n == 2) {
                double x1, y1, x2, y2, r;
                int red, green, blue;
                input >> x1 >> y1 >> x2 >> y2 >> r;
                input >> red >> green >> blue;
                line k;
                k.set(x1, y1, x2, y2, r);
                k.set_outline_color(RGB(red, green, blue));
                push_back(k);
            }
            else {
                polygon k;
                for (int i = 0; i < n; i++) {
                    int X, Y;
                    input >> X >> Y;
                    k.push_back(X, Y);

                }
                int red, green, blue;
                input >> red >> green >> blue;
                k.set_fill_color(RGB(red, green, blue));
                k.set_outline_color(TX_BLACK);
                push_back(k);
            }

         }
    }

    void put_to_file (string name) {
        ofstream output;
        output.open(name);
        int il = 0, ip = 0, ic = 0;
        for (unsigned int i = 0; i < order.size(); i++) {
            output << coments[i] << "\n";
            if (order[i] == CIRCLE) {
                c[ic].print(output);
                ic++;
            }
            if (order[i] == POLYGON) {
                p[ip].print(output);
                ip++;
            }
            if (order[i] == LINE) {
                l[il].print(output);
                il++;
            }
        }
    }

    void draw() {
        int pi = 0, li = 0, ci = 0;
        for (auto x : order) {
            if (x == CIRCLE) {
                c[ci].draw();
                ci++;
            }
            if (x == LINE) {
                l[li].draw();
                li++;
            }
            if (x == POLYGON) {
                p[pi].draw();
                pi++;
            }
        }
    }
} ;

int main() {
    SetConsoleCP(1251);
    int xsize, ysize;
    string name;
    string save_name;
    string output_name;
    cout << "¬ведите ширину и высоту картинки:\n";
    cin >> xsize >> ysize;
    cout << "¬ведите название картинки вместе с типом\n(картинка должна лежать в одной папке с исполн€емым файлом): \n";
    cin >> name;
    cout << "¬ведите название файла с сохранением (с типом, но Ѕ≈« коментариев),\nесли его нет то введите NULL: \n";
    cin >> save_name;
    cout << "¬ведите название файла в который будет сохранен рисунок (с типом)\n";
    cin >> output_name;
    txCreateWindow(xsize, ysize);
    HDC image = txLoadImage(name.c_str());
    Object o;
    if (save_name != "NULL") {
        o.get_from_file(save_name);
    }
    vector<circle> contour;
    bool save = 0;
    while (!GetAsyncKeyState(VK_ESCAPE)) {
        txSetFillColor(TX_WHITE);
        txClear();
        txBitBlt(txDC(), 0, 0, 0, 0, image);
        for (auto x : contour)
            x.draw();
        if (GetAsyncKeyState('d')) {
            o.draw();
            while(GetAsyncKeyState('d'));
        }
        if (GetAsyncKeyState('s') && GetAsyncKeyState(VK_SHIFT)) {
            o.put_to_file(output_name);
            save = 1;
        }
        if (GetAsyncKeyState(VK_LBUTTON)) {
            while (GetAsyncKeyState(VK_LBUTTON)) {
                txSetFillColor(TX_WHITE);
                txClear();
                txBitBlt(txDC(), 0, 0, 0, 0, image);
                for (auto x : contour)
                    x.draw();
                txSetFillColor(RGB(255, 0, 0));
                txSetColor(TX_BLACK);
                txCircle(txMouseX(), txMouseY(), 2);
                txSleep(10);
            }
            POINT p = txMousePos();
            circle k;
            k.set(p.x, p.y, 2);
            k.set_fill_color(RGB(255, 0, 0));
            k.set_outline_color(TX_BLACK);
            contour.push_back(k);
        }

        txSleep(10);
    }
    txDeleteDC(image);
}
