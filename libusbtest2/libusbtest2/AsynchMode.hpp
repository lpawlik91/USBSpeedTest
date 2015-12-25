#pragma once

#include "Mode.hpp"

#include <iostream>
#include <pthread.h>
//#include <unistd.h>


class AsynchMode : public Mode
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
	AsynchMode(int bufforSize, unsigned count, int vid, int pid, bool printOnlyResult);
	/**
	 * Domy�lny destruktor.
	 */
	virtual ~AsynchMode();
	/**
	 * Metoda maj�ca na celu zmierzenie czasu przesy�u danych metod� asynchroniczn�.
	 */
	virtual int doTest() override;
	/**
	 * Rozbudowanie metody inicjalizuj�cej o dodatkowe zainicjalizowani mutex�w oraz odpowiedni� alokacj� transfer�w.
	 */
	virtual void initProcedures() override;
	/**
	 * Rozbudowanie metody o dodatkowe zniszczenie mutex�w i zwolnienie pami�ci po transferach.
	 */
	void closeLibUsb() override;
private:
	pthread_mutex_t _sender_lock;
	pthread_mutex_t _receiver_lock;
	pthread_cond_t _sender_cond;
	pthread_cond_t _receiver_cond;

	libusb_transfer* _senderTransfer;
	libusb_transfer* _receiverTransfer;
	pthread_t _receiverThread;

};


