#include <math.h>
#include <stdio.h>
const int n=29;//max number of characters
int interpreter (char napis[], int dlugoscnapis)
{
	double liczby[29]={0};
		char znaki[29]={0};
		int licznik_liczb = 0;
		int licznik_znakow = 0;
		//zamiana napisow typu sin cos tg ctg na litery q w e r
		for(int i = 0; i+2< dlugoscnapis; i++)
	    {
	        if(napis[i] == 's' && napis[i+1] == 'i' && napis[i+2] == 'n')//wykrywanie sin
	        {
	            napis[i]='q';
	            napis[i+1]=0;
	            napis[i+2]=0;
	            for(int j = i+1; (j+2)<dlugoscnapis; j++)
	            {
	                napis[j] = napis[j+2];
	            }
	            //trzeba zmniejszyc wartosc dlugoscnapis
	            dlugoscnapis-=2;

	            //czyszczenie reszty napisu
	            for(int i=dlugoscnapis; i<n; i++)
	            {
	                napis[i]=0;
	            }
	            i--;
	        }
	        else if(napis[i] == 'c' && napis[i+1] == 'o' && napis[i+2] == 's')//wykrywanie cos
	        {
	            napis[i]='w';
	            napis[i+1]=0;
	            napis[i+2]=0;
	            for(int j = i+1; (j+2)<dlugoscnapis; j++)
	            {
	                napis[j] = napis[j+2];
	            }
	            //trzeba zmniejszyc wartosc dlugoscnapis
	            dlugoscnapis-=2;

	            //czyszczenie reszty napisu
	            for(int i=dlugoscnapis; i<n; i++)
	            {
	                napis[i]=0;
	            }
	            i--;
	        }
	        else if(napis[i] == 'c' && napis[i+1] == 't' && napis[i+2] == 'g')//wykrywanie ctg
	        {
	            napis[i]='r';
	            napis[i+1]=0;
	            napis[i+2]=0;
	            for(int j = i+1; (j+2)<dlugoscnapis; j++)
	            {
	                napis[j] = napis[j+2];
	            }
	            //trzeba zmniejszyc wartosc dlugoscnapis
	            dlugoscnapis-=2;

	            //czyszczenie reszty napisu
	            for(int i=dlugoscnapis; i<n; i++)
	            {
	                napis[i]=0;
	            }
	            i--;
	        }
	        else if(napis[i] == 't' && napis[i+1] == 'g')//wykrywanie tg
	        {
	            napis[i]='e';
	            napis[i+1]=0;
	            for(int j = i+1; (j+1)<dlugoscnapis; j++)
	            {
	                napis[j] = napis[j+1];
	            }
	            //trzeba zmniejszyc wartosc dlugoscnapis
	            dlugoscnapis-=1;

	            //czyszczenie reszty napisu
	            for(int i=dlugoscnapis; i<n; i++)
	            {
	                napis[i]=0;
	            }
	            i--;
	        }
	    }

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

		//zamienia z int na liczby z przecinkami
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
	                //cout << "\nStara liczba " << liczby[j] << "->" << liczby[j+1];
	                liczby[j] = liczby[j+1];
	                //cout << "\nStary znak " << znaki[j] << "->" << znaki[j+1];
	                znaki[j] = znaki[j+1];
	            }
	            //trzeba zmniejszyc licznik liczb i znakow
	            licznik_liczb--;
	            licznik_znakow--;
	            i--;
	        }
	    }

	    ////////////////////////////////////////////////////////////
	    ///SINUS //if it was hard to write it should be EASY to understand
	     for(int i=0; i<licznik_znakow; i++)
	    {
	        if(znaki[i]=='q')
	        {//zabezpiczenie bo ************* przy jego braku w sensie sin(180) i wielokrotnosci
	            if(((int)(liczby[i+1]) %180 )== 0)
	            {
	                liczby[i]=0;//bo sinus powoduje ze w tablicy liczb pojawia sie puste miejsce
	            }
	            else
	                liczby[i]=(sin(liczby[i+1]*M_PI/180.0f));

	            //przesuwamy reszte liczb i znakow w lewo
	            for(int x=i+1; x<licznik_liczb; x++)
	            {
	                liczby[x]=liczby[x+1];
	            }
	            licznik_liczb--;

	            for(int b=i; b<licznik_znakow; b++)
	            {
	                znaki[b]=znaki[b+1];
	            }
	            licznik_znakow--;
	            i--;
	        }
	    }

	    //COSINUS//
	    for(int i=0; i<licznik_znakow; i++)
	    {
	        if(znaki[i]=='w')
	        {//zabezpiczenie bo ************* przy jego braku w sensie cos(90) + 180k
	            if( (((int)(liczby[i+1])+90) %180 )== 0)
	            {
	                liczby[i]=0;//bo sinus powoduje ze w tablicy liczb pojawia sie puste miejsce
	            }
	            else
	                liczby[i]=(cos(liczby[i+1]*M_PI/180.0f));

	            if(liczby[i]<0.001)
	                liczby[i]=0;

	            //przesuwamy reszte liczb i znakow w lewo
	            for(int x=i+1; x<licznik_liczb; x++)
	            {
	                liczby[x]=liczby[x+1];
	            }
	            licznik_liczb--;

	            for(int b=i; b<licznik_znakow; b++)
	            {
	                znaki[b]=znaki[b+1];
	            }
	            licznik_znakow--;
	            i--;
	        }
	    }

	    //TANGENS//
	    for(int i=0; i<licznik_znakow; i++)
	    {
	        if(znaki[i]=='e')
	        {
	            if( (((int)(liczby[i+1]) + 90) %180 )== 0)//asymptota
	            {
	                //czuszczenie tablicy znakow i napisu
	                for(int i=0; i<n; i++)
	                {
	                    napis[i]=0;
	                }

	                napis[0]='e';
	                napis[1]='r';
	                napis[2]='r';
	                napis[3]='o';
	                napis[4]='r';
	                return 5;
	            }
	            else
	            {
	                liczby[i]=(tan(liczby[i+1]*M_PI/180.0f));
	                if(liczby[i]<0.001 || liczby[i]>1000)
	                    liczby[i]=0;

	                for(int x=i+1; x<licznik_liczb; x++)
	                {
	                    liczby[x]=liczby[x+1];
	                }
	                licznik_liczb--;

	                for(int b=i; b<licznik_znakow; b++)
	                {
	                    znaki[b]=znaki[b+1];
	                }
	                licznik_znakow--;
	                i--;
	            }
	        }
	    }


	    //COTANGENS//
	    for(int i=0; i<licznik_znakow; i++)
	    {
	        if(znaki[i]=='r')
	        {
	            if( ((int)(liczby[i+1]) %180 )== 0)//asymptota
	            {
	                //czuszczenie tablicy znakow i napisu
	                for(int i=0; i<n; i++)
	                {
	                    napis[i]=0;
	                }

	                napis[0]='e';
	                napis[1]='r';
	                napis[2]='r';
	                napis[3]='o';
	                napis[4]='r';
	                return 5;

	            }
	            else
	            {
	                liczby[i]=(cos(liczby[i+1]*M_PI/180.0f))/(sin(liczby[i+1]*M_PI/180.0f));
	                if(liczby[i]<0.001 || liczby[i]>1000)
	                    liczby[i]=0;

	                for(int x=i+1; x<licznik_liczb;x++)
	                {
	                    liczby[x]=liczby[x+1];
	                }
	                licznik_liczb--;

	                for(int b=i; b<licznik_znakow; b++)
	                {
	                    znaki[b]=znaki[b+1];
	                }
	                licznik_znakow--;
	                i--;
	            }
	        }
	    }
	    ///////////////////////////////////////////////////////////

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
	            licznik_znakow--;i--;
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
	            liczby[i]-=liczby[i+1];
	            if( liczby[i]>-0.00001 && liczby[i]<0.00001)
	            {
	                liczby[i]=0;
	            }
	            for(int j = i+1; j<licznik_znakow; j++)
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

	    //zamiania z double na tablice charow
	    double wynikdouble=liczby[0];
	    int part1=wynikdouble;//bedzie reprezentowac czesc calkowita
	    char wynikchar1[29]={0};//bedzie reprezentowac czesc calkowita
	    //zapisanie wyniku jako tablica char
	    //szukamy czesci calkowitej
		int dlugoscchar1=0;
		if(part1 == 0)
	    {
	        wynikchar1[0]='0';
	        dlugoscchar1=1;
	    }
	    else
	    {
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

		if(wynikdouble>pow(2,31))
	    {
	    	napis[0]='e';
	    	napis[1]='r';
	    	napis[2]='r';
	    	napis[3]='o';
	    	napis[4]='r';
	    	return 5;
	    }
		else
		{
			while(dlugoscchar>n)
	    	{//jesli wynik jest dluzszy niz da sie wyswietlic na ekranie to zmniejsz dokladnosc po przecinku
	    		dlugoscchar--;
	    		dlugoscchar2--;
	    	}

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
}
