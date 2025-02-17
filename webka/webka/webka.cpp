#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem> // Pro vytvoøení adresáøe (od C++17)
#include <chrono> // Pro mìøení èasu
#include <ctime> // Pro získání aktuálního datumu

int main() {
    // Otevøení webkamery (0 je výchozí kamera)
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) //Kontrola zda se kamera otevøela
    {
        std::cerr << "Chyba: Nelze otevøít webkameru" << std::endl;
        return -1;
    }

    // Nastavení cesty k výstupnímu adresáøi pro zaznamenávání videa
    std::string outputDir = "C:/Users/zen/Desktop/WebKamera/Zaznam";
    if (!std::filesystem::exists(outputDir))  // Pokud neexistuje
    {
        std::filesystem::create_directories(outputDir); // Vytvoøení adresáøe pokud není
    }

    // Nastavení parametrù pro záznam videa
    cv::Mat frame; // Snímek z webkamery
    cv::VideoWriter videoWriter; // Objekt pro záznam videa
    bool recording = false; // Pøíznak zda se nahrává
    int frameWidth = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH)); // Šíøka snímku
    int frameHeight = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT)); // Výška snímku
    double fps = 12.5; // Nastavení FPS na 12.5 pro zpomalení záznamu

    auto startTime = std::chrono::steady_clock::time_point(); // Èas zahájení nahrávání

    bool showDot = true; // Pøíznak zda se má zobrazit teèka která signalizuje že se nahrává zaznam
    auto lastBlinkTime = std::chrono::steady_clock::now(); // Pro øízení blikání teèky

	while (true) // Smyèka pro zachycení snímkù a zobrazení snímkù z webkamery
    {
        // Zachycení snímku z webkamery
        cap >> frame;
		if (frame.empty()) // Kontrola jestli není snímek prázdny
        {
            std::cerr << "Chyba: Nelze zachytit snímek" << std::endl;
            break;
        }

        // Získání aktuálního datumu
		auto now = std::chrono::system_clock::now(); // Aktuální èas za pomocí (auto now)
        std::time_t now_c = std::chrono::system_clock::to_time_t(now); // Pøevod na èasový typ
        std::tm now_tm; // deklaruje promìnnou která je strukturovaným jako pole pro rok, mìsíc, den, hodiny, minuty, sekundy

        // Pøevod na strukturu s èasem
#ifdef _WIN32
        localtime_s(&now_tm, &now_c);

#endif

        // Textový øetìzec pro uložení data
        char dateText[20]; 
		std::strftime(dateText, sizeof(dateText), "%Y-%m-%d", &now_tm); // "%Y-%m-%d" = rok-mìsíc-den

        // Zobrazení aktuálního datumu v levém dolním rohu
        cv::putText(frame, dateText, cv::Point(10, frameHeight - 20),
            cv::FONT_HERSHEY_SIMPLEX, 0.25, cv::Scalar(255, 255, 255), 1); // Zvìtšeno o 1/4 a pøesunuto více do levého dolního rohu

        
        if (recording)
        {
            auto elapsedTime = std::chrono::steady_clock::now() - startTime; // Èasovaè od zahájení nahrávání
            int seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsedTime).count(); // Pøevod na sekundy
            int minutes = seconds / 60;
            seconds %= 60; // Zbytek po dìlení 60
            std::string timeText = cv::format("%02d:%02d", minutes, seconds); // Formátování èasu na minutu:sekundy
            cv::putText(frame, timeText, cv::Point(frameWidth - 80, frameHeight - 10),
                cv::FONT_HERSHEY_SIMPLEX, 0.33, cv::Scalar(0, 255, 0), 1); // Zvìtšeno o 1/3 a pøesunuto více do pravého dolního rohu
        }

        // Pokud nahráváme, zapisujeme snímek do souboru
        if (recording) {
            videoWriter.write(frame); // Zápis snímku do videa

            // Blikající èervená teèka v levém horním rohu
            auto currentTime = std::chrono::steady_clock::now(); // Aktuální èas
			if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastBlinkTime).count() > 500) // Zjisruje zda ubehlo 500msod posledniho bliknuti, a pokud ano pøepne stav teèky
            {
                showDot = !showDot; // Pøepnutí stavu teèky
                lastBlinkTime = currentTime;    // Uložení èasu posledního bliknutí
            }
            if (showDot)
            {
				cv::circle(frame, cv::Point(20, 20), 4, cv::Scalar(0, 0, 255), -1); // Zjištuje zda se má zobrazit teèka a pokud ano zobrazí ji
            }
        }

        // Zobrazení snímku v oknì
        cv::imshow("Webkamera", frame);

        // Získání stisknuté klávesy
        char key = static_cast<char>(cv::waitKey(30));

        if (key == 'q') {
            // Ukonèení programu
            break;
        }
        else if (key == 'x') 
        {
            // Pøepnutí stavu nahrávání
            if (recording)  
            {
                // Spuštìní nahrávání
                int fileIndex = 0;
                std::string fileName; // Název souboru
                do
                {
                    fileName = outputDir + "/zaznam_" + std::to_string(fileIndex++) + ".avi"; // Název souboru se zvetšováním èísla u každeho nového zaznamu
                }
                while (std::filesystem::exists(fileName)); // Kontrola zda soubor již existuje

                videoWriter.open(fileName, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, cv::Size(frameWidth, frameHeight)); // Otevøení souboru pro zápis
                if (!videoWriter.isOpened())  //
                {
                    std::cerr << "Chyba: Nelze vytvoøit video soubor" << std::endl;
                }

                else
                {
                    std::cout << "Nahrávání zahájeno. Ukládá se do: " << fileName << std::endl;
                    recording = true;
                    startTime = std::chrono::steady_clock::now(); // Uložení èasu zahájení nahrávání
                }
            }
			else
            {
                // Zastavení nahrávání
                std::cout << "Nahrávání zastaveno." << std::endl;
                recording = false;
                videoWriter.release();
            }
        }
    }

    // Uvolnìní zdrojù
    cap.release();
    videoWriter.release();
    cv::destroyAllWindows();

    return 0;
}
