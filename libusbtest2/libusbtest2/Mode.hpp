#pragma once

#include "DebugPrinter.hpp"
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include "libusb.h"

/** 
 * Klasa Mode zawieraj�ca podstawow� implementacj� metod inicjalizacyjnych oraz ko�cz�cych przesy�anie danych.
 * Jej to klasa nadrz�dna dla poszczeg�lnych metod przesy�u danych, zawiera czysto wirtualn� metod� doTest().
 * Ka�d� prost� implementacj� inicjalizacji mo�na dowolnie rozwija� w klasie pochodnej.
 */
class Mode
{
public:
	/** 
	 * Konstruktor maj�cy za zadanie wprowadzenie podstawowych ustawie�.
	 * \param bufforSize - rozmiar buforu jako liczba calkowita
	 * \param count - ilosc powt�rze� z jakimi bufor ma zosta� wys�ana (ilo�� danych == count x bufforSize)
	 * \param vid - vendor id, czyli jeden z parametr�w identyfikacji pod��czonego do USB urz�dzenia (a dok�adniej identyfikator sprzedawcy/producenta)
	 * \param pid - product id, drugi z parametr�w identyfikacji urz�dzenia
	 * \param printOnlyResult - flaga okre�laj�ca czy na wyj�ciu maj� zosta� wypisane wyniki w postaci umo�liwiaj�cej tworzenie wykres�w czy czytelniejszej dla u�ytkownika
	 */
	Mode(int bufforSize, unsigned count, int vid, int pid, bool printOnlyResult) : _bufforSize(bufforSize), _count(count), _vid(vid), _pid(pid), _printOnlyResult(printOnlyResult)
	{
		_debugPrinter.set(!printOnlyResult);
	}
	/**
	 * Domy�lny destruktor klasy.
	 */
	virtual ~Mode();
	/**
	 * Metoda generuj�ca dane wej�ciowe, w klasie Mode zawarta jest podstawowa jej implementacja.. mo�e zosta� dowolnie zmieniona w klasach pochodnych.
	 * \param data - wska�nik do wysy�anych w przysz�o�ci danych
	 * \param size - rozmiar wysy�anych danych
	 */
	virtual int generateSymulatedData(unsigned char* data, const int size);
	/**
	 * Prosta metoda maj�ca na celu wypisanie wynik�w.
	 */
	virtual void printFinalInformation();
	/**
	 * Metoda wywo�uj�ca najwa�niejsze metody inicjalizacyjne w odpowiedniej kolejno�ci.
	 */
	virtual void initProcedures();
	/**
	 * Metoda pobieraj�ca odpowiedni kontekst dla dzia�a� na urz�dzeniu USB, w wypadku niepowodzenia rzucany jest std::runtime_error.
	 */
	virtual void getContext();
	/**
	 * Metoda ustawia uchwyt do pod�aczonego urz�dzenia na podstawie znanych vid oraz pid, w wypadku niepowodzenia rzucany jest wyj�tek.
	 */
	virtual void getDeviceHandle();
	/**
	 * Metoda odpowiada za kolejny etap inicjalizacji.
	 */
	virtual void proceedWithInitUsb();
	/**
	 * Metoda odpowiadaj�ca za czynno�ci po zako�czeniu testu, czyli zwalnianie pamieci etc..
	 */
	virtual void closeLibUsb();
	/**
	 * Czysto wirtualna metoda kt�rej zadaniem jest wykonanie testu, czyli zmierzenie szybko��i przesy�ania danych po USB z odpowiedni� implementacj� libUSB.
	 */
	virtual int doTest() = 0;
	
protected:
	int _bufforSize;					//!< rozmiar u�ywanego podczas przesy�u bufora danych
	unsigned _count;					//!< licznik wys�ania bufora danych
	int _vid;							//!< vendor id, jeden z identyfikator�w pod��czonego urz�dzenia
	int _pid;							//!< product id, drugi z identyfikator�w pod��czonego urz�dzenia
	double _timeResult;					//!< wynikowy czas w sekundach
	DebugPrinter _debugPrinter;			//!< drukarka celem latwiejszej obs�ugi wypisywanych informacji (zast�pstwo dla poziomu logowania)
	bool _printOnlyResult;				//!< flaga od kt�rej uzale�nione jest czy wypisywane maj� by� wszystkie informacje czy tylko wyniki
	libusb_context* _ctx;				//!< kontekst biblioteki libusb
	libusb_device_handle* _dev_handle;  //!< uchwyt do pod��czonego urz�dzenia

};

