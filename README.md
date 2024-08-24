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
*Soon.*

## What You Need

- **Windows OS**
- **C++ Compiler**

## Important Notes

- **Permissions**: You may need administrative rights to inject DLLs into certain processes.
- **Use Wisely**: DLL injection can affect your system. Don't inject any random DLL and be sure to know what you're doing.

## License

This project is licensed under the MIT License. Check out the [LICENSE](https://github.com/LoulouNoLegend/LD-Injector/blob/master/LICENSE.txt) file for more details.
