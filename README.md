# Tactics Game

Praca inżynierska pt. "Gra turowa na trójwymiarowej mapie zaimplementowana z wykorzystaniem OpenGL".

# Budowanie

Budowanie napisane w `cmake`, więc teoretycznie powinno działać wszędzie, ale testowane przez autora
tylko pod środowiskiem `Visual Studio 2017`, statycznym linkowaniem, z użyciem `vcpkg` do pobierania zależności.

Obecne zależności:
* `SDL2` -- tworzenie okna, obsługa klawiatury itp.
* `assimp` -- ładowanie modeli 3D
* `zlib` -- zależność `assimp`

Nieobowiązkowe:
* `Visual Leak Detector` (https://kinddragon.github.io/vld/) do raportowania wycieków pamięci po zakończeniu programu

Póki co wykrywane są tylko 2-3 leaki wynikające z używania biblioteki SDL2.

Niektóre zależności są budowane bezpośrednio z projektem:
* `glad` -- funkcje OpenGL
* `plog` -- logowanie
* `glm` -- operacje matematyczne
* `irrxml` -- zależność `assimp`
* `nlohmann-json` -- obsługa plików JSON

## Zmodyfikowane zależności

Pliki źródłowe `plog` zostały nieznacznie zmodyfikowane, aby plugin `ReSharper C++` nie zgłaszał nieprawdziwych błędów.
