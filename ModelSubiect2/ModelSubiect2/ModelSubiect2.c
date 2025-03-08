#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<malloc.h>
#include<string.h>

typedef struct Reteta {

	unsigned int nr_reteta, nrMedicamentePrescrise;
	char** listaMedicamentelorPrescrise;
	char* numeMedic;
	int valoareBruta;
	int procentCompensare;


}reteta;

reteta creareReteta(unsigned int nr_reteta, unsigned int nrMedicamentePrescrise,
	char** listaMedicamentelorPrescrise, char* numeMedic, int valoareBruta, int procentCompensare) {

	reteta r;
	r.nr_reteta = nr_reteta;
	r.nrMedicamentePrescrise = nrMedicamentePrescrise;
	r.valoareBruta = valoareBruta;
	r.procentCompensare = procentCompensare;

	r.numeMedic = (char*)malloc(sizeof(char) * (strlen(numeMedic) + 1));
	strcpy(r.numeMedic, numeMedic);

	//paracetamol nuforen aspirina 
	r.listaMedicamentelorPrescrise = (char**)malloc(sizeof(char*) * r.nrMedicamentePrescrise);
	for (int i = 0;i < r.nrMedicamentePrescrise;i++) {
		r.listaMedicamentelorPrescrise[i] =(char*)malloc(sizeof(char)*(strlen(listaMedicamentelorPrescrise[i])+1));
		strcpy(r.listaMedicamentelorPrescrise[i], listaMedicamentelorPrescrise[i]);
	}
	

	return r;


}

void afisareReteta(reteta r) {

	printf("\n Numarul retetei este %d, numar medicamente prescrise %d, nume medic %s, valoare bruta %d, procent compensare %d", r.nr_reteta,r.nrMedicamentePrescrise, r.numeMedic, r.valoareBruta, r.procentCompensare);
	printf("\n lista medicamente: ");
	for (int i = 0;i < r.nrMedicamentePrescrise;i++) {
		printf("\n %d: %s", i, r.listaMedicamentelorPrescrise[i]);
	}
}

typedef struct Nod {

	reteta info;
	struct Nod* next;

}nod;


nod* creareNod(reteta r, nod* next) {

	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = creareReteta(r.nr_reteta, r.nrMedicamentePrescrise, r.listaMedicamentelorPrescrise, r.numeMedic, r.valoareBruta, r.procentCompensare);
	nou->next = next;

	return nou;

}

nod* inserareInceput(nod* cap, reteta r) {

	nod* nou = creareNod(r, NULL);
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
			afisareReteta(p->info);
			p = p->next;

		}

	}

}

nod* dezalocareLista(nod* cap) {
	if (cap) {
		nod* p = cap;
		cap = cap->next;
		free(p->info.numeMedic);
		for (int i = 0;i < p->info.nrMedicamentePrescrise;i++) {
			free(p->info.listaMedicamentelorPrescrise[i]);
		}
		free(p->info.listaMedicamentelorPrescrise);
		free(p);
		cap = NULL;

	}

	return cap;
}



typedef struct Hashtable {

	int dim;
	nod** vector;


}ht;

ht initializareHashtable(int dim) {

	ht h;
	h.dim = dim;
	h.vector = (nod**)malloc(sizeof(nod*) * dim); //alocam memorie pt vector
	for (int i = 0; i < h.dim; i++) {
		h.vector[i] = NULL;  //adresa de inceput a fiecarei liste in parte

	}

	return h;

}

int hashcode(ht h,int nr_reteta) {

	return nr_reteta % h.dim;
}


int inserareHashtable(ht h, reteta r) {

	if (h.vector) {
		int poz = hashcode(h, r.nr_reteta);
		h.vector[poz] = inserareInceput(h.vector[poz], r);
		return poz;

	}
	else {
		return -1;
	}
}



void afisareHashtable(ht h) {

	if (h.vector) {
		for (int i = 0; i < h.dim; i++) {
			if (h.vector[i]) {
				printf("\n Pe pozitia %d", i);
				afisareLista(h.vector[i]);
			}
			

		}

	}

}

ht dezalocareHashtable(ht h) {
	if (h.vector) {
		for (int i = 0; i < h.dim; i++)
		{
			h.vector[i] = dezalocareLista(h.vector[i]);
		}

		free(h.vector);
		h.vector = NULL;
		h.dim = 0;
	}
	return h;

}

void PrezentaMedicamentinLS(nod* cap, const char* medicament, int nr) {
	int contor = 0;
	if (cap) {
		while (cap && cap->info.nr_reteta != nr)
		{
			cap = cap->next;
		}
		if (cap) {
			for (int i = 0;i < cap->info.nrMedicamentePrescrise;i++) {
				if (strcmp(cap->info.listaMedicamentelorPrescrise[i], medicament) == 0) {
					contor = 1;
				}
			}
		}
		if (contor == 1) {
			printf("\n am gasit medicamentul");
		}
		else {
			printf("\nnu am gasit medicamentul");

		}
	}
}

void PrezentaMedicamentinHT(ht h, const char* medicament, int nr) {
	if (h.vector) {
		int poz = hashcode(h, nr);
		PrezentaMedicamentinLS(h.vector[poz], medicament, nr);
	}
}
float ValoareNetaRetete(ht h) {
	float val = 0;
	if (h.vector) {
		for (int i = 0;i < h.dim;i++) {
			nod* p = h.vector[i];
			while (p) {
				float procent = (float)p->info.procentCompensare / 100;
				val += (float)((float)p->info.valoareBruta) * (1 - procent);

				p = p->next;
			}
		}
	}
	return val;
}
int NrReteteCompensateDeUnMedic(ht h, const char* nume) {
	int nr = 0;
	if (h.vector) {
		for (int i = 0;i < h.dim;i++) {
			nod* p = h.vector[i];
			while (p) {
				if (strcmp(p->info.numeMedic, nume) == 0) {
					nr++;
				}
				p = p->next;
			}
		}
	}
	return nr;
}
void NrRetetePerMedic(ht h) {
	if (h.vector) {
		for (int i = 0;i < h.dim;i++) {
			nod* p = h.vector[i];
			while (p) {
				int nr = NrReteteCompensateDeUnMedic(h, p->info.numeMedic);
				printf("\n medicul %s are %d retete", p->info.numeMedic, nr);
				p = p->next;
			}
		}
	}
}
void main() {

	ht h = initializareHashtable(6);
	char** med = (char**)malloc(sizeof(char*) * 3);
	med[0]= (char*)malloc(sizeof(char) * (strlen("Nurofen") + 1));
	strcpy(med[0], "Nurofen");
	med[1] = (char*)malloc(sizeof(char) * (strlen("Paracemaol" ) + 1));
	strcpy(med[1], "Paracemaol");
	med[2] = (char*)malloc(sizeof(char) * (strlen("Nurofen" ) + 1));
	strcpy(med[2], "Nurofen");
	inserareHashtable(h, creareReteta(6, 3, med, "Andrei", 100, 10));
	inserareHashtable(h, creareReteta(2, 3, med, "Ion", 100, 10));
	inserareHashtable(h, creareReteta(3, 3, med, "Ion", 90, 10));

	/*inserareHashtable(h, creareReteta(4, 14, "Aspenter", "Mihaela", 100, 10));
	inserareHashtable(h, creareReteta(5, 15, "Espumisan", "Remus", 100, 10));
	inserareHashtable(h, creareReteta(6, 16, "Coldrex", "Andreea", 100, 10));*/

	afisareHashtable(h);
	PrezentaMedicamentinHT(h, "Nurofen", 3);
	PrezentaMedicamentinHT(h, "abc", 3);
	printf("\n valoare neta retete %5.2f", ValoareNetaRetete(h));
	NrRetetePerMedic(h);
	h = dezalocareHashtable(h);
	printf("\n Afisare dupa dezalocare");

	afisareHashtable(h);

}