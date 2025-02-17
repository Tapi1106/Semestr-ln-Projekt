# Semestr-ln-Projekt
Rozhraní pro kolimátor



# Program Webkamera 
- Slouží na zobrazení a zachycení daného pohledu z webkamery
- Program také disponuje zachycení pohledu do videa do předem stanovené složky

# Pro zprovoznění Webkamera
1) Je potřeba implementovat knihovnu OpenCV
   - Je potřeba stáhnout přiložený soubor opencv-4.10.0-windows
   - Poté ve Visual Studio 2022 ve vlastnostech projektu v Adresáře VC++ nastavit v Adresáře souborů k zahrnutí cestu k souborům které jsme extrahovali z předešlého souboru tedy ...\opencv\build\include
   - Dále nastavit v Adresáři knihoven cestu k ...\opencv\build\x64\vc16\lib
   - Poté v Linker > Vstup vložit soubor opencv_world4100d.lib do Dalších závislostí
   - jako poslední musíme změnit v C/C++ > Jazyk Standardu jazyka C++ na Standard ISO C++17 (/std:c++17) aby nám fungoval #include <filesystem>

# Do budoucna
- Přidání rozpoznávání postav 
- Převést daný kód podle hardwaru na jakém od bude pracovat


# Program rozhraní
- Slouží k nastavení míření, a to jak barevně tak i velikostí a tvarem

# ,
Pro zprovoznění Rozhraní
1) Je potřeba implementovat knihovnu ImuGui
   - Je potřeba stáhnout pokud (již není zahrnutá v projektu) složku ImuGui a přidat ji do složky programu a poté do projektu zahrnout v Imugui > ImGui imgui.cpp, imgui.h, imgui_demo.cpp, imgui_draw.cpp, imgui_internal.h, imgui_tables.cpp, imgui_widgets.cpp,
     imstb_rectpack.h, imstb_textedit.h, imstb_truetype.h
   - Dále zahrnout v Imugui > ImGui > backends soubory imgui_impl_dx9.cpp, imgui_impl_dx9.h, imgui_impl_win32.cpp, imgui_impl_win32.h
   - Poté stáhnout subory kolimator_1.h, kolimator_2.h, kolimator_3.h, kolimator_4.h a vloit je do složky projektu a zahrnout je ( pokud již nejsou zahrnuté v projektu)
   - Jak další krok ve vlastnostech projektu v Adresáře VC++ nastavit v Adresáře souborů k zahrnutí $(ProjectDir)ImuGui\ImGui\backends, $(ProjectDir)ImuGui\ImGui ,$(ProjectDir)ImGui\ImGui\backends, $(DXSDK_DIR)Include
   - Poté do Administrátora Příkazového řádku vložit MsiExec.exe /passive /X{F0C3E5D1-1ADE-321E-8167-68EF0DE699A5} a MsiExec.exe /passive /X{1D8E6291-B0D5-35EC-8441-6616F567A0F7}
   - Po tomto kroku stáhnou a naistalovat https://www.microsoft.com/en-us/download/details.aspx?id=6812 
   - Po naistalování tohoto souboru stanout a naistalovat https://www.microsoft.com/en-us/download/details.aspx?id=26999
   - Jak další krok ve vlastnostech projektu v Adresáře VC++ nastavit v Adresářích knhoven $(DXSDK_DIR)Lib\x64
   - Posledním krokem je v Linker > Vstup vložit soubor d3dx9.lib, d3d9.lib do Dalších závislostí

# Do budoucna
- Převést daný kód podle hardwaru na jakém od bude pracovat a pořípadě převést do OpenCV
