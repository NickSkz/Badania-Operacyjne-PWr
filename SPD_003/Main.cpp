#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <chrono>

using namespace std::chrono;

using namespace std;

//Czas algorytmu dla 120 tasks
microseconds allTheTime;

//koncowa permutacja
vector<vector<int>> resPerm;

int checkTimes(int maschinen, vector<vector<int>>& permutacion, vector<vector<int>>& time);

int neeeeeeeeeeeh(vector<vector<int>> excercises, int maschinen)
{
	//cMax -> inf
	int cMax{ 100000000 };

	//zapamietaj bo bedziemy usuwac zeby nam nie zmniejszylo ilosc iteracji petli
	int n = excercises.size();
	resPerm.clear();

	//dla wzsystkich zadan
	for (int i = 0; i < n; ++i) {
		// wez zadanie o najwiekszej wadze i je usun
		vector<int> what = excercises.front();
		excercises.erase(excercises.begin());

		//pierwsze po prostu wloz
		if (i == 0) {
			resPerm.push_back(what);
			continue;
		}

		//buffer na tymczasowa najkorzystniejsza permutacje
		vector<vector<int>> chosenPerm;

		int cMaxMax = 10000000;
		//po zad umiejscowioinych
		for (int j = 0; j <= resPerm.size(); ++j)
		{
			//zapamietaj permu w jaka wchodzilismy w tym etapie - i testuj gdzie najlepiej put zadanie
			vector<vector<int>> temp{ resPerm };

			//wloz na probe wektor
			temp.emplace(temp.begin() + j, what);

			//tablica czasow dla zadan na maszynach
			vector<vector<int>> time(temp.size());
			for (int k = 0; k < time.size(); ++k) {
				for(int z = 0; z < maschinen; ++z)
					time[k].push_back(0);
			}
			//sprawdz czy czas zsumowany jest lepszy
			int challengerC {
				checkTimes(maschinen, temp, time)
			};
			
			//Jezeli wyliczone nowe c jest lepsze - mniejsze - zammien
			if (challengerC < cMaxMax){
				cMaxMax = challengerC;
				chosenPerm = temp;
			}
		}

		//ustaw cmax i permutacje
		cMax = cMaxMax;
		resPerm = chosenPerm;
	}

	//zwroc cmax
	return cMax;
} 

//Dajemy na wejscie jakąs kolejnosc zadan - tworzymy tabele czasow odpowiadajaca zakonczeniu danego zadania na danej maszynie
//przez co ta tabela ma wymiar taki jak dane wejsciowe. Zaczynamy, jako ze czasy sa puste - wpisujemy 1 zadanie - wpisujemy całe gdyż
//nie jest blokowane prezz zadne inne zadanie, nastpenie analizujac kolejne zadania gdy jest to pierwsza maszyna dla zad to wpisujemy czas
//dla danego zadania plus czas poprzedniego z tej maszyny (komorka wyzej), a potem liczymy czas na kolejnych maszynach sprawdzajac czy zadanie
//poprzednie juz z niej zeszlo jezeli tak to po prostu przechodzimy na ta maszyne - dodajemy czas skonczenia z poprzedniej + czas wykonania, jezeli
//nie, uwzgledniamy czas poprzedniego zadania na maszynie + dodajemy czas trwania. I tak do konca
int checkTimes(int maschinen, vector<vector<int>>& permutacion, vector<vector<int>>& time)
{
	//po maszynach i zadaniach
	for (int i = 0; i < permutacion.size(); i++) {
		for (int j = 0; j < maschinen; j++) {
			
			//jezeli to pierwsze zadanie
			if (i == 0 and j != 0) 
				//time = time pierwszego zadnia na maszynie + czas z poprzedniej maszyny - maszyn tyle ile j
				time[0][j] = permutacion[0][j] + time[0][j - 1];
			
			//jezeli to pierwsza maszyna
			else if (j == 0 and i != 0)
				//time = time zadania na tej maszynie (pierwszej) + time zadania ktore bylo na niej wczesniej
				time[i][0] = permutacion[i][0] + time[i - 1][0];
			
			//Jezeli to poczatek wyliczania
			else if (i == 0 and j == 0)
				//Po prostu wsadz pierwszy czas
				time[0][0] = permutacion[0][0];
			else {
				//Jesli zadanie poprzednie juz zeszlo z maszyny - mozemy umieszczać
				if (time[i][j - 1] < time[i - 1][j])
					time[i][j] = permutacion[i][j] + time[i - 1][j];
				//Jezeli na maszynie siedzi tam jeszcze wczesniejsze zadanie -> time = time zad + czas skonczenia poprzedniego zadania na maszynine
				else
					time[i][j] = permutacion[i][j] + time[i][j - 1];
			}

		}
	}
	//zwroc calkowity czas - ostatnia komorka w macierzy
	return time[permutacion.size() - 1][maschinen - 1];
}



int main() {
	string s, wzor;
	ifstream plik;
	//n-liczba zadan, P-czas wykonania, W-wspolczynnik kary, D-zadany termin zakonczenia
	int maschinen, ileExcercises;

	plik.open("neh.data.txt");

	//czytaj z pliku wszystkie dane
	for (int i = 0; i < 121; i++)
	{
		//pomin znaki poprzedzajace istotne dane
		if(i < 10)
			wzor = "data.00" + to_string(i) + ":";
		else if (i < 100)
			wzor = "data.0" + to_string(i) + ":";
		else
			wzor = "data." + to_string(i) + ":";
		cout << wzor << endl;

		while (s != wzor)
			plik >> s;
		//czytaj ile zadan, ile maszyn
		plik >> ileExcercises;
		plik >> maschinen;
		vector<vector<int>> excercises(ileExcercises);

		//dlugosc zadania - dajemy to do 2 nadmiarowej komorki (w 1 nadmiarowej bazowa kolejnosc)
		int pseudoLange = 0;
		int buff;
		for (int i = 0; i < excercises.size(); ++i) {
			pseudoLange = 0;
			for (int j = 0; j < maschinen; ++j) {
				plik >> buff;
				excercises[i].push_back(buff);
				pseudoLange += buff;
			}
			//Doklejamy info o oryginalnej kolejnosci zeby permu na koniec podac i jeszcze o dlugosci zeby zoptymalizowac prace comparatora w sortowaniu
			excercises[i].push_back(i);
			excercises[i].push_back(pseudoLange);
		}
		
		//chrono by sprawdzic ile sie wykonuje
		auto start = high_resolution_clock::now();

		//posortuj tak zeby zadnaie o najwiekszej wadze byla na poczatku wektora zadan + comparator
		stable_sort(excercises.begin(), excercises.end(),
			[](const vector<int>& lhs, const vector<int>& rhs) {
				return lhs[lhs.size()-1] > rhs[rhs.size()-1]; 
			}
		);
		//dzialaj nehem
		int cMax = neeeeeeeeeeeh(excercises, maschinen);
		
		auto stop = high_resolution_clock::now();
		//Dodaj czas do sumy calego wykonywania
		allTheTime += duration_cast<microseconds>(stop - start);

		//Wyprintuj cMax
		cout << cMax << endl;

		//Wyprintuj ostateczna permutacje - kolejnosc bazowa zapisana w 1 nadmiarowej komorce kazdego zadania (w 2 dlugosc trwania)
		for (auto item : resPerm)
			cout << item[maschinen] + 1 << " ";
		cout << endl << endl;
	}

	cout << allTheTime.count() << endl;

	plik.close();
}
