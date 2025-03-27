#include <Windows.h>


#include "telegramclient.h"

#include <iostream>
#include <io.h>
#include <fcntl.h>

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(1251);

	TelegramClient telegramClient;

	telegramClient.loop();

	return 0;
}
