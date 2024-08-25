# Loulou's DLL Injector

**LDI** is a straightforward Windows application that allows you to inject a DLL (Dynamic Link Library) into a running process. Whether you’re a developer testing your code or someone looking to modify an application, this tool provides an easy way to accomplish that task. ***LDI uses the Win32 API, which makes this only compatible with Windows.***

*I made a template from the DLL that was used while programming that tool so you cna compile your own one! Just [click here](https://github.com/LoulouNoLegend/DLL-Template)*

## How to Use (2 methods)

### Compiling it yourself
1. **Download the Source Code**: Get the latest version from [GitHub](https://github.com/LoulouNoLegend/LD-Injector).
2. **Build the Application**: Compile the source code using a C++ compiler on Windows. Ensure you have the necessary Windows SDK libraries.
3. **Run the Apps**: Launch the compiled application and the app you want your DLL to be in.
4. **Follow the Prompts**: Use the provided dialogs to select your executable and DLL files.
5. **Enjoy!**

### Downloading the app itself
1. **Download the zip file**: [Open the release tab](https://github.com/LoulouNoLegend/LD-Injector/releases/) and scroll a little bit to find the files
2. **Unzip the executable**: Open the zip file and extract the executable somewhere on your pc
3. **Open the executable and enjoy!**

## What You Need

- **Windows OS**
- **Visual Studio Community** *(Made with VSC 2022)*

## Important Notes

- **Permissions**: You may need administrative rights to inject DLLs into certain processes.
- **Use Wisely**: DLL injection can affect your system. Don't inject any random DLL and be sure to know what you're doing.
- **I'm not responsible for any problems related to injecting an unsafe DLL. Be sure that you can trust the DLL you inject.**

## License

[Loulou's DLL Injector](https://github.com/LoulouNoLegend/LDI-DLL-Injector) © 2024 by [LoulouNoLegend](https://github.com/LoulouNoLegend) is licensed under [Creative Commons Attribution 4.0 International](https://creativecommons.org/licenses/by/4.0/?ref=chooser-v1)
Check out the [LICENSE](https://github.com/LoulouNoLegend/LD-Injector/blob/master/LICENSE.txt) file for more details.
