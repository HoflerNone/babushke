#include "Gamefield.h"
#include "Windows.h"
#include "Settings.h"

using namespace PingPong;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew GameField);
	return 0;
}