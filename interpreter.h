#include <math.h>
#include <stdio.h>
const int n=31;//max number of characters
signed int interpreter (char napis[], int dlugoscnapis)
{
	//Liczby wrzucamy na jedna liste
	//znaki wrzucamy na druga liste
	//szukamy znakow o najwieksym priorytecie,
	//wykonujemy adekwatne do tego operacje
	//itd itd

	double liczby[31]={0};
	char znaki[31]={0};
	int licznik_liczb = 0;
	int licznik_znakow = 0;
	//zamiana z wejsciowej tablicy charow na liczby oraz znaki matematyczne
	for(int i=0; i<dlugoscnapis; i++)//szukamy mnozenia lub dzielenia
	{
		if(napis[i] >='0' && napis[i]<='9')
		{
			liczby[licznik_liczb]=liczby[licznik_liczb]*10 + napis[i]-'0';
		}
		else
		{
			znaki[licznik_znakow]=napis[i];
			licznik_znakow++;
			licznik_liczb++;
		}
		napis[i]=0;
	}
    licznik_liczb++;//Âżeby uwzglednil ostatnia liczbe w rownaniu

	//zamiania z int na iczby z przecinkami
	for(int i = 0; i< licznik_znakow; i++)
    {
        if(znaki[i] == '.')
        {
            int temp = liczby[i+1];
            for(; temp != 0; )
            {
                liczby[i+1]/=10;
                temp/=10;
            }
            liczby[i+1] +=liczby[i];
            for(int j = i; j<licznik_znakow; j++)
            {
                liczby[j] = liczby[j+1];
                znaki[j] = znaki[j+1];
            }
            //trzeba zmniejszyc licznik liczb i znakow
            licznik_liczb--;
            licznik_znakow--;
            i--;
        }
    }
	//szukamy potegowania
    for(int i = 0; i< licznik_znakow; i++)
    {
        if(znaki[i] == '^')
        {
            liczby[i+1]=pow(liczby[i], liczby[i+1]);
            for(int j = i; j<licznik_znakow; j++)
            {
                liczby[j] = liczby[j+1];
                znaki[j] = znaki[j+1];
            }
            //trzeba zmniejszyc licznik liczb i znakow
            licznik_liczb--;
            licznik_znakow--;
            i--;
        }
    }
	//szukamy mnozenia
    for(int i = 0; i< licznik_znakow; i++)
    {
        if(znaki[i] == '*')
        {
            liczby[i+1]*=liczby[i];
            for(int j = i; j<licznik_znakow; j++)
            {
                liczby[j] = liczby[j+1];
                znaki[j] = znaki[j+1];
            }
            //trzeba zmniejszyc licznik liczb i znakow
            licznik_liczb--;
            licznik_znakow--;
            i--;
        }
    }
	//szukamy dzielenia
    for(int i = 0; i< licznik_znakow; i++)
    {
        if(znaki[i] == '/')
        {
            liczby[i+1]/=liczby[i];
            for(int j = i; j<licznik_znakow; j++)
            {
                liczby[j] = liczby[j+1];
                znaki[j] = znaki[j+1];
            }
            //trzeba zmniejszyc licznik liczb i znakow
            licznik_liczb--;
            licznik_znakow--;
            i--;
        }
    }
	//szukamy dodawania
    for(int i = 0; i< licznik_znakow; i++)
    {
        //cout <<"\nZnak " << i << " to " << znaki[i];
        if(znaki[i] == '+')
        {
            liczby[i+1]+=liczby[i];
            for(int j = i; j<licznik_znakow; j++)
            {
                liczby[j] = liczby[j+1];
                znaki[j] = znaki[j+1];
            }
            //trzeba zmniejszyc licznik liczb i znakow
            licznik_liczb--;
            licznik_znakow--;
            i--;
        }
    }
	//szukamy odejmowania
    for(int i = 0; i< licznik_znakow; i++)
    {
        if(znaki[i] == '-')
        {
            liczby[i+1]-=liczby[i];
            for(int j = i; j<licznik_znakow; j++)
            {
                liczby[j] = liczby[j+1];
                znaki[j] = znaki[j+1];
            }
            //trzeba zmniejszyc licznik liczb i znakow
            licznik_liczb--;
            licznik_znakow--;
            i--;
        }
    }
    double wynikdouble = liczby[0];
	signed int dlugoscchar = sprintf(napis, "%g", wynikdouble);
	return dlugoscchar;
}
