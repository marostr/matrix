#ifndef aghComplex_H
	#define aghComplex_H
	#include <iostream>
	
	using namespace std;
	
class aghComplex {

private:
	double real, i;

public:

	aghComplex(double real=0, double i=0);
	
	
	
	aghComplex& operator=(aghComplex other); //przypisanie
	aghComplex operator+(aghComplex other); //dodawanie
	aghComplex operator-(aghComplex other); //odejmowanie
	aghComplex operator*(aghComplex other); //mnozenie
	aghComplex& operator+=(aghComplex other);
	aghComplex operator/(aghComplex other); //dzielenie
	bool operator==(aghComplex other) { return !(this->real != other.real || this->i != other.i); }
	bool operator!=(aghComplex other) { return !(*this == other); }
	
	friend ostream& operator<<(ostream &out, aghComplex &ref);
	aghComplex pow(int n); //potega
	
};


#endif
