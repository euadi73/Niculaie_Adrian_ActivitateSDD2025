#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef struct Masina {

	char* model;
	int hp;

}masina;

masina creareMasina(const char* model, int hp) {

	masina m;
	m.model = (char*)malloc(sizeof(char) * (strlen(model) + 1));
	strcpy(m.model, model);
	m.hp = hp;

	return m;

}

void afisareMasina(masina m) {

	printf("\n Masina cu modelul %s are %d cai putere", m.model, m.hp);

}

typedef struct Nod {

	masina info;
	struct Nod* prev;
	struct Nod* next;


}nod;

nod* creareNod(masina info, nod* prev, nod* next) {

	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = creareMasina(info.model, info.hp);
	nou->next = next;
	nou->prev = prev;


	return nou;
}

typedef struct LDI {

	nod* prim;
	nod* ultim;


}ldi;


ldi inserareInceput(ldi lista, masina m) {

	nod* nou = creareNod(m, NULL, NULL);
	if (lista.prim) {

		nou->next = lista.prim;
		lista.prim->prev = nou;
		lista.prim = nou;

	}
	else {

		lista.prim = lista.ultim = nou;

	}

	return lista;
}


ldi inserareFinal(ldi lista, masina m) {
	nod* nou = creareNod(m, NULL, NULL);
	if (lista.ultim) {
		nou->prev = lista.ultim;
		lista.ultim->next = nou;
		lista.ultim = nou;

	}
	else {

		lista.prim = lista.ultim = nou;

	}

	return lista;
}

void afisareInceputFinal(ldi lista) {
	if (lista.prim) {
		nod* p = lista.prim;
		while (p) {
			afisareMasina(p->info);
			p = p->next;
		}


	}

}


void afisareFinalInceput(ldi lista) {
	if (lista.ultim) {
		nod* p = lista.ultim;
		while (p) {
			afisareMasina(p->info);
			p = p->prev;
		}

	}

}

ldi dezalocareLista(ldi lista) {

	if (lista.prim) {
		while (lista.prim) {
			nod* p = lista.prim;
			lista.prim = lista.prim->next;
			free(p->info.model);
			free(p);


		}

		lista.prim = lista.ultim = NULL;

	}

	return lista;

}

masina CautareMasinaDupaHp(ldi lista, int hp) {
	if (lista.prim) {
		nod* p = lista.prim;
		while (p && p->info.hp != hp) {//cat timp mai am noduri in lista (exista p) si cat timp nu am gasit hp cautat
			//parcurgem lista
			p = p->next;
		}
		if (p) {
			//inseamna ca a gasit hp
			return p->info;
		}
		else {
			//a existat lista, dar nu mi-a gasit hp cautat
			return creareMasina("Nu exista hp", -1);
		}
	}
	else {
		//returnam obiect fictiv
		return creareMasina("Nu exista lista", -1);
	}
}


masina CautareMasinaDupaModel(ldi lista, const char* model) {
	if (lista.prim) {
		nod* p = lista.prim;
		while (p && strcmp(p->info.model, model) != 0) {//cat timp mai am noduri in lista (exista p) si cat timp nu am gasit modelul cautat
			//parcurgem lista
			p = p->next;
		}
		if (p) {
			//inseamna ca a gasit hp
			return p->info;
		}
		else {
			//a existat lista, dar nu mi-a gasit modelul cautat
			return creareMasina("Nu exista model", -1);
		}
	}
	else {
		//returnam obiect fictiv
		return creareMasina("Nu exista lista", -1);
	}
}




void main() {

	ldi lista;
	lista.prim = lista.ultim = NULL;
	lista = inserareInceput(lista, creareMasina("BMW", 160));
	lista = inserareInceput(lista, creareMasina("Mercedes-Benz", 130));
	lista = inserareInceput(lista, creareMasina("Ford", 100));

	lista = inserareFinal(lista, creareMasina("VW", 95));
	lista = inserareFinal(lista, creareMasina("Cupra", 150));
	lista = inserareFinal(lista, creareMasina("Renault", 90));

	printf("\n Afisare inceputFinal ");
	afisareInceputFinal(lista);

	printf("\n Afisare finalInceput ");
	afisareFinalInceput(lista);

	printf("\n Cautare masina dupa hp");
	afisareMasina(CautareMasinaDupaHp(lista, 95));
	printf("\n Cautare masina dupa hp");
	afisareMasina(CautareMasinaDupaHp(lista, 200));

	printf("\n Cautare masina dupa model");
	afisareMasina(CautareMasinaDupaModel(lista, "Ford"));
	printf("\n Cautare masina dupa model");
	afisareMasina(CautareMasinaDupaHp(lista, "ABC"));



	lista = dezalocareLista(lista);

	printf("\n Afisare lista dupa dezalocare ");
	afisareInceputFinal(lista);
}

