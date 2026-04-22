#pragma once
#include <windows.h>
#include <malloc.h>

class WinMouseInput
{
private :
    struct MouseDelta { int dx, dy; };

public : 
    MouseDelta frameDelta = {};

public : 

    WinMouseInput();

    //This tells windows we want to get his inputs
    void registerRawMouse(HWND hwnd) {
        RAWINPUTDEVICE rid;
        rid.usUsagePage = 0x01;
        rid.usUsage = 0x02;
        rid.dwFlags = RIDEV_INPUTSINK; //Get inputs even when window is unfocused
        rid.hwndTarget = hwnd;
        RegisterRawInputDevices(&rid, 1, sizeof(rid));
    }

    // Update the delta 
    void handleRawInput(LPARAM lParam) {
        UINT size = 0;
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));
        RAWINPUT* raw = (RAWINPUT*)_malloca(size);
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, raw, &size, sizeof(RAWINPUTHEADER));

        if (raw->header.dwType == RIM_TYPEMOUSE) {
            frameDelta.dx += raw->data.mouse.lLastX;
            frameDelta.dy += raw->data.mouse.lLastY;
        }
    }

    //receive the windows events 
    LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
        if (msg == WM_INPUT)
            handleRawInput(lp);
        return DefWindowProc(hwnd, msg, wp, lp);
    }

    void resetDelta() { frameDelta = {}; }
};

