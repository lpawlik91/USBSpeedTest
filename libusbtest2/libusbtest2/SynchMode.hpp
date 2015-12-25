#pragma once

#include "Mode.hpp"

/**
 * Klasa ma zadanie zmierzy� czas przesy�u danych metod� synchroniczn�.
 * Klasa korzysta w wi�kszo�ci z domy�lnej implementacji metod inicjalizacyjnych (oraz ko�cz�cych test).
 */
class SynchMode : public Mode
{
public:
    /** 
	 * Konstruktor maj�cy za zadanie wprowadzenie podstawowych ustawie� (przekazanie ich klasie nadrzednej).
	 * \param bufforSize - rozmiar buforu jako liczba calkowita
	 * \param count - ilosc powt�rze� z jakimi bufor ma zosta� wys�ana (ilo�� danych == count x bufforSize)
	 * \param vid - vendor id, czyli jeden z parametr�w identyfikacji pod��czonego do USB urz�dzenia (a dok�adniej identyfikator sprzedawcy/producenta)
	 * \param pid - product id, drugi z parametr�w identyfikacji urz�dzenia
	 * \param printOnlyResult - flaga okre�laj�ca czy na wyj�ciu maj� zosta� wypisane wyniki w postaci umo�liwiaj�cej tworzenie wykres�w czy czytelniejszej dla u�ytkownika
	 */
	SynchMode(int bufforSize, unsigned count, int vid, int pid, bool printOnlyResult);
	/**
	 * Domy�lny destruktor
	 */
	virtual ~SynchMode();
	/**
	 * Metoda maj�ca za zadanie zmierzenie czasu przesy�u danych metod� synchroniczn�.
	 */
	virtual int doTest() override;

private:

};

