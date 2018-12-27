# Tactics Game

Praca inżynierska pt. "Gra turowa na trójwymiarowej mapie zaimplementowana z wykorzystaniem OpenGL".

# Budowanie

Potrzebny jest [`cmake`](https://cmake.org/) oraz wypisane niżej zależności. Zalecany sposób pobrania zależności poprzez użycie [`vcpkg`](https://github.com/Microsoft/vcpkg).
Na Windowsie 10 z Microsoft Visual Studio 2017 na pewno działa statyczne linkowanie. Dynamiczne nie było testowane.
Grę da się skompilować na Linuksie `Ubuntu LTS 18.04`. Po poprawnej kompilacji można wygenerować plik [AppImage](https://appimage.org/) skryptem `linux_deploy.sh` zakładając, że pliki zostały skompilowane do folderu `build`.

Obecne zależności:
* [`SDL2`](https://www.libsdl.org/) -- tworzenie okna, obsługa klawiatury itp.
* [`assimp`](http://www.assimp.org/) -- ładowanie modeli 3D
* [`zlib`](https://zlib.net/) -- zależność `assimp`

Nieobowiązkowe:
* [`Visual Leak Detector`](https://kinddragon.github.io/vld/) do raportowania wycieków pamięci po zakończeniu programu

Póki co wykrywane są tylko 2-3 leaki wynikające z używania biblioteki SDL2.

Niektóre zależności są budowane bezpośrednio z projektem:
* [`glad`](https://glad.dav1d.de/) -- funkcje OpenGL
* [`plog`](https://github.com/SergiusTheBest/plog) -- logowanie
* [`glm`](https://glm.g-truc.net/0.9.9/index.html) -- operacje matematyczne
* [`irrxml`](https://www.ambiera.com/irrxml/) -- zależność `assimp`
* [`nlohmann-json`](https://github.com/nlohmann/json) -- obsługa plików JSON
* [`args`](https://github.com/Taywee/args) -- obsługa argumentów do linii poleceń
* [`boxer`](https://github.com/aaronmjacobs/Boxer) -- obsługa prostych okien dialogowych

## Zmodyfikowane zależności

Pliki źródłowe `plog` zostały nieznacznie zmodyfikowane, aby plugin `ReSharper C++` nie zgłaszał nieprawdziwych błędów.
Używana rewizja to [95d28c24d102973e0cb6b7f6084144a7401b5671](https://github.com/SergiusTheBest/plog/commit/95d28c24d102973e0cb6b7f6084144a7401b5671).
Zmiany są następujące:

```
$ diff ../../../../Desktop/plog-master/include/plog plog/
Common subdirectories: ../../../../Desktop/plog-master/include/plog/Appenders and plog/Appenders
Common subdirectories: ../../../../Desktop/plog-master/include/plog/Converters and plog/Converters
Common subdirectories: ../../../../Desktop/plog-master/include/plog/Formatters and plog/Formatters
diff ../../../../Desktop/plog-master/include/plog/Log.h plog/Log.h
13c13
< #if defined(_MSC_VER) && _MSC_VER >= 1600 && !defined(__INTELLISENSE__) && !defined(__INTEL_COMPILER) && !defined(__llvm__) // >= Visual Studio 2010, skip IntelliSense, Intel Compiler and Clang Code Model
---
> #if defined(_MSC_VER) && _MSC_VER >= 1600 && !defined(__INTELLISENSE__) && !defined(__INTEL_COMPILER) && !defined(__llvm__) && !defined(__RESHARPER__) // >= Visual Studio 2010, skip IntelliSense, Intel Compiler and Clang Code Model
```
