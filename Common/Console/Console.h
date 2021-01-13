#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "../Console/fmt/core.h"
#include "../Console/fmt/color.h"

#include <Windows.h>
#include <iostream>

#pragma warning( disable : 4996 )

#ifndef template_args
#define template_args template <typename ...args_t>
#endif

namespace Console
{
	//__forceinline void SetupConsole()
	//{
	//	BlockInput(true);

	//	const auto console_out = GetStdHandle(STD_OUTPUT_HANDLE);
	//	const auto console_in = GetStdHandle(STD_INPUT_HANDLE);
	//	const auto console_wnd = GetConsoleWindow();
	//	const auto console_coord = COORD{ static_cast<SHORT>(1), static_cast<SHORT>(1) };

	//	LONG style = GetWindowLong(console_wnd, GWL_STYLE);
	//	style &= ~(WS_BORDER | WS_CAPTION | WS_THICKFRAME);
	//	SetWindowLong(console_wnd, GWL_STYLE, style);

	//	DWORD console_mode;

	//	CONSOLE_CURSOR_INFO cci;
	//	cci.dwSize = 1;
	//	cci.bVisible = true;
	//	SetConsoleCursorInfo(console_out, &cci);

	//	auto cfi = CONSOLE_FONT_INFOEX
	//	{
	//		sizeof(CONSOLE_FONT_INFOEX), // cbSize
	//		0,                             // nFont
	//		{ 0, 14 },                     // dwFontSize
	//		FF_MODERN,                     // FontFamily
	//		FW_MEDIUM,                     // FontWeight
	//		L"Unispace"                    // FaceName
	//	};

	//	SetConsoleOutputCP(CP_UTF8);
	//	SetConsoleCP(CP_UTF8);

	//	//SetCurrentConsoleFontEx(console_out, FALSE, &cfi);
	//	SetWindowLong(console_wnd, GWL_STYLE, GetWindowLong(console_wnd, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
	//	EnableScrollBar(console_wnd, 0, 0);
	//	ShowScrollBar(console_wnd, SB_BOTH, FALSE);

	//	SetLayeredWindowAttributes(console_wnd, NULL, 240, LWA_ALPHA);

	//	GetConsoleMode(console_in, &console_mode);
	//	SetConsoleMode(console_in, ENABLE_EXTENDED_FLAGS | (console_mode & ~ENABLE_QUICK_EDIT_MODE));

	//	SetConsoleMode(console_out, 0x7);

	//	SetConsoleTitle("");

	//	BlockInput(false);
	//}

	template_args __forceinline void PrintCategory(const std::string_view project_name)
	{
		std::string pre{ "\n" }; for (auto idx = 0u; idx < 2; idx++) { pre += std::string{ " " }; }

		fmt::print(pre);
		fmt::print(fg(fmt::color::hot_pink) | fmt::emphasis::underline | fmt::emphasis::bold, project_name.data());
		fmt::print("\n\n");
	}

	__forceinline void ResetConsole()
	{
		system(("cls"));
		PrintCategory("Diminsh.VIP");
		fmt::print(("\n\n"));
	}

	template_args __forceinline void print(const std::string_view format, args_t... args)
	{
		fmt::print(" [ ");
		fmt::print(fg(fmt::color::yellow), "--");
		fmt::print(" ] ");
		fmt::print(format.data(), args...);
	}
	template_args __forceinline void print(const std::wstring_view format, args_t... args)
	{
		fmt::print(" [ ");
		fmt::print(fg(fmt::color::yellow), "--");
		fmt::print(" ] ");
		fmt::print(format.data(), args...);
	}

	template_args __forceinline void PrintInfo(const std::string_view format, args_t... args)
	{
		fmt::print(" [ ");
		fmt::print(fg(fmt::color::deep_sky_blue), "->");
		fmt::print(" ] ");
		fmt::print(format.data(), args...);
	}
	template_args __forceinline void PrintInfo(const std::wstring_view format, args_t... args)
	{
		fmt::print(" [ ");
		fmt::print(fg(fmt::color::deep_sky_blue), "->");
		fmt::print(" ] ");
		fmt::print(format.data(), args...);
	}

	template_args __forceinline void PrintOk(const std::string_view format, args_t... args)
	{
		fmt::print(" [ ");
		fmt::print(fg(fmt::color::lime_green), "Ok");
		fmt::print(" ] ");
		fmt::print(format.data(), args...);
	}
	template_args __forceinline void PrintOk(const std::wstring_view format, args_t... args)
	{
		fmt::print(" [ ");
		fmt::print(fg(fmt::color::lime_green), "Ok");
		fmt::print(" ] ");
		fmt::print(format.data(), args...);
	}

	template_args __forceinline void PrintFail(const std::string_view format, args_t... args)
	{
		fmt::print(" [");
		fmt::print(fg(fmt::color::red), "Fail");
		fmt::print("] ");
		fmt::print(format.data(), args...);
	}
	template_args __forceinline void PrintFail(const std::wstring_view format, args_t... args)
	{
		fmt::print(" [");
		fmt::print(fg(fmt::color::red), "Fail");
		fmt::print("] ");
		fmt::print(format.data(), args...);
	}

	template_args __forceinline void PrintStatus(const std::string_view format, bool status, args_t... args)
	{
		fmt::print(" [");
		fmt::print(fg(status ? fmt::color::green : fmt::color::red), status ? "On" : "Off");
		fmt::print("] ");
		fmt::print(format.data(), args...);
	}
	template_args __forceinline void PrintStatus(const std::wstring_view format, bool status, args_t... args)
	{
		fmt::print(" [");
		fmt::print(fg(status ? fmt::color::green : fmt::color::red), status ? "On" : "Off");
		fmt::print("] ");
		fmt::print(format.data(), args...);
	}

}

#endif