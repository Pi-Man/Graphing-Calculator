#ifndef MATH_CPP
#define MATH_CPP
#define _USE_MATH_DEFINES
#include<string>
#include<sstream>
#include "math.h"
using namespace std;

Number::Number() : real(0), imaginary(0) {};
Number::Number(double x, double y) : real(x), imaginary(y) {}

bool Number::isReal() {
	return abs(imaginary) < 1e-10;
}

bool Number::isImaginary() {
	return abs(real) < 1e-10;
}

Number Number::operator+ (const Number& other) {
	return Number(real + other.real, imaginary + other.imaginary);
}

Number Number::operator- (const Number& other) {
	return Number(real - other.real, imaginary - other.imaginary);
}

Number Number::operator* (const Number& other) {
	return Number(real * other.real - imaginary * other.imaginary, real * other.imaginary + imaginary * other.real);
}

Number Number::operator/ (const Number& other) {
	double x = other.real * other.real + other.imaginary * other.imaginary;
	return Number((real * other.real + imaginary * other.imaginary) / x, (real * other.imaginary + imaginary * other.real) / x);
}

extern Number k;

Number Number::operator^ (const Number& other) {

	double r = real * real + imaginary * imaginary;
	double theta = atan2(imaginary, real);

	if (r == 0) {
		if (other.real == 0 && other.imaginary == 0) {
			return NAN;
		}
		else {
			return 0;
		}
	}

	Number z(log(r) / 2, theta + 2*(k)*M_PI);

	Number z2 = z * other;

	double r2 = pow(M_E, z2.real);

	double x = r2 * cos(z2.imaginary);
	double y = r2 * sin(z2.imaginary);

	return Number(x, y);
}

Number Number::operator% (const Number& other) {
	return Number(fmod(real, other.real));
}

Number& Number::operator+= (const Number& other) {
	real += other.real;
	imaginary += other.imaginary;
	return *this;
}

Number& Number::operator-= (const Number& other) {
	real -= other.real;
	imaginary -= other.imaginary;
	return *this;
}

Number& Number::operator*= (const Number& other) {
	double r = real * real + imaginary * imaginary;
	double theta = atan2(imaginary, real);

	if (r == 0) {
		if (other.real == 0 && other.imaginary == 0) {
			real = NAN;
			imaginary = 0;
			return *this;
		}
		else {
			real = 0;
			imaginary = 0;
			return *this;
		}
	}

	Number z(log(r) / 2, theta + 2 * (k)*M_PI);

	Number z2 = z * other;

	double r2 = pow(M_E, z2.real);

	real = r2 * cos(z2.imaginary);
	imaginary = r2 * sin(z2.imaginary);

	return *this;
}

Number& Number::operator/= (const Number& other) {
	double x = other.real * other.real + other.imaginary * other.imaginary;
	real = (real * other.real + imaginary * other.imaginary) / x;
	imaginary = (real * other.imaginary + imaginary * other.real) / x;
	return *this;
}

Number& Number::operator^= (const Number& other) {
	real = pow(real, other.real);
	return *this;
}

Number::operator double() const {
	return real;
}

Vec2::Vec2(Number xIn, Number yIn) : x(xIn), y(yIn) {}

string Vec2::toString() {
	stringstream stream;
	stream << "{x: " << x.real << ", y: " << y.real << "}";
	return stream.str();
}

Number Math::clamp(Number value, Number min, Number max) {
	if (value < min) {
		return min;
	}
	if (value > max) {
		return max;
	}
	return value;
}

int Math::mod(int base, int dividend) {
	while (base < 0) {
		base += dividend;
	}
	return base % dividend;
}

#endif