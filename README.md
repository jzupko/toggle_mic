# Toggle Mic
Super simple utility to toggle the mute status of the default communications capture device on Windows.

# How to Build
* Install [Visual Studio](https://visualstudio.microsoft.com/vs/community/).
* Launch an `x64 Native Tools Command Prompt` (launch `vcvars64.bat`).
* Run `build.bat`. A Console command-line utility will be written to `bin\Windows\Release\x64\toggle_mic.exe`.
  * Alternatively, run `build.bat /DTOGGLE_MIC_NO_CONSOLE=1` to compile as a Windows subsystem GUI application with no Window. Atypical case that may be useful for some integration scenarios.

# How to Use
* Just run `toggle_mic.exe`. If successful, it will toggle the mute state of your default capture (communications) device. On failure, an error will be written to the console (unless compiled with `/DTOGGLE_MIC_NO_CONSOLE=1`).
