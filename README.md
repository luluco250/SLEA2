# SLEA2
***Skip Launcher &amp; ENB Auto 2***

This is a small program that can be used to trick some games into skipping their launchers and optionally open/close a program like ENB injector automatically.

To use, place it in the same directory as the game's executable and run it at least once to generate a configurations file called "slea.ini":
- Set "ExecutableName" to the name of the executable you want to run when SLEA is executed.
- Anything in "CommandLine" will be passed to the game as command line arguments. You can also pass them to the SLEA executable and it'll pass them through as well.
- If "InjectorName" isn't empty, SLEA will run it, wait for the game to terminate and then close the injector automatically.

An example usage for The Elder Scrolls IV: Oblivion on Steam would be:
1. Rename "OblivionLauncher.exe" to something else like "OblivionLauncher_REAL.exe".
2. Copy "slea.exe" to the game's directory and rename it as "OblivionLauncher.exe".
3. Set "ExecutableName" to "Oblivion.exe" (no quotes).
4. Optionally set "InjectorName" to "ENBInjector.exe" (again, no quotes).
5. Now the game launcher will be completely skipped when you launch it through Steam!

**Tested on Morrowind, Oblivion and Fallout New Vegas.**
