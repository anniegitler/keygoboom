#include <windows.h>
#include <mmsystem.h>
#include <cstdlib>
#include <ctime>
#include <string>

#pragma comment(lib, "winmm.lib")

class KEYBOOM
{
public:

    const char* sounds[5] =
    {
        "Bruh.wav",
        "Cartoon.wav",
        "Fart.wav",
        "heehee.wav",
        "fahhh.wav"
    };

    int id = 0;

    void load()
    {
        for (int i = 0; i < 20; i++)
        {
            std::string cmd =
                "open \"" + std::string(sounds[i % 5]) +
                "\" type waveaudio alias s" + std::to_string(i);

            mciSendStringA(cmd.c_str(), NULL, 0, NULL);
        }
    }

    void boom()
    {
        std::string cmd =
            "play s" + std::to_string(id) + " from 0";

        mciSendStringA(cmd.c_str(), NULL, 0, NULL);

        id++;

        if (id == 20)
            id = 0;
    }
};

KEYBOOM keysound;
HHOOK keyboardHook;

LRESULT CALLBACK keyboard_proc(
    int nCode,
    WPARAM wParam,
    LPARAM lParam)
{
    if (nCode == HC_ACTION &&
        (wParam == WM_KEYDOWN ||
            wParam == WM_SYSKEYDOWN))
    {
        keysound.boom();
    }

    return CallNextHookEx(
        keyboardHook,
        nCode,
        wParam,
        lParam
    );
}

int main()
{
    srand((unsigned)time(NULL));

    keysound.load();

    keyboardHook = SetWindowsHookExA(
        WH_KEYBOARD_LL,
        keyboard_proc,
        GetModuleHandleA(NULL),
        0
    );

    if (!keyboardHook)
        return 1;

    MSG msg;

    while (GetMessageA(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    UnhookWindowsHookEx(keyboardHook);
}