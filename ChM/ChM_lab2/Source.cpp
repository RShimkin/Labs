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

// НЕ РАБОТАЕТ
auto generateData(int diap) {

	double dif = diap / 5;
	auto magic = [](vector<vector<double>> a) {
		vector<vector<double>> res;

	};

	vector<vector<double>> a;
	vector<double> f;

	random_device rd;
	mt19937 one;
	one.seed(rd());
	uniform_real_distribution<> urd(-diap+dif, diap-dif);
	//decltype(urd) sec(diap - dif, diap);
	decltype(urd) sec(0, 1/diap); 
	
	for (int i = 0; i < n; i++) {
		vector<double> tmp;
		tmp.push_back(sec(one));
		for (int k = 1; k < l; k++)
			tmp.push_back(urd(one));
		a.push_back(tmp);
		f.push_back(urd(one));
	}

	return make_tuple(a, f);
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

	vector<vector<double>> a;
	vector<double> f;

	string buf;
	getline(in, buf);
	for (int i = 0; i < n; i++) {
		getline(in, buf);
		if (buf.size()) {
			auto line = split(buf, ' ');
			vector<double> tmp;
			for (int k = 0; k < l; k++)
				(i + k) >= n ? tmp.push_back(0) : tmp.push_back(atof(line[i + k].c_str()));
			a.push_back(tmp);
			f.push_back(atof(line[line.size() - 1].c_str()));
		}
	}
	in.close();

	return make_tuple(a, f);
}

auto straight(vector<vector<double>> a) {
	auto k0 = [](int i) -> int {
		return max(0, i - l + 1);
	};
	auto kn = [](int i) -> int {
		return min(n - 1, i + l - 1);
	};

	vector<vector<double>> t;
	float tmp;
	for (int i = 0; i < n; i++) {
		//cout << ">> i=" << i << endl;
		vector<double> cur;
		tmp = 0;

		int k0i = k0(i);
		//cout << "ko=" << k0i << "\t";
		for (int k = k0i; k <= i - 1; k++)
			tmp += t[k][i - k] * t[k][i - k];
		//cout << "tmp=" << tmp << " ";
		tmp = sqrt(a[i][0] - tmp);
		//cout << tmp << endl;
		cur.push_back(tmp);

		float num = kn(i);
		for (int j = i + 1; j <= num; j++) {
			tmp = 0;
			for (int k = k0(j); k <= i - 1; k++)
				tmp += t[k][i - k] * t[k][j - k];
			tmp = (a[i][j - i] - tmp) / cur[0];
			cur.push_back(tmp);
		}
		int leng = cur.size();
		for (int k = leng; k < l; k++)
			cur.push_back(0);
		t.push_back(cur);
	}

	return t;
}

auto solve(vector<vector<double>> t, vector<double> f, vector<double> F) {
	vector<double> y, Y;
	float tmp, TMP;
	for (int i = 0; i < n; i++) {
		TMP = tmp = 0;
		for (int k = 0; k <= i - 1; k++) {
			int s = i - k;
			if (s < l) {
				tmp += (t[k][i - k] * y[k]);
				TMP += (t[k][i - k] * Y[k]);
			}
			else {
				tmp += 0;
				TMP += 0;
			}	
			//cout << "k=" << k << "\ti-k=" << i - k << endl;
		}
		tmp = (f[i] - tmp) / t[i][0];
		TMP = (F[i] - TMP) / t[i][0];
		y.push_back(tmp);
		Y.push_back(TMP);
	}

	vector<double> x(n), X(n);
	for (int i = n - 1; i >= 0; i--) {
		TMP = tmp = 0;
		for (int k = i + 1; k < n; k++) {
			int s = k - i;
			if (s < l) {
				tmp += (t[i][s] * x[k]);
				TMP += (t[i][s] * X[k]);
			}
			else {
				tmp += 0;
				TMP += 0;
			}	
		}
		tmp = (y[i] - tmp) / t[i][0];
		x[i] = tmp;
		TMP = (Y[i] - TMP) / t[i][0];
		X[i] = TMP;
	}

	return make_tuple(x, X);
}

auto multiply(vector<vector<double>> a, vector<double> x) {
	vector<double> res(n);
	for (int i = 0; i < n; i++) {
		double temp = 0;
		for (int k = (i - l + 1) < 0 ? 0 : i - l + 1 ; k < i; k++)
			temp += a[k][i-k] * x[k];
		for (int k = i; k < i + l && k < n; k++)
			temp += a[i][k - i] * x[k];
		res[i] = temp;
	}
	return res;
}

double compare(vector<double> f, vector<double> res) {
	double def = 0;
	double temp = 0;
	for (int i = 0; i < n; i++) {
		temp = f[i] - res[i];
		def += temp * temp;
	}
	return sqrt(def);
}

double delta(vector<double> F) {
	double curmax = 0;
	for (int i = 0; i < F.size(); i++) {
		curmax = max(abs(F[i] - 1), curmax);
	}
	return curmax;
}

void print(vector<vector<double>> m) {
	for (auto line: m) {
		for (auto x: line)
			cout << x << " ";
		cout << endl;
	}
	cout << endl;
}

void print(vector<double> v, bool vert = false) {
	for (auto x : v) {
		cout << x << " ";
		if (vert) cout << "\n";
	}
	cout << endl;
}

void multrans(double** g, double** a, int N) {
	for (int x = 0; x < N; x++) {
		for (int y = 0; y < N; y++) {
			double tmp = 0;
			for (int z = 0; z < N; z++)
				tmp += g[x][z] * g[y][z]; //  [f[z][y];
			a[x][y] = tmp;
			//cout << tmp;
		}
		//cout << endl;
	}
}

auto genStripe(int N, int L, int diap = 10, bool bad = false) {

	double** a = new double* [N];
	double** g = new double* [N];
	for (int i = 0; i < N; i++) {
		a[i] = new double[N] {0};
		g[i] = new double[N] {0};
	}

	random_device rd;	mt19937 one;	one.seed(rd());
	uniform_real_distribution<> sym(1, diap);
	decltype(sym) norm(1/diap, 1);

	for (int i = 0; i < N; i++) {
		for (int k = max(0, i - L + 1); k <= i; k++) {
			if (k != i)
				g[i][k] = sym(one);
			else {
				if (bad)
					g[i][k] = norm(one);
				else 
					g[i][k] = sym(one);
			}
		}
	}

	multrans(g, a, N);

	//print(g,N);

	//print(a,N);

	vector<vector<double>> matr(N);
	for (int i = 0; i < N; i++) {
		for (int k = 0; k < N; k++)
			matr[i].push_back(a[i][k]);
	}

	vector<double> f;
	for (int i = 0; i < N; i++)
		f.push_back(sym(one));

	return make_tuple(matr, f);
}

void task2(){
	vector<vector<double>> a, t, m;
	vector<double> f, x, X, F;

	n = 20;
	l = 3;
	auto inputData = genStripe(n, l, 10);//getData();  // получаем или генерируем ленточную матрицу
	m = std::get<0>(inputData);
	f = std::get<1>(inputData);
	F = vector<double>(n, 0);

	cout << "Изначальная матрица:\n";
	print(m);
	cout << "Столбец свободных членов:\n";
	print(f);

	for (int i = 0; i < m.size(); i++) {
		for (int k = 0; k < m[i].size(); k++)
			F[i] += m[i][k];
		auto line = m[i];
		vector<double> tmp;
		for (int k = 0; k < l; k++)
			(i + k) >= n ? tmp.push_back(0) : tmp.push_back(line[i + k]);
		a.push_back(tmp);
	}
	cout << endl << endl;
	cout << "А:\n";
	print(a);

	t = straight(a);
	print(t);
	auto tup = solve(t, f, F);
	x = std::get<0>(tup);
	X = std::get<1>(tup);
	for (auto num : x)
		cout << num << " ";
	vector<double> res = multiply(a, x);
	double Compare = compare(f, res);
	double Delta = delta(X);
	cout << "compare=" << Compare;
	cout << " delta=" << Delta;
}

void task() {
	vector<vector<double>> a, t;
	vector<double> f, F, x, X;

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
	vector<double> res = multiply(a, x);
	double delta = compare(f, res);
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
	task2();
}