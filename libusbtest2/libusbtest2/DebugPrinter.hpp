#pragma once


#include <string>
#include <sstream>
#include <iostream>

/**
 * Klasa pomocnicza, która ma za zadanie w określonych okolicznościach (zmienna składowa _enabled) ma zadanie blokować strumień wyjściowy.
 */
class DebugPrinter
{
public:
	/**
	 * Domyślny konstruktor.
	 * Ustawia zmienna _enabled na wartość true.
	 */
	DebugPrinter() : _enabled(true)
	{
	}
	/**
	 * Konstruktor pozwalający ustawić zmienną _enabled na dowolną podaną w argumencie.
	 */
	DebugPrinter(bool enabled) : _enabled(enabled)
	{
	}
	/**
	 * Przeładowany operator<< dla tablicy znaków, pozwalający w wypadku niedozwolonego wypisywania ciągów znaków na blokade strumienia wyjściowego.
	 */
	DebugPrinter& operator<<(const char ss[])
	{
		if(_enabled)
			std::cout << ss;
		return *this;
	}
	/**
	 * Przeładowany operator<< dla tablicy znaków, pozwalający w wypadku niedozwolonego wypisywania liczb całkowitych na blokade strumienia wyjściowego.
	 */
	DebugPrinter& operator<<(const int& d)
	{
		if(_enabled)
			std::cout << d;
		return *this;
	}
	/**
	 * Metoda pozwalająca na odblokowanie/zablokowanie strumienia wyjściowego.
	 */
	void set(bool enabled)
	{
		_enabled = enabled;
	}

private:
	bool _enabled;		//!< zmienna określająca możliwość wypisywania danych na wyjście.
};
