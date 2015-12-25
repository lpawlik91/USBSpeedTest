#pragma once

#include "DebugPrinter.hpp"
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include "libusb.h"

/** 
 * Klasa Mode zawieraj¹ca podstawow¹ implementacjê metod inicjalizacyjnych oraz koñcz¹cych przesy³anie danych.
 * Jej to klasa nadrzêdna dla poszczególnych metod przesy³u danych, zawiera czysto wirtualn¹ metodê doTest().
 * Ka¿d¹ prost¹ implementacjê inicjalizacji mo¿na dowolnie rozwijaæ w klasie pochodnej.
 */
class Mode
{
public:
	/** 
	 * Konstruktor maj¹cy za zadanie wprowadzenie podstawowych ustawieñ.
	 * \param bufforSize - rozmiar buforu jako liczba calkowita
	 * \param count - ilosc powtórzeñ z jakimi bufor ma zostaæ wys³ana (iloœæ danych == count x bufforSize)
	 * \param vid - vendor id, czyli jeden z parametrów identyfikacji pod³¹czonego do USB urz¹dzenia (a dok³adniej identyfikator sprzedawcy/producenta)
	 * \param pid - product id, drugi z parametrów identyfikacji urz¹dzenia
	 * \param printOnlyResult - flaga okreœlaj¹ca czy na wyjœciu maj¹ zostaæ wypisane wyniki w postaci umo¿liwiaj¹cej tworzenie wykresów czy czytelniejszej dla u¿ytkownika
	 */
	Mode(int bufforSize, unsigned count, int vid, int pid, bool printOnlyResult) : _bufforSize(bufforSize), _count(count), _vid(vid), _pid(pid), _printOnlyResult(printOnlyResult)
	{
		_debugPrinter.set(!printOnlyResult);
	}
	/**
	 * Domyœlny destruktor klasy.
	 */
	virtual ~Mode();
	/**
	 * Metoda generuj¹ca dane wejœciowe, w klasie Mode zawarta jest podstawowa jej implementacja.. mo¿e zostaæ dowolnie zmieniona w klasach pochodnych.
	 * \param data - wskaŸnik do wysy³anych w przysz³oœci danych
	 * \param size - rozmiar wysy³anych danych
	 */
	virtual int generateSymulatedData(unsigned char* data, const int size);
	/**
	 * Prosta metoda maj¹ca na celu wypisanie wyników.
	 */
	virtual void printFinalInformation();
	/**
	 * Metoda wywo³uj¹ca najwa¿niejsze metody inicjalizacyjne w odpowiedniej kolejnoœci.
	 */
	virtual void initProcedures();
	/**
	 * Metoda pobieraj¹ca odpowiedni kontekst dla dzia³añ na urz¹dzeniu USB, w wypadku niepowodzenia rzucany jest std::runtime_error.
	 */
	virtual void getContext();
	/**
	 * Metoda ustawia uchwyt do pod³aczonego urz¹dzenia na podstawie znanych vid oraz pid, w wypadku niepowodzenia rzucany jest wyj¹tek.
	 */
	virtual void getDeviceHandle();
	/**
	 * Metoda odpowiada za kolejny etap inicjalizacji.
	 */
	virtual void proceedWithInitUsb();
	/**
	 * Metoda odpowiadaj¹ca za czynnoœci po zakoñczeniu testu, czyli zwalnianie pamieci etc..
	 */
	virtual void closeLibUsb();
	/**
	 * Czysto wirtualna metoda której zadaniem jest wykonanie testu, czyli zmierzenie szybkoœæi przesy³ania danych po USB z odpowiedni¹ implementacj¹ libUSB.
	 */
	virtual int doTest() = 0;
	
protected:
	int _bufforSize;					//!< rozmiar u¿ywanego podczas przesy³u bufora danych
	unsigned _count;					//!< licznik wys³ania bufora danych
	int _vid;							//!< vendor id, jeden z identyfikatorów pod³¹czonego urz¹dzenia
	int _pid;							//!< product id, drugi z identyfikatorów pod³¹czonego urz¹dzenia
	double _timeResult;					//!< wynikowy czas w sekundach
	DebugPrinter _debugPrinter;			//!< drukarka celem latwiejszej obs³ugi wypisywanych informacji (zastêpstwo dla poziomu logowania)
	bool _printOnlyResult;				//!< flaga od której uzale¿nione jest czy wypisywane maj¹ byæ wszystkie informacje czy tylko wyniki
	libusb_context* _ctx;				//!< kontekst biblioteki libusb
	libusb_device_handle* _dev_handle;  //!< uchwyt do pod³¹czonego urz¹dzenia

};

