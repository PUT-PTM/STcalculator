#include <math.h>
#include <stdio.h>
const int n=31;//max number of characters
int interpreter (char napis[], int dlugoscnapis)
{
	/*
	Liczby wrzucamy na jedna liste
	znaki wrzucamy na druga liste
	szukamy znakow o najwieksym priorytecie,
	wykonujemy adekwatne do tego operacje
	itd itd
	*/
	double liczby[30]={0};//
	char znaki[30]={0};//
	int licznik_liczb = 0;//
	int licznik_znakow = 0;//
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
	}
    licznik_liczb++;//zeby uwzglednil ostatnia liczbe w rownaniu

    //odtad zaczynaja sie obliczenia
    //tu moga byc operacje jednoargumentowenp sin cos
    //tu sie juz koncza
    while(licznik_znakow>= licznik_liczb)//przy oper dwuargumentowych musi byc o 1 mniej znakow od liczb
    {
    	for(int i=0; i<licznik_znakow; i++)
    		znaki[i]=znaki[i+1];
    	licznik_znakow--;
    }

	//zamienia z int na iczby z przecinkami
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
    for(int i = 0; i< licznik_znakow; i++)
    {
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
    double wynikdouble;
    /////////////////////////////////////
    //zamiania z double na tablice charow
    wynikdouble=liczby[0];
    int part1=wynikdouble;//bedzie reprezentowac czesc calkowita
    char wynikchar1[31]={0};//bedzie reprezentowac czesc calkowita
    //zapisanie wyniku jako tablica char
    //szukamy czesci calkowitej
	int dlugoscchar1=0;
	while(part1 != 0)
    {
        for(int i=dlugoscchar1-1; i>=0; i--)
        {
            wynikchar1[i+1] = wynikchar1[i];
        }
        wynikchar1[0] = (part1%10) + '0';
        part1 /= 10;
        dlugoscchar1++;
    }

    //szukamy czesci po przecinku
    double part2=wynikdouble;//bedzie reprezentowac czesc po przecinku
    char wynikchar2[31]={0};//bedzie reprezentowac czesc po przecinku
    part2 -= floor(part2);//odejmujemy zeby zostala tylko czes po przecinku
    int dlugoscchar2=0;
	for(int i=0; part2 != 0 && i<5 ; i++)//dokladnosc do 5 miejsca po przecinku
    {
        part2 *= 10;//przesuwamy wartozs za przecinkiem przed przecinek
        wynikchar2[i] = floor(part2) + '0';
        part2 = part2 - floor(part2);//odejmujemy zeby zostala tylko czes po przecinku
        dlugoscchar2++;
    }
    //uciecie zer nieznaczacych na koncu
    for(int i=4; i>=0; i--)
    {
        if(wynikchar2[i] == '0')
        {
            wynikchar2[i] = 0;
            dlugoscchar2--;
        }
        else
        {
            break;
        }
    }

    //czuszczenie tablicy znakow i napisu
    for(int i=0; i<n; i++)
    {
    	napis[i]=0;
    	znaki[i]=0;
    }

    //skladanie dwoch tablic w jedna
    int dlugoscchar= dlugoscchar1 + dlugoscchar2+1;

    //przepisanie pierwszej tablicy do wyniku
    int licz;
    for(licz=0; licz<dlugoscchar1; licz++)
    {
     	napis[licz]=wynikchar1[licz];
    }

    //dodanie kropki
    napis[licz]='.';
    licz++;

    //przepisanie drugiej tablicy do wyniku
    for(int i=0; i<dlugoscchar2; i++, licz++)
    {
    	napis[licz]=wynikchar2[i];
    }

    //jesli nie ma wartosci po przecinku to trzeba wyswietlic chociaz jedno zero
    if(napis[dlugoscchar-1] == '.')
    {
    	napis[licz]='0';
    	licz++;
    	dlugoscchar++;
    }
    return dlugoscchar;
}
