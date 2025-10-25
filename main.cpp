#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <cstring>
using namespace std;

HANDLE hStdin;
DWORD fdwSaveOldMode;

VOID ErrorExit(LPCSTR);
VOID KeyEventProc(KEY_EVENT_RECORD);
VOID MouseEventProc(MOUSE_EVENT_RECORD);
VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD);

string input_chars = "";
int temp = 0;

int main(VOID) {
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
        system("cls");
        cout << "Idle IncremenTerminal v0.01b\nInput \"help\" for all possible commands!\n----------------------------------------";
        /* cout << temp; */
        cout << "\n\n\n\n\n\n\n\n\n\n" << "this is where the actual data goes in" << endl << ">" << input_chars;

        /* temp++;
        if (temp >= 30) temp = 0; */

        if (!ReadConsoleInput(
            hStdin,
            irInBuf,
            128,
            &cNumRead
        ))
        ErrorExit("ReadConsoleInput");

        for (i = 0; i < cNumRead; i++) {
            switch (irInBuf[i].EventType) {
                case KEY_EVENT:
                    KeyEventProc(irInBuf[i].Event.KeyEvent);
                    break;
                case MOUSE_EVENT:
                    MouseEventProc(irInBuf[i].Event.MouseEvent);
                    break;
                case WINDOW_BUFFER_SIZE_EVENT:
                    ResizeEventProc(irInBuf[i].Event.WindowBufferSizeEvent);
                    break;
                case FOCUS_EVENT:
                case MENU_EVENT:
                    break;
                default:
                    ErrorExit("Unknown event type");
                    break;
            }
        }

        Sleep(33);
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
            input_chars = "";
        }
        else if (ker.uChar.AsciiChar > 32 && ker.uChar.AsciiChar < 127 && input_chars.length() < 128) {
            input_chars.push_back(ker.uChar.AsciiChar);
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

