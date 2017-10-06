/*
	SLEA 2:
	  Skip Launcher & ENB Auto

	Summary of program functionality:
	->main()
	->get strings from ini file
	->terminate if no executable was specified
	->get executable folder from argv[0] - filename
	->construct commandline through vector<char> for CreateProcess()
	->run executable, terminate if failed
	->quit if no injector was specified
	->run injector, terminate if failed
	->wait for executable to close
	->terminate injector
	->quit
*/

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>

#include <string>
#include <sstream>
#include <vector>
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

	string exe_name = config.Get("General", "ExecutableName");
	string cmdline = config.Get("General", "CommandLine");
	string inj_name = config.Get("General", "InjectorName");

	// Terminate if ExecutableName is empty
	if (exe_name.empty())
		return -1;

	// Let's get the directory through the
	// first argument and removing the filename
	string folder = argv[0];
	folder.erase(folder.rfind('\\') + 1);

	// We'll be using this to construct the
	// commandline for the executable
	vector<char> final_cmdline;

	// Pass first argument (path)
	for (const auto& c : folder)
		final_cmdline.push_back(c);
	for (const auto& c : exe_name)
		final_cmdline.push_back(c);
	final_cmdline.push_back(' ');

	// Passthrough arguments
	for (int i = 1; i < argc; ++i) {
		for (int c = 0; argv[i][c] != 0; ++c)
			final_cmdline.push_back(argv[i][c]);

		final_cmdline.push_back(' ');
	}

	// Pass arguments specified in file
	for (const auto& c : cmdline)
		final_cmdline.push_back(c);
	
	// Null-terminate commandline
	final_cmdline.push_back(0);

	STARTUPINFO si_inj;
	PROCESS_INFORMATION pi_inj;
	bool using_injector = false;

	// Run injector if one is specified
	if (!inj_name.empty()) {
		using_injector = true;

		si_inj = { sizeof(si_inj) };
		if (!CreateProcess(
			inj_name.c_str(),
			0, 0, 0, false,
			0, 0, 0,
			&si_inj,
			&pi_inj
		)) return -3;
	}

	STARTUPINFO si_exe = { sizeof(si_exe) };
	PROCESS_INFORMATION pi_exe;

	// Run executable
	if (!CreateProcess(
		exe_name.c_str(),
		&final_cmdline[0],
		0, 0, false,
		0, 0, 0,
		&si_exe,
		&pi_exe
	)) return -2;

	if (using_injector) {
		// Wait for executable to close
		WaitForSingleObject(pi_exe.hProcess, INFINITE);
		CloseHandle(pi_exe.hProcess);
		CloseHandle(pi_exe.hThread);
		
		// And close the injector
		TerminateProcess(pi_inj.hProcess, 0);
		CloseHandle(pi_inj.hProcess);
		CloseHandle(pi_inj.hThread);
	} else {
		CloseHandle(pi_exe.hProcess);
		CloseHandle(pi_exe.hThread);
	}

	return 0;
}
