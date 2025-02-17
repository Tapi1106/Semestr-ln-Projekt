#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem> // Pro vytvo�en� adres��e (od C++17)
#include <chrono> // Pro m��en� �asu
#include <ctime> // Pro z�sk�n� aktu�ln�ho datumu

int main() {
    // Otev�en� webkamery (0 je v�choz� kamera)
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) //Kontrola zda se kamera otev�ela
    {
        std::cerr << "Chyba: Nelze otev��t webkameru" << std::endl;
        return -1;
    }

    // Nastaven� cesty k v�stupn�mu adres��i pro zaznamen�v�n� videa
    std::string outputDir = "C:/Users/zen/Desktop/WebKamera/Zaznam";
    if (!std::filesystem::exists(outputDir))  // Pokud neexistuje
    {
        std::filesystem::create_directories(outputDir); // Vytvo�en� adres��e pokud nen�
    }

    // Nastaven� parametr� pro z�znam videa
    cv::Mat frame; // Sn�mek z webkamery
    cv::VideoWriter videoWriter; // Objekt pro z�znam videa
    bool recording = false; // P��znak zda se nahr�v�
    int frameWidth = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH)); // ���ka sn�mku
    int frameHeight = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT)); // V��ka sn�mku
    double fps = 12.5; // Nastaven� FPS na 12.5 pro zpomalen� z�znamu

    auto startTime = std::chrono::steady_clock::time_point(); // �as zah�jen� nahr�v�n�

    bool showDot = true; // P��znak zda se m� zobrazit te�ka kter� signalizuje �e se nahr�v� zaznam
    auto lastBlinkTime = std::chrono::steady_clock::now(); // Pro ��zen� blik�n� te�ky

	while (true) // Smy�ka pro zachycen� sn�mk� a zobrazen� sn�mk� z webkamery
    {
        // Zachycen� sn�mku z webkamery
        cap >> frame;
		if (frame.empty()) // Kontrola jestli nen� sn�mek pr�zdny
        {
            std::cerr << "Chyba: Nelze zachytit sn�mek" << std::endl;
            break;
        }

        // Z�sk�n� aktu�ln�ho datumu
		auto now = std::chrono::system_clock::now(); // Aktu�ln� �as za pomoc� (auto now)
        std::time_t now_c = std::chrono::system_clock::to_time_t(now); // P�evod na �asov� typ
        std::tm now_tm; // deklaruje prom�nnou kter� je strukturovan�m jako pole pro rok, m�s�c, den, hodiny, minuty, sekundy

        // P�evod na strukturu s �asem
#ifdef _WIN32
        localtime_s(&now_tm, &now_c);

#endif

        // Textov� �et�zec pro ulo�en� data
        char dateText[20]; 
		std::strftime(dateText, sizeof(dateText), "%Y-%m-%d", &now_tm); // "%Y-%m-%d" = rok-m�s�c-den

        // Zobrazen� aktu�ln�ho datumu v lev�m doln�m rohu
        cv::putText(frame, dateText, cv::Point(10, frameHeight - 20),
            cv::FONT_HERSHEY_SIMPLEX, 0.25, cv::Scalar(255, 255, 255), 1); // Zv�t�eno o 1/4 a p�esunuto v�ce do lev�ho doln�ho rohu

        
        if (recording)
        {
            auto elapsedTime = std::chrono::steady_clock::now() - startTime; // �asova� od zah�jen� nahr�v�n�
            int seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsedTime).count(); // P�evod na sekundy
            int minutes = seconds / 60;
            seconds %= 60; // Zbytek po d�len� 60
            std::string timeText = cv::format("%02d:%02d", minutes, seconds); // Form�tov�n� �asu na minutu:sekundy
            cv::putText(frame, timeText, cv::Point(frameWidth - 80, frameHeight - 10),
                cv::FONT_HERSHEY_SIMPLEX, 0.33, cv::Scalar(0, 255, 0), 1); // Zv�t�eno o 1/3 a p�esunuto v�ce do prav�ho doln�ho rohu
        }

        // Pokud nahr�v�me, zapisujeme sn�mek do souboru
        if (recording) {
            videoWriter.write(frame); // Z�pis sn�mku do videa

            // Blikaj�c� �erven� te�ka v lev�m horn�m rohu
            auto currentTime = std::chrono::steady_clock::now(); // Aktu�ln� �as
			if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastBlinkTime).count() > 500) // Zjisruje zda ubehlo 500msod posledniho bliknuti, a pokud ano p�epne stav te�ky
            {
                showDot = !showDot; // P�epnut� stavu te�ky
                lastBlinkTime = currentTime;    // Ulo�en� �asu posledn�ho bliknut�
            }
            if (showDot)
            {
				cv::circle(frame, cv::Point(20, 20), 4, cv::Scalar(0, 0, 255), -1); // Zji�tuje zda se m� zobrazit te�ka a pokud ano zobraz� ji
            }
        }

        // Zobrazen� sn�mku v okn�
        cv::imshow("Webkamera", frame);

        // Z�sk�n� stisknut� kl�vesy
        char key = static_cast<char>(cv::waitKey(30));

        if (key == 'q') {
            // Ukon�en� programu
            break;
        }
        else if (key == 'x') 
        {
            // P�epnut� stavu nahr�v�n�
            if (recording)  
            {
                // Spu�t�n� nahr�v�n�
                int fileIndex = 0;
                std::string fileName; // N�zev souboru
                do
                {
                    fileName = outputDir + "/zaznam_" + std::to_string(fileIndex++) + ".avi"; // N�zev souboru se zvet�ov�n�m ��sla u ka�deho nov�ho zaznamu
                }
                while (std::filesystem::exists(fileName)); // Kontrola zda soubor ji� existuje

                videoWriter.open(fileName, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, cv::Size(frameWidth, frameHeight)); // Otev�en� souboru pro z�pis
                if (!videoWriter.isOpened())  //
                {
                    std::cerr << "Chyba: Nelze vytvo�it video soubor" << std::endl;
                }

                else
                {
                    std::cout << "Nahr�v�n� zah�jeno. Ukl�d� se do: " << fileName << std::endl;
                    recording = true;
                    startTime = std::chrono::steady_clock::now(); // Ulo�en� �asu zah�jen� nahr�v�n�
                }
            }
			else
            {
                // Zastaven� nahr�v�n�
                std::cout << "Nahr�v�n� zastaveno." << std::endl;
                recording = false;
                videoWriter.release();
            }
        }
    }

    // Uvoln�n� zdroj�
    cap.release();
    videoWriter.release();
    cv::destroyAllWindows();

    return 0;
}
