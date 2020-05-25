#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <vector>

using namespace std;


//wartosc k dla danego zadania
int wylicz_k(int C, int W, int D)
{
	return C > D ? (C - D) * W : 0;
}

int witi(int n, int P[], int W[], int D[])
{
	int kombinacje = pow(2, n); //ilosc kombinacji
	vector<int> tablica = vector<int>(kombinacje, 0); //do przechowania F dla kazdej kombinacji, + pierwsza = 0 

	for (int i = 1; i < kombinacje; i++) //iteruj po kazdej kombinacji
	{
		int C = 0;
		tablica[i] = 1000000; //do nadpisywania mniejsza wartoscia

		//wylicz C - iteruj po kazdym zadaniu, a jesli nalezy do kombinacji, to dodaj
		//jego czas wykonania
		for (int j = 0; j < n; j++)
		{
			if (i & (1 << j))
				C += P[j];
		}
		//wylicz F - iteruj po kazdym zadaniu, a jesli nalezy do kombinacji, to wylicz
		//F i spradz, czy jest najmniejsze
		for (int j = 0; j < n; j++)
		{
			if (i & (1 << j))
			{
				//wyrzuc j-element z kombinacji, oblicz F[\{j}] i dodaj K[j]
				int idx = i - (1 << j);
				int F = tablica[idx] + wylicz_k(C, W[j], D[j]);
				if (F < tablica[i]) //nadpisz tablice, jesli nowe F jest mniejsze
				{
					tablica[i] = F;
				}
			}
		}
	}

	cout << endl;

	return tablica[kombinacje - 1];
}


int main() {
	string s, wzor;
	ifstream plik;
	//n-liczba zadan, P-czas wykonania, W-wspolczynnik kary, D-zadany termin zakonczenia
	int n, P[99], W[99], D[99];

	//czytaj z pliku wszystkie dane
	for (int i = 10; i <= 20; i++)
	{
		plik.open("data.txt");
		//pomin znaki poprzedzajace istotne dane
		wzor = "data." + to_string(i) + ":";
		cout << wzor << endl;
		while (s != wzor)
			plik >> s;
		//czytaj n, P, W, D z pliku
		plik >> n;
		for (int i = 0; i < n; i++)
			plik >> P[i] >> W[i] >> D[i];
		plik.close();
		cout << witi(n, P, W, D) << endl << endl;
	}



}