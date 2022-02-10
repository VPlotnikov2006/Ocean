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

template <typename T>
struct triple {
    T first;
    T second;
    T third;
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
    Circle (double X, double Y, double R, COLORREF f, COLORREF o) : x(X), y(Y), r(R), fill_color(f), outline_color(o) {}
    Circle& operator= (const Circle& c) {
        x = c.x;
        y = c.y;
        r = c.r;
        fill_color = c.fill_color;
        outline_color = c.outline_color;
        return *this;
    }

    triple<double> Creturn() {
        triple<double> ans = {x, y, r};
        return ans;
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

    void set(double X, double Y, double R) {
        x = X;
        y = Y;
        r = R;
    }
    void set(double X, double Y, COLORREF fc, COLORREF oc) {
        x = X;
        y = Y;
        fill_color = fc;
        outline_color = oc;
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
        for (int i = 0; i < n; i++)
            a[i].set_rotation(-a[i].get_ang());
        up = 1 - up;
    }

    void inverse_y () {
        for (int i = 0; i < n; i++)
            a[i].set_rotation(txPI - a[i].get_ang());
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
        triple<double> p = c.Creturn();
        double dx = p.first - X;
        double dy = p.second - Y;
        R = sqrt(dx * dx + dy * dy);
        ang = acos(dx / R);
        if (dy < 0)
            ang = -ang;
        r = p.third;
    }
    Rotating_Circle(double X, double Y, Circle c, COLORREF fc, COLORREF oc) : x(0), y(0), ang(0), R(0), r(0), fill_color(fc), outline_color(oc) {
        x = X;
        y = Y;
        triple<double> p = c.Creturn();
        double dx = p.first - X;
        double dy = p.second - Y;
        R = sqrt(dx * dx + dy * dy);
        ang = acos(dx / R);
        if (dy < 0)
            ang = -ang;
        r = p.third;
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
        c.set(X, Y, r);
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
        up = 1 - up;
    }

    void inverse_y() {
        ang = txPI - ang;
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
        ans.x = x;
        ans.y = y;
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
            //txSleep(200);
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
    ro_t test;
    test.get_from_file("fish1.1.txt", 1);
    test.set_rotation_point_xy(300, 150);
    test.set_xy(600, 400);
    test.draw(SHOW_CENTER);
    while (!GetAsyncKeyState(VK_ESCAPE)) {
        txSetFillColor(TX_WHITE);
        txClear();
        test.rotate_by_mouse(RINVERSE_Y);
        test.draw(SHOW_CENTER);
        txSleep(10);
    }
}
