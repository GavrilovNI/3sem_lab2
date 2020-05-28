#pragma once
#include<windows.h>
#include<iostream>

enum class Colors
{
    COLOR_BLACK = 0,
    COLOR_BLUE = 1,
    COLOR_GREEN = 2,
    COLOR_CYAN = 3,
    COLOR_RED = 4,
    COLOR_MAGENTA = 5,
    COLOR_BROWN = 6,
    COLOR_LIGHTGRAY = 7,
    COLOR_DARKGRAY = 8,
    COLOR_LIGHTBLUE = 9,
    COLOR_LIGHTGREEN = 10,
    COLOR_LIGHTRED = 12,
    COLOR_LIGHTMAGENTA = 13,
    COLOR_YELLOW = 14,
    COLOR_WHITE = 15
};

struct Vector2i
{
    int x, y;
    Vector2i(int x = 0, int y = 0)
    {
        this->x = x;
        this->y = y;
    }
    Vector2i& operator+(const Vector2i& other)
    {
        Vector2i res(x + other.x, y + other.y);
        return res;
    }
};

//класс консоли
class TConsole
{
private:
    HANDLE InputHandle;                         //дескриптор буфера ввода
    HANDLE OutputHandle;                        //дескриптор буфера вывода
    CONSOLE_SCREEN_BUFFER_INFO ScreenBufInfo;   //информация об экранном буфере консоли

    void SetAtribute(WORD atribute, bool value)
    {
        GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
        WORD TextAttr = ScreenBufInfo.wAttributes;
        if (value)
        {
            TextAttr = TextAttr | atribute;
        }
        else
        {
            TextAttr = TextAttr & ~atribute;
        }


        SetConsoleTextAttribute(OutputHandle, TextAttr);
    }
    bool CheckAtribute(WORD atribute)
    {
        GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
        WORD TextAttr = ScreenBufInfo.wAttributes;
        return (TextAttr & atribute);
    }
public:
    std::ostream& Out;              //поток вывода
    std::istream& In;               //поток ввода
    //конструктор
    TConsole() :In(std::cin), Out(std::cout) 
    {
        InputHandle = GetStdHandle(STD_INPUT_HANDLE);
        OutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
    }
    //функция удаления символов от курсора до конца строки
    void ClrEol()
    {
        GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
        DWORD NOAW;
        //LPDWORD NOAW;
        //ZeroMemory(&NOAW, sizeof(NOAW));
        DWORD Count, Size;
        Size = ScreenBufInfo.srWindow.Right - ScreenBufInfo.dwCursorPosition.X;
        FillConsoleOutputAttribute(OutputHandle, ScreenBufInfo.wAttributes, Size, ScreenBufInfo.dwCursorPosition, &Count);
        FillConsoleOutputCharacter(OutputHandle, ' ',
            ScreenBufInfo.dwSize.X - ScreenBufInfo.dwCursorPosition.X,
            ScreenBufInfo.dwCursorPosition, &NOAW);
            //ScreenBufInfo.dwCursorPosition, NOAW);
    }
    //функция очистки экрана
    void ClrScr()
    {
        DWORD NOAW;
        ZeroMemory(&NOAW, sizeof(NOAW));
        ScreenBufInfo.dwCursorPosition.X = 0;
        ScreenBufInfo.dwCursorPosition.Y = 0;
        FillConsoleOutputCharacter(OutputHandle, ' ', ScreenBufInfo.dwSize.X * ScreenBufInfo.dwSize.Y,
            ScreenBufInfo.dwCursorPosition, &NOAW);
        GotoXY(0, 0);
    }

    void ClrLine()
    {
        GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
        DWORD NOAW;
        ZeroMemory(&NOAW, sizeof(NOAW));
        ScreenBufInfo.dwCursorPosition.X = 0;
        FillConsoleOutputCharacter(OutputHandle, ' ', ScreenBufInfo.dwSize.X,
            ScreenBufInfo.dwCursorPosition, &NOAW);
        GotoXY(0, 0);
    }
    //функция задержки выполнения
    void Delay(WORD MS)
    {
        SleepEx(MS, FALSE);
    }
    //функция удаления строки
    void DelLine()
    {
        GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
        CHAR_INFO CI;
        ScreenBufInfo.srWindow.Top = WhereY();
        CI.Char.UnicodeChar = ' ';
        CI.Attributes = ScreenBufInfo.wAttributes;
        COORD Coord;
        Coord.X = ScreenBufInfo.srWindow.Left;
        Coord.Y = ScreenBufInfo.srWindow.Top - 1;
        DWORD Size, Count;
        Size = ScreenBufInfo.srWindow.Right - ScreenBufInfo.srWindow.Left + 1;
        ScrollConsoleScreenBuffer(OutputHandle, &ScreenBufInfo.srWindow, 0, Coord, &CI);
        FillConsoleOutputAttribute(OutputHandle, ScreenBufInfo.wAttributes, Size, Coord, &Count);
    }
    //функция перемещения курсора
    void GotoXY(Vector2i pos)
    {
        GotoXY(pos.x, pos.y);
    }
    void GotoXY(int X, int Y)
    {
        ScreenBufInfo.dwCursorPosition.X = X;
        ScreenBufInfo.dwCursorPosition.Y = Y;
        if (!SetConsoleCursorPosition(OutputHandle, ScreenBufInfo.dwCursorPosition))
        {
            GotoXY(0, 0);
            DelLine();
        }
    }
    //функция добавления строки
    void InsLine()
    {
        GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
        SMALL_RECT ScreenRect = ScreenBufInfo.srWindow;
        ScreenRect.Top = ScreenBufInfo.dwCursorPosition.Y - 1 + ScreenBufInfo.srWindow.Top;
        ScreenRect.Bottom = ScreenBufInfo.srWindow.Bottom - 1;
        CHAR_INFO CI;
        CI.Char.UnicodeChar = ' ';
        CI.Attributes = ScreenBufInfo.wAttributes;
        COORD Coord;
        Coord.X = ScreenRect.Left;
        Coord.Y = ScreenRect.Top + 1;
        DWORD dwSize = ScreenRect.Right - ScreenRect.Left + 1;
        ScrollConsoleScreenBuffer(OutputHandle, &ScreenRect, 0, Coord, &CI);
        Coord.Y--;
        DWORD dwCount;
        FillConsoleOutputAttribute(OutputHandle, ScreenBufInfo.wAttributes, dwSize, Coord, &dwCount);
    }
    //функция проверки нажатия клавиши
    bool KeyPressed()
    {
        bool flag = false;
        DWORD NumberOfEvents;
        GetNumberOfConsoleInputEvents(InputHandle, &NumberOfEvents);
        if (NumberOfEvents > 0)
        {
            INPUT_RECORD IR;
            DWORD NumRead;
            if (PeekConsoleInput(InputHandle, &IR, 1, &NumRead))
            {
                if ((IR.EventType == KEY_EVENT) && (IR.Event.KeyEvent.bKeyDown)) flag = true;
                else ReadConsoleInput(InputHandle, &IR, 1, &NumRead);
            }
        }
        return flag;
    }
    //функция чтения символа с клавиатуры
    WCHAR ReadKey()
    {
        DWORD NumRead;
        INPUT_RECORD IR;
        do
        {
            do
            {
            } while (!KeyPressed());
            ReadConsoleInput(InputHandle, &IR, 1, &NumRead);
        } while (IR.Event.KeyEvent.uChar.UnicodeChar == 0);
        return IR.Event.KeyEvent.uChar.UnicodeChar;
    }
    //функция задания цвета фона
    void SetBackgroundColor(Colors Color)
    {
        GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
        WORD TextAttr = ScreenBufInfo.wAttributes;
        TextAttr = ((int)Color << 4) | (TextAttr & ~0xF0);
        SetConsoleTextAttribute(OutputHandle, TextAttr);
    }
    //функция задания цвета текста
    void SetTextColor(Colors Color)
    {
        GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
        WORD TextAttr = ScreenBufInfo.wAttributes;
        TextAttr = ((int)Color & 0x0F) | (TextAttr & ~0x0F);
        SetConsoleTextAttribute(OutputHandle, TextAttr);
    }

    Colors GetTextColor()
    {
        GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
        WORD TextAttr = ScreenBufInfo.wAttributes;

        return (Colors)(TextAttr & 0x0F);
    }

    Colors GetBackgroundColor()
    {
        GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
        WORD TextAttr = ScreenBufInfo.wAttributes;

        return (Colors)((TextAttr >> 4) & 0x0F);
    }

    void SetUnderscore(bool value)
    {
        SetAtribute(COMMON_LVB_UNDERSCORE, value);
    }
    bool IsUnderscore()
    {
        return CheckAtribute(COMMON_LVB_UNDERSCORE);
    }

    void SetLVertical(bool value)
    {
        SetAtribute(COMMON_LVB_GRID_LVERTICAL, value);
    }
    bool IsLVertical()
    {
        return CheckAtribute(COMMON_LVB_GRID_LVERTICAL);
    }
    void SetRVertical(bool value)
    {
        SetAtribute(COMMON_LVB_GRID_RVERTICAL, value);
    }
    bool IsRVertical()
    {
        return CheckAtribute(COMMON_LVB_GRID_RVERTICAL);
    }
    void SetHorisontal(bool value)
    {
        SetAtribute(COMMON_LVB_GRID_HORIZONTAL, value);
    }
    bool IsHorisontal()
    {
        return CheckAtribute(COMMON_LVB_GRID_HORIZONTAL);
    }
    Vector2i Where()
    {
        GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
        return Vector2i(ScreenBufInfo.dwCursorPosition.X, ScreenBufInfo.dwCursorPosition.Y);
    }
    //функция получения текущей координаты X курсора
    int WhereX()
    {
        GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
        return ScreenBufInfo.dwCursorPosition.X;
    }
    //функция получения текущей координаты Y курсора
    int WhereY()
    {
        GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
        return ScreenBufInfo.dwCursorPosition.Y;
    }
    //функция задания размеров окна
    void Window(BYTE X, BYTE Y)
    {
        SMALL_RECT ConsoleRect;
        ConsoleRect.Top = 0;
        ConsoleRect.Left = 0;
        ConsoleRect.Right = X - 1;
        ConsoleRect.Bottom = Y - 1;
        COORD BufSize;
        BufSize.X = X;
        BufSize.Y = Y;
        SetConsoleScreenBufferSize(OutputHandle, BufSize);
        SetConsoleWindowInfo(OutputHandle, TRUE, &ConsoleRect);
        GotoXY(0, 0);
    }
    Vector2i GetWindowSize()
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;

        GetConsoleScreenBufferInfo(OutputHandle, &csbi);
        int x, y;
        x = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

        return Vector2i(x, y);
    }
};
