/*
	SLEA 2.1:
	  Skip Launcher & ENBAuto
*/

#define NOMINMAX
//#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>

#include <string>
#include <sstream>
#include <fstream>

#include "INI.hpp"

using namespace std;
using namespace slea;

int main(int argc, char** argv) {
	INI config("slea.ini");
	if (!config) {
		ofstream file("slea.ini");
		file << "[General]\n"
		        "ExecutableName=\n"
		        "CommandLine=\n"
		        "InjectorName=\n";
		return 1;
	}

	string exe_name = config["General"]["ExecutableName"];
	string cmdline = config["General"]["CommandLine"];
	string inj_name = config["General"]["InjectorName"];

	// Terminate if ExecutableName is empty
	if (exe_name.empty())
		return -1;

	// Let's construct the command line arguments
	stringstream cmdline_ss;
	
	// Passthrough the arguments given to SLEA
	for (int i = 1; i < argc; ++i)
		cmdline_ss << argv[i] << ' ';

	// Pass the arguments in the ini file
	cmdline_ss << cmdline;

	string cmdline_str = cmdline_ss.str();

	SHELLEXECUTEINFO inj_sei;
	bool using_injector = false;

	// Run injector if one is specified
	if (!inj_name.empty()) {
		using_injector = true;

		inj_sei = { sizeof(inj_sei) };
		inj_sei.fMask = SEE_MASK_NOCLOSEPROCESS;
		inj_sei.lpFile = inj_name.c_str();
		inj_sei.nShow = SW_SHOW;
		
		if (!ShellExecuteEx(&inj_sei)) {
			return -3;
		}
	}

	// Run executable
	SHELLEXECUTEINFO exe_sei = { sizeof(exe_sei) };
	exe_sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	exe_sei.lpFile = exe_name.c_str();
	exe_sei.nShow = SW_SHOW;
	exe_sei.lpParameters = cmdline_str.c_str();
	
	if (!ShellExecuteEx(&exe_sei)) {
		if (using_injector)
			TerminateProcess(inj_sei.hProcess, 0);
		return -2;
	}

	if (using_injector) {
		WaitForSingleObject(exe_sei.hProcess, INFINITE);
		TerminateProcess(inj_sei.hProcess, 0);
		CloseHandle(inj_sei.hProcess);
	}

	CloseHandle(exe_sei.hProcess);
	return 0;
}
