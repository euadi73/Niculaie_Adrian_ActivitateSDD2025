#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<malloc.h>
#include<string.h>

typedef struct Proiect {

	unsigned int cod_proiect;
	char* titlu_proiect;
	char* beneficiar;
	unsigned char numar_executanti;
	float buget_alocat;

}proiect;

proiect creareProiect(unsigned int cod_proiect, const char* titlu_proiect, const char* beneficiar, unsigned char* numar_executanti, float buget_alocat) {

	proiect p;
	p.cod_proiect = cod_proiect;
	p.buget_alocat = buget_alocat;

	p.titlu_proiect = (char*)malloc(sizeof(char) * (strlen(titlu_proiect) + 1));
	strcpy(p.titlu_proiect, titlu_proiect);

	p.beneficiar = (char*)malloc(sizeof(char) * (strlen(beneficiar) + 1));
	strcpy(p.beneficiar, beneficiar);
	p.numar_executanti = numar_executanti;

	return p;
}

void afisareProiect(proiect p) {

	printf("\Proiectul are codul %d, titlu proiect %s , nume beneficiar %s, numar executanti %d, buget alocat %5.2f ", p.cod_proiect, p.titlu_proiect, p.beneficiar, p.numar_executanti, p.buget_alocat);
}

typedef struct Nod {

	proiect info;
	struct Nod* next;

}nod;

nod* creareNod(proiect p, nod* next) {

	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = creareProiect(p.cod_proiect, p.titlu_proiect, p.beneficiar, p.numar_executanti, p.buget_alocat);
	nou->next = next;

	return nou;

}

nod* inserareInceput(nod* cap, proiect p) {

	nod* nou = creareNod(p, NULL);
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
			afisareProiect(p->info);
			p = p->next;
		}
	}
}

nod* dezalocareLista(nod* cap) {

	if (cap) {
		nod* p = cap;
		cap = cap->next;
		free(p->info.beneficiar);
		free(p);
		cap = NULL;
	}
	return cap;

}

proiect extragereProiectDupaNumeLS(nod** cap, const char* nume) {
	if (*cap) {
		

		nod* p = *cap;//p-2 p-1 p p+1   p p.next p,next.next
		if (strcmp((*cap)->info.beneficiar,nume)==0) {
			proiect pr = creareProiect((*cap)->info.cod_proiect, (*cap)->info.titlu_proiect,(*cap)->info.beneficiar, (*cap)->info.numar_executanti, (*cap)->info.buget_alocat);
			nod* aux = *cap;
			(*cap) = (*cap)->next;
			free(aux->info.beneficiar);
			free(aux->info.titlu_proiect);
			free(aux);
			return pr;
		}
		else {
			while (p->next && (strcmp((p->next)->info.beneficiar, nume) != 0)) {
				p = p->next;
			}
			if (p->next) {
				proiect pr = creareProiect((p->next)->info.cod_proiect, (p->next)->info.titlu_proiect, (p->next)->info.beneficiar, (p->next)->info.numar_executanti, (p->next)->info.buget_alocat);

				nod* aux = p->next;
				p->next = p->next->next;
				free(aux->info.beneficiar);
				free(aux->info.titlu_proiect);
				free(aux);
				return pr;
			}
			else {
				return creareProiect(0, "", "", -1, 0);
			}
		}

	}
	else {
		return creareProiect(0, "", "", -1, 0);
	}
}


typedef struct Hashtable {

	int dim;
	nod** vector;

}ht;

ht initHashtable(int dim) {
	ht h;
	h.dim = dim;
	h.vector =(nod**)malloc(sizeof(nod*) * dim);
	for (int i = 0; i < h.dim; i++) {
		h.vector[i] = NULL;
	}

	return h;
}


int hashcode(ht h, int cod_proiect) {

	return cod_proiect % h.dim;

}

int hashcode2(ht h, const char* beneficiar) {

	return beneficiar[0] % h.dim;// 'a'

}
int hashcode3(ht h, const char* beneficiar) {
	int sum = 0;
	for (int i = 0;i < strlen(beneficiar);i++) {
		sum += beneficiar[i];
	}
	return sum % h.dim;// 'a'

}
int inserareHashtable(ht h, proiect p) {

	if (h.vector) {
		int poz = hashcode2(h,p.beneficiar);
		h.vector[poz] = inserareInceput(h.vector[poz], p);

		return poz;

	}
	else {
		return -1;
	}
}

void afisareHashtable(ht h) {
	if (h.vector) {
		for (int i = 0; i < h.dim; i++) {
			printf("\n Pe pozitia %d ", i);
			afisareLista(h.vector[i]);
		}
	}
}



ht dezalocareHashtable(ht h) {

	if (h.vector) {

		for (int i = 0; i < h.dim; i++) {

			h.vector[i] = dezalocareLista(h.vector[i]);

		}

		free(h.vector);
		h.vector = NULL;
		h.dim = 0;

	}

	return h;


}

proiect cautareProiectDupaBeneficiarInHashtable(ht h, char* beneficiar) {

	if (h.vector) {
		int gasit = 0;
		for (int i = 0; i < h.dim; i++) {
			nod* p = h.vector[i];

			while (p) {
				if (strcmp(p->info.beneficiar, beneficiar) == 0) {
					gasit = 1;
					return p->info;
				}

				p = p->next;
			}
		}
		if (gasit == 0) {
			return creareProiect(0, "Nu exista proiectul, ", 0, 0, 0.0);
		}

	}

	else {
		return creareProiect(0, "Nu exista proiectul, ", 0, 0, 0.0);
	}

}

float SumaBugetPtBeneficiarLS(nod* cap, const char* beneficiar) {
	float sum = 0;
	if (cap) {
		while (cap) {
			if (strcmp(cap->info.beneficiar, beneficiar) == 0) {
				sum += cap->info.buget_alocat;
			}
			cap = cap->next;
		}
	}
	return sum;
}
float SumaBugetPtBeneficiarHt(ht h, const char* beneficiar) {
	float sum = 0;
	if (h.vector) {
		int poz = hashcode2(h, beneficiar);
		sum = SumaBugetPtBeneficiarLS(h.vector[poz], beneficiar);
	}
	return sum;
}
ht ModificareNumeBeneficiar(ht h, const char* numevechi, const char* numenou) {
	if (h.vector) {
		int poz = hashcode2(h, numevechi);
		proiect p = extragereProiectDupaNumeLS(&h.vector[poz], numevechi);
		if (p.buget_alocat != 0) {
			free(p.beneficiar);
			p.beneficiar = (char*)malloc(sizeof(char) * (strlen(numenou) + 1));
			strcpy(p.beneficiar, numenou);
			inserareHashtable(h, p);
		}
	}
	return h;
}


//mutare din structura a in structura b 
//parcurg a si inserez in b

//imi returneaza b

nod* DinHTInLS(ht h, float buget) {
	nod* lista = NULL;
	//parcurg a
	if (h.vector) {
		for (int i = 0;i < h.dim;i++) {
			nod* p = h.vector[i];
			while (p) {
				//conditia de filtrare
				if (p->info.buget_alocat > buget) {
					//inserez in b
					lista = inserareInceput(lista, p->info);
				}
				p = p->next;
			}
		}
	}
	return lista;
}
void main() {

	ht h = initHashtable(5);

	inserareHashtable(h, creareProiect(1, "Flow Request", "Niculaie Adrian", 1, 5000));
	inserareHashtable(h, creareProiect(10, "proiect nou", "Niculaie Adrian", 1, 5000));
	inserareHashtable(h, creareProiect(2, "Data Analysis", "Maria Popescu", 2, 8000));
	inserareHashtable(h, creareProiect(3, "Mobile App Development", "Alexandru Ionescu", 3, 12000));
	inserareHashtable(h, creareProiect(4, "Web Development", "Elena Stan", 2, 10000));
	inserareHashtable(h, creareProiect(5, "Database Management", "Mihai Radu", 1, 6000));
	inserareHashtable(h, creareProiect(6, "AI Research", "Ana Maria", 4, 15000));

	afisareHashtable(h);

	/*printf("\n Afisare proiect cautat dupa numele beneficiarului");
	afisareProiect(cautareProiectDupaBeneficiarInHashtable(h, "Niculaie Adrian"));

	printf("\n Afisare proiect cautat dupa numele beneficiarului");
	afisareProiect(cautareProiectDupaBeneficiarInHashtable(h, "Andrei Mihai"));*/

	printf("\n suma buget pe beneficiar %5.2f", SumaBugetPtBeneficiarHt(h, "Niculaie Adrian"));

	h = ModificareNumeBeneficiar(h, "Maria Popescu", "Nume Nou");
	printf("\n------------------------------------\n");
	afisareHashtable(h);

	nod* lista = DinHTInLS(h, 9000);
	printf("\n\n\n\n afisare lista \n");
	afisareLista(lista);

	h = dezalocareHashtable(h);
	printf("\n Afisare dupa dezalocare");

	afisareHashtable(h);

}