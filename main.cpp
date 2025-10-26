#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
using namespace std;

string buffer = "";

HANDLE hStdin;
DWORD fdwSaveOldMode;

VOID ErrorExit(LPCSTR);
VOID KeyEventProc(KEY_EVENT_RECORD);
VOID MouseEventProc(MOUSE_EVENT_RECORD);
VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD);

string inputChars = "";
int temp = 0;

int menu = 0;

bool newsTickerRunning = false; // Dictates if the news ticker is even showing anything
int newsTickerTime = 0; // Time between messages / scrolling speed
string newsTickerMsg = ""; // The message pulled from the array
int newsTick = 0; // Letter index in newsTickerMsg
int i;
string newsTickerMsgArr[2] = {"this is a test message", "oidfjsoojdifoisdjfiosjodf"};

void ClearScreen() {
    COORD cursorPosition;
    cursorPosition.X = 0;
    cursorPosition.Y = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}

int main(VOID) {
    srand(time(0));
    newsTickerTime = (rand() % 60) + 20;

    system("title Idle IncremenTerminal");

    DWORD cNumRead, fdwMode, i;
    INPUT_RECORD irInBuf[128];

    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE)
        ErrorExit("GetStdHandle");
    
    if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
        ErrorExit("GetConsoleMode");
    
    fdwMode = (ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS) & ~ENABLE_QUICK_EDIT_MODE;
    if (!SetConsoleMode(hStdin, fdwMode))
        ErrorExit("SetConsoleMode");
    
    while (true)
    {
        buffer = "Idle IncremenTerminal v0.01b\nInput \"help\" for all possible commands!\n----------------------------------------\n";
        
        if (!newsTickerRunning) {
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
        }

        buffer += "\n" + to_string((int)newsTickerRunning) + "\n" + newsTickerMsg + "\n" + to_string(newsTickerTime) + "\n";
        cout << endl << newsTickerRunning << endl << newsTickerMsg << endl << newsTickerTime << endl;

        switch (menu) {
            case 0: // main menu
                buffer += "\n\n\n\n\n\n\n\n\n\n" + (string)"this is where the actual data goes";
                break;
        }
        buffer += "\n>" + inputChars;

        bool consoleInput = ReadConsoleInput(
            hStdin,
            irInBuf,
            10,
            &cNumRead
        );

        for (i = 0; i < cNumRead; i++) {
            switch (irInBuf[i].EventType) {
                case KEY_EVENT:
                    KeyEventProc(irInBuf[i].Event.KeyEvent);
                    break;
                case MOUSE_EVENT:
                    // MouseEventProc(irInBuf[i].Event.MouseEvent);
                    break;
                case WINDOW_BUFFER_SIZE_EVENT:
                    // ResizeEventProc(irInBuf[i].Event.WindowBufferSizeEvent);
                    break;
                case FOCUS_EVENT:
                case MENU_EVENT:
                    break;
                default:
                    ErrorExit("Unknown event type");
                    break;
            }
        }

        ClearScreen();
        cout << buffer;
        Sleep(100);
        buffer = "";
    }

    SetConsoleMode(hStdin, fdwSaveOldMode);

    return 0;
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
            /* printf("enter"); */
            inputChars = "";
        }
        else if (ker.uChar.AsciiChar > 32 && ker.uChar.AsciiChar < 127 && inputChars.length() < 10) {
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

