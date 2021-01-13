#include <windows.h>
namespace Privs {
	extern bool RaiseAndGetHandle(LPCSTR priv, HANDLE* driverhandle);
}