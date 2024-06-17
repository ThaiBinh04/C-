#include <bits/stdc++.h>
#include <Windows.h>

using namespace std;

class Screen {
    HANDLE handle;
public:
    Screen() {
        handle = GetStdHandle(STD_OUTPUT_HANDLE); //Ham khoi tao mac dinh
    }
public:
    Screen& SetColor(int value) {
        SetConsoleTextAttribute(handle, value);
        return *this;
    }
    Screen& SetCursorPos(int x, int y) {
        COORD p = {x, y};
        SetConsoleCursorPosition(handle, p);
        return *this;
    }
    Screen& ShowCursor(bool visible = true) {
        CONSOLE_CURSOR_INFO i;
        i.dwSize = 100;
        i.bVisible = visible;
        SetConsoleCursorInfo(handle, &i);
        return *this;
    }
public:
    template<class T>
    Screen& operator<<(T*value) {
        cout << value;
        return *this;
    }
};

int n = 8;
Screen scr;

class Process {

protected:
    int value;
    virtual void Init() = 0;
    virtual void Next() = 0;
    virtual bool EOP() = 0;
    void Write() {
        scr.SetCursorPos(0,0).ShowCursor(0);
        for (int i = 0; i < n; i++) {
            scr.SetColor(value & (1 << i) ? 10:8);
            cout << (value & (1 << i) ? 'o' : ' ');
        }
        Sleep(100); system("CLS");
    }

public:
    void Play() {
        Init(), Write();
        while (!EOP()) {
            Next(); Write();
        }
    }   
};

class SingleL2R : public Process {
public:
    void Init() override {
        value = 1;
    }

    void Next() override {
        value <<= 1;
    }

    bool EOP() override {
        return value >= (1 << n);
    }
};

class SingleR2L : public Process {
public:
    void Init() override {
        value = 1 << (n - 1);
    }

    void Next() override {
        value >>= 1;
    }

    bool EOP() override {
        return value == 0;
    }
};

class TurnOnL2R : public SingleL2R {
public:
    void Next() override {
        SingleL2R::Next();
        value |= 1;
    }
};

class TurnOffR2L : public SingleR2L {
public:
    void Init() override {
        value = (1 << n) - 1;
    }
};

int main() {
    while (1) {
        SingleL2R().Play();
        SingleR2L().Play();
        TurnOnL2R().Play();
        TurnOffR2L().Play();
    }
}