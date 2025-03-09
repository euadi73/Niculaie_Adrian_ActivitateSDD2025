#define _CRT_SECURE_NO_WARNINGS

#include <malloc.h>
#include <string.h>
#include <stdio.h>

typedef struct Voucher {

	unsigned int nr_voucher;
	char* nume_beneficiar;
	char* data_expirare;
	float valoare;

}voucher;

voucher creareVoucher(unsigned int nr_voucher, const char* nume_beneficiar, const char* data_expirare, float valoare) {

	voucher v;
	
	v.nr_voucher = nr_voucher;

	v.nume_beneficiar = (char*)malloc(sizeof(char) * (strlen(nume_beneficiar) + 1));
	strcpy(v.nume_beneficiar, nume_beneficiar);

	v.data_expirare = (char*)malloc(sizeof(char) * (strlen(data_expirare) + 1));
	strcpy(v.data_expirare, data_expirare);

	v.valoare = valoare;

	return v;
}

void afisareVoucher(voucher v) {
	printf("\n Nr voucherului: %d, nume beneficiar %s, data expirare %s, valoare: %5.2f", 
				v.nr_voucher, v.nume_beneficiar, v.data_expirare, v.valoare);
}

typedef struct Nod {

	voucher info;
	struct Nod* prev;
	struct Nod* next;


}nod;


nod* creareNod(voucher info, nod* prev, nod* next) {

	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = creareVoucher(info.nr_voucher, info.nume_beneficiar, info.data_expirare, info.valoare);
	nou->next = next;
	nou->prev = prev;

	return nou;

}


typedef struct LDI {

	nod* prim;
	nod* ultim;

}ldi;



ldi inserareInceput(ldi lista, voucher v) {

	nod* nou = creareNod(v, NULL, NULL);
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


ldi inserareFinal(ldi lista, voucher v) {
	nod* nou = creareNod(v, NULL, NULL);
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
		nod* v = lista.prim;
		while (v) {
			afisareVoucher(v->info);
			v = v->next;
		}


	}

}

void afisareFinalInceput(ldi lista) {
	if (lista.ultim) {
		nod* v = lista.ultim;
		while (v) {
			afisareVoucher(v->info);
			v = v->prev;
		}

	}

}



float valoareVouchereInLuna(ldi lista, const char* luna) {
	float suma = 0;
	if (lista.prim) {
		nod* p = lista.prim;
		while (p) {

			if (strstr(p->info.data_expirare, luna) != NULL) {
				suma += p->info.valoare;
			}
			p = p->next;
		}
	}
	return suma;
}



int numarBeneficiariCuDouaAparitii(ldi lista) {
	int numarBeneficiari = 0;

	if (lista.prim) {
		nod* p = lista.prim;
		while (p) {
			int aparitii = 1;
			nod* q = p->next;
			while (q) {

				if (strcmp(p->info.nume_beneficiar, q->info.nume_beneficiar) == 0) {
					aparitii++;
				}
				q = q->next;
			}
			if (aparitii >= 2) {
				numarBeneficiari++;
			}
			p = p->next;
		}
	}

	return numarBeneficiari;
}




ldi dezalocareLista(ldi lista) {

	if (lista.prim) {
		while (lista.prim) {
			nod* v = lista.prim;
			lista.prim = lista.prim->next;
			free(v->info.nume_beneficiar);
			free(v);


		}

		lista.prim = lista.ultim = NULL;

	}

	return lista;

}

void Sortare(voucher* vector, int nr) {

	for (int i = 0; i < nr - 1; i++) {
		for (int j = i + 1; j < nr; j++) {
			if (vector[i].valoare > vector[j].valoare) {

				voucher aux = vector[i];
				vector[i] = vector[j];
				vector[j] = aux;

			}
		}
	}


}


int numarElementePestePrag(ldi lista, float prag) {

	int nr = 0;
	if (lista.prim) {
		nod* p = lista.prim;
		while (p) {
			if (p->info.valoare > prag) {
				nr++;
			}
			p = p->next;
		}
	}

	return nr;
}


voucher* puneInVector(ldi lista, float prag) {

	int nr = numarElementePestePrag(lista, prag);
	voucher* vector = (voucher*)malloc(sizeof(voucher) * nr);
	int index = 0;

	if (lista.prim) {
		nod* p = lista.prim;
		while (p) {
			if (p->info.valoare > prag) {
				vector[index++] = p->info;

			}
			p = p->next;
		}
	}
	return vector;
}


void main() {

	ldi lista;
	lista.prim = lista.ultim = NULL;

	lista = inserareInceput(lista, creareVoucher(1, "Niculaie Adrian", "01.02.2025", 150));
	lista = inserareInceput(lista, creareVoucher(8, "Popescu Ion", "01.03.2025", 200));
	lista = inserareInceput(lista, creareVoucher(12, "Mihaela Andrei", "04.05.2025", 350));

	lista = inserareFinal(lista, creareVoucher(16, "Dinu Mihai", "01.05.2025", 450));
	lista = inserareFinal(lista, creareVoucher(24, "Valentin Mircea", "01.06.2025", 700));
	lista = inserareFinal(lista, creareVoucher(36, "Valentin Mircea", "01.07.2025", 650));


	printf("\n Afisare inceput - final ");
	afisareInceputFinal(lista);

	printf("\n---------------------------------------------------");

	printf("\n Afisare finalInceput ");
	afisareFinalInceput(lista);

	printf("\n---------------------------------------------------");

	printf("\n Suma valorilor voucher-elor expirate in luna mai: %5.2f", valoareVouchereInLuna(lista, "05"));

	printf("\n---------------------------------------------------");

	printf("\n Numarul de beneficiari cu cel putin doua aparitii: %d", numarBeneficiariCuDouaAparitii(lista));

	printf("\n---------------------------------------------------");

	int nr = numarElementePestePrag(lista, 350);
	voucher* vector = puneInVector(lista, 350);
	Sortare(vector, nr);
	for (int i = 0; i < nr; i++) {
		afisareVoucher(vector[i]);
	}

	printf("\n---------------------------------------------------");
	lista = dezalocareLista(lista);

	printf("\n Afisare lista dupa dezalocare ");
	afisareInceputFinal(lista);

}

