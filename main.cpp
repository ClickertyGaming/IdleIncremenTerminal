#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cctype>
using namespace std;

typedef bool (WINAPI *RCIEX)(HANDLE, PINPUT_RECORD, DWORD, LPDWORD, USHORT);

#ifndef CONSOLE_READ_NOREMOVE
#define CONSOLE_READ_NOREMOVE   0x0001
#endif

#ifndef CONSOLE_READ_NOWAIT
#define CONSOLE_READ_NOWAIT     0x0002
#endif

BOOL WINAPI ReadConsoleInputExA(
    _In_ HANDLE hConsoleInput,
    _Out_writes_(nLength) PINPUT_RECORD lpBuffer,
    _In_ DWORD nLength,
    _Out_ LPDWORD lpNumberOfEventsRead,
    _In_ USHORT wFlags);

BOOL WINAPI ReadConsoleInputExW(
    _In_ HANDLE hConsoleInput,
    _Out_writes_(nLength) PINPUT_RECORD lpBuffer,
    _In_ DWORD nLength,
    _Out_ LPDWORD lpNumberOfEventsRead,
    _In_ USHORT wFlags);

string buffer = "";

string bigEmpty = "                                       ";

HANDLE hStdin;
HANDLE hStdout;
DWORD fdwSaveOldMode;

VOID ErrorExit(LPCSTR);
VOID KeyEventProc(KEY_EVENT_RECORD);
VOID MouseEventProc(MOUSE_EVENT_RECORD);
VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD);
void UpdateScreen(HANDLE, int);
void UpdateNewsTicker();
void ParseInput(string);

string inputChars = "";
int temp = 0;

enum MenuState {
    MENU_MAIN,
    MENU_SETTINGS,
    MENU_CREDITS,
    MENU_HELP
};
enum MenuState menu = MENU_MAIN;

bool newsTickerRunning = false; // Dictates if the news ticker is even showing anything
int newsTickerTime = 0; // Time between messages / scrolling speed
string newsTickerMsg = ""; // The message pulled from the array
int newsTick = 0; // Letter index in newsTickerMsg
int newsSpeed = 1; // Scroll speed of newsTickerMsg
string newsTickerMsgArr[3] = {"this is a test message", "oidfjsoojdifoisdjfiosjodf", "no way this actually works dude"};

/* TODO:
Fix the news ticker function
*/

int main(VOID) {
    srand(time(0));
    newsTickerTime = (rand() % 60) + 20;

    system("title Idle IncremenTerminal");

    DWORD cNumRead, fdwMode, i;
    LPDWORD cNumUnread;
    INPUT_RECORD irInBuf[128], kirInBuf[128];

    RCIEX fRCIEXA = (RCIEX)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "ReadConsoleInputExA");
    RCIEX fRCIEXW = (RCIEX)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "ReadConsoleInputExW");

    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE)
        ErrorExit("GetStdHandle");

    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdout == INVALID_HANDLE_VALUE)
        ErrorExit("GetStdHandle");
    
    if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
        ErrorExit("GetConsoleMode");
    
    fdwMode = (ENABLE_WINDOW_INPUT | ENABLE_EXTENDED_FLAGS) & ~ENABLE_QUICK_EDIT_MODE & ~ENABLE_ECHO_INPUT;
    if (!SetConsoleMode(hStdin, fdwMode))
        ErrorExit("SetConsoleMode");

    while (true)
    {
        buffer = "Idle IncremenTerminal v0.00a\nInput \"help\" for all possible commands!\n----------------------------------------\n";
        
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

        buffer += "\n----------------------------------------\n";

        switch (menu) {
            case MENU_MAIN: // main menu
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + (string)"this is where the actual data goes" + bigEmpty;
                break;
            case MENU_SETTINGS:
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                break;
            case MENU_CREDITS:
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                break;
            case MENU_HELP:
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + (string)"help menu" + bigEmpty;
                break;
            default:
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + bigEmpty;
                buffer += "\n" + (string)"this is the default fallback screen" + bigEmpty;
                break;
        }

        buffer += "\n>";

        bool consoleInput = fRCIEXW(
            hStdin,
            irInBuf,
            128,
            &cNumRead,
            (CONSOLE_READ_NOWAIT)
        );

        for (i = 0; i < cNumRead; i++) {
            switch (irInBuf[i].EventType) {
                case KEY_EVENT:
                    KeyEventProc(irInBuf[i].Event.KeyEvent);
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

        UpdateScreen(hStdout, 100);
        /* cout << buffer;
        Sleep(100);
        buffer = ""; */
    }

    SetConsoleMode(hStdin, fdwSaveOldMode);

    return 0;
}

void ParseInput(string input) {
    string lowerInput;
    for (char c : input)
    {
        lowerInput += tolower(c);
    }
    
    if (lowerInput == "help") {
        menu = MENU_HELP;
    }
}

void UpdateScreen(HANDLE hConsole, int delay) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    SMALL_RECT scrollRect;
    COORD scrollTarget;
    CHAR_INFO fill;

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;

    /* scrollRect.Left = 0;
    scrollRect.Top = 0;
    scrollRect.Right = csbi.dwSize.X;
    scrollRect.Bottom = csbi.dwSize.Y;

    scrollTarget.X = 0;
    scrollTarget.Y = (SHORT)(0 - csbi.dwSize.Y);

    fill.Char.UnicodeChar = TEXT(' ');
    fill.Attributes = csbi.wAttributes;

    ScrollConsoleScreenBuffer(hConsole, &scrollRect, NULL, scrollTarget, &fill); */

    csbi.dwCursorPosition.X = 0;
    csbi.dwCursorPosition.Y = 0;

    SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);

    cout << buffer;
    
    csbi.dwCursorPosition.X = 1;
    csbi.dwCursorPosition.Y = 16;

    SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);

    cout << bigEmpty;

    SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);

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
            for (int i = -40; i < 0; i++) {
                if (newsTick + i >= 0 && newsTick + i < newsTickerMsg.length()) {
                    buffer += newsTickerMsg[newsTick + i];
                } else {
                    buffer += " ";
                }
            }
            if (++newsTick > newsTickerMsg.length()+40) {
                buffer += "\r ";
                newsTick = 0;
                newsTickerRunning = false;
                srand(clock());
                newsTickerTime = (rand()%60)+20;
            }
        }
    }
}

VOID ErrorExit(LPCSTR lpszMessage) {
    fprintf(stderr, "%s\n", lpszMessage);
    SetConsoleMode(hStdin, fdwSaveOldMode);
    ExitProcess(0);
}

VOID KeyEventProc(KEY_EVENT_RECORD ker) {
    /* printf("Key event: ");
    if (ker.bKeyDown) printf("key pressed: ");
    else printf("key released: ");
    cout << ker.uChar.AsciiChar << endl; */
    if (ker.bKeyDown) {
        if (ker.uChar.AsciiChar == 13) {
            ParseInput(inputChars);
            inputChars = "";
        }
        else if (ker.uChar.AsciiChar == 8 && inputChars.length() > 0) {
            inputChars.pop_back();
        }
        else if (ker.uChar.AsciiChar > 32 && ker.uChar.AsciiChar < 127 && inputChars.length() < 128) {
            inputChars.push_back(ker.uChar.AsciiChar);
        }
    }
}

VOID MouseEventProc(MOUSE_EVENT_RECORD mer) {
    #ifndef MOUSE_HWHEELED
    #define MOUSE_HWHEELED 0x0008
    #endif
        /* printf("Mouse event: "); */
        switch(mer.dwEventFlags) {
            case 0:
                /* if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) printf("left button press \n");
                else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED) printf ("right button press \n");
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

VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr) {
    /* printf("Resize event\n");
    printf("Console screen buffer is %d columns by %d rows.\n", wbsr.dwSize.X, wbsr.dwSize.Y); */
}

