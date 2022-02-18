//Плотников
#include <TXLib.h>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;

enum Drawing_Method {SHOW_CENTER, NOT_SHOW_CENTER};
enum Rotation_Method {RINVERSE_X, RINVERSE_Y, RINVERSE_XY, RNOT_INVERSE};
enum Inverse_Method {IINVERSE_X, IINVERSE_Y, IINVERSE_XY};
enum Object_Type {CIRCLE, POLYGON};

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
    Circle (double X, double Y, double R, COLORREF f, COLORREF o) : x(X), y(Y), r(R), fill_color(f), outline_color(o) {}
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
    Line (double X1, double Y1, double X2, double Y2, double R) : x1(X1), y1(Y1), x2(X2), y2(Y2), r(R) {}
    Line (POINT p1, POINT p2, double R) : x1(p1.x), y1(p1.y), x2(p2.x), y2(p2.y), r(R) {}
    Line (double X1, double Y1, double X2, double Y2, double R, COLORREF oc) : x1(X1), y1(Y1), x2(X2), y2(Y2), r(R), outline_color(oc) {}
    Line (POINT p1, POINT p2, double R, COLORREF oc) : x1(p1.x), y1(p1.y), x2(p2.x), y2(p2.y), r(R), outline_color(oc) {}

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
    double x;
    double y;
    double r;
    double ang;
    COLORREF color = TX_NULL;
public:
    Rotating_point() : x(0), y(0), r(0), ang(0) {}
    Rotating_point(double X, double Y, double x0, double y0, COLORREF c) : x(0), y(0), r(0), ang(0), color(c) {
        double dx = x0 - X;
        double dy = y0 - Y;
        x = X;
        y = Y;
        r = sqrt(dx * dx + dy * dy);
        ang = acos (dx / r);
        if (dy < 0)
            ang = -ang;
    }
    Rotating_point(double X, double Y, POINT p, COLORREF c) : x(0), y(0), r(0), ang(0), color(c)  {
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

    void draw (Drawing_Method m = NOT_SHOW_CENTER) {
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
    double x;
    double y;
    int n;
    vector<r_t> a;
    COLORREF fill_color = TX_NULL;
    COLORREF outline_color = TX_NULL;
    double a0 = 0;
    bool right = 1;
    bool up = 1;

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
    Rotating_polygon() : x(0), y(0), n(0), a(0) {}
    Rotating_polygon(double X, double Y, int N, POINT *p ) : x(X), y(Y), n(N), a(0) {
        a.resize(n);
        for (int i = 0; i < n; i++)
            a[i].set(p[i], x, y);
    }
    Rotating_polygon(double X, double Y, int N, vector<POINT> p) : x(X), y(Y), n(N), a(0) {
        a.resize(n);
        for (int i = 0; i < n; i++)
            a[i].set(p[i], x, y);
    }
    Rotating_polygon(double X, double Y, int N, POINT *p, COLORREF fc, COLORREF oc) : x(X), y(Y), n(N), a(0), fill_color(fc), outline_color(oc) {
        a.resize(n);
        for (int i = 0; i < n; i++)
            a[i].set(p[i], x, y);
    }
    Rotating_polygon(double X, double Y, int N, vector<POINT> p, COLORREF fc, COLORREF oc) : x(X), y(Y), n(N), a(0), fill_color(fc), outline_color(oc) {
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

    void draw (Drawing_Method m = NOT_SHOW_CENTER) {
        //bool r = right, u = up;
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
        //right = r;
        //up = u;
        *this = cpy;
    }

    void rotate_by_mouse (Rotation_Method m = RNOT_INVERSE) {
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
};

typedef class Rotating_Circle rc_t;
class Rotating_Circle {
private:
    double x;
    double y;
    double ang;
    double R;
    double r;
    double a0 = 0;
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
    Rotating_Circle() : x(0), y(0), ang(0), R(0), r(0) {}
    Rotating_Circle(double X, double Y, Circle c) : x(0), y(0), ang(0), R(0), r(0) {
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
    Rotating_Circle(double X, double Y, Circle c, COLORREF fc, COLORREF oc) : x(0), y(0), ang(0), R(0), r(0), fill_color(fc), outline_color(oc) {
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
        rc_t cpy(X, Y, c);
        *this = cpy;
    }

    void draw (Drawing_Method m = NOT_SHOW_CENTER) {
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

    void inverse(Inverse_Method i) {
        if (i == IINVERSE_X)
            inverse_x();
        if (i == IINVERSE_Y)
            inverse_y();
        if (i == IINVERSE_XY)
            inverse_xy();
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
    double x;
    double y;
    double r1;
    double r2;
    double a1;
    double a2;
    double a0 = 0;
    double r;
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
    Rotating_Line () : x(0), y(0), r1(0), r2(0), a1(0), a2(0), r(0) {}
    Rotating_Line (double X, double Y, POINT p1, POINT p2, double R) : x(X), y(Y), r1(0), r2(0), a1(0), a2(0), r(R) {
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
    Rotating_Line (double X, double Y, double X1, double Y1, double X2, double Y2, double R) : x(X), y(Y), r1(0), r2(0), a1(0), a2(0), r(R) {
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
    Rotating_Line (double X, double Y, POINT p1, POINT p2, double R, COLORREF oc) : x(X), y(Y), r1(0), r2(0), a1(0), a2(0), r(R), outline_color(oc) {
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
    Rotating_Line (double X, double Y, double X1, double Y1, double X2, double Y2, double R, COLORREF oc) : x(X), y(Y), r1(0), r2(0), a1(0), a2(0), r(R), outline_color(oc) {
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

    void draw(Drawing_Method m = NOT_SHOW_CENTER) {
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

typedef class Rotating_object ro_t;
class Rotating_object {
private:
    double x;
    double y;
    double cn;
    double pn;
    vector<Object_Type> order;
    vector<rc_t> circle;
    vector<rp_t> polygon;
public:
    Rotating_object () : x(0), y(0), cn(0), pn(0), order(0), circle(0), polygon(0) {}

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

    POINT returnXY() {
        POINT ans;
        ans.x = LONG(x);
        ans.y = LONG(y);
        return ans;
    }

    void get_from_file(char* name, double s) {
        ifstream input;
        input.open(name);
        int n;
        while(input >> n) {
            if (n == -1){
                double X, Y, r;
                int red, green, blue;

                input >> X >> Y >> r;
                input >> red >> green >> blue;
                X *= s;
                Y *= s;
                r *= s;
                rc_t c (0, 0, {X, Y, r}, RGB(red, green, blue), RGB(red, green, blue));
                push_back(c);

            }
            else {
                vector<POINT> p(n);
                for (int i = 0; i < n; i++) {
                     double X, Y;
                     input >> X >> Y;
                     X *= s;
                     Y *= s;
                     p[i].x = LONG(X);
                     p[i].y = LONG(Y);
                }
                int r, g, b;
                input >> r >> g >> b;
                rp_t c(0, 0, n, p, RGB(r, g, b), RGB(r, g, b));
                push_back(c);
            }
        }
        x = 0;
        y = 0;
        input.close();
    }

    void set_xy(double X, double Y) {
        x = X;
        y = Y;
        for (int i = 0; i < cn; i++)
            circle[i].set_xy(X, Y);
        for (int i = 0; i < pn; i++)
            polygon[i].set_xy(X, Y);
    }

    void set_rotation(double A) {
        for (int i = 0; i < cn; i++)
            circle[i].set_rotation(A);
        for (int i = 0; i < pn; i++)
            polygon[i].set_rotation(A);
    }

    void inverse(Inverse_Method m) {
        for (int i = 0; i < cn; i++)
            circle[i].inverse(m);
        for (int i = 0; i < pn; i++)
            polygon[i].inverse(m);
    }

    void set_rotation_point_xy(double X, double Y) {
        x = X;
        y = Y;
        for (int i = 0; i < cn; i++)
            circle[i].set_rotation_point_xy(X, Y);
        for (int i = 0; i < pn; i++)
            polygon[i].set_rotation_point_xy(X, Y);
    }

    void draw(Drawing_Method m = NOT_SHOW_CENTER) {
        int ic = 0, ip = 0;
        for (int i = 0; i < cn + pn; i++) {
            if (order[i] == CIRCLE) {
                circle[ic].draw(m);
                ic++;
            }
            if (order[i] == POLYGON) {
                polygon[ip].draw(m);
                ip++;
            }
        }
    }

    void rotate_by_mouse(Rotation_Method m = RNOT_INVERSE) {
        for (int i = 0; i < cn; i++)
            circle[i].rotate_by_mouse(m);
        for (int i = 0; i < pn; i++)
            polygon[i].rotate_by_mouse(m);
    }
};


int main() {
    txCreateWindow(1200, 800);
    txClear();
    rl_t test(0,0,50,50,100,100, 5,RGB(0,255,0));
    test.set_xy(600,400);
    for (double ang = 0; ang < 2 * txPI && !GetAsyncKeyState(VK_ESCAPE); ang += 0.01) {
        txSetFillColor(TX_WHITE);
        txClear();
        test.draw(SHOW_CENTER);
        test.inverse_y();
        test.draw(SHOW_CENTER);
        test.inverse_y();
        test.set_rotation(ang);
        txSleep(10);
    }
}
