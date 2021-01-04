//POP 2019 01 14 projekt 2 Laskowska Nikola IBM 2 175464
//Microsoft Visual Studio 2017

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <windows.h>

using namespace std;

int wybor_opcji();
int ilosc_znakow_w_linii();
int ilosc_lini_pliku();
void menu_lista();
bool usuwanie_bajtu(const wstring& PathName, LARGE_INTEGER NewSize);

int main()
{
	cout << "-----Wyswietlanie plikow-----" << endl;
	cout << endl;

	//wprowadzanie nazwy pliku
	string NazwaPliku;
	cout << "Podaj nazwe pliku, ktory chcesz przegladac: ";
	cin >> NazwaPliku;

	//otwieranie pliku
	fstream OtwieranyPlik;
	OtwieranyPlik.open(NazwaPliku.c_str(), ios::binary | ios::in | ios::out);

	//sprawdzanie czy plik sie otwiera
	while (!OtwieranyPlik)
	{
		cout << "Plik nie zostal otwarty." << endl;
	}

	//wybieramy ile znakow chcemy w rzedzie
	int ilosc = ilosc_znakow_w_linii();
	while (ilosc == -1)
	{
		cout << "Wybrano nieprawidlowa opcje! Wybierz jeszcze raz!" << endl;
		ilosc = ilosc_znakow_w_linii();
	}

	//wybieramy ile rzedow chcemy
	int iloscLini = ilosc_lini_pliku();
	while (iloscLini == -1)
	{
		cout << "Wybrano nieprawidlowa opcje! Wybierz jeszcze raz!" << endl;
		iloscLini = ilosc_lini_pliku();
	}

	bool kontynuacja = true;
	int len;
	int obecnaPozycja = 0;
	while (kontynuacja)
	{
		//funkcja dzieki ktorej czysci nam sie konsola
		system("CLS");
		OtwieranyPlik.clear();
		OtwieranyPlik.seekg(obecnaPozycja, ios::beg);

		//sposob z zadania laboratoryjnego, na ktorym mialam bazowac - dodalam opcje "kontrolowania" ilosci rzedow i kolumn
		unsigned char txt[16];
		int j;
		for (j = 0; j < iloscLini; j++)
		{
			int pozycja = OtwieranyPlik.tellg();
			OtwieranyPlik.read(reinterpret_cast<char *>(txt), ilosc);
			len = OtwieranyPlik.gcount();
			if (len == 0)
				break;
			cout << "0x" << pozycja << "\t| ";
			for (int i = 0; i < len; i++)
			{
				cout << setfill('0') << setw(2) << hex << static_cast<int>(txt[i]) << " ";
			}
			for (int i = len; i < ilosc; i++)
				cout << setfill(' ') << setw(3) << " ";
			cout << "| ";
			for (int i = 0; i < len; i++)
			{
				if (txt[i] == '\n' || txt[i] == '\r' || txt[i] == '\t' || txt[i] == '\b' || txt[i] == '\a')
					cout << '.';
				else
					cout << txt[i];
			}
			for (int i = len; i < ilosc; i++)
				cout << ' ';
			cout << " |" << endl;
		}
		if (j < iloscLini)
		{
			for (int i = j; i < iloscLini; i++)
			{
				cout << "EMPTY"
					<< "\t| " << endl;
			}
		}

		//wybieramy co chcemy zrobic z naszym plikiem
		int wybor = wybor_opcji();
		while (wybor == -1)
		{
			cout << "Wybrano nieprawidlowa opcje! Wybierz jeszcze raz!" << endl;
			wybor = wybor_opcji();
		}
		if (wybor == 1)
		{
			obecnaPozycja = 0;
		}
		else if (wybor == 2)
		{
			OtwieranyPlik.clear();
			OtwieranyPlik.seekg(0, ios::end);
			obecnaPozycja = OtwieranyPlik.tellg();
		}
		else if (wybor == 3)
		{
			int WybranyBajt;
			cout << "Podaj numer bajtu, do ktorego chcesz przejsc: ";
			cin >> WybranyBajt;
			obecnaPozycja = WybranyBajt - 1;
		}
		else if (wybor == 4)
		{
			obecnaPozycja += (iloscLini - 1) * ilosc;
		}

		else if (wybor == 5)
		{
			obecnaPozycja -= obecnaPozycja - (iloscLini - 1) * ilosc >= 0 ? (iloscLini - 1) * ilosc : 0;
		}
		else if (wybor == 6)
		{
			int WybranyBajtZmiana;
			int PolozenieBajtu;
			char format;
			char Zmiana;
			cout << "Wybierz numer bajtu, ktory chcesz zmienic: ";
			cin >> WybranyBajtZmiana;
			PolozenieBajtu = WybranyBajtZmiana - 1;

			//wybieranie formatu zmienianego bajtu
			cout << "Wybierz format, w ktorym wstawiasz bajt: a) ASCII b) dziesietny c) szesnastkowy ";
			cin >> format;
			while (format != 'a' && format != 'b' && format != 'c')
			{
				cout << "Nieprawidlowy format. Wybierz format: a) ASCII b) dziesietny c) szesnastkowy ";
				cin >> format;
			}
			cout << "Podaj na co chcesz zmienic: ";
			if (format == 'a')
			{
				cin >> Zmiana;
			}
			else if (format == 'b')
			{
				int ZmianaInt;
				cin >> ZmianaInt;
				Zmiana = static_cast<char>(ZmianaInt);
			}
			else if (format == 'c')
			{
				int ZmianaInt;
				cin >> hex >> ZmianaInt;
				Zmiana = static_cast<char>(ZmianaInt);
			}
			OtwieranyPlik.clear();
			OtwieranyPlik.seekp(PolozenieBajtu, ios::beg);
			OtwieranyPlik.write(reinterpret_cast<char *>(&Zmiana), sizeof(char));
			OtwieranyPlik.flush();
		}
		else if (wybor == 7)
		{
			char format;
			char Zmiana;
			cout << "Wybierz format, w ktorym wstawiasz bajt: a) ASCII b) dziesietny c) szesnastkowy ";

			cin >> format;
			while (format != 'a' && format != 'b' && format != 'c')
			{
				cout << "Nieprawidlowy format. Wybierz format: a) ASCII b) dziesietny c) szesnastkowy ";
				cin >> format;
			}
			cout << "Podaj na co chcesz zmienic: ";
			if (format == 'a')
			{
				cin >> Zmiana;
			}
			else if (format == 'b')
			{
				int ZmianaInt;
				cin >> ZmianaInt;
				Zmiana = static_cast<char>(ZmianaInt);
			}
			else if (format == 'c')
			{
				int ZmianaInt;
				cin >> hex >> ZmianaInt;
				Zmiana = static_cast<char>(ZmianaInt);
			}
			OtwieranyPlik.clear();
			OtwieranyPlik.seekp(0, ios::end);
			OtwieranyPlik.write(reinterpret_cast<char *>(&Zmiana), sizeof(char));
			OtwieranyPlik.flush();
		}
		else if (wybor == 8)
		{
			//usuwanie bajtu - biblioteki standardowe nie dostarczają funkcji do zmiany rozmiaru pliku, dlatego uzylam funkcji systemowych - w tym przypadku WinAPI

			//najpierw, sprawdzamy rozmiar naszego obecnego pliku
			ifstream in(NazwaPliku, ifstream::ate | ifstream::binary);
			long long size = in.tellg();
			in.close();

			//następnie tworzymy argumenty do funkcji WinAPI - CreateFileW, SetFilePointerEx i SetEndOfFile, będzie nam potrzebny rozmiar docelowego pliku typu _LARGE_INTEGER oraz LPCWSTR, który możemy uzyskać za pomocą wstring
			_LARGE_INTEGER MniejszyPlik;
			MniejszyPlik.QuadPart = size - 1;
			wstring ws;
			wstring wsTmp(NazwaPliku.begin(), NazwaPliku.end());
			ws = wsTmp;

			//zamykamy fstream, bo inaczzej funkcja CreateFileW wyrzuca blad o pliku zajetym przez inny proces
			OtwieranyPlik.close();

			usuwanie_bajtu(ws, MniejszyPlik);

			//po usunieciu znowu otwieramy fstream
			OtwieranyPlik.open(NazwaPliku.c_str(), ios::binary | ios::in | ios::out);
		}
		else if (wybor == 9)
		{
			cout << "Koniec pracy programu";
			kontynuacja = false;
			break;
		}
	}

	OtwieranyPlik.close();

	return 0;
}

//wybieramy co chcemy zrobic z plikiem
int wybor_opcji()
{
	menu_lista();
	string input;
	cin >> input;

	if (input[0] >= '0' && input[0] <= '9')
		return (int)input[0] - '0';

	if (input[0] >= 'a' && input[0] <= 'i')
		return (int)input[0] - 'a' + 1;

	if (input[0] >= 'A' && input[0] <= 'I')
		return (int)input[0] - 'A' + 1;

	return -1;
}

//ustalanie ilosci kolumn
int ilosc_znakow_w_linii()
{
	string input;
	cout << "Ile chcesz widziec znakow w linii? Podaj wartosc od 1 do 16 ";
	cin >> input;
	int ilosc;
	try
	{
		ilosc = stoi(input);
	}
	catch (invalid_argument &)
	{
		return -1;
	}
	catch (out_of_range &)
	{
		return -1;
	}
	return ilosc >= 1 && ilosc <= 16 ? ilosc : -1;
}

//ustalanie ilosci wierszy
int ilosc_lini_pliku()
{
	string input;
	cout << "Ile chcesz widziec lini pliku? Podaj wartosc od 1 do 20 ";
	cin >> input;
	int ilosc;
	try
	{
		ilosc = stoi(input);
	}
	catch (invalid_argument &)
	{
		return -1;
	}
	catch (out_of_range &)
	{
		return -1;
	}
	return ilosc >= 1 && ilosc <= 20 ? ilosc : -1;
}

//funkcja ktora pokazuje nam menu
void menu_lista() {
	cout << endl;
	cout << "Jezeli chcesz \n(a) przejsc na poczatek pliku - wcisnij 1" << endl;
	cout << "(b) przejsc na koniec pliku - wcisnij 2" << endl;
	cout << "(c) przejsc do dowolnego bajtu pliku - wcisnij 3" << endl;
	cout << "(d) przesunac plik w dol - wcisnij 4" << endl;
	cout << "(e) przesunac plik w gore - wcisnij 5" << endl;
	cout << "(f) zmienic bajt - wcisnij 6" << endl;
	cout << "(g) dodac bajt - wcisnij 7" << endl;
	cout << "(h) usunac bajt - wcisnij 8" << endl;
	cout << "(i) wyjsc - wcisnij 9" << endl;
}

//funkcja usuwania bajtu - rozwiazanie zaczerpniete ze stackoverflow
bool usuwanie_bajtu(const wstring& PathName, LARGE_INTEGER NewSize) {

	//otwieramy nasz plik funkcją CreateFileW i otrzymujemy uchwyt na ten plik
	HANDLE hFile = CreateFileW(PathName.c_str(), GENERIC_WRITE, FILE_SHARE_READ,
		NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);

	//jezeli plik nie otworzyl sie pomyslnie
	if (hFile == INVALID_HANDLE_VALUE) {
		return false;
	}

	//uruchamiamy funkcje usuwania i sprawdzamy czy zaszlo pomyslnie
	bool status = (SetFilePointerEx(hFile, NewSize, NULL, FILE_BEGIN) && SetEndOfFile(hFile));

	//zamykamy wcześniej otrzymany uchwyt
	CloseHandle(hFile);

	//zwracamy true jeśli wszystkie funkcje wykonały się poprawnie, false jeśli nie
	return status;
}
