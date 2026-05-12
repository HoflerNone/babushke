#include "MainMenuPing.h"
#include "Windows.h"

using namespace PingPong;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    Application::Run(gcnew MainMenuPing());

    return 0;
}