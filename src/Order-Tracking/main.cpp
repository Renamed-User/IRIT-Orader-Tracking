#include <iostream>

#include <windows.h>

extern "C" {
#include <sqlite3.h>
}

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	std::cout << "SQLite версия: " << sqlite3_libversion() << std::endl;

	return 0;
}