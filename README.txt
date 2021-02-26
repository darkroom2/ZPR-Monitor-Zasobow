po uruchomieniu programu nalezy podac liczbe aby uruchomic watki:
* _pollThread - który odpytuje ResProvider'a o zuzycie zasobow
    oraz wrzuca reprezentacje uzycia zasobow do kontenera wspoldzielonego,
* _conditionThread - który sprawdza czy warunki powaidomienia są spełnione
    i jesli tak to szuka winowajcy oraz po jego znalezieniu przekazuje do ServerManagera


aby zakonczyc program nalezy podac stringa lub znak nonAlpha.

ConditionManager::conditionMet() ustawiajac w returnie funkcji odpowiednia wartosc symulujemy dzialanie programu

Po zakonczeniu dzialania programu, w destruktorze klasy (po zakonczeniu ww. watkow)
    wypisany zostaje vector do ktorego watek _pollThread dodaje pollowane uzycia (a watek _conditionThread sprawdza te wartosci na spelnienie warunku)
    (docelowo container wspoldzielony) << to w celach testowych