#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>
#include "LongDouble.h"

using namespace std;

#define uc unsigned char
#define ld LongDouble

const uc offset = 20;

auto task(int var, bool debug) {
	uc num = var + offset;

	float prevf, elemf = static_cast<float>(1 / M_E);
	double prevd, elemd = static_cast<double>(1 / M_E);
	ld prevld(0);
	ld exp("2.7182818284590452353602874713526624977572");
	cout << exp * 2 << "\n";
	ld elemld(ld(1) / exp);

	for (uc i = 2; i < num + 2; i++) {
		if (debug) cout << (int)i-1 << "  " << elemf << "    " << elemd << "    " << elemld << "\n";
		prevf = elemf;
		prevd = elemd;
		prevld = elemld;
		elemld = ld(1) - ld(int(i)) * prevld;
		elemf = 1 - i * prevf;
		elemd = 1 - i * prevd;
	}

	cout << "float: " << prevf << "\t" << elemf << "\n";
	cout << "double: " << prevd << "\t" << elemd;
}

int main() {
	task(16, true);
}