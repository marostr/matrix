#include "aghComplex.h"




aghComplex::aghComplex(double real, double i)
{
this->real=real;
this->i=i;
}


aghComplex& aghComplex::operator=(aghComplex wzor)
{
this->real = wzor.real;
this->i    = wzor.i;

	return *this;
} 

ostream& operator<<(ostream& out,aghComplex &ref)
{
out<< ref.real << 'i' << ref.i;
return out;
}

aghComplex aghComplex::operator+(aghComplex other)
{
aghComplex r;	
	
r.real=this->real+other.real;
r.i = this->i+other.i;
return r;
}

aghComplex aghComplex::operator-(aghComplex other)
{
aghComplex r;	
	
r.real = this->real-other.real;
r.i    = this->i-other.i;
	return r;
}

aghComplex& aghComplex::operator+=(aghComplex other)
{
(*this).real += other.real;
(*this).i	 += other.i;

return *this;	
}

aghComplex aghComplex::operator*(aghComplex other)
{
aghComplex r;	
	

r.real=(this->real)*(other.real) - (this->i)*(other.i);
r.i=this->real*(other.i)+(other.real)*(this->i);
	return r;

} 

	
	
aghComplex aghComplex::operator/(aghComplex other)
{
double a,b,c,d;

a=(*this).real;
b=(*this).i;
c=other.real;
d=other.i;

aghComplex r;

r.real =  (a*c+b*d)/(c*c+d*d);
r.i    =  (b*c-a*d)/(c*c+d*d);

return r;	
}

aghComplex aghComplex::pow(int n)
{
aghComplex res;
	
if(n==1) return (*this);
else res=res*pow(n-1);

return res;
}
