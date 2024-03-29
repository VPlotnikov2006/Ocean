//Плотников
#include <TXLib.h>
#include <vector>
#include <cmath>
#include <fstream>
#include <time.h>

using namespace std;

enum Drawing_Method {
    SHOW_CENTER,
    NOT_SHOW_CENTER
};

enum Rotation_Method {
    RINVERSE_X,
    RINVERSE_Y,
    RINVERSE_XY,
    RNOT_INVERSE
};

enum Inverse_Method {
    IINVERSE_X,
    IINVERSE_Y,
    IINVERSE_XY
};

enum Object_Type {
    CIRCLE,
    POLYGON,
    LINE
};

class Circle {
private:
    double x;
    double y;
    double r;
    COLORREF fill_color = TX_NULL;
    COLORREF outline_color = TX_NULL;
public:
    Circle () : x(0), y(0), r(0) {}

    Circle (double X, double Y, double R) : x(X), y(Y), r(R) {}

    Circle (double X, double Y, double R, COLORREF f, COLORREF o): x(X),
                                                                   y(Y),
                                                                   r(R),
                                                                   fill_color(f),
                                                                   outline_color(o)
                                                                   {}
    Circle& operator= (const Circle& c) {
        x = c.x;
        y = c.y;
        r = c.r;
        fill_color = c.fill_color;
        outline_color = c.outline_color;
        return *this;
    }

    POINT returnXY() {
        POINT ans = {LONG(x), LONG(y)};
        return ans;
    }

    double returnR() {
        return r;
    }

    void draw () {
        txSetFillColor(fill_color);
        txSetColor(outline_color);
        txCircle(x, y, r);
    }

    void set(double X, double Y) {
        x = X;
        y = Y;
    }

    void set(double R) {
        r = R;
    }

    void set(COLORREF fc, COLORREF oc) {
        fill_color = fc;
        outline_color = oc;
    }
};

class Line {
private:
    double x1;
    double y1;
    double x2;
    double y2;
    double r;
    COLORREF outline_color = TX_NULL;
public:
    Line () : x1(0), y1(0), x2(0), y2(0), r(0) {}

    Line (double X1, double Y1, double X2, double Y2, double R): x1(X1),
                                                                 y1(Y1),
                                                                 x2(X2),
                                                                 y2(Y2),
                                                                 r(R)
                                                                 {}
    Line (POINT p1, POINT p2, double R): x1(p1.x),
                                         y1(p1.y),
                                         x2(p2.x),
                                         y2(p2.y),
                                         r(R)
                                         {}
    Line (double X1, double Y1, double X2, double Y2, double R, COLORREF oc): x1(X1),
                                                                              y1(Y1),
                                                                              x2(X2),
                                                                              y2(Y2),
                                                                              r(R),
                                                                              outline_color(oc)
                                                                              {}
    Line (POINT p1, POINT p2, double R, COLORREF oc): x1(p1.x),
                                                      y1(p1.y),
                                                      x2(p2.x),
                                                      y2(p2.y),
                                                      r(R),
                                                      outline_color(oc)
                                                      {}

    Line& operator= (const Line &l) {
        x1 = l.x1;
        y1 = l.y1;
        x2 = l.x2;
        y2 = l.y2;
        outline_color = l.outline_color;
        return *this;
    }

    void draw () {
        txSetColor(outline_color, r);
        txLine(x1, y1, x2, y2);
    }

    void set (double X1, double Y1, double X2, double Y2) {
        x1 = X1;
        y1 = Y1;
        x2 = X2;
        y2 = Y2;
    }

    void set (double R) {
        r = R;
    }

    void set (COLORREF oc) {
        outline_color = oc;
    }

    POINT get1 () {
        POINT ans = {LONG(x1), LONG(y1)};
        return ans;
    }

    POINT get2 () {
        POINT ans = {LONG(x2), LONG(y2)};
        return ans;
    }
};

typedef class Rotating_point r_t;
class Rotating_point {
private:
    double x; //Координата точки вращения
    double y; //Координата точки вращения
    double r; //Длина радиус-вектора
    double ang; //Угол радиус-вектора
    COLORREF color = TX_NULL; //Цвет точки
public:
    Rotating_point(): x(0), y(0), r(0), ang(0) {}

    Rotating_point(double X, double Y, double x0, double y0, COLORREF c): x(0),
                                                                          y(0),
                                                                          r(0),
                                                                          ang(0),
                                                                          color(c) {
        double dx = x0 - X;
        double dy = y0 - Y;
        x = X;
        y = Y;
        r = sqrt(dx * dx + dy * dy);
        ang = acos (dx / r);
        if (dy < 0)
            ang = -ang;
    }

    Rotating_point(double X, double Y, POINT p, COLORREF c): x(0), y(0), r(0), ang(0), color(c)  {
        double dx = p.x - X;
        double dy = p.y - Y;
        x = X;
        y = Y;
        r = sqrt(dx * dx + dy * dy);
        ang = acos (dx / r);
        if (dy < 0)
            ang = -ang;
    }

    Rotating_point& operator= (const r_t &k) {
        x = k.x;
        y = k.y;
        r = k.r;
        ang = k.ang;
        color = k.color;
        return *this;
    }

    void set(POINT p, double X, double Y) {
        r_t cpy(X, Y, p, color);
        *this = cpy;
    }

    void Preturn(POINT &p) {
        p.x = LONG(x + r * cos(ang));
        p.y = LONG(y + r * sin(ang));
    }

    void Preturn(POINT &p, double da) {
        p.x = LONG(x + r * cos(ang + da));
        p.y = LONG(y + r * sin(ang + da));
    }

    void set_rotation_point_xy(double X, double Y) {
        POINT p;
        Preturn(p);
        r_t cpy(X, Y, p, color);
        *this = cpy;
    }

    void set_xy (double X, double Y) {
        x = X;
        y = Y;
    }

    void size (double s) {
        r /= s;
    }

    double get_ang () {
        return ang;
    }

    void set_rotation(double A) {
        ang = A;
    }

    void inverse_x () {
        ang = -ang;
    }

    void inverse_y () {
        ang = txPI - ang;
    }

    void inverse_xy () {
        inverse_x ();
        inverse_y ();
    }

    void inverse (Inverse_Method m){
        if (m == IINVERSE_X )
            inverse_x();
        if (m == IINVERSE_Y)
            inverse_y();
        if (m == IINVERSE_XY)
            inverse_xy();
    }

    void draw (Drawing_Method m=NOT_SHOW_CENTER) {
        txSetPixel(x + r * cos(ang), y + r * sin(ang), color);
        if (m == SHOW_CENTER)
            txSetPixel(x, y, RGB(255, 0, 0));
    }

    void rotate_by_mouse () {
        if (GetAsyncKeyState(VK_LBUTTON)) {
            double mx = txMouseX(), my = txMouseY();
            double dx = mx - x, dy = my - y;
            double R = sqrt(dx * dx + dy * dy);
            ang = acos (dx / R);
            if (dy < 0)
                ang = -ang;
        }
    }
};

typedef class Rotating_polygon rp_t;
class Rotating_polygon {
private:
    double x; //Координата точки вращения
    double y; //Координата точки вращения
    int n; //Кол-во точек в полигоне
    vector<r_t> a; //Массив всех точек
    COLORREF fill_color = TX_NULL; //Цвет заливки полигона
    COLORREF outline_color = TX_NULL; //Цвет границы полигона
    double a0 = 0; //Общий угол поворота
    bool right = 1; //Необходимо для разворота по оси Y при отрисовке
    bool up = 1;//Необходимо для разворота по оси Xпри отрисовке

    void rinverse_x () {
        for (int i = 0; i < n; i++)
            a[i].set_rotation(-a[i].get_ang());
        up = 1 - up;
    }

    void rinverse_y () {
        for (int i = 0; i < n; i++)
            a[i].set_rotation(-a[i].get_ang());
        right = 1 - right;
    }

public:
    Rotating_polygon(): x(0), y(0), n(0), a(0) {}

    Rotating_polygon(double X, double Y, int N, POINT *p ): x(X), y(Y), n(N), a(0) {
        a.resize(n);
        for (int i = 0; i < n; i++)
            a[i].set(p[i], x, y);
    }

    Rotating_polygon(double X, double Y, int N, vector<POINT> p): x(X), y(Y), n(N), a(0) {
        a.resize(n);
        for (int i = 0; i < n; i++)
            a[i].set(p[i], x, y);
    }

    Rotating_polygon(double X, double Y, int N, POINT *p, COLORREF fc, COLORREF oc): x(X),
                                                                                     y(Y),
                                                                                     n(N),
                                                                                     a(0),
                                                                                     fill_color(fc),
                                                                                     outline_color(oc) {
        a.resize(n);
        for (int i = 0; i < n; i++)
            a[i].set(p[i], x, y);
    }

    Rotating_polygon(double X, double Y, int N, vector<POINT> p, COLORREF fc, COLORREF oc): x(X),
                                                                                            y(Y),
                                                                                            n(N),
                                                                                            a(0),
                                                                                            fill_color(fc),
                                                                                            outline_color(oc)
                                                                                            {
        a.resize(n);
        for (int i = 0; i < n; i++)
            a[i].set(p[i], x, y);
    }

    Rotating_polygon& operator= (const rp_t &k) {
        x = k.x;
        y = k.y;
        n = k.n;
        a = k.a;
        a0 = k.a0;
        right = k.right;
        up = k.up;
        return *this;
    }

    void Vreturn(POINT *p) {
        const rp_t cpy = *this;
        for (int i = 0; i < n; i++)
            a[i].Preturn(p[i], a0);
        *this = cpy;
    }

    void set_size(int N) {
        n = N;
    }

    void set(vector<POINT> P) {
        rp_t cpy(x, y, P.size(),  P, fill_color, outline_color);
        *this = cpy;
    }

    void set_fc(COLORREF fc) {
        fill_color = fc;
    }

    void set_oc(COLORREF oc) {
        outline_color = oc;
    }

    void set_rotation_point_xy(double X, double Y) {
        POINT p[n];
        Vreturn(p);
        rp_t cpy(X, Y, n, p, fill_color, outline_color);
        *this = cpy;
    }

    void set_xy(double X, double Y) {
        x = X;
        y = Y;
        for (int i = 0; i < n; i++)
            a[i].set_xy (x, y);
    }

    void set_rotation (double A) {
        a0 = A;
    }

    void inverse_x () {
        a0 = -a0;
        for (int i = 0; i < n; i++)
            a[i].inverse_x();
        up = 1 - up;
    }

    void inverse_y () {
        a0 = -a0;
        for (int i = 0; i < n; i++)
            a[i].inverse_y();
        right = 1 - right;
    }

    void inverse_xy () {
        inverse_x();
        inverse_y();
    }

    void inverse (Inverse_Method m) {
        if (m == IINVERSE_X)
            inverse_x();
        if (m == IINVERSE_Y)
            inverse_y();
        if (m == IINVERSE_XY)
            inverse_xy();
    }

    void size(double s) {
        for (int i = 0; i < n; i++) {
            a[i].size(s);
        }
    }

    void draw (Drawing_Method m=NOT_SHOW_CENTER) {
        const rp_t cpy = *this;
        POINT p[n];
        Vreturn(p);
        txSetColor(outline_color);
        txSetFillColor(fill_color);
        txPolygon(p, n);
        if (m == SHOW_CENTER) {
            txSetColor(RGB(255, 0, 0));
            txSetFillColor(RGB(255, 0, 0));
            txCircle(x, y, 5);
        }
        *this = cpy;
    }

    void rotate_by_mouse (Rotation_Method m=RNOT_INVERSE) {
        if (GetAsyncKeyState(VK_LBUTTON)) {
            double mx = txMouseX(), my = txMouseY();
            double dx = mx - x, dy = my - y;
            double R = sqrt(dx * dx + dy * dy);
            a0 = acos (dx / R);
            if (dy < 0)
                a0 = -a0;
            if ((m == RINVERSE_X) && ((a0 < 0 && !up) || (a0 > 0 && up)))
                rinverse_x();
            if ((m == RINVERSE_Y) && ((fabs(a0) < txPI / 2 && !right) || (fabs(a0) > txPI / 2 && right)))
                rinverse_y();
            if ((m == RINVERSE_XY) && ((a0 < 0 && !up) || (a0 > 0 && up)))
                rinverse_x();
            if ((m == RINVERSE_XY) && ((fabs(a0) < txPI / 2 && !right) || (fabs(a0) > txPI / 2 && right)))
                rinverse_y();
        }
    }

    friend ifstream& operator>> (ifstream &input, rp_t &p) {
        vector<POINT> P(p.n);
        for (int i = 0; i < p.n; i++) {
            double X, Y;
            input >> X >> Y;
            P[i].x = LONG(X);
            P[i].y = LONG(Y);
        }
        int r, g, b;
        input >> r >> g >> b;
        p.set(P);
        p.set_fc(RGB(r, g, b));
        p.set_oc(RGB(0, 0, 0));
        return input;
    }
};

typedef class Rotating_Circle rc_t;
class Rotating_Circle {
private:
    double x; //Координата точки вращения
    double y; //Координата точки вращения
    double ang; //Начальный угол
    double R; //Радиус-вектор
    double r; //Радиус круга
    double a0 = 0; //Угол поворота
    COLORREF fill_color = TX_NULL;
    COLORREF outline_color = TX_NULL;
    bool up = 1;
    bool right  = 1;

    void rinverse_x() {
        ang = - ang;
        up = 1 - up;
    }

    void rinverse_y() {
        ang = - ang;
        right = 1 - right;
    }

public:
    Rotating_Circle(): x(0), y(0), ang(0), R(0), r(0) {}

    Rotating_Circle(double X, double Y, Circle c): x(0), y(0), ang(0), R(0), r(0) {
        x = X;
        y = Y;
        POINT p = c.returnXY();
        double dx = p.x - X;
        double dy = p.y - Y;
        R = sqrt(dx * dx + dy * dy);
        ang = acos(dx / R);
        if (dy < 0)
            ang = -ang;
        r = c.returnR();
    }

    Rotating_Circle(double X, double Y, Circle c, COLORREF fc, COLORREF oc): x(0),
                                                                             y(0),
                                                                             ang(0),
                                                                             R(0),
                                                                             r(0),
                                                                             fill_color(fc),
                                                                             outline_color(oc) {
        x = X;
        y = Y;
        POINT p = c.returnXY();
        double dx = p.x - X;
        double dy = p.y - Y;
        R = sqrt(dx * dx + dy * dy);
        ang = acos(dx / R);
        if (dy < 0)
            ang = -ang;
        r = c.returnR();
    }

    Rotating_Circle& operator= (const Rotating_Circle &t) {
        x = t.x;
        y = t.y;
        ang = t.ang;
        R = t.R;
        r = t.r;
        a0 = t.a0;
        fill_color = t.fill_color;
        outline_color = t.outline_color;
        up = t.up;
        right = t.right;
        return *this;
    }

    void Creturn (Circle &c) {
        double X = x + R * cos(ang + a0);
        double Y = y + R * sin(ang + a0);
        c.set(X, Y);
        c.set(r);
    }

    void set_rotation(double A) {
        a0 = A;
    }

    void set_xy (double X, double Y) {
        x = X;
        y = Y;
    }

    void set_rotation_point_xy (double X, double Y) {
        Circle c;
        Creturn(c);
        rc_t cpy(X, Y, c, fill_color, outline_color);
        *this = cpy;
    }

    void size (double s) {
        r /= s;
        R /= s;
    }

    void draw (Drawing_Method m=NOT_SHOW_CENTER) {
        Circle c(0, 0, r, fill_color, outline_color);
        Creturn(c);
        c.draw();
        if (m == SHOW_CENTER) {
            txSetColor(RGB(255, 0, 0));
            txSetFillColor(RGB(255, 0, 0));
            txCircle(x, y, 5);
        }
    }

    void inverse_x() {
        ang = - ang;
        a0 = -a0;
        up = 1 - up;
    }

    void inverse_y() {
        ang = txPI - ang;
        a0 = -a0;
        right = 1 - right;
    }

    void inverse_xy() {
        inverse_x();
        inverse_y();
    }

    void set_r (double K) {
        R = K;
    }

    void set (rc_t cpy) {
        *this = cpy;
    }

    void set_fc (COLORREF fc) {
        fill_color = fc;
    }

    void set_oc (COLORREF oc) {
        outline_color = oc;
    }

    void inverse(Inverse_Method i) {
        if (i == IINVERSE_X)
            inverse_x();
        if (i == IINVERSE_Y)
            inverse_y();
        if (i == IINVERSE_XY)
            inverse_xy();
    }

    friend ifstream& operator>> (ifstream &input, rc_t &c) {
        double X, Y, r;
        int red, green, blue;
        input >> X >> Y >> r;
        input >> red >> green >> blue;
        rc_t cpy(0,0, {X, Y, r}, RGB(red, green, blue), RGB(0, 0, 0));
        c.set(cpy);
        return input;
    }

    void rotate_by_mouse (Rotation_Method m = RNOT_INVERSE) {
        if (GetAsyncKeyState(VK_LBUTTON)) {
            double mx = txMouseX();
            double my = txMouseY();
            double dx = mx - x;
            double dy = my - y;
            double dr = sqrt(dx * dx + dy * dy);
            double newang = acos (dx / dr);
            if (dy < 0)
                newang = -newang;
            set_rotation(newang);
            if ((m == RINVERSE_X) && ((a0 < 0 && !up) || (a0 > 0 && up)))
                rinverse_x();
            if ((m == RINVERSE_Y) && ((fabs(a0) < txPI / 2 && !right) || (fabs(a0) > txPI / 2 && right)))
                rinverse_y();
            if ((m == RINVERSE_XY) && ((a0 < 0 && !up) || (a0 > 0 && up)))
                rinverse_x();
            if ((m == RINVERSE_XY) && ((fabs(a0) < txPI / 2 && !right) || (fabs(a0) > txPI / 2 && right)))
                rinverse_y();
        }
    }
};

typedef class Rotating_Line rl_t;
class Rotating_Line {
private:
    double x; //Координата точки вращения
    double y;//Координата точки вращения
    double r1; //Радиус вектор первой точки
    double r2; //Радиус вектор второй точки
    double a1; //Угол поворота первой точки
    double a2; //Угол поворота второй точки
    double a0 = 0; //Общий угол поворота
    double r; //Толщина линии
    COLORREF outline_color = TX_NULL;
    bool up = 1;
    bool right = 1;

    void rinverse_y () {
        a1 = -a1;
        a2 = -a2;
        right = 1 - right;
    }

    void rinverse_x () {
        a1 = -a1;
        a2 = -a2;
        up = 1 - up;
    }

public:
    Rotating_Line (): x(0), y(0), r1(0), r2(0), a1(0), a2(0), r(0) {}

    Rotating_Line (double X, double Y, POINT p1, POINT p2, double R): x(X),
                                                                      y(Y),
                                                                      r1(0),
                                                                      r2(0),
                                                                      a1(0),
                                                                      a2(0),
                                                                      r(R) {
        double dx1 = p1.x - X;
        double dy1 = p1.y - Y;
        double dx2 = p2.x - X;
        double dy2 = p2.y - Y;
        r1 = sqrt(dx1 * dx1 + dy1 * dy1);
        r2 = sqrt(dx2 * dx2 + dy2 * dy2);
        a1 = acos(dx1 / r1);
        a2 = acos(dx2 / r2);
        if (dy1 < 0)
            a1 = -a1;
        if (dy2 < 0)
            a2 = -a2;
    }

    Rotating_Line (double X, double Y, double X1, double Y1, double X2, double Y2, double R): x(X),
                                                                                              y(Y),
                                                                                              r1(0),
                                                                                              r2(0),
                                                                                              a1(0),
                                                                                              a2(0),
                                                                                              r(R) {
        double dx1 = X1 - X;
        double dy1 = Y1 - Y;
        double dx2 = X2 - X;
        double dy2 = Y2 - Y;
        r1 = sqrt(dx1 * dx1 + dy1 * dy1);
        r2 = sqrt(dx2 * dx2 + dy2 * dy2);
        a1 = acos(dx1 / r1);
        a2 = acos(dx2 / r2);
        if (dy1 < 0)
            a1 = -a1;
        if (dy2 < 0)
            a2 = -a2;
    }

    Rotating_Line (double X, double Y, POINT p1, POINT p2, double R, COLORREF oc): x(X),
                                                                                   y(Y),
                                                                                   r1(0),
                                                                                   r2(0),
                                                                                   a1(0),
                                                                                   a2(0),
                                                                                   r(R),
                                                                                   outline_color(oc) {
        double dx1 = p1.x - X;
        double dy1 = p1.y - Y;
        double dx2 = p2.x - X;
        double dy2 = p2.y - Y;
        r1 = sqrt(dx1 * dx1 + dy1 * dy1);
        r2 = sqrt(dx2 * dx2 + dy2 * dy2);
        a1 = acos(dx1 / r1);
        a2 = acos(dx2 / r2);
        if (dy1 < 0)
            a1 = -a1;
        if (dy2 < 0)
            a2 = -a2;
    }

    Rotating_Line (double X, double Y, double X1, double Y1, double X2, double Y2, double R, COLORREF oc):
                   x(X),
                   y(Y),
                   r1(0),
                   r2(0),
                   a1(0),
                   a2(0),
                   r(R),
                   outline_color(oc) {
        double dx1 = X1 - X;
        double dy1 = Y1 - Y;
        double dx2 = X2 - X;
        double dy2 = Y2 - Y;
        r1 = sqrt(dx1 * dx1 + dy1 * dy1);
        r2 = sqrt(dx2 * dx2 + dy2 * dy2);
        a1 = acos(dx1 / r1);
        a2 = acos(dx2 / r2);
        if (dy1 < 0)
            a1 = -a1;
        if (dy2 < 0)
            a2 = -a2;
    }

    Rotating_Line& operator= (const Rotating_Line &l) {
        x = l.x;
        y = l.y;
        r1 = l.r1;
        r2 = l.r2;
        a1 = l.a1;
        a2 = l.a2;
        a0 = l.a0;
        r = l.r;
        outline_color = l.outline_color;
        up = l.up;
        right = l.right;
        return *this;
    }

    void Lreturn(Line& l) {
        double X1 = x + r1 * cos(a1 + a0);
        double Y1 = y + r1 * sin(a1 + a0);
        double X2 = x + r2 * cos(a2 + a0);
        double Y2 = y + r2 * sin(a2 + a0);
        l.set(X1, Y1, X2, Y2);
        l.set(r);
        l.set( outline_color);
    }

    void set_rotation(double A) {
        a0 = A;
    }

    void size (double s) {
        r1 /= s;
        r2 /= s;
        r /= s;
    }

    void set_xy (double X, double Y) {
        x = X;
        y = Y;
    }

    void set_rotation_point_xy(double X, double Y) {
        Line l;
        Lreturn(l);
        rl_t cpy(X, Y, l.get1(), l.get2(), r, outline_color);
        *this = cpy;
    }

    void draw(Drawing_Method m=NOT_SHOW_CENTER) {
        Line l;
        Lreturn(l);
        l.draw();
        if (m == SHOW_CENTER) {
            txSetColor(RGB(255, 0, 0));
            txSetFillColor(RGB(255, 0, 0));
            txCircle(x, y, 5);
        }
    }

    void inverse_x() {
        a0 = -a0;
        a1 = -a1;
        a2 = -a2;
        up = 1 - up;
    }

    void inverse_y () {
        a0 = -a0;
        a1 = txPI - a1;
        a2 = txPI - a2;
        right = 1 - right;
    }

    void inverse_xy (){
        inverse_x();
        inverse_y();
    }

    void inverse(Inverse_Method i) {
        if (i == IINVERSE_X)
            inverse_x();
        if (i == IINVERSE_Y)
            inverse_y();
        if (i == IINVERSE_XY)
            inverse_xy();
    }

    void set(rl_t line) {
        *this = line;
    }

    friend ifstream& operator>> (ifstream &input, rl_t &c) {
        double X1, Y1, X2, Y2, r;
        input >> X1 >> Y1;
        input >> X2 >> Y2;
        input >> r;
        int red, green, blue;
        input >> red >> green >> blue;
        rl_t cpy(0,0, X1, Y1, X2, Y2, r, RGB(red, green, blue));
        c.set(cpy);
        return input;
    }

    void rotate_by_mouse (Rotation_Method m=RNOT_INVERSE) {
        if (GetAsyncKeyState(VK_LBUTTON)) {
            double mx = txMouseX();
            double my = txMouseY();
            double dx = mx - x;
            double dy = my - y;
            double dr = sqrt(dx * dx + dy * dy);
            double newang = acos (dx / dr);
            if (dy < 0)
                newang = -newang;
            set_rotation(newang);
            if ((m == RINVERSE_X) && ((a0 < 0 && !up) || (a0 > 0 && up)))
                rinverse_x();
            if ((m == RINVERSE_Y) && ((fabs(a0) < txPI / 2 && !right) || (fabs(a0) > txPI / 2 && right)))
                rinverse_y();
            if ((m == RINVERSE_XY) && ((a0 < 0 && !up) || (a0 > 0 && up)))
                rinverse_x();
            if ((m == RINVERSE_XY) && ((fabs(a0) < txPI / 2 && !right) || (fabs(a0) > txPI / 2 && right)))
                rinverse_y();
        }
    }
};

typedef class Rotating_object ro_t;
class Rotating_object {
private:
    double x; //Координата точки вращения
    double y; //Координата точки вращения
    int cn; //Кол-во кругов
    int pn; //Кол-во полигонов
    int ln; //Кол-во линий
    vector<Object_Type> order; //Порядок рисования фигур
    vector<rc_t> circle; //Круги
    vector<rp_t> polygon; //Полигоны
    vector<rl_t> line; //Линии
public:
    Rotating_object () : x(0), y(0), cn(0), pn(0), ln(0), order(0), circle(0), polygon(0), line(0) {}

    void push_back(rc_t c) {
        order.push_back(CIRCLE);
        cn++;
        circle.push_back(c);
    }

    void push_back(rp_t p) {
        order.push_back(POLYGON);
        pn++;
        polygon.push_back(p);
    }

    void push_back(rl_t l) {
        order.push_back(LINE);
        ln++;
        line.push_back(l);
    }

    POINT returnXY() {
        POINT ans;
        ans.x = LONG(x);
        ans.y = LONG(y);
        return ans;
    }

    void size(double s) {
        for (auto k : line)
            k.size(s);
        for (auto k : circle)
            k.size(s);
        for (auto k : polygon)
            k.size(s);
    }

    void get_from_file(const char* name, double s=1, double X=0, double Y=0) {
        ifstream input;
        input.open(name);
        int n;
        while(input >> n) {
            if (n == 1){
                rc_t c;
                input >> c;
                c.size(s);
                push_back(c);
            }
            else if (n == 2){
                rl_t l;
                input >> l;
                l.size(s);
                push_back(l);
            }
            else {
                rp_t p;
                p.set_size(n);
                input >> p;
                p.size(s);
                push_back(p);
            }
        }
        x = 0;
        y = 0;
        set_rotation_point_xy(X / s, Y / s);
        input.close();
    }

    void set_xy(double X, double Y) {
        x = X;
        y = Y;
        for (int i = 0; i < cn; i++)
            circle[i].set_xy(X, Y);
        for (int i = 0; i < pn; i++)
            polygon[i].set_xy(X, Y);
        for (int i = 0; i < ln; i++)
            line[i].set_xy(X, Y);
    }

    void set_rotation(double A) {
        for (int i = 0; i < cn; i++)
            circle[i].set_rotation(A);
        for (int i = 0; i < pn; i++)
            polygon[i].set_rotation(A);
        for (int i = 0; i < ln; i++)
            line[i].set_rotation(A);
    }

    void inverse(Inverse_Method m) {
        for (int i = 0; i < cn; i++)
            circle[i].inverse(m);
        for (int i = 0; i < pn; i++)
            polygon[i].inverse(m);
        for (int i = 0; i < ln; i++)
            line[i].inverse(m);
    }

    void set_rotation_point_xy(double X, double Y) {
        x = X;
        y = Y;
        for (int i = 0; i < cn; i++)
            circle[i].set_rotation_point_xy(X, Y);
        for (int i = 0; i < pn; i++)
            polygon[i].set_rotation_point_xy(X, Y);
        for (int i = 0; i < ln; i++)
            line[i].set_rotation_point_xy(X, Y);
    }

    void draw(Drawing_Method m=NOT_SHOW_CENTER) {
        int ic = 0, ip = 0, il = 0;
        for (int i = 0; i < cn + pn + ln; i++) {
            if (order[i] == CIRCLE) {
                circle[ic].draw(m);
                ic++;
            }
            if (order[i] == POLYGON) {
                polygon[ip].draw(m);
                ip++;
            }
            if (order[i] == LINE) {
                line[il].draw(m);
                il++;
            }
        }
    }

    void rotate_by_mouse(Rotation_Method m=RNOT_INVERSE) {
        for (int i = 0; i < cn; i++)
            circle[i].rotate_by_mouse(m);
        for (int i = 0; i < pn; i++)
            polygon[i].rotate_by_mouse(m);
        for (int i = 0; i < ln; i++)
            line[i].rotate_by_mouse(m);
    }
};


