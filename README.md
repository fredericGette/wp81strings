# wp81strings

Attempt to create an equivalent of the [Linux strings command](https://en.wikipedia.org/wiki/Strings_(Unix)) or the [Sysinternals command](https://learn.microsoft.com/en-us/sysinternals/downloads/strings) for Windows Phone 8.1  

## Usage

![usage](Capture01.PNG)

![result](Capture02.PNG)
The strings are displayed between brackets '[ ]'.  
The number, before a string, corresponds to the distance in bytes between this string and the previous.  
The suffix 'w' marks a [wide-character](https://learn.microsoft.com/en-us/windows/win32/midl/wchar-t) string.

## Requirements

### Deployment

- [Install a telnet server on the phone](/wp81documentation/telnetOverUsb/README.md), in order to run the application.  
- Manually copy the executable in the shared folder of the phone. This folder is mounted in `C:\Data\USERS\Public\Documents`.  

### Compilation

- [Visual Studio 2015 with the Windows Phone SDK](/wp81documentation/ConsoleApplicationBuilding/README.md).
