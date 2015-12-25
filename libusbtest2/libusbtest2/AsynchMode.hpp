#pragma once

#include "Mode.hpp"

#include <iostream>
#include <pthread.h>
//#include <unistd.h>


class AsynchMode : public Mode
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
	AsynchMode(int bufforSize, unsigned count, int vid, int pid, bool printOnlyResult);
	/**
	 * Domyœlny destruktor.
	 */
	virtual ~AsynchMode();
	/**
	 * Metoda maj¹ca na celu zmierzenie czasu przesy³u danych metod¹ asynchroniczn¹.
	 */
	virtual int doTest() override;
	/**
	 * Rozbudowanie metody inicjalizuj¹cej o dodatkowe zainicjalizowani mutexów oraz odpowiedni¹ alokacjê transferów.
	 */
	virtual void initProcedures() override;
	/**
	 * Rozbudowanie metody o dodatkowe zniszczenie mutexów i zwolnienie pamiêci po transferach.
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


