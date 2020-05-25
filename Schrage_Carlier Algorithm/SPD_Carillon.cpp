#include <iostream>
#include <queue>
#include <functional>
#include <cmath>
#include <string>
#include <fstream>
#include <tuple>
#include <list>
#include <algorithm>    

using namespace std;

// ======================================POMOCNICZNE===================================================================
struct Zadanie
{
	Zadanie(int R, int P, int Q, int I) :
	 R(R), P(P), Q(Q), I(I) { }
	 
	 int R, P, Q, I;
};

// do sortowania kolejki, od najmniejszego R
bool sortN(const Zadanie& zad1, const Zadanie& zad2)
{
	return zad1.R > zad2.R;
}

// do sortowania kolejki, od najwiekszego Q
bool sortG(const Zadanie& zad1, const Zadanie& zad2)
{
	return zad1.Q < zad2.Q;
}

// SCHRAGE BEZ PODZIALU ==============================================================================================
// priority_queue : template <class T, class Container, class Compare>
tuple<vector<Zadanie>, int> schrage(vector<Zadanie> data)
{
	int czas = 0;
	int cmax = 0;
	vector<int> permutacja;
	//priority_queue<Zadanie, vector<Zadanie>, function<bool(const Zadanie&, const Zadanie&)> > N(&sortN);
	//priority_queue<Zadanie, vector<Zadanie>, function<bool(const Zadanie&, const Zadanie&)> > G(&sortG);

	vector<Zadanie> pseudoN;
	vector<Zadanie> G;

	int howMany = data.size();
	int len = data.size();
	while(len--)
	{
		Zadanie temp(data[len].R, data[len].P, data[len].Q, data[len].I);
		pseudoN.push_back(temp);
	}

	//posortuj tak zeby zadnaie o najwiekszej wadze byla na poczatku wektora zadan
	stable_sort(pseudoN.begin(), pseudoN.end(),
		[](const Zadanie& lhs, const Zadanie& rhs) {
			return lhs.R > rhs.R;
		}
	);

	vector<Zadanie> N = pseudoN;


	
	while (N.size() || G.size())
	{
		while(N.size() && (N.back()).R <= czas)
		{
			Zadanie zad = N.back();
			N.pop_back();
			G.insert(G.begin(), zad);
		}

		//posortuj tak zeby zadnaie o najwiekszej wadze byla na poczatku wektora zadan
		stable_sort(G.begin(), G.end(),
			[](const Zadanie& lhs, const Zadanie& rhs) {
				return lhs.Q < rhs.Q;
			}
		);


		
		if (!G.size())
		{
			czas = (N.back()).R;
		}
		else
		{
			Zadanie zad = G.back();
			G.pop_back();
			czas += zad.P;
			cmax = max(cmax, czas + zad.Q);
			permutacja.push_back(zad.I);
		}
	}
	
	vector<Zadanie> perm;
	for (int i : permutacja)
	{
		cout << i+1 << " ";
		perm.push_back(data[i]);
	}
	cout << endl <<" cmax " << cmax << " " ;
	return make_tuple(perm, cmax);
}

// SCHRAGE Z PODZIALEM ===========================================================================================
int schragePodzial(vector<Zadanie> data)
{
	int czas = 0;
	int cmax = 0;
	Zadanie zad0(0, 0, 0, 0);

	
	vector<Zadanie> pseudoN;
	vector<Zadanie> G;
	
	int howMany = data.size();
	int len = data.size();
	while(len--)
	{
		Zadanie temp(data[len].R, data[len].P, data[len].Q, data[len].I);
		pseudoN.push_back(temp);
	}

	//posortuj tak zeby zadnaie o najwiekszej wadze byla na poczatku wektora zadan
	stable_sort(pseudoN.begin(), pseudoN.end(),
		[](const Zadanie& lhs, const Zadanie& rhs) {
			return lhs.R > rhs.R;
		}
	);

	vector<Zadanie> N = pseudoN;

	
	while (N.size() || G.size())
	{
		while(N.size() && (N.back()).R <= czas)
		{
			Zadanie temp = N.back();
			N.pop_back();
			G.insert(G.begin(), temp);
			
			if (temp.Q > zad0.Q)
			{
				zad0.P = czas - temp.R;
				czas = temp.R;
				if (zad0.P > 0)
					G.insert(G.begin(), zad0);
			}
		}

		//posortuj tak zeby zadnaie o najwiekszej wadze byla na poczatku wektora zadan
		stable_sort(G.begin(), G.end(),
			[](const Zadanie& lhs, const Zadanie& rhs) {
				return lhs.Q < rhs.Q;
			}
		);


		
		if (!G.size())
		{
			czas = (N.back()).R;
		}
		else
		{
			Zadanie temp = G.back();
			G.pop_back();
			zad0 = temp;
			czas += temp.P;
			cmax = max(cmax, czas + temp.Q);
		}
	}
	cout << "cmax: " << cmax ;
	return cmax;
}

// ===============================SCIEZKA=========================================================================
tuple<int, int, int> sciezka_krytyczna(tuple<vector<Zadanie>, int> tpl)
{
	int a, b, c;
	a = b = c = -1;
	vector<Zadanie> permutacja = get<0>(tpl);
	int cmax = get<1>(tpl);
	
	int czas = permutacja[0].R;
	int n = permutacja.size();
	
	// znajdz b
	for(int i=0; i<n; i++)
	{
		Zadanie temp = permutacja[i];
		czas = max(czas, temp.R);
		czas += temp.P;
		
		if((czas + permutacja[i].Q) == cmax) 
			b = i;
	}
	
	// znajdz a
	czas = permutacja[0].R;
	for(int i=0; i<n; i++)
	{
		Zadanie temp = permutacja[i];
		if(a == -1) 
		{
			int suma = 0;
			for(int j=i; j <= b; j++)
				suma += permutacja[j].P;
			
			suma += permutacja[b].Q;
			if(cmax == permutacja[i].R+suma)
				a = i;
		}
		
	}
	
	for(int i=a; i<=b; i++)
		if(permutacja[i].Q < permutacja[b].Q)
			c = i;
	
	
	return make_tuple(a, b, c);
}
// ========================================CARLIER===================================================================
void carlier(vector<Zadanie> dane, int &UB)
{
	// zrob schrage i rozpakuj
	auto tpl = schrage(dane);
	vector<Zadanie> schr = get<0>(tpl);
	int schr_cmax = get<1>(tpl); 
	
	if (schr_cmax < UB)
		UB = schr_cmax;
	
	// zrob sciezke krytyczna i rozpakuj
	auto sciezka = sciezka_krytyczna(tpl);
	
	int a = get<0>(sciezka);
	int b = get<1>(sciezka);
	int c = get<2>(sciezka);
	
	if (c == -1)
		return;

	// wyznaczenie prim	
	int r_prim = 999999, q_prim = 999999, p_prim = 0;
	for(int i=c+1; i<=b; i++)
	{
		if(schr[i].R < r_prim)
			r_prim = schr[i].R;
		
		if(schr[i].Q < q_prim)
			q_prim = schr[i].Q;
			
		p_prim += schr[i].P;
	}
	
	// wylicz LowerBound z r
	int r_temp = schr[c].R;
	schr[c].R = max(schr[c].R, r_prim + p_prim);
	int LB = schragePodzial(schr);
	
	if(LB < UB)
		carlier(schr, UB);
	
	schr[c].R = r_temp;
	
	// wylicz LowerBound z q
	int q_temp = schr[c].Q;
	schr[c].Q = max(schr[c].Q, q_prim + p_prim);
	LB = schragePodzial(schr);
	
	if(LB < UB)
		carlier(schr, UB);
	
	schr[c].Q = q_temp;
	
//	for(auto zad: schr)
//		cout << zad.I << " ";

//	cout <<"THIS IS IT " << UB;
}
// ==================================================================================================================
int main()
{
	string s, wzor;
	int n;
	ifstream plik;
	//otworz i czytaj plik z danymi
	plik.open("schr.data.txt");
	for (int i=0; i<=8; i++)
	{
		wzor = "data.00" + to_string(i) + ":";
		cout << "=================================" << endl << wzor << endl;
		while (s != wzor) //pomin niepotrzebne znaki
			plik >> s;
		
		vector<Zadanie> dane;
		vector<Zadanie> trueDane;
		// wczytaj dane do struktury Zadanie
		plik >> n;
		int r, p, q;
		for (int j=0; j<n; j++)
		{
			plik >> r >> p >> q;
			dane.push_back(Zadanie(r,p,q,j));
		}
		
		
		for (auto i : dane)
		{
			trueDane.push_back(i);
		}

		cout << "SCHRAGE" << endl;
		schrage(dane);
		cout << endl << "SCHRAGE PODZIAL" << endl;
		schragePodzial(dane);
		cout << endl;
		int UB = 99999;
		carlier(dane, UB);
		cout << endl;
		cout << "Cariler: " << endl;
		cout << UB << endl;

	}

	
	
	return 0;
}
