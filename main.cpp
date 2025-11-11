/*
  ___    _ _        ___                                        _____                   _             _ 
 |_ _|__| | | ___  |_ _|_ __   ___ _ __ ___ _ __ ___   ___ _ _|_   _|__ _ __ _ __ ___ (_)_ __   __ _| |
  | |/ _` | |/ _ \  | || '_ \ / __| '__/ _ \ '_ ` _ \ / _ \ '_ \| |/ _ \ '__| '_ ` _ \| | '_ \ / _` | |
  | | (_| | |  __/  | || | | | (__| | |  __/ | | | | |  __/ | | | |  __/ |  | | | | | | | | | | (_| | |
 |___\__,_|_|\___| |___|_| |_|\___|_|  \___|_| |_| |_|\___|_| |_|_|\___|_|  |_| |_| |_|_|_| |_|\__,_|_|

MIT License

Copyright (c) 2025 Clickerty

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.



A full idle incremental running entirely within Windows Terminal! Made in C++

If you're here to look for clues and secrets in the final game:
  ____   ____ ____      _    __  __ _ _ _ 
 / ___| / ___|  _ \    / \  |  \/  | | | |
 \___ \| |   | |_) |  / _ \ | |\/| | | | |
  ___) | |___|  _ <  / ___ \| |  | |_|_|_|
 |____/ \____|_| \_\/_/   \_\_|  |_(_|_|_)
                                          
Don't ruin the fun!!! (please)                                    

If you're here to analyze the code and see how some things are done:

Welcome! If you want to contribute to this project, issues and pull requests are always happily appreciated!





If you think this game is too similar to something like Antimatter Dimensions (which you should play if you haven't):

shut up no it isn't








extra padding so no code is immediately visible
*/

#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <array>
#include <map>
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
    MENU_SWITCH,
    MENU_MANUAL
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
int ClampInt(int, int, int);
int WrapInt(int, int, int);
string FindInStr(string, char, int);

string inputChars = "";
int temp = 0;

int screenWidth = 60;

bool newsTickerRunning = false; // Dictates if the news ticker is even showing anything
int newsTickerTime = 0; // Time between messages / scrolling speed
string newsTickerMsg = ""; // The message pulled from the array
int newsTick = 0; // Letter index in newsTickerMsg
int newsSpeed = 1; // Scroll speed of newsTickerMsg
string newsTickerMsgArr[3] = {"this is a test message", "oidfjsoojdifoisdjfiosjodf", "no way this actually works dude"};

int help_page = 1;
int help_pages = 1;
string command = "";
map<string, string> manual = {
	{"menu", "a"}
};

bool debug = false;
string debugInfo = "";

int main() {
    srand(time(0));
    newsTickerTime = (rand() % 60) + 20;

    system("title Idle IncremenTerminal");
    system("cls");

    for (int i = 0; i < screenWidth; i++) {
        bigEmpty += " ";
    }

    DWORD numOfCharactersRead, modeFlags, i;
    LPDWORD numOfCharactersUnread;
    INPUT_RECORD inputRecordsInBuffer[screenWidth], keyInputRecordsInBuffer[screenWidth];

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

        buffer = "Idle IncremenTerminal v0.00a\n" + bigEmpty + "\r";
        if (debug == true) buffer += debugInfo;
        else buffer += "Input \"menu help\" for all possible commands!";

        buffer += "\n";
        for (int i = 0; i < screenWidth; i++) {
            buffer += "-";
        }
        buffer += "\n";

        UpdateNewsTicker();

        buffer += "\n";
        for (int i = 0; i < screenWidth; i++) {
            buffer += "-";
        }
        buffer += "\n";

        switch (menu) {
            case MENU_MAIN:
                UpdateMenuReset();
                UpdateMenuLine("this is where the actual data goes", 10);
                break;
            case MENU_SETTINGS:
                UpdateMenuReset();
                UpdateMenuLine("settings menu", 10);
                break;
            case MENU_CREDITS:
                UpdateMenuReset();
                UpdateMenuLine("Clickerty               Everything", 1);
                UpdateMenuLine("credits menu", 10);
                break;
            case MENU_HELP:
                UpdateMenuReset();
                UpdateMenuLine("menu [<args>]         Shows the menu specified in [<args>]", 1);
                UpdateMenuLine("    - Leave empty to see a list of all available menus", 2);
                UpdateMenuLine("help [<command>]      Extra info about the inputted command", 8);
                UpdateMenuLine("    - Tip: Leave <command> empty for a complete manual", 9);
                UpdateMenuLine("    - Alias: manual [<command>]", 10);
                break;
            case MENU_MANUAL:
            	UpdateMenuReset();
            	if (manual.count(command) == 0) {
            	UpdateMenuLine("Command not recognized. Input \"help\" with no arguments", 1);
            	UpdateMenuLine("for a complete list of every available command. Learn more", 2);
            	UpdateMenuLine("about each command by inputting \"help [<command>]\" or", 3);
            	UpdateMenuLine("\"manual [<command>]\"", 4);
				} else {
					UpdateMenuLine("> " + command, 1);
					UpdateMenuLine(manual.at(command), 2);
					UpdateMenuLine("<- (" + to_string(help_page) + "/" + to_string(help_pages) + ") ->", 10);
				}
            	break;
            case MENU_SWITCH:
                UpdateMenuReset();
                UpdateMenuLine("> main          Main menu", 1);
                UpdateMenuLine("> settings      Settings menu", 2);
                UpdateMenuLine("> credits       Credits menu", 3);
                UpdateMenuLine("> help          Help menu", 4);
                UpdateMenuLine("> manual        Manual", 5);
                UpdateMenuLine("> cancel        Go back to the previous menu", 9);
                UpdateMenuLine("Enter the name of a menu to switch to it", 10);
                break;
            default:
                UpdateMenuReset();
                UpdateMenuLine("this is the default fallback screen", 10);
                break;
        }

        for (int i = 0; i < (sizeof(menuBuffer)/sizeof(menuBuffer[0])); i++) {
            buffer += menuBuffer[i];
        }

        buffer += "\n>";

        bool consoleInput = ReadConsoleInputExW(inputHandle, inputRecordsInBuffer, screenWidth, &numOfCharactersRead, (CONSOLE_READ_NOWAIT));

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
    }

    SetConsoleMode(inputHandle, modeFlagsOld);

    return 0;
}

// ------------------
//  Custom functions
// ------------------

void ParseInput(string input) {
    string lowerInput;
    string splitInput[3] = {" ", " ", " "};
    for (int i = 0; i < input.length(); i++) {
        lowerInput += tolower(input[i]);
    }
    if (lowerInput == "debug") debug = debug ? false: true;
    splitInput[0] = FindInStr(lowerInput, ' ', 0);
    splitInput[1] = FindInStr(lowerInput, ' ', 1);
    splitInput[2] = FindInStr(lowerInput, ' ', 2);
    debugInfo = splitInput[0] + " " + splitInput[1] + " " + splitInput[2];
    if (menu == MENU_SWITCH) {
        if (splitInput[0] == "help") SetMenu(MENU_HELP);
        else if (splitInput[0] == "main") SetMenu(MENU_MAIN);
        else if (splitInput[0] == "settings") SetMenu(MENU_SETTINGS);
        else if (splitInput[0] == "credits") SetMenu(MENU_CREDITS);
        else if (splitInput[0] == "manual" ) SetMenu(MENU_MANUAL);
        else if (splitInput[0] == "cancel") SetMenu(menuPrev);
    }
    if (splitInput[0] == "menu") {
        if (splitInput[1] == "help") SetMenu(MENU_HELP);
        else if (splitInput[1] == "main") SetMenu(MENU_MAIN);
        else if (splitInput[1] == "settings") SetMenu(MENU_SETTINGS);
        else if (splitInput[1] == "credits") SetMenu(MENU_CREDITS);
        else if (splitInput[1] == "manual") SetMenu(MENU_MANUAL);
        else if (menu != MENU_SWITCH) SetMenu(MENU_SWITCH);
    }
    if (splitInput[0] == "help" || splitInput[0] == "manual") {
    	command = splitInput[1] == splitInput[0] ? "": splitInput[1];
    	SetMenu(MENU_MANUAL);
	}
}

void SetMenu(MenuState newMenu) {
    menuPrev = menu;
    menu = newMenu;
}

void UpdateMenuLine(string text, int lineNum) {
	menuBuffer[ClampInt(lineNum-1, 0, 9)] = "\n" + text + bigEmpty;
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

    cout << bigEmpty << endl << bigEmpty << endl << bigEmpty << endl << bigEmpty << endl << bigEmpty << endl << bigEmpty << endl << bigEmpty;

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
        else if (keyInputRecord.uChar.AsciiChar == 8 && inputChars.length() > 0) inputChars.pop_back();
        else if (keyInputRecord.uChar.AsciiChar >= 32 && keyInputRecord.uChar.AsciiChar < 127 && inputChars.length() < screenWidth) inputChars.push_back(keyInputRecord.uChar.AsciiChar);
        else if (menu == MENU_MANUAL && keyInputRecord.wVirtualKeyCode == VK_RIGHT) help_page = WrapInt(++help_page, 1, help_pages);
        else if (menu == MENU_MANUAL && keyInputRecord.wVirtualKeyCode == VK_LEFT) help_page = WrapInt(--help_page, 1, help_pages);
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

int ClampInt(int num, int min, int max) {
    num = num < min ? min: num;
    num = num > max ? max: num;
    return num;
}

int WrapInt(int num, int min, int max) {
	num = num < min ? max: num;
	num = num > max ? min: num;
	return num;
}

string FindInStr(string str, char delimiter, int index) {
	string temp = "";
	string tempStr = str;
	int i = 0;
	while (i <= index) {
		if (tempStr.find_first_of(delimiter) != string::npos) {
			temp = tempStr.substr(0, tempStr.find_first_of(delimiter));
			tempStr.erase(0, tempStr.find_first_of(delimiter) + 1);
		} else {
			i = index;
			temp = tempStr;
		}
		i++;
	}
	return temp;
}
