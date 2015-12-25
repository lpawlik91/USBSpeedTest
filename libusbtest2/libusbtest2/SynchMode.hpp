#pragma once

#include "Mode.hpp"

/**
 * Klasa ma zadanie zmierzyæ czas przesy³u danych metod¹ synchroniczn¹.
 * Klasa korzysta w wiêkszoœci z domyœlnej implementacji metod inicjalizacyjnych (oraz koñcz¹cych test).
 */
class SynchMode : public Mode
{
public:
    /** 
	 * Konstruktor maj¹cy za zadanie wprowadzenie podstawowych ustawieñ (przekazanie ich klasie nadrzednej).
	 * \param bufforSize - rozmiar buforu jako liczba calkowita
	 * \param count - ilosc powtórzeñ z jakimi bufor ma zostaæ wys³ana (iloœæ danych == count x bufforSize)
	 * \param vid - vendor id, czyli jeden z parametrów identyfikacji pod³¹czonego do USB urz¹dzenia (a dok³adniej identyfikator sprzedawcy/producenta)
	 * \param pid - product id, drugi z parametrów identyfikacji urz¹dzenia
	 * \param printOnlyResult - flaga okreœlaj¹ca czy na wyjœciu maj¹ zostaæ wypisane wyniki w postaci umo¿liwiaj¹cej tworzenie wykresów czy czytelniejszej dla u¿ytkownika
	 */
	SynchMode(int bufforSize, unsigned count, int vid, int pid, bool printOnlyResult);
	/**
	 * Domyœlny destruktor
	 */
	virtual ~SynchMode();
	/**
	 * Metoda maj¹ca za zadanie zmierzenie czasu przesy³u danych metod¹ synchroniczn¹.
	 */
	virtual int doTest() override;

private:

};

