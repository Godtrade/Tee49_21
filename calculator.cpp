#undef UNICODE
#undef _UNICODE
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define ID_INPUT1 101
#define ID_INPUT2 102
#define ID_BTN_ADD 201
#define ID_BTN_SUB 202
#define ID_BTN_MUL 203
#define ID_BTN_DIV 204

HWND hInput1, hInput2;

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		case WM_CREATE: {
            CreateWindow("STATIC", "Number 1:", WS_VISIBLE | WS_CHILD, 10, 20, 70, 20, hwnd, NULL, NULL, NULL);
            hInput1 = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 80, 20, 140, 20, hwnd, (HMENU)ID_INPUT1, NULL, NULL);

            CreateWindow("STATIC", "Number 2:", WS_VISIBLE | WS_CHILD, 10, 55, 70, 20, hwnd, NULL, NULL, NULL);
            hInput2 = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 80, 55, 140, 20, hwnd, (HMENU)ID_INPUT2, NULL, NULL);

            CreateWindow("BUTTON", "+", WS_VISIBLE | WS_CHILD, 15, 100, 45, 45, hwnd, (HMENU)ID_BTN_ADD, NULL, NULL);
            CreateWindow("BUTTON", "-", WS_VISIBLE | WS_CHILD, 65, 100, 45, 45, hwnd, (HMENU)ID_BTN_SUB, NULL, NULL);
            CreateWindow("BUTTON", "*", WS_VISIBLE | WS_CHILD, 115, 100, 45, 45, hwnd, (HMENU)ID_BTN_MUL, NULL, NULL);
            CreateWindow("BUTTON", "/", WS_VISIBLE | WS_CHILD, 165, 100, 45, 45, hwnd, (HMENU)ID_BTN_DIV, NULL, NULL);
            break;
        }
		case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            if (wmId >= ID_BTN_ADD && wmId <= ID_BTN_DIV) {
                char buf1[100], buf2[100];
                
                GetWindowText(hInput1, buf1, 100);
                GetWindowText(hInput2, buf2, 100);

                double num1 = atof(buf1);
                double num2 = atof(buf2);
                double result = 0;
                char t[100]; 

                if (wmId == ID_BTN_ADD) result = num1 + num2;
                else if (wmId == ID_BTN_SUB) result = num1 - num2;
                else if (wmId == ID_BTN_MUL) result = num1 * num2;
                else if (wmId == ID_BTN_DIV) {
                    if (num2 != 0) result = num1 / num2;
                    else {
                        MessageBox(hwnd, "Cannot divide by zero!", "Error", MB_OK | MB_ICONERROR);
                        break;
                    }
                }

                sprintf(t, "%f", result); // เปลี่ยนตัวเลขเป็น c-string
                MessageBox(hwnd, t, "Result", MB_OK | MB_ICONINFORMATION); // แสดง Message Box
            }
            break;
        }
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize	 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor	 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 0));
	wc.lpszClassName = "WindowClass";
	wc.hIcon	 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm	 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","My Calculator",WS_VISIBLE | WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		250, /* width */
		200, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	while(GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg); /* Send it to WndProc */
	}
	return msg.wParam;
}
