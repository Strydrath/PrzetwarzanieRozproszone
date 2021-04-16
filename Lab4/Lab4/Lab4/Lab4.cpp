// Lab4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <string>
#include <tchar.h>
#include <strsafe.h>
#include <math.h>
#include <vector>
#include <chrono>
using namespace std;


#define MAX_THREADS 7
#define BUF_SIZE 255

typedef struct MyData {
	float pPrzedzialu;
	float kPrzedzialu;
	float step;
	vector<int>f;
	float* wyniki;
	int id;
};

float funkcja(vector<int>f, float x) {
	float wynik = 0;
	for (int i = f.size()-1; i >= 0; i--) {
		wynik += f[i] * pow(x, i);
	}
	return wynik;
}

float trapez(float x1, float y1, float x2, float y2) {
	float a = abs(0 - y1);
	float b = abs(0 - y2);
	float h = x2 - x1;
	return ((a + b) * h) / 2;

}
DWORD WINAPI policzCalke(LPVOID lpParam)
{
	MyData pDataArray;
	pDataArray = *(MyData*)lpParam;
	float start = pDataArray.pPrzedzialu;
	float stop = pDataArray.kPrzedzialu;
	float step = pDataArray.step;
	vector<int>f = pDataArray.f;
	float now = start;
	float lx = start;
	float ly = funkcja(f,lx);
	float nx;
	float ny;
	float pole=0;
	while (now < stop) {
		nx = lx + step;
		ny = funkcja(f, nx);
		pole += trapez(lx, ly, nx, ny);
		lx = nx;
		ly = ny;
		now += step;
	}
	(*pDataArray.wyniki) = pole;
	return 0;
}


float program(vector<int>f, float start, float stop,float step, int threadCount) {
	HANDLE* watki = new HANDLE[threadCount];
	DWORD* ids = new DWORD[threadCount];
	MyData* dane = new MyData[threadCount];
	float* solutions = new float[threadCount];

	float delta = (stop - start) / threadCount;
	stop = start + delta;
	for (int i = 0; i < threadCount; i++) {
		dane[i] = { start, stop, step,f, &solutions[i], i };
		watki[i] = CreateThread(NULL, 0, policzCalke, (void*)&dane[i], 0, &ids[i]);
		start += delta;
		stop += delta;
	}
	WaitForMultipleObjects(threadCount, watki, true, INFINITE);
	float solutionSum = 0;
	for (int i = 0; i < threadCount; i++) {
		solutionSum += solutions[i];
	}

	delete[] watki;
	delete[] ids;
	delete[] dane;
	delete[] solutions;
	return solutionSum;
}


int main()
{
	DWORD  id;
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	CreateProcess(TEXT("c:\\windows\\notepad.exe"), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	vector<int> funks[5];
	funks[0].push_back(1);
	funks[0].push_back(2);
	funks[0].push_back(3);
	funks[1].push_back(3);
	funks[1].push_back(2);
	funks[1].push_back(4);
	funks[1].push_back(5);
	funks[2].push_back(-3);
	funks[2].push_back(2);
	funks[2].push_back(1);
	funks[2].push_back(1);
	string f;
	cout << "Wybierz wielomian"<<endl;
	for (int i = 0; i < 3; i++) {
		f = "";
		for (int j = funks[i].size() - 1; j > 0; j--) {
			f += to_string(funks[i][j]);
			f += "x^";
			f += to_string(j);
			f += '+';
		}
		f += to_string(funks[i][0]);
		cout << f<<endl;
	}
	int n;
	float p;
	float k;
	int nw;
	float s = 0.0001;
	cin >> n;
	cout << endl << "podaj poczatek przedzialu" << endl;
	cin >> p;
	cout << endl << "podaj koniec przedzialu" << endl;
	cin >> k;
	cout << endl << "podaj liczbe watkow" << endl;
	cin >> nw;
	if (nw > MAX_THREADS) {
		cout << "ZA DUZO WATKOW" << endl;
	}
	else {
		chrono::steady_clock::time_point begin = chrono::steady_clock::now();
		cout << " Wynik:" << program(funks[n], p, k, s, nw) << endl;
		chrono::steady_clock::time_point end = chrono::steady_clock::now();
		cout << "Czas: " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << " mikrosekund." << endl;
		cout << "2 watki:" << endl;
		begin = chrono::steady_clock::now();
		cout << " Wynik:" << program(funks[n], p, k, s, 2) << endl;
		end = chrono::steady_clock::now();
		cout << "Czas: " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << " mikrosekund." << endl;
		cout << "4 watki:" << endl;
		begin = chrono::steady_clock::now();
		cout << " Wynik:" << program(funks[n], p, k, s, 4) << endl;
		end = chrono::steady_clock::now();
		cout << "Czas: " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << " mikrosekund." << endl;
	}

	return 0;
}
