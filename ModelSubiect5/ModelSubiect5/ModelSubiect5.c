#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef struct Moneda {

	char* tara;
	int valoare_nominala;
	float greutate;
	int an_emitere;

}moneda;

moneda creareMoneda(const char* tara, int valoare_nominala, float greutate, int an_emitere) {

	moneda m;
	m.tara = (char*)malloc(sizeof(char) * (strlen(tara) + 1));
	strcpy(m.tara, tara);
	m.valoare_nominala = valoare_nominala;
	m.an_emitere = an_emitere;
	m.greutate = greutate;

	return m;
}

void afisareMoneda(moneda m) {

	printf("\n Moneda din tara %s are valoarea nominala %d , greutatea %5.2f, anul emiterii este %d", m.tara, m.valoare_nominala, m.greutate, m.an_emitere);

}

typedef struct Nod {

	moneda info;
	struct Nod* next;

}nod;

nod* creareNod(moneda info, nod* next) {

	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = creareMoneda(info.tara, info.valoare_nominala, info.greutate, info.an_emitere);
	nou->next = next;

	return nou;
}

nod* inserareInceput(nod* cap, moneda m) {
	nod* nou = creareNod(m, NULL);
	if (cap) {
		nou->next = cap;
		cap = nou;

	}
	else {
		cap = nou;

	}
	return cap;

}

void afisareLista(nod* cap) {

	if (cap) {
		nod* p = cap;
		while (p) {
			afisareMoneda(p->info);
			p = p->next;
		}
	}

}

nod* dezalocareLista(nod* cap) {

	if (cap) {
		while (cap) {
			nod* p = cap;
			cap = cap->next;
			free(p->info.tara);
			free(p);
		}
		cap = NULL;
	}
	return cap;
}


moneda cautareMonedaDupaGreutate(nod* cap, float greutate) {

	if (cap) {
		nod* p = cap;
		while (p && p->info.greutate != greutate) {
			p = p->next;
		}
		if (p) {
			return p->info;
		}
		else {
			return creareMoneda("Nu exista greutatea cautata", 0, 0.0, 0);
		}
	}
	else {

		return creareMoneda("Nu exista greutatea cautata", 0, 0.0, 0);
	}
}


moneda cautareMonedaCuGreutateMaxima(nod* cap) {
	moneda maxMoneda;
	float maxGreutate = 0.0;

	if (cap) {
		nod* p = cap;
		while (p) {
			if (p->info.greutate > maxGreutate) { 
				maxGreutate = p->info.greutate; 
				maxMoneda = p->info;
			}
			p = p->next;
		}
	}

	return maxMoneda; 
}


void main() {


	nod* lista = NULL;

	lista = inserareInceput(lista, creareMoneda("EURO", 8, 5.3, 1980));
	lista = inserareInceput(lista, creareMoneda("LEVA", 7, 6.3, 1952));
	lista = inserareInceput(lista, creareMoneda("LIRA", 6, 7.3, 1942));
	lista = inserareInceput(lista, creareMoneda("PESO", 5, 8.3, 1999));
	lista = inserareInceput(lista, creareMoneda("DOLAR", 4, 9.3, 1935));


	afisareLista(lista);

	printf("\n Cautare moneda dupa greutate");
	afisareMoneda(cautareMonedaDupaGreutate(lista, 8.3));
	printf("\n Cautare moneda dupa greutate");
	afisareMoneda(cautareMonedaDupaGreutate(lista, 5.3));

	printf("\n Cautare moneda cu greutatea maxima");
	afisareMoneda(cautareMonedaCuGreutateMaxima(lista));


	lista = dezalocareLista(lista);
	printf("\n Afisare lista dupa dezalocare");

	afisareLista(lista);

}

//implementati functia care sterge o moneda din lista simplu inlantuita primid greutatea monezii cautate