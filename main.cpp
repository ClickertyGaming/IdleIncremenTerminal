#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cctype>
using namespace std;

typedef bool (WINAPI *ReadConsoleInputEx)(HANDLE, PINPUT_RECORD, DWORD, LPDWORD, USHORT);

#ifndef CONSOLE_READ_NOREMOVE
#define CONSOLE_READ_NOREMOVE   0x0001
#endif

#ifndef CONSOLE_READ_NOWAIT
#define CONSOLE_READ_NOWAIT     0x0002
#endif

string buffer = "";
string bigEmpty = "";
string menuBuffer[10] = {"\n" + bigEmpty, "\n" + bigEmpty, "\n" + bigEmpty, "\n" + bigEmpty, "\n" + bigEmpty, "\n" + bigEmpty, "\n" + bigEmpty, "\n" + bigEmpty, "\n" + bigEmpty, "\n" + bigEmpty};
HANDLE inputHandle;
HANDLE outputHandle;
DWORD modeFlagsOld;

enum MenuState {
    MENU_MAIN,
    MENU_SETTINGS,
    MENU_CREDITS,
    MENU_HELP,
    MENU_SWITCH
};
enum MenuState menu = MENU_MAIN;
enum MenuState menuPrev = menu;

VOID ErrorExit(LPCSTR);
VOID KeyEventProc(KEY_EVENT_RECORD);
VOID MouseEventProc(MOUSE_EVENT_RECORD);
VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD);
void UpdateScreen(HANDLE, int);
void UpdateNewsTicker();
void ParseInput(string);
void UpdateMenuLine(string, int);
void UpdateMenuReset();
void SetMenu(MenuState);
int clampi(int, int, int);

string inputChars = "";
int temp = 0;

int screenWidth = 50;

bool newsTickerRunning = false; // Dictates if the news ticker is even showing anything
int newsTickerTime = 0; // Time between messages / scrolling speed
string newsTickerMsg = ""; // The message pulled from the array
int newsTick = 0; // Letter index in newsTickerMsg
int newsSpeed = 1; // Scroll speed of newsTickerMsg
string newsTickerMsgArr[3] = {"this is a test message", "oidfjsoojdifoisdjfiosjodf", "no way this actually works dude"};

int main(VOID) {
    srand(time(0));
    newsTickerTime = (rand() % 60) + 20;

    system("title Idle IncremenTerminal");
    system("cls");

    for (int i = 0; i < screenWidth; i++) {
        bigEmpty += " ";
    }

    DWORD numOfCharactersRead, modeFlags, i;
    LPDWORD numOfCharactersUnread;
    INPUT_RECORD inputRecordsInBuffer[128], keyInputRecordsInBuffer[128];

    ReadConsoleInputEx ReadConsoleInputExA = (ReadConsoleInputEx)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "ReadConsoleInputExA");
    ReadConsoleInputEx ReadConsoleInputExW = (ReadConsoleInputEx)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "ReadConsoleInputExW");

    inputHandle = GetStdHandle(STD_INPUT_HANDLE);
    if (inputHandle == INVALID_HANDLE_VALUE)
        ErrorExit("GetStdHandle");

    outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (outputHandle == INVALID_HANDLE_VALUE)
        ErrorExit("GetStdHandle");
    
    if (!GetConsoleMode(inputHandle, &modeFlagsOld))
        ErrorExit("GetConsoleMode");
    
    modeFlags = (ENABLE_WINDOW_INPUT | ENABLE_EXTENDED_FLAGS) & ~ENABLE_QUICK_EDIT_MODE & ~ENABLE_ECHO_INPUT;
    if (!SetConsoleMode(inputHandle, modeFlags))
        ErrorExit("SetConsoleMode");

    while (true)
    {
        bigEmpty = "";
        for (int i = 0; i < screenWidth; i++) {
            bigEmpty += " ";
        }

        buffer = "Idle IncremenTerminal v0.00a\nInput \"menu help\" for all possible commands!";

        buffer += "\n";
        for (int i = 0; i < screenWidth; i++) {
            buffer += "-";
        }
        buffer += "\n";

        UpdateNewsTicker();

        /* if (!newsTickerRunning) {
            newsTickerRunning = bool(--newsTickerTime == 0);
            if (newsTickerRunning) {
                newsTickerTime = 3;
                srand(clock());
                newsTickerMsg = newsTickerMsgArr[rand()%(newsTickerMsgArr->length())];
            }
        } else {
            if (--newsTickerTime == 0) {
                newsTickerTime = 3;
                for (i = -40; i < 0; i++) {
                    if (newsTick + i < 0 || newsTick + i >= newsTickerMsg.length()) {
                        buffer += "";
                    } else {
                        buffer += newsTickerMsg[newsTick + i];
                    }
                }
                if (++newsTick >= newsTickerMsg.length()+40) {
                    newsTick = 0;
                    newsTickerRunning = false;
                    srand(clock());
                    newsTickerTime = (rand() % 60) + 20;
                }
            }   
        } */

        // buffer += "\n" + to_string((int)newsTickerRunning) + "\n" + newsTickerMsg + "\n" + to_string(newsTickerTime) + "\n";
        // cout << endl << newsTickerRunning << endl << newsTickerMsg << endl << newsTickerTime << endl;

        buffer += "\n";
        for (int i = 0; i < screenWidth; i++) {
            buffer += "-";
        }
        buffer += "\n";

        switch (menu) {
            case MENU_MAIN: // main menu
                /* buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + (string)"this is where the actual data goes" + bigEmpty; */
                UpdateMenuReset();
                UpdateMenuLine("this is where the actual data goes", 10);
                break;
            case MENU_SETTINGS:
                /* buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty; */
                UpdateMenuReset();
                UpdateMenuLine("settings menu", 10);
                break;
            case MENU_CREDITS:
                /* buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty; */
                UpdateMenuReset();
                UpdateMenuLine("credits menu", 10);
                break;
            case MENU_HELP:
                /* buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + (string)"help menu" + bigEmpty; */
                UpdateMenuReset();
                UpdateMenuLine("help menu", 10);
                break;
            case MENU_SWITCH:
                UpdateMenuReset();
                UpdateMenuLine("> main          Main menu", 1);
                UpdateMenuLine("> settings      Settings menu", 2);
                UpdateMenuLine("> credits       Credits menu", 3);
                UpdateMenuLine("> help          Help menu", 4);
                UpdateMenuLine("> cancel        Go back to the previous menu", 9);
                UpdateMenuLine("Enter the name of a menu to switch to it", 10);
                break;
            default:
                /* buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + (string)"this is the default fallback screen" + bigEmpty; */
                UpdateMenuReset();
                UpdateMenuLine("this is the default fallback screen", 10);
                break;
        }

        for (int i = 0; i < (sizeof(menuBuffer)/sizeof(menuBuffer[0])); i++) {
            buffer += menuBuffer[i];
        }

        buffer += "\n>";

        bool consoleInput = ReadConsoleInputExW(inputHandle, inputRecordsInBuffer, 128, &numOfCharactersRead, (CONSOLE_READ_NOWAIT));

        for (i = 0; i < numOfCharactersRead; i++) {
            switch (inputRecordsInBuffer[i].EventType) {
                case KEY_EVENT:
                    KeyEventProc(inputRecordsInBuffer[i].Event.KeyEvent);
                    break;
                case MOUSE_EVENT:
                    break;
                case WINDOW_BUFFER_SIZE_EVENT:
                    break;
                case FOCUS_EVENT:
                case MENU_EVENT:
                    break;
                default:
                    // ErrorExit("Unknown event type");
                    break;
            }
        }

        UpdateScreen(outputHandle, 100);
        /* cout << buffer;
        Sleep(100);
        buffer = ""; */
    }

    SetConsoleMode(inputHandle, modeFlagsOld);

    return 0;
}

// ------------------
//  Custom functions
// ------------------

void ParseInput(string input) {
    string lowerInput;
    for (char c : input)
    {
        lowerInput += tolower(c);
    }
    if (menu == MENU_SWITCH) {
        if (lowerInput == "help") SetMenu(MENU_HELP);
        else if (lowerInput == "main") SetMenu(MENU_MAIN);
        else if (lowerInput == "settings") SetMenu(MENU_SETTINGS);
        else if (lowerInput == "credits") SetMenu(MENU_CREDITS);
        else if (lowerInput == "cancel") SetMenu(menuPrev);
    }
    if (lowerInput == "menu") SetMenu(MENU_SWITCH);
    else if (lowerInput == "menu help") SetMenu(MENU_HELP);
    else if (lowerInput == "menu main") SetMenu(MENU_MAIN);
    else if (lowerInput == "menu settings") SetMenu(MENU_SETTINGS);
    else if (lowerInput == "menu credits") SetMenu(MENU_CREDITS);
}

void SetMenu(MenuState newMenu) {
    menuPrev = menu;
    menu = newMenu;
}

void UpdateMenuLine(string text, int lineNum) {
    menuBuffer[clampi(lineNum-1, 0, 9)] = "\n" + text + bigEmpty;
}

void UpdateMenuReset() {
    for (int i = 0; i < (sizeof(menuBuffer)/sizeof(menuBuffer[0])); i++) {
        menuBuffer[i] = "\n" + bigEmpty;
    }
}

void UpdateScreen(HANDLE hConsole, int delay) {
    CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;
    SMALL_RECT scrollRect;
    COORD scrollTarget;
    CHAR_INFO fill;

    if (!GetConsoleScreenBufferInfo(hConsole, &consoleScreenBufferInfo)) return;

    /* scrollRect.Left = 0;
    scrollRect.Top = 0;
    scrollRect.Right = consoleScreenBufferInfo.dwSize.X;
    scrollRect.Bottom = consoleScreenBufferInfo.dwSize.Y;

    scrollTarget.X = 0;
    scrollTarget.Y = (SHORT)(0 - consoleScreenBufferInfo.dwSize.Y);

    fill.Char.UnicodeChar = TEXT(' ');
    fill.Attributes = consoleScreenBufferInfo.wAttributes;

    ScrollConsoleScreenBuffer(hConsole, &scrollRect, NULL, scrollTarget, &fill); */

    consoleScreenBufferInfo.dwCursorPosition.X = 0;
    consoleScreenBufferInfo.dwCursorPosition.Y = 0;

    SetConsoleCursorPosition(hConsole, consoleScreenBufferInfo.dwCursorPosition);

    cout << buffer;
    
    consoleScreenBufferInfo.dwCursorPosition.X = 1;
    consoleScreenBufferInfo.dwCursorPosition.Y = 16;

    SetConsoleCursorPosition(hConsole, consoleScreenBufferInfo.dwCursorPosition);

    cout << bigEmpty;

    SetConsoleCursorPosition(hConsole, consoleScreenBufferInfo.dwCursorPosition);

    cout << inputChars;

    buffer = "";

    Sleep(delay);
}

void UpdateNewsTicker() {
    if (!newsTickerRunning) {
        if (--newsTickerTime == 0) {
            newsTickerTime = newsSpeed;
            newsTickerRunning = true;
            srand(clock());
            newsTickerMsg = newsTickerMsgArr[rand()%(sizeof(newsTickerMsgArr)/sizeof(newsTickerMsgArr[0]))];
        }
    }
    else {
        if (--newsTickerTime == 0) {
            newsTickerTime = newsSpeed;
            for (int i = -screenWidth; i < 0; i++) {
                if (newsTick + i >= 0 && newsTick + i < newsTickerMsg.length()) {
                    buffer += newsTickerMsg[newsTick + i];
                } else {
                    buffer += " ";
                }
            }
            if (++newsTick > newsTickerMsg.length()+screenWidth) {
                buffer += "\r ";
                newsTick = 0;
                newsTickerRunning = false;
                srand(clock());
                newsTickerTime = (rand()%60)+20;
            }
        }
    }
}

// ------------------------------------
//  Microsoft's command line functions
// ------------------------------------

VOID ErrorExit(LPCSTR lpszMessage) {
    fprintf(stderr, "%s\n", lpszMessage);
    SetConsoleMode(inputHandle, modeFlagsOld);
    ExitProcess(0);
}

VOID KeyEventProc(KEY_EVENT_RECORD keyInputRecord) {
    /* printf("Key event: ");
    if (keyInputRecord.bKeyDown) printf("key pressed: ");
    else printf("key released: ");
    cout << keyInputRecord.uChar.AsciiChar << endl; */
    if (keyInputRecord.bKeyDown) {
        if (keyInputRecord.uChar.AsciiChar == 13) {
            ParseInput(inputChars);
            inputChars = "";
        }
        else if (keyInputRecord.uChar.AsciiChar == 32) {
            inputChars += " ";
        }
        else if (keyInputRecord.uChar.AsciiChar == 8 && inputChars.length() > 0) {
            inputChars.pop_back();
        }
        else if (keyInputRecord.uChar.AsciiChar > 32 && keyInputRecord.uChar.AsciiChar < 127 && inputChars.length() < 128) {
            inputChars.push_back(keyInputRecord.uChar.AsciiChar);
        }
    }
}

VOID MouseEventProc(MOUSE_EVENT_RECORD mouseInputRecord) {
    #ifndef MOUSE_HWHEELED
    #define MOUSE_HWHEELED 0x0008
    #endif
        /* printf("Mouse event: "); */
        switch(mouseInputRecord.dwEventFlags) {
            case 0:
                /* if (mouseInputRecord.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) printf("left button press \n");
                else if (mouseInputRecord.dwButtonState == RIGHTMOST_BUTTON_PRESSED) printf ("right button press \n");
                else printf("button press\n"); */
                break;
            case DOUBLE_CLICK:
                /* printf("double click\n"); */
                break;
            case MOUSE_HWHEELED:
                /* printf("horizontal mouse wheel\n"); */
                break;
            case MOUSE_MOVED:
                /* printf("mouse moved\n"); */
                break;
            case MOUSE_WHEELED:
                /* printf("vertical mouse wheel\n"); */
                break;
            default:
                /* printf("unknown\n"); */
                break;
        }
}

VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD windowBufferSizeRecord) {
    /* printf("Resize event\n");
    printf("Console screen buffer is %d columns by %d rows.\n", windowBufferSizeRecord.dwSize.X, windowBufferSizeRecord.dwSize.Y); */
}

// -------------------
//  Utility functions
// -------------------

int clampi(int num, int min, int max) {
    num = num < min ? min: num;
    num = num > max ? max: num;
    return num;
}

