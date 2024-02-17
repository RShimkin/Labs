#include <stdlib.h>
#include <Windows.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <random>


using namespace std;

int n, l;

long double fRand(double fMin, double fMax)
{
	long double f = (long double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

// НЕ РАБОТАЕТ
auto generateData(int diap) {

	long double dif = diap / 5;
	auto magic = [](vector<vector<long double>> a) {
		vector<vector<long double>> res;

	};

	vector<vector<long double>> a;
	vector<long double> f;

	random_device rd;
	mt19937 one;
	one.seed(rd());
	uniform_real_distribution<> urd(-diap+dif, diap-dif);
	//decltype(urd) sec(diap - dif, diap);
	decltype(urd) sec(0, 1/diap); 
	
	for (int i = 0; i < n; i++) {
		vector<long double> tmp;
		tmp.push_back(sec(one));
		for (int k = 1; k < l; k++)
			tmp.push_back(urd(one));
		a.push_back(tmp);
		f.push_back(urd(one));
	}

	return make_tuple(a, f);
}

auto GenStripe(int N, int L, int diap = 10, bool bad = false) {

}

auto getData() {
	string filename;
	cout << "Enter the file name: ";
	cin >> filename;
	ifstream in(filename);

	in >> n >> l;

	auto split = [](string str, char delim) -> vector<string> {
		vector<string> res;
		int pos = str.find(delim);
		while (pos != string::npos) {
			string cur = str.substr(0, pos);
			res.push_back(cur);
			str = str.substr(pos + 1, str.size() - pos - 1);
			pos = str.find(delim);
		}
		if (str.length()) res.push_back(str);
		return res;
	};

	vector<vector<long double>> a;
	vector<long double> f;

	string buf;
	getline(in, buf);
	for (int i = 0; i < n; i++) {
		getline(in, buf);
		if (buf.size()) {
			auto line = split(buf, ' ');
			vector<long double> tmp;
			for (int k = 0; k < l; k++)
				(i + k) >= n ? tmp.push_back(0) : tmp.push_back(atof(line[i + k].c_str()));
			a.push_back(tmp);
			f.push_back(atof(line[line.size() - 1].c_str()));
		}
	}
	in.close();

	return make_tuple(a, f);
}

auto straight(vector<vector<long double>> a) {
	auto k0 = [](int i) -> int {
		return max(0, i - l + 1);
	};
	auto kn = [](int i) -> int {
		return min(n - 1, i + l - 1);
	};

	vector<vector<long double>> t;
	long double tmp;
	for (int i = 0; i < n; i++) {
		//cout << ">> i=" << i << endl;
		vector<long double> cur;
		tmp = a[i][0];

		int k0i = k0(i);
		//cout << "ko=" << k0i << "\t";
		for (int k = k0i; k <= i - 1; k++)
			tmp -= t[k][(long)i - k] * t[k][(long)i - k];
		//cout << "tmp=" << tmp << " ";
		if (tmp < 0)
			throw tmp;
		tmp = sqrt(abs(tmp));
		//cout << tmp << endl;
		cur.push_back(tmp);

		long double num = kn(i);
		for (int j = i + 1; j <= num; j++) {
			//tmp = 0;
			tmp = a[i][(long)j - i];
			for (int k = k0(j); k <= i - 1; k++)
				tmp -= t[k][(long)i - k] * t[k][(long)j - k];
			tmp = (tmp) / cur[0];
			cur.push_back(tmp);
		}
		int leng = cur.size();
		for (int k = leng; k < l; k++)
			cur.push_back(0);
		t.push_back(cur);
		//cout << i << endl;
	}

	return t;
}

auto solve(vector<vector<long double>> t, vector<long double> f, vector<long double> F) {
	cout << "---------------------------------------\n\n";
	vector<long double> y, Y;
	long double tmp, TMP;
	for (int i = 0; i < n; i++) {
		TMP = tmp = 0;
		for (int k = 0; k <= i - 1; k++) {
			int s = i - k;
			if (s < l) {
				long double one = t[k][i - k] * y[k];
				long double two = t[k][i - k] * Y[k];
				tmp += (one);
				TMP += (two);
			}
		}
		tmp = (f[i] - tmp) / t[i][0];
		TMP = (F[i] - TMP) / t[i][0];
		y.push_back(tmp);
		Y.push_back(TMP);
	}

	vector<long double> x(n), X(n);
	for (int i = n - 1; i >= 0; i--) {
		TMP = tmp = 0;
		for (int k = i + 1; k < n; k++) {
			int s = k - i;
			if (s < l) {
				long double one = t[i][s] * x[k];
				long double two = t[i][s] * X[k];
				tmp += (one);
				TMP += (two);
			}
			else {
				tmp += 0;
				TMP += 0;
			}	
		}
		tmp = (y[i] - tmp) / t[i][0];
		x[i] = tmp;
		//cout << "X[" << i << "]=(" << Y[i] << "-" << TMP << ")/" << t[i][0];
		TMP = (Y[i] - TMP) / t[i][0];
		//cout << "=" << TMP << endl;
		X[i] = TMP;
	}

	return make_tuple(x, X);
}

auto multiply(vector<vector<long double>> a, vector<long double> x) {
	vector<long double> res(n);
	for (int i = 0; i < n; i++) {
		long double temp = 0;
		for (int k = (i - l + 1) < 0 ? 0 : i - l + 1 ; k < i; k++)
			temp += a[k][i-k] * x[k];
		for (int k = i; k < i + l && k < n; k++)
			temp += a[i][k - i] * x[k];
		res[i] = temp;
	}
	return res;
}

long double compare(vector<long double> f, vector<long double> res) {
	long double def = 0;
	long double temp = 0;
	for (int i = 0; i < n; i++) {
		temp = f[i] - res[i];
		def += temp * temp;
	}
	return sqrt(def);
}

long double delta(vector<long double> F) {
	long double curmax = 0;
	for (int i = 0; i < F.size(); i++) {
		curmax = max(abs(F[i] - 1), curmax);
	}
	return curmax;
}

void print(vector<vector<long double>> m) {
	for (auto line: m) {
		for (auto x: line)
			cout << x << " ";
		cout << endl;
	}
	cout << endl;
}

void print(vector<long double> v, bool vert = false) {
	for (auto x : v) {
		cout << x << " ";
		if (vert) cout << "\n";
	}
	cout << endl;
}

void multrans(long double** g, long double** a, int N) {
	for (int x = 0; x < N; x++) {
		for (int y = 0; y < N; y++) {
			long double tmp = 0;
			for (int z = 0; z < N; z++)
				tmp += g[x][z] * g[y][z]; //  [f[z][y];
			a[x][y] = tmp;
			//cout << tmp;
		}
		//cout << endl;
	}
}

auto genStripe(int N, int L, double diap = 10, bool bad = false) {

	diap /= 4;

	long double** a = new long double* [N];
	long double** g = new long double* [N];
	for (int i = 0; i < N; i++) {
		a[i] = new long double[N] {0};
		g[i] = new long double[N] {0};
	}

	random_device rd;	mt19937 one;	one.seed(rd());
	uniform_real_distribution<> sym(1, diap);
	decltype(sym) norm(1/diap, 1);

	for (int i = 0; i < N; i++) {
		for (int k = max(0, i - L + 1); k <= i; k++) {
			if (k != i)
				//g[i][k] = sym(one);
				g[i][k] = bad ? fRand(1, diap) : fRand(0, diap);
			else {
				if (bad)
					g[i][k] = fRand(1 / diap, 1);//norm(one);
				else
					g[i][k] = fRand(0, diap) * diap;//sym(one);
			}
		}
	}

	multrans(g, a, N);

	//print(g,N);

	//print(a,N);

	vector<vector<long double>> matr(N);
	for (int i = 0; i < N; i++) {
		for (int k = 0; k < N; k++)
			matr[i].push_back(a[i][k]);
	}

	vector<long double> f;
	for (int i = 0; i < N; i++)
		f.push_back(fRand(0,0.1));//sym(one));

	return make_tuple(matr, f);
}

void task2(){
	vector<vector<long double>> a, t, m;
	vector<long double> f, x, X, F;

	n = 30;
	l = 3;
	auto inputData = genStripe(n, l, 10);//getData();  // получаем или генерируем ленточную матрицу
	m = std::get<0>(inputData);
	f = std::get<1>(inputData);
	F = vector<long double>(n, 0);

	cout << "Изначальная матрица:\n";
	//print(m);
	cout << "Столбец свободных членов:\n";
	//print(f);

	for (int i = 0; i < m.size(); i++) {
		for (int k = 0; k < m[i].size(); k++)
			F[i] += m[i][k];
		auto line = m[i];
		vector<long double> tmp;
		for (int k = 0; k < l; k++)
			(i + k) >= n ? tmp.push_back(0) : tmp.push_back(line[i + k]);
		a.push_back(tmp);
	}
	cout << endl << endl;
	cout << "А:\n";
	print(a);

	try {
		t = straight(a);
	}
	catch (double val) {
		throw val;
	}
	
	cout << "\nT:\n";
	print(t);
	auto tup = solve(t, f, F);
	x = std::get<0>(tup);
	X = std::get<1>(tup);
	print(X);
	vector<long double> res = multiply(a, x);
	long double Compare = compare(f, res);
	long double Delta = delta(X);
	cout << "compare=" << Compare;
	cout << " delta=" << Delta;
}

void task() {
	vector<vector<long double>> a, t;
	vector<long double> f, F, x, X;

	char ch;
	cout << "File?(y/n): ";
	cin >> ch;

	n = 10;
	l = 3;
	auto inputData = generateData(10);

	bool get = true;

	while (get) {
		if (ch == 'y') {
			inputData = getData();
		}
		a = std::get<0>(inputData);
		f = std::get<1>(inputData);
		try {
			t = straight(a);
			get = false;
		}
		catch (int) {
			cout << "Determinant is not > 0 ?!\n";
			get = true;
		}
	}

	auto tup = solve(t, f, F);
	x = std::get<0>(tup);
	X = std::get<1>(tup);
	for (auto num : x)
		cout << num << " ";
	vector<long double> res = multiply(a, x);
	long double delta = compare(f, res);
	cout << "delta=" << delta;
	/*
	ifstream in("2.txt");
	string buf;
	while (getline(in, buf)) {
		cout << buf << endl;
	}
	*/
}

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	srand(time(0));
	bool bad = true;
	while (bad) {
		try {
			task2();
			bad = false;
		}
		catch (int i) {
			system("cls");
		}
	}
}