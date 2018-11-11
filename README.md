# my-learnopengl-playground

Moja praca inżynierska, na razie w zasadzie tutorial na podstawie https://learnopengl.com/ ,
tylko że z architekturą budowaną pod przyszłą grę (na razie to trochę overengineering).

# Budowanie

Budowanie napisane w `cmake`, więc teoretycznie powinno działać wszędzie, ale testowane przeze mnie
tylko pod `Visual Studio 2017`, statyczne linkowanie, z użyciem `vcpkg` do pobierania zależności.

Obecne zależności:
* `SDL2`

Nieobowiązkowe:
* `Visual Leak Detector` (https://kinddragon.github.io/vld/) do raportowania wycieków pamięci po zakończeniu programu

Póki co wykrywane są tylko 2-3 leaki wynikające z używania biblioteki SDL2.

Niektóre zależności są budowane bezpośrednio z projektem, ale wypisuję je dla porządku:
* `glad`
* `plog` (https://github.com/SergiusTheBest/plog)

# Licencja

Wszelkie prawa zastrzeżone.

## Zmodyfikowane zależności

Pliki źródłowe `plog` zostały nieznacznie zmodyfikowane, aby plugin `ReSharper C++` nie zgłaszał nieprawdziwych błędów.
