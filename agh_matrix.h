 #ifndef AGHM_H
	#define AGHM_H
	#include <iostream>
	#include <cstdarg>
	#include "aghException.h"
	using namespace std;

	
	
template <typename TYPE> 
class aghMatrix {

	TYPE** matrix;
	int rows,cols;

public:
	
	aghMatrix(int rows=10,int cols=10);
	aghMatrix(const aghMatrix&);
	~aghMatrix();
	
	void print(void) const;
	void setItem(int, int, TYPE);
	void setItems(TYPE*);
	void setItems(int , int , ...);
		
	TYPE& operator()(int ,int);
	aghMatrix<TYPE>& operator=(const aghMatrix<TYPE>);
	bool operator==(const aghMatrix<TYPE>&);
	bool operator!=(const aghMatrix<TYPE>&);
	aghMatrix<TYPE> operator+(const aghMatrix<TYPE>);
	aghMatrix<TYPE> operator*(const aghMatrix<TYPE>&);
	
	
	
};	

//--------------------------------------------------------------------------------------------------



template <typename TYPE>
aghMatrix<TYPE>::aghMatrix(int _rows, int _cols)
: rows(_rows), cols(_cols)
{
	if (_rows < 1 || _cols <1) throw aghException(-1, "Wrong size of matrix!");

	this->matrix = new TYPE*[this->rows];

	for(int i=0; i<this->rows; i++)
		this->matrix[i] = new TYPE[this->cols];
	
}

//--------------------------------------------------------------------------------------------------
template <typename TYPE>
aghMatrix<TYPE>::~aghMatrix()
{
	for(int i=0; i <this->rows; i++)
		delete [] matrix[i];

	delete [] matrix;

	matrix = NULL;
	this->cols  = 0;
	this->rows = 0;
}

//--------------------------------------------------------------------------------------------------
template <typename TYPE>
aghMatrix<TYPE>::aghMatrix(const aghMatrix &wzor)
{
	this->rows = wzor.rows;
	this->cols = wzor.cols;
	
	this->matrix = new TYPE*[this->rows];

	for(int i=0; i<this->rows; i++)
		this->matrix[i] = new TYPE[this->cols];

	for(int i=0; i<this->rows; i++)
		for(int k=0; k<this->cols; ++k)
			this->matrix[i][k] = wzor.matrix[i][k]; 
}

//--------------------------------------------------------------------------------------------------
template <typename TYPE>
void aghMatrix<TYPE>::print(void) const
{
	for(int i=0; i<this->rows; i++)
	{
		for(int k=0; k<this->cols; ++k)
			std::cout<< this->matrix[i][k] << ' ';
		std::cout<<'\n';	 
	}
	std::cout<<"\t\t ::"<<rows<<"x"<<cols<<"::\n"; //!!!!!!!!!!
}

//--------------------------------------------------------------------------------------------------
template <typename TYPE>
void aghMatrix<TYPE>::setItem(int _w, int _h, TYPE obj)
{
	if (_w < 1 || _h <1) throw aghException(-1, "Wrong size of matrix!");

	this->matrix[_w][_h] = obj;
}

//--------------------------------------------------------------------------------------------------

template <> inline
void aghMatrix<char*>::setItem(int _w, int _h, char* obj)
{
	//kasuje stary napis i alokuje miejsce na nowy
	delete this->matrix[_w][_h]; 
	this->matrix[_w][_h] = new char[strlen(obj)+1];
	strcpy(this->matrix[_w][_h],obj);
	
}

//--------------------------------------------------------------------------------------------------

template <typename TYPE>
void aghMatrix<TYPE>::setItems(TYPE* const _source)
{	
	int k=0;

	for(int i=0; ; i++)
	{
	if ( i ==  (this->cols)*(this->rows) ) return ;
	if (i%cols == 0 && i>0) k++;
 
	this->matrix[k][i%cols] = _source[i];
	}
}

//--------------------------------------------------------------------------------------------------
template <typename TYPE>
void aghMatrix<TYPE>::setItems(int r,int c, ...)
{
	(*this).~aghMatrix();  //usuwanie starej macierzy, tworzenie nowej

	this->rows = r;
	this->cols = c;

	matrix = new TYPE*[r];
	for(int i=0; i<r; ++i) 
		matrix[i] = new TYPE [c];
	
	va_list ap;		//wczytywanie wartości z listy inicjalizacyjnej do macierzy
	va_start(ap,c);

	for(int i=0; i<r; ++i)
		for(int k=0; k<c; ++k)
		{
			this->matrix[i][k] = va_arg(ap, TYPE);
		}

	va_end(ap);
}

//--------------------------------------------------------------------------------------------------

template <> inline
void aghMatrix<char>::setItems(int r,int c, ...)
{
//specjalny przypadek listy inicjalizacyjnej dla typu char, gdyz trzeba 
//go odczytywac z listy jako int
	(*this).~aghMatrix();

	this->rows = r;
	this->cols = c;

	matrix = new char*[r];
	for(int i=0; i<r; ++i) 
		matrix[i] = new char[c];
	
	va_list ap;
	va_start(ap,c);

	for(int i=0; i<r; ++i)
		for(int k=0; k<c; ++k)
		{
			this->matrix[i][k] = va_arg(ap, int);
		}
	
	va_end(ap);
}

//--------------------------------------------------------------------------------------------------

template <typename TYPE>
TYPE& aghMatrix<TYPE>::operator()(int _col,int _row)
{
	if(_row >= rows || _col >= cols || _row <0 || _col<0) throw aghException(0,"index out of range!\n");

	return this->matrix[_col][_row]; //zwraca zadany wyraz
}	

//--------------------------------------------------------------------------------------------------

template <typename TYPE>
bool aghMatrix<TYPE>::operator==(const aghMatrix& M)
{
	//porownanie macierzy na zasadzie: wymiary i kazdy wyraz z kazdym 
	if(M.cols != this->cols || M.rows != this->rows) return false;

	for(int i=0; i < this->rows; ++i)
		for(int k=0; k <this->cols; ++k)
			if((*this).matrix[i][k] != M.matrix[i][k]) return false;

return true;
}

//--------------------------------------------------------------------------------------------------

template <> inline
bool aghMatrix<char*>::operator==(const aghMatrix &M)
{
	//specjalny przypadek operatora == dla typu char*
	//uzywany strcmp
	if(M.cols != this->cols || M.rows != this->rows) return false;

	for(int i=0; i < this->rows; i++)
		for(int k=0; k <this->cols; k++)
			if( strcmp( (*this).matrix[i][k], M.matrix[i][k]) ) return false;
	
return true;
}
//----------------------------------------------------------------------------------------------------

template <typename TYPE>
bool aghMatrix<TYPE>::operator!=(const aghMatrix<TYPE>& M)
{
return !(*this==M);
}	

//----------------------------------------------------------------------------------------------------

template <typename TYPE>
aghMatrix<TYPE>& aghMatrix<TYPE>::operator=(aghMatrix<TYPE> m)
{

	(*this).~aghMatrix(); //desktruktor obiektu this, aby byla mozliwosc utworzenia nowej macierzy
 
	this->rows = m.rows; //wpisywanie nowych wartosci
	this->cols = m.cols;
 
	this->matrix = new TYPE*[this->rows]; //tworzenie nowej macierzy
 
	for(int i=0; i<this->rows; i++) //alokowanie miejsca na macierz
			this->matrix[i] = new TYPE[this->cols];
 
	for(int i=0; i<this->rows; i++)
			for(int k=0; k<this->cols; k++) //przepisywanie po kolei wartosci
                this->matrix[i][k] = m.matrix[i][k];
                
return *this; 
}

//-----------------------------------------------------------------------------------------------------

template <> inline
aghMatrix<char*>& aghMatrix<char*>::operator=(aghMatrix<char*> m)
{
	//specjalny przypadek operatora przypisania
	//kopiowanie stringow zamiast przepisywania wyrazow
	(*this).~aghMatrix();
 
	this->rows = m.rows;
	this->cols = m.cols;
 
	this->matrix = new char**[this->rows];
 
	for(int i=0; i<this->rows; i++)
			this->matrix[i] = new char*[this->cols];
        
	for(int i=0; i<this->rows; i++)
		for(int k=0; k<this->cols; k++)
		{
		this->matrix[i][k] = new char[strlen(m.matrix[i][k])+1];	
		strcpy(this->matrix[i][k], m.matrix[i][k]);
		}
                
return *this;
}


//--------------------------------------------------------------------------------------------------

template <typename TYPE>
aghMatrix<TYPE> aghMatrix<TYPE>::operator+(const aghMatrix<TYPE> M)
{
	if ( (this->rows != M.rows) || (this->cols != M.cols) ) 
	throw aghException(0,"Cols and/or rows do not match!\n");

	aghMatrix<TYPE> Result(M.rows,M.cols);

	for(int i=0; i < this->rows; i++) //zwykly algorytm na dodawanie macierzy
        for(int k=0; k < this->cols; k++)
			Result.matrix[i][k] = ( (*this).matrix[i][k] + M.matrix[i][k] );

return Result;
}

//--------------------------------------------------------------------------------------------------

template <typename TYPE>
aghMatrix<TYPE> aghMatrix<TYPE>::operator*( const aghMatrix<TYPE>& M)
{
	if( (*this).cols != M.rows ) throw aghException(0,"wrong matrix size!\n");

	aghMatrix<TYPE> m( (*this).rows, M.cols ); 
	for( int i=0; i<m.rows; i++ )
		for( int j=0; j<m.cols; j++ )
			m.matrix[i][j] = 0; 		//uzupelnianie 0 nowo utworzonej macierzy

	for( int i=0; i<m.rows; i++ )
		for( int j=0; j<m.cols; j++ ) 
			for( int k=0; k<(*this).cols; k++ )
				m.matrix[i][j] += (*this).matrix[i][k] * M.matrix[k][j]; //zwykly algorytm na mnozenie macierzy

	return m;
}

//--------------------------------------------------------------------------------------------------

template <> inline
aghMatrix<char> aghMatrix<char>::operator*(const aghMatrix<char>& M)
{
	if( (*this).cols != M.rows ) throw aghException(0,"wrong matrix size!\n");

	aghMatrix<char> m( (*this).rows, M.cols );
	for( int i=0; i<m.rows; i++ )
		for( int j=0; j<m.cols; j++ )
			m.matrix[i][j] = 0;

	for( int i=0; i<m.rows; i++ )
		for( int j=0; j<m.cols; j++ ) 
		{
			for( int k=0; k<(*this).cols; k++ )
			{
				m.matrix[i][j] += ( (((*this).matrix[i][k]-97) * (M.matrix[k][j]-97)  )%26); //mnozenie liter jak w zwyklej macierzy
			}																				 //z uwzglednieniem przesuniecia (97) char i modulo
			m.matrix[i][j] = (m.matrix[i][j] % 26) + 97;
		}
return m;
}

//--------------------------------------------------------------------------------------------------
 
template <> inline
aghMatrix<char> aghMatrix<char>::operator+(const aghMatrix<char> M)
{
    if ( (this->rows != M.rows) || (this->cols != M.cols) )
                throw aghException(-1,"colss and/or rowss do not match!\n");
 
	aghMatrix<char> Result(M.rows,M.cols);

	for(int i=0; i < this->rows; ++i)
        for(int k=0; k < this->cols; ++k)
        {
			Result.matrix[i][k] = ( (*this).matrix[i][k]-97 + M.matrix[i][k]-97 ) % 26; //dodawanie liter jak w zwyklej macierzy
			Result.matrix[i][k] += 97; 													//z uwzglednieniem przesuniecia (97) char i modulo
		}
 
return Result;
}	

//--------------------------------------------------------------------------------------------------

template <> inline
aghMatrix<char*> aghMatrix<char*>::operator+(const aghMatrix<char*> M)
{
	if ( (this->rows != M.rows) || (this->cols != M.cols) )
		throw aghException(-1,"colss and/or rowss do not match!\n");
	
	aghMatrix<char*> Result(M.rows, M.cols);
	string temp1, temp2;
	
		for(int i=0; i < this->rows; ++i)
			for(int k=0; k < this->cols; ++k)
			{ 
				temp1 = string((*this).matrix[i][k]); //zamiana char* na string
				temp2 = string(M.matrix[i][k]);
				temp1 += temp2; //laczenie stringow
	
				for(int m=0; m< temp1.length(); ++m) 
							for(int n=m+1; n<temp1.length(); ++n)
								if(temp1[m]  == temp1[n]) temp1.erase(temp1.begin()+n); //usuwanie powtorzen ze stringow po dodawaniu     
          
            Result.matrix[i][k] = new char[temp1.length()+1]; //alokujemy odpowiednia ilosc pamieci na string w macierzy wynikowej
			strcpy (Result.matrix[i][k], temp1.c_str()); //zamiana string na char*, dodanie znaku konca stringu
			Result.matrix[i][k][temp1.length()] = '\0';	
			}

return Result;
}

//--------------------------------------------------------------------------------------------------

template <> inline
aghMatrix<char*> aghMatrix<char*>::operator*(const aghMatrix<char*>& M)
{
        if( (*this).cols != M.rows ) throw aghException(0,"wrong matrix size!\n");
 
        aghMatrix<char*> Result( (*this).rows, M.cols );
        unsigned found;
        string temp1, temp2, res1;
       
        for( int i=0; i<Result.rows; i++ )
        {
                for( int j=0; j<Result.cols; j++ )
                {
                        for( int k=0; k<(*this).cols; k++ )
                        {              
                                temp1 = string((*this).matrix[i][k]); //przepisywanie wyrazow do string
                                temp2 = string(M.matrix[k][j]);
                                for(int l=0; l <= temp1.length(); l++) //petla odpowiedzialna za 'mnozenie'
                                {
                                        found = temp2.find(temp1[l], 1); //szukanie tych samych liter w drugim stringu (algorytm 'mnozenia' wyrazow)
                                        if ( found>0 && found <= temp2.length() )
											res1.push_back( temp2.at(found) ); //jesli znajdzie to dodaje (algorytm 'dodawania' wyrazow) litere do stringa wynikowego
                                }      
                                temp1.clear(); temp2.clear();
                           
                        }
                                for(int m=0; m< res1.length(); ++m) 
									for(int n=m+1; n<res1.length(); ++n)
										if(res1[m]  == res1[n]) res1.erase(res1.begin()+n); //usuwanie podwojnych liter w stringu wynikowym (dokonczenie dodawania)
                      
                        Result.matrix[i][j] = new char[res1.length()+1]; //alokacja pamieci dla stringa wynikowego w macierzy wynikowej
                        strcpy (Result.matrix[i][j], res1.c_str()); //kopiowanie stringa wynikowego do macierzy, zamiana string na char*
                        Result.matrix[i][j][res1.length()]='\0';
                        res1.clear();
                }
         }
return Result;
}




#endif
