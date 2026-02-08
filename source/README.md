# WaMax

## MAX SDK

* C examples: https://github.com/Cycling74/max-sdk.git
* C++ examples: https://github.com/Cycling74/min-devkit

## wa cmake package

put the content of https://github.com/xavierxeon-helper/CMakePackages.git
in your CMAKE_PREFIX_PATH

## error

### qlocale.h
```
QString toString(ulong number, int fieldWidth, char32_t fillChar) const
```

## windows compile

hint: use vscode

### setup

https://learn.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170

 * set QT_DIR=C:\dev\Qt\6.8.0\msvc2022_64
 * set CMAKE_PREFIX_PATH=%QT_DIR%\lib\cmake

### build from visual studio command line

```
cmake ..
cmake --build . --target ALL_BUILD --config Release -- /nologo /verbosity:minimal /maxcpucount
```

## debug 

### qtcreator
exe: /Applications/Max.app/Contents/MacOS/Max

### vscode
install CMAKE extension