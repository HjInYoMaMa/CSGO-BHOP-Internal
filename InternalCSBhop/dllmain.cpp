#include <Windows.h>

//Offsets, 30 June 2022
struct gameOffsets
{
    DWORD lPlayer = 0xDBF4CC; //dwLocalPlayer
    DWORD fJump = 0x52858A8; //dwForceJump
    DWORD flags = 0x104; //m_fFlags
    DWORD velocity = 0x114; //m_vecVelocity
}offsets;

//Values
struct values
{
    DWORD localPlayer;
    DWORD gameModule;
    BYTE flag;
}val;

//Vector
struct vector
{
    float x, y, z;
};

//Just a little extension, If you type something in the chat and using space then your player starts jumping or you have tabbed out and pressing space it will still jump
bool isPlayerMoving() //This feature will fix it, have fun!
{
    vector playerVel = *(vector*)(val.localPlayer + offsets.velocity);
    int vel = playerVel.x + playerVel.y + playerVel.z;
    if (vel != 0)
        return true;
    else
        return false;
}

void main()
{
    val.gameModule = (DWORD)GetModuleHandleA("client.dll");
    val.localPlayer = *(DWORD*)(val.gameModule + offsets.lPlayer);

    if (val.localPlayer == NULL)
        while (val.localPlayer == NULL)
            val.localPlayer = *(DWORD*)(val.gameModule + offsets.lPlayer);
    //Looping
    while (true)
    {
        val.localPlayer = *(DWORD*)(val.gameModule + offsets.lPlayer);
        val.flag = *(BYTE*)(val.localPlayer + offsets.flags);
        //isPlayerMoving
        if (isPlayerMoving())
            if (GetAsyncKeyState(VK_SPACE) && val.flag & (1 << 0))
                *(DWORD*)(val.gameModule + offsets.fJump) = 6;
    }

}
BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)main, NULL, NULL, NULL);
    }
    return TRUE;
}