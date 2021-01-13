#include <iostream>
#include <windows.h>
#include <sstream>
#include <string>
#include <random>
#include <fstream>
#include <thread>
#include "ida.h"
#include "Protection/VMProtect/VMP.h"
#include "Protection/BattlEye/RawBattlEye.h"
#include "Protection/Xor/StringEncryption.h"
#include "Protection/BattlEye/Services/Services.h"
#include "Protection/BattlEye/Privliges/Privliges.h"

#pragma comment(lib, "urlmon.lib")
using namespace std;

std::string Key = "";
HANDLE DriverHandle;
DWORD ProcessToProtect = GetCurrentProcessId();

std::string GetHwUID()
{
	if (!VMProtectIsValidImageCRC()) exit(0);
	HW_PROFILE_INFO hwProfileInfo;
	if (!VMProtectIsValidImageCRC()) exit(0);
	std::string szHwProfileGuid = "";
	if (!VMProtectIsValidImageCRC()) exit(0);
	if (GetCurrentHwProfileA(&hwProfileInfo) != NULL) {
		if (!VMProtectIsValidImageCRC()) exit(0);
		szHwProfileGuid = hwProfileInfo.szHwProfileGuid;
		if (!VMProtectIsValidImageCRC()) exit(0);
	}
	if (!VMProtectIsValidImageCRC()) exit(0);
	return szHwProfileGuid;
}
std::string RandomString()
{
	std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

	std::random_device rd;
	std::mt19937 generator(rd());

	std::shuffle(str.begin(), str.end(), generator);

	return str.substr(0, 27);
}
std::string CenterString(char* s)
{
	int l = strlen(s);

	int pos = (int)((80 - l) / 2);
	std::stringstream ss;
	for (int i = 0; i < pos; i++)
		ss << "  ";

	ss << s;
	return ss.str();
}
void w(std::string line, bool center = false)
{
	if (!VMProtectIsValidImageCRC()) exit(0);
	char* cstr = (char*)line.c_str();
	if (!VMProtectIsValidImageCRC()) exit(0);
	if (center) std::cout << CenterString(cstr) << std::endl;
	if (!VMProtectIsValidImageCRC()) exit(0);
	else std::cout << "[Diminsh.VIP] " << line << std::endl;
	if (!VMProtectIsValidImageCRC()) exit(0);
}
string LoadR6S() {
	VMProtectBegin("LoadR6S");
	if (!VMProtectIsValidImageCRC()) exit(0);
	return "success";

	VMProtectEnd();
}
string GetWebsiteText(LPCSTR url) {
	if (!VMProtectIsValidImageCRC()) exit(0);
	char szFileName[80] = "_";
	HRESULT hr = URLDownloadToFile(NULL, url, szFileName, 0, NULL);
	if (!VMProtectIsValidImageCRC()) exit(0);
	if (hr == S_OK)
	{
		if (!VMProtectIsValidImageCRC()) exit(0);
		ifstream CheckFile;
		CheckFile.open(szFileName);
		stringstream strStream;
		strStream << CheckFile.rdbuf();
		string response = strStream.str();
		CheckFile.close();
		remove(szFileName);
		if (!VMProtectIsValidImageCRC()) exit(0);
		return response;

	}
	else {
		if (!VMProtectIsValidImageCRC()) exit(0);
		w(_xor_("[-] Error: Connection failed"));
		if (!VMProtectIsValidImageCRC()) exit(0);
		return  _xor_("ERRORINTERNAL");
	}
}

bool Authenticate(std::string key) {
	VMProtectBegin("Authenticate");
	if (!VMProtectIsValidImageCRC()) exit(0);
	std::stringstream oss;
	oss << _xor_("https://www.diminsh.vip/3sRFDTASac4rS/auth.php?authkey=4gh874fgh4fg7dfsdASSAIHBSDAI&command=0&param=") << key << _xor_("&hwid=") << GetHwUID();
	std::string url = oss.str();
	if (!VMProtectIsValidImageCRC()) exit(0);
	string result = GetWebsiteText(url.c_str());
	if (!VMProtectIsValidImageCRC()) exit(0);
	if (result == ("ERRORINTERNAL")){
		return false;
	}
	if (!VMProtectIsValidImageCRC()) exit(0);
	if (result == ("54330F2E111E2C1F2DB43B985FA8CCF6175E328ABB785B2865F620CD759DD13E")) {
		w(_xor_("[-] Wrong key"));
		return false;
	}
	if (!VMProtectIsValidImageCRC()) exit(0);
	else if (result == ("DEFGC4664FD02BB5F9F1C8FCC2B16A877303E2B2C9A61CE3386F211FFC76CABC")) {
		w(_xor_("[-] License expired"));
		return false;
	}
	if (!VMProtectIsValidImageCRC()) exit(0);
	else if (result == ("04C8C4664FD02BB5F9F1C8FCC2B16A877303E2B2C9A61CE3386F211FFC76C03C")) {
		w(_xor_("[-] Wrong HWID"));
		if (!VMProtectIsValidImageCRC()) exit(0);
		std::stringstream oss;
		oss << _xor_("https://www.diminsh.vip/3sRFDTASac4rS/auth.php?authkey=4gh874fgh4fg7dfsdASSAIHBSDAI&command=5&param=") << key;
		std::string url = oss.str();
		if (!VMProtectIsValidImageCRC()) exit(0);
		string resultcount = GetWebsiteText(url.c_str());
		if (!VMProtectIsValidImageCRC()) exit(0);
		if (resultcount == ("ERRORINTERNAL")) {
			if (!VMProtectIsValidImageCRC()) exit(0);
			return false;
		}
		if (!VMProtectIsValidImageCRC()) exit(0);
		if (resultcount == _xor_("0")) {
			if (!VMProtectIsValidImageCRC()) exit(0);
			w(_xor_("[-] You don't have any HWID resets left"));
			if (!VMProtectIsValidImageCRC()) exit(0);
			return false;
		}
		if (!VMProtectIsValidImageCRC()) exit(0);
		std::stringstream oss1;
		oss1 << _xor_("[?] Do you want to reset your hwid? You have ") << resultcount << _xor_(" resets left (y/n)");
		std::string outputyesno = oss1.str();
		if (!VMProtectIsValidImageCRC()) exit(0);
		w(outputyesno);
		std::string input = "n";
		getline(cin, input);
		if (input != "y") {
			return false;
		}
		if (!VMProtectIsValidImageCRC()) exit(0);
		std::stringstream oss2;
		oss2 << _xor_("https://www.diminsh.vip/3sRFDTASac4rS/auth.php?authkey=4gh874fgh4fg7dfsdASSAIHBSDAI&command=6&param=") << key;
		std::string url1 = oss2.str();
		if (!VMProtectIsValidImageCRC()) exit(0);
		string resultreset = GetWebsiteText(url1.c_str());
		if (!VMProtectIsValidImageCRC()) exit(0);
		if (resultreset == ("ERRORINTERNAL")) {
			if (!VMProtectIsValidImageCRC()) exit(0);
			return false;
		}

		if (FAILED(resultreset == ("9AE436528AC012B22D2A926F967C7848876CE2D592A4CF3E778DEC3CF9282355"))) {
			if (!VMProtectIsValidImageCRC()) exit(0);
			w(_xor_("[-] There was a error resetting your HWID"));
			if (!VMProtectIsValidImageCRC()) exit(0);
		}

		w(_xor_("[+] HWID reset. Please restart the application"));

		Sleep(4000);

		exit(0);

		return false;
	}
	if (!VMProtectIsValidImageCRC()) exit(0);
	else if (result == ("BC648B6588CD8F9AEEF31DB2D07B9951A72DB7660CC3F0B827D8312B51878D5B")) {
		if (!VMProtectIsValidImageCRC()) exit(0);
		return true;
	}
	if (!VMProtectIsValidImageCRC()) exit(0);

	return false;
	if (!VMProtectIsValidImageCRC()) exit(0);

	VMProtectEnd();
}
bool AuthentifyUser() {
	VMProtectBegin("AuthentifyUser");
	if (!VMProtectIsValidImageCRC()) exit(0);
	w("Please enter your key");

	getline(cin, Key);

	if (Authenticate(Key)) {
		if (!VMProtectIsValidImageCRC()) exit(0);
		w("Key authenticated successfully.");
		w("Initialzing software...");
		if (!VMProtectIsValidImageCRC()) exit(0);
		return true;
	}
	else {
		if (!VMProtectIsValidImageCRC()) exit(0);
		w("Key authentication failed. #0002");
		if (!VMProtectIsValidImageCRC()) exit(0);
		return false;

	}
	if (!VMProtectIsValidImageCRC()) exit(0);
	VMProtectEnd();

}

void BEStartProtection() {
	VMProtectBegin("BEStartProtection");
	if (!VMProtectIsValidImageCRC()) exit(0);
	CreateFileFromMemory("C:\\Windows\\System32\\Drivers\\vuldriver.sys", (const char*)driver, sizeof(driver));
	printf("Driver Protection started\n");
	if (Service::Create("C:\\Windows\\System32\\Drivers\\vuldriver.sys")) {
		printf("[$] Started Service\n");

		if (Privs::RaiseAndGetHandle(SE_TCB_NAME, &DriverHandle)) {
			printf("Driver Handle: %x\n Protection PID: %i", DriverHandle, ProcessToProtect);

			if (DeviceIoControl(DriverHandle, 0x22e048, &ProcessToProtect, sizeof(ProcessToProtect), 0, 0, 0, 0))
				printf("Called add_to_list\n");

			if (DeviceIoControl(DriverHandle, 0x22e050, 0, 0, 0, 0, 0, 0))
				printf("Called init\n");

			printf("Your process + %x is now protected\n", ProcessToProtect);
		}
	}
	if (!VMProtectIsValidImageCRC()) exit(0);
	VMProtectEnd();
}
void BEEndProtection(int Sleep) {
	VMProtectBegin("BEEndProtection");
	if (!VMProtectIsValidImageCRC()) exit(0);
	printf("Closing Loader\n");
	Service::Close("EQProtect");
	exit(0);
	if (!VMProtectIsValidImageCRC()) exit(0);
	VMProtectEnd();
}

uintptr_t EncryptManagers(uintptr_t ManagerOffset, uintptr_t GlobalKey) {
	if (!VMProtectIsValidImageCRC()) exit(0);
	uintptr_t Resault = NULL;

	Resault = ManagerOffset;

	Resault = Resault * 69;

	Resault = __ROL8__(Resault, 0x420);

	if (!VMProtectIsValidImageCRC()) exit(0);
	return Resault;

}
uintptr_t DecryptManagers(uintptr_t EncryptedManger, uintptr_t GlobalKey) {
	if (!VMProtectIsValidImageCRC()) exit(0);
	uintptr_t Resault = NULL;

	Resault = EncryptedManger;

	Resault = __ROR8__(Resault, 0x420);

	Resault = Resault / 69;

	if (!VMProtectIsValidImageCRC()) exit(0);
	return Resault;

}

void HeartBeat() {
	VMProtectBegin("HeartBeat Thread");
	if (!VMProtectIsValidImageCRC()) exit(0);
	while (VMProtectIsValidImageCRC()) {
		if (!VMProtectIsValidImageCRC()) exit(0);

		if (!VMProtectIsValidImageCRC()) exit(0);
	}
	if (!VMProtectIsValidImageCRC()) exit(0);
	VMProtectEnd();
}

int main() {

	VMProtectBegin("Main Thread");

	BEStartProtection();

	if (!VMProtectIsValidImageCRC()) exit(0);

	SetConsoleTitle(RandomString().c_str());

	if (!VMProtectIsValidImageCRC()) exit(0);
	if (AuthentifyUser())
	{
		if (!VMProtectIsValidImageCRC()) exit(0);
		w("ooga");
		std::cin.get();
		if (!VMProtectIsValidImageCRC()) exit(0);
	}
	if (!VMProtectIsValidImageCRC()) exit(0);

	printf("encrypted managers %p \n", EncryptManagers(0x7096828, 0x0));
	printf("decrypted managers %p \n", DecryptManagers(EncryptManagers(0x7096828, 0x0), 0x0));
	if (!VMProtectIsValidImageCRC()) exit(0);

	std::cin.get();

	BEEndProtection(2000);

	std::cin.get();

	VMProtectEnd();

}