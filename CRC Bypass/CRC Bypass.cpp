#include <iostream>
#include <filesystem>
#include <Windows.h>
#include <thread>

std::string BEDLLPath{ std::filesystem::temp_directory_path().string() + "BEClient2.dll" };

void BEClient2Steal() {
	fopen(BEDLLPath.c_str(), "wb");
	while (true) { Sleep(16000); }
}

int main()
{
	printf("Starting CRC Bypass \n");

	Sleep(2500);

	while (!std::filesystem::exists(BEDLLPath)) {
		printf("Waiting For Game To Start \n");
		Sleep(250);
	}
	std::thread FUCKBE{ BEClient2Steal };

	FUCKBE.detach();
	while(true) { 
		printf("Done :) Minimizing DONT CLOSE\n");
		Sleep(2500);
		::ShowWindow(::GetConsoleWindow(), SW_HIDE);
		Sleep(10000); 
	}
}

