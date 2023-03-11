#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct element {
	char *valoare;
	char *cheie;
};

// Functie care cauta un element in hashMap dupa cheie si returneaza valoarea,
// daca o gaseste; in caz contrar, intoarce NULL
char* cautare(struct element *hashMap, int n, char *cheie)
{
	int i = 0;

	while (i < n) {
		if (strcmp(hashMap[i].cheie, cheie) == 0)
			return hashMap[i].valoare;

		i++;
	}

	return NULL;
}

// Functie care adauga un element intr-un hashMap
struct element* adaugare(struct element *hashMap, char *cheie, char *valoare, int *i)
{
	char *idx = NULL, *prev = NULL, *val = NULL;
	char *arg = NULL, *valoare_noua = NULL;
	
	// retin cheia
	memcpy(hashMap[*i].cheie, cheie, strlen(cheie) + 1);

	// verific daca trebuie sa fac prelucrari in sirul valoare
	if (valoare != NULL && 
		strstr(valoare, " ") != NULL) {
		idx = valoare;
		prev = valoare;
		// retin noul sir prelucrat in variabila valoare_noua
		valoare_noua = calloc(50, sizeof(char));

		if (valoare_noua == NULL) {
			return NULL;
		}

		// cat timp nu am terminat de iterat prin tot sirul
		// reprezentat de variabila valoare
		while (strstr(idx, " ") != NULL) {
			idx = strstr(idx, " ") + 1;
			arg = calloc(10, sizeof(char)); 

			if (arg == NULL) {
				return NULL;
			}

			// extrag argumentul curent
			memcpy(arg, prev, strlen(prev) - strlen(idx) - 1);

			// verific daca nu este un operator
			if (strstr(arg, "+") == NULL &&
				strstr(arg, "-") == NULL &&
				strstr(arg, "*") == NULL) {
				// verific daca valoarea sa se gaseste printre
				// elementele adaugate in hashMap
				val = cautare(hashMap, *i, arg);
				if (val != NULL) {
					// daca da, adaug valoarea respectiva in sirul 
					// nou pe care il formez
					strcat(valoare_noua, val);
					strcat(valoare_noua, " ");
				} else {
					// altfel, adaug direct variabila in sir
					strcat(valoare_noua, arg);
					strcat(valoare_noua, " ");
				}
			} else {
				// daca argumentul este un operator, il adaug direct 
				// in sirul final
				strcat(valoare_noua, arg);
				strcat(valoare_noua, " ");
			}

			prev = idx;
			free(arg);
		}

		if (prev != NULL) {
			// veriific daca a ramas o parte a sirului neprelucrata
			// si o adaug in sir
			if (strstr(prev, "+") == NULL &&
				strstr(prev, "-") == NULL &&
				strstr(prev, "*") == NULL) {
				val = cautare(hashMap, *i, prev);
				if (val != NULL)
					strcat(valoare_noua, val);
				else
					strcat(valoare_noua, prev);
			} else {
				strcat(valoare_noua, prev);
			}
		}
	}

	// adaug in hashMap valorile
	if (valoare == NULL) 
		memcpy(hashMap[*i].valoare, "", 1);
	else if (valoare_noua != NULL)
		memcpy(hashMap[*i].valoare, valoare_noua, strlen(valoare_noua) + 1);
	else 
		memcpy(hashMap[*i].valoare, valoare, strlen(valoare) + 1);

	// incrementez numarul de elemente din hashMap
	*i = *i + 1;

	if (valoare_noua != NULL)
		free(valoare_noua);

	return hashMap;
}

// Functie care inlocuieste valoarea unei chei cu o valoare noua
struct element* inlocuire(struct element *hashMap, char *cheie, char *valoare_noua, int n)
{
	int i = 0;

	while (i < n) {
		if (strcmp(hashMap[i].cheie, cheie) == 0) {
			memcpy(hashMap[i].valoare, valoare_noua, strlen(valoare_noua) + 1);
			break;
		}
		i++;
	}

	return hashMap;
}

// Functie care sterge un element din hashMap
struct element* stergere(struct element *hashMap, char *cheie, int *n)
{
	int i = 0, j;

	while (i < *n) {
		if (strcmp(hashMap[i].cheie, cheie) == 0)
			break;
		i++;
	}

	j = i;

	while (j < *n - 1) {
		memcpy(hashMap[i].cheie, hashMap[i + 1].cheie, strlen(hashMap[i + 1].cheie) + 1);
		memcpy(hashMap[i].valoare, hashMap[i + 1].valoare, strlen(hashMap[i + 1].valoare) + 1);
		j++;
	}

	*n = *n - 1;

	return hashMap;
}

// Functie care determina pozitiile cheilor din hashMap care se
// gasesc intr-un sir oarecare
void cautare_in_sir(struct element *hashMap, char *sir, int n, 
					int *nr_valori, int* pozitii)
{
	int i = 0;

	while (i < n) {
		if (strstr(sir, hashMap[i].cheie) != NULL) {
			pozitii[*nr_valori] = i;
			*nr_valori = *nr_valori + 1;
		}
		i++;
	}
}

// Functie care afiseaza un sir la tastatura sau intr-un fisier 
// de iesire.
void afisare(char *sir, FILE *fout)
{
	if (fout != NULL)
		fprintf(fout, "%s\n", sir);
	else
		printf("%s\n", sir);
}

// Functie care prelucreaza partea de fisiere incluse in alte fisiere.
char* fisiere_incluse (char *cale_curenta, char **cai, int numar_cai, 
						char *nume_fisier, char *returned, 
						char *string, char* rezultat)
{
	FILE *fin;
	int j;
	char* fisier_input = NULL;
	char* subsir = NULL, *fisier_nou = NULL;
	char *rezultat_nou = NULL;

	// Concatenez numele fisierului curent cu calea in 
	// care se afla fisierul de input.
	strcat(cale_curenta, nume_fisier);
	
	fin = fopen(cale_curenta, "rb");
	// Daca nu am putut sa il deschid, inseamna ca nu se afla
	// in directorul curent.
	if (fin == NULL) {
		// iau la mana toate caile date ca parametru in linia de comanda,
		// stocate in vectorul cai ma opresc la prima cale
		// pentru care fisierul se deschide
		for (j = 0; j < numar_cai; j++) {
			fisier_input = calloc(50, sizeof(char));
			if (fisier_input == NULL)
				return NULL;

			strcat(fisier_input, cai[j]);
			strcat(fisier_input, nume_fisier);

			fin = fopen(fisier_input, "rb");
			if (fin != NULL) {
				free(fisier_input);
				break;
			}
			free(fisier_input);
		}
	}
	
	if (fin != NULL) {
		// citesc din fisier
		while (1) {
			returned = fgets(string, 50, fin);
			if (returned == NULL)
				break;
			
			// daca fisierul meu mai include alt fisier
			if (strstr(string, "#include ") != NULL) {
				fisier_nou = calloc(50, sizeof(char));
				if (fisier_nou == NULL)
					return NULL;

				rezultat_nou = calloc(400, sizeof(char));
				if (rezultat_nou == NULL)
					return NULL;	
				
				subsir = strstr(string, " ") + 2;
				memcpy(fisier_nou, subsir, strlen(subsir) - 2);
				// apelez functia recursiv si retin continutul fisierului
				// in variabila rezultat_nou
				 rezultat_nou = fisiere_incluse(cale_curenta, cai, 
				 								numar_cai, fisier_nou, 
												returned, string, rezultat_nou);
				if (rezultat_nou == NULL)
					return NULL;

				strcat(rezultat, rezultat_nou);

				free(fisier_nou);
				free(rezultat_nou);
			} else {
				strcat(rezultat, string);
			}
		}
	}

	if (fin != NULL) {
		fclose(fin);
	} else {
		printf("File included does not exist!\n");
		exit(1);
	}

	return rezultat;
}

// Functie care implementeaza directia "define"
char* implementare_define(char *subsir, char *valoare, int dimensiune,
							char *cheie, char *valoare_noua, char *linie,
							struct element *hashMap, int nr_elemente)
{
	int ok = 0;
	size_t n, x;
	char *idx = NULL, *prev = NULL, *val = NULL;
	char *arg = NULL;
	subsir = strstr(linie, " ") + 1;

	if (strstr(subsir, " ") != NULL) {
		valoare = strstr(subsir, " ") + 1;
		x = strlen(valoare);
		n = strlen(subsir);
		dimensiune = n - x - 1;
		//determin cheia
		memcpy(cheie, subsir, dimensiune);
	} else {
		valoare = NULL;
		memcpy(cheie, subsir, strlen(subsir));
	}

	// daca valoarea cheii se intinde pe mai multe linii	
	if (valoare != NULL &&
		strstr(valoare, "\\") != NULL &&
		strstr(valoare, "\\n") == NULL) {
		memcpy(valoare_noua, valoare, strlen(valoare) - 1);
		linie = strtok(NULL, "\n");

		// cat timp instructiunea nu s-a terminat
		while (linie != NULL && strstr(linie, "\\") != NULL) {
			valoare = strstr(linie, " ");

			while (valoare != NULL && valoare[0] == ' ')
				valoare++;

			// adaug argumentul curent la noua variabila
			if (valoare_noua[strlen(valoare_noua) - 1] != ' ')
				strcat(valoare_noua, " ");

			strncat(valoare_noua, valoare, strlen(valoare) - 1);
			linie = strtok(NULL, "\n");
		}

		// fac pasii din while si pentru bucata de sir ramasa
		valoare = strstr(linie, " ");
		while (valoare[0] == ' ')
			valoare++;

		if (valoare_noua[strlen(valoare_noua) - 1] != ' ')
			strcat(valoare_noua, " ");

		strncat(valoare_noua, valoare, strlen(valoare));

		// marchez faptul ca am intrat in acest for
		ok = 1;
	} else if (valoare != NULL && 
		strstr(valoare, " ") != NULL) {
		idx = valoare;
		prev = valoare;

		// in caz contrar, daca valoarea se intinde pe o singura linie,
		// dar are mai multe argumente
		while (strstr(idx, " ") != NULL) {
			idx = strstr(idx, " ") + 1;
			arg = calloc(10, sizeof(char)); 

			if (arg == NULL) {
				return NULL;
			}

			// determin argumentul curent
			memcpy(arg, prev, strlen(prev) - strlen(idx) - 1);

			// verific daca este sau nu operator
			if (strstr(arg, "+") == NULL &&
				strstr(arg, "-") == NULL &&
				strstr(arg, "*") == NULL) {
				// daca nu este, verific daca il gasesc in hashMap
				val = cautare(hashMap, nr_elemente, arg);
				// daca ii gasesc valoarea in hashMap, o adaug;
				// altfel, adaug direct cheia
				if (val != NULL) {
					strcat(valoare_noua, val);
					strcat(valoare_noua, " ");
				} else {
					strcat(valoare_noua, arg);
					strcat(valoare_noua, " ");
				}
			} else {
				// adaug operatorul in sirul nou creat
				strcat(valoare_noua, arg);
				strcat(valoare_noua, " ");
			}

			prev = idx;
			free(arg);
		}

		// repet pasii din while pentru ultima bucata de sir
		if (prev != NULL) {
			if (strstr(prev, "+") == NULL &&
				strstr(prev, "-") == NULL &&
				strstr(prev, "*") == NULL) {
				val = cautare(hashMap, nr_elemente, prev);
				if (val != NULL)
					strcat(valoare_noua, val);
				else
					strcat(valoare_noua, prev);
			} else {
				strcat(valoare_noua, prev);
			}
		}

		// marchez faptul ca am intrat in acest if
		ok = 1;
	}

	// daca nu am intrat in niciunul din if-urile de mai sus,
	// voi asocia noului sir creat valoarea vechiului sir, nemodificata
	if (ok == 0) {
		memcpy(valoare_noua, valoare, strlen(valoare));
	}

	return valoare_noua;
}

// Functie care prelucreaza partea de fisiere header
char* includere_headere(char *buffer, char *subsir, char* cale_curenta,
							char** cai, int numar_cai, char* returned, 
							char* string, char* buffer_nou)
{
	char* rezultat = NULL;
	char *prima_parte = NULL;
	char *a_doua_parte = NULL;
	char* fisier_input = NULL;
	char *partea_de_mijloc = NULL;

	// aici retin prima parte, cea de dinaintea sirului "#include"
	prima_parte = calloc(100, sizeof(char));
	if (prima_parte == NULL)
		return NULL;

	// aici retin valoarea fisierului de input
	fisier_input = calloc(50, sizeof(char));
	if (fisier_input == NULL)
		return NULL;

	// aici retin continutul citit din fisierul/fisierele header
	rezultat = calloc(400, sizeof(char));
	if (rezultat == NULL)
		return NULL;
	
	// aici retin continutul de la linia cu sirul "#include"
	partea_de_mijloc = strstr(buffer, "#include");
	memcpy(prima_parte, buffer, strlen(buffer) - strlen(partea_de_mijloc));
	// aici retin ce este dupa linia ce contine sirul "#include"
	a_doua_parte = strstr(partea_de_mijloc, "\n") + 1;
	
	subsir = strstr(partea_de_mijloc, " ") + 2;
	memcpy(fisier_input, subsir, strlen(subsir) - strlen(a_doua_parte) - 2);

	rezultat = fisiere_incluse(cale_curenta, cai, numar_cai, fisier_input, 
								returned, string, rezultat);

	if (rezultat == NULL)
		return NULL;

	memcpy(buffer_nou, prima_parte, strlen(prima_parte));
	strcat(buffer_nou, rezultat);
	strcat(buffer_nou, a_doua_parte);

	free(prima_parte);
	free(fisier_input);
	free(rezultat);

	return buffer_nou;
}

// Functie care elibereaza memoria alocata
void eliberare_memorie(char *buffer, char *string, struct element *hashMap,
						char **paths, int *pozitii, 
						char *cale_curenta, char *context_literal, FILE *fout) 
{	
	int i = 0;
	
	free(buffer);
	free(string);

	for (i = 0; i < 20; i++) {
		free(hashMap[i].cheie);
		free(hashMap[i].valoare);
	}

	free(hashMap);

	for (i = 0; i < 10; i++)
		free(paths[i]);

	free(paths);
	free(pozitii);

	if (cale_curenta != NULL)
		free(cale_curenta);

	if (context_literal != NULL)
		free(context_literal);

	if (fout != NULL)
		fclose(fout);
}

int main(int argc, char *argv[])
{
	size_t x, n, dim = 0;
	FILE *fout = NULL, *fin = NULL;
	int i = 0, index = 0, numar_cai = 0, j, nr_valori, pozitie, context_lit;
	char *buffer = NULL, *returned = NULL, *string = NULL, **paths = NULL;
	char *simbol = NULL, *mapare = NULL, *cale_curenta = NULL;
	char *fisier_input = NULL, *linie = NULL;
	char *subsir = NULL, *valoare = NULL, *valoare_noua = NULL, *cheie = NULL;
	char *fisier_de_iesire = NULL, *subsir1 = NULL;
	char *argument = NULL, *val = NULL, *linie_noua = NULL;
	char *subsir2 = NULL, *context_literal = NULL, *idx = NULL, *prev = NULL;
	int *pozitii = NULL, gasit = 0, ret;
	char *partea_de_mijloc = NULL;
	char *rezultat = NULL, *buffer_nou = NULL;
	struct element *hashMap = calloc(20, sizeof(struct element));

	if (hashMap == NULL)
		return 12;

	buffer = calloc(400, sizeof(char));
	if (buffer == NULL)
		return 12;

	paths = calloc(10, sizeof(char*));
	if (paths == NULL)
		return 12;

	for (i = 0; i < 10; i++) {
		paths[i] = calloc(50, sizeof(char));

		if (paths[i] == NULL)
			return 12;
	} 

	string = calloc(50, sizeof(char));
	if (string == NULL)
		return 12;

	pozitii = calloc(10, sizeof(int));
	if (pozitii == NULL)
		return 12;	

	for (i = 0; i < 20; i++) {
		hashMap[i].cheie = calloc(50, sizeof(char));
		hashMap[i].valoare = calloc(50, sizeof(char));

		if (hashMap[i].cheie == NULL || hashMap[i].valoare == NULL)
			return 12;
	}

	if (argc == 1) { 
		// daca primesc doar fisierul executabil, citesc de la tastatura si afisez continutul
		fgets(buffer, 200, stdin);
		printf("%s", buffer);
	} else {
		i = 1;
		while (i < argc) {
			// In urmatoarele doua if-uri, tratez cazul in care primesc un define in linia de comanda
			if (strcmp(argv[i], "-D") == 0) {
				if (strchr(argv[i + 1], '=') == NULL) {
					hashMap = adaugare(hashMap, argv[i + 1], "", &index);
					if (hashMap == NULL) {
						return 12;
					}
				} else {
					simbol = strtok(argv[i + 1], "=");
					mapare = strtok(NULL, "=");
					hashMap = adaugare(hashMap, simbol, mapare, &index);
					if (hashMap == NULL)
						return 12;
				}
				i++;
			} else if (argv[i][0] == '-' && argv[i][1] == 'D') {
				mapare = NULL;
				simbol = strtok(argv[i], "=") + 2;

				if (simbol == NULL)
					simbol = argv[i] + 2;
				else
					mapare = strtok(NULL, "=");

				hashMap = adaugare(hashMap, simbol, mapare, &index);
				if (hashMap == NULL) 
					return 12;
			} else if (strcmp(argv[i], "-I") == 0) {
				// In acest if si in urmatorul tratez cazul in care primesc cai catre fisiere header
				memcpy(paths[numar_cai], argv[i + 1], strlen(argv[i + 1]));

				if (strstr(argv[i + 1], "\\") != NULL)
					strcat(paths[numar_cai], "\\");
				else if (strstr(argv[i + 1], "/") != NULL)
					strcat(paths[numar_cai], "/");

				numar_cai++;
				i++;
			} else if (argv[i][0] == '-' && argv[i][1] == 'I') {
				memcpy(paths[numar_cai], argv[i] + 2, strlen(argv[i] + 2));

				if (strchr(argv[i], '\\') != NULL)
					strcat(paths[numar_cai], "\\");
				else if (strchr(argv[i], '/') != NULL)
					strcat(paths[numar_cai], "/");

				numar_cai++;
			} else if (strcmp(argv[i], "-o") == 0) {
				// In acest if si in urmatorul tratez cazul in care primesc calea catre un fisier de iesire
				fout = fopen(argv[i + 1], "w");
				i++;
			} else if (argv[i][0] == '-' && argv[i][1] == 'o') {
				fisier_de_iesire = argv[i] + 2;

				fout = fopen(fisier_de_iesire, "w");
			} else if ((argv[i][0] == '_') && (strstr(argv[i], ".in") != NULL)) {
				// Aici tratez cazul in care primesc un fisier de intrare
				fin = fopen(argv[i], "rb");

				if (fin == NULL)
					exit(1);

				subsir1 = strrchr(argv[i], '\\');
				if (subsir1 == NULL)
					subsir1 = strrchr(argv[i], '/');

				subsir1 = subsir1 + 1;

				// Aici retin calea curenta, pe baza sirului fisierului de intrare
				cale_curenta = calloc(50, sizeof(char));
				if (cale_curenta == NULL)
					return 12;

				memcpy(cale_curenta, argv[i], strlen(argv[i]) - strlen(subsir1));

				// citesc din fisier
				while (1) {
					returned = fgets(string, 50, fin);
					
					if (returned == NULL)
						break;

					strcat(buffer, string);
				}
				fclose(fin);
			} else if ((argv[i][0] == '_') && (strstr(argv[i], ".out") != NULL)) {
				// Daca primesc un fisier de iesire, il deschid ca sa scriu in el
				fout = fopen(argv[i], "w");
			} else {
				exit(1);
			}
			i++;
		}

		// Aici tratez cazul in care am un fisier header inclus in fisierul meu de intrare
		partea_de_mijloc = strstr(buffer, "#include");
		if (partea_de_mijloc != NULL) {
			buffer_nou = calloc(400, sizeof(char));
			if (buffer_nou == NULL) 
				return 12;

			buffer_nou = includere_headere(buffer, subsir, cale_curenta, 
											paths, numar_cai, returned, string, buffer_nou);

			free(buffer);
			buffer = calloc(400, sizeof(char));
			if (buffer == NULL)
				return 12;
			
			if (buffer_nou == NULL) 
				return 12;

			// Reconstruiesc buffer-ul, alipind la continutul lui continutul
			// fisierlui/fisierelor header.
			memcpy(buffer, buffer_nou, strlen(buffer_nou));

			free(buffer_nou);
		}

		// Iterez prin buffer linie cu linie
		linie = strtok(buffer, "\n");

		while (linie != NULL) {
			if (strstr(linie, "#define") != NULL) {
				// daca am define pe linia curenta, extrag simbolul si valoarea
				cheie = calloc(50, sizeof(char));
				if (cheie == NULL) {
					return 12;
				}

				valoare_noua = calloc(50, sizeof(char));
				if (valoare_noua == NULL)
					return 12;
				
				valoare_noua = implementare_define(subsir, valoare, dim, cheie, 
									valoare_noua, linie, hashMap, index);
				if (cautare(hashMap, index, cheie) == NULL) {
					// daca nu, atunci adaug o pereche moua in hashMap
					hashMap = adaugare(hashMap, cheie, valoare_noua, &index);
					if (hashMap == NULL)
						return 12;
				} else {
					// daca am deja cheia in hashMap, inlocuiesc valoarea
					// acesteia cu noua valoare
					hashMap = inlocuire(hashMap, cheie, valoare_noua, index);
				}

				free(cheie);
				free(valoare_noua);
			} else if (strstr(linie, "#undef ") != NULL) {
				// daca am undefine, sterg cheia respectiva din hashMap, impreuna cu valoarea sa
				cheie = strstr(linie, " ") + 1;
				hashMap = stergere(hashMap, cheie, &index);		
			} else if (strstr(linie, "#if ") != NULL) {
				int gasit = 0;

				while (strstr(linie, "#endif") == NULL) {
					if (gasit == 0 && strstr(linie, " 1") != NULL) {
						linie = strtok(NULL, "\n");
						afisare(linie, fout);
						gasit = 1;
					} else if (gasit == 0 && strstr(linie, " 0") != NULL) {
						linie = strtok(NULL, "\n");
					} else if (gasit == 0) {
						if (strcmp(linie, "#else") == 0) {
							linie = strtok(NULL, "\n");
							afisare(linie, fout);
							gasit = 1;
						} else {
							cheie = strstr(linie, " ") + 1;
							valoare = cautare(hashMap, index, cheie);
							
							if (valoare != NULL) {
								if (strcmp(valoare, "1") == 0) {
									linie = strtok(NULL, "\n");
									if (strstr(linie, "#define ") != NULL) {
										cheie = calloc(50, sizeof(char));
										if (cheie == NULL)
											return 12;

										valoare_noua = calloc(50, sizeof(char));
										if (valoare_noua == NULL)
											return 12;

										subsir = strstr(linie, " ") + 1;
										valoare_noua = implementare_define(subsir, valoare, 
															dim, cheie, 
															valoare_noua, linie,
															hashMap, index);
										if (cautare(hashMap, index, cheie) == NULL) {
											// daca nu am cheia in hashMap, atunci adaug o pereche noua
											hashMap = adaugare(hashMap, cheie, valoare_noua, &index);
											if (hashMap == NULL)
												return 12;
										} else {
											// daca am deja cheia in hashMap, inlocuiesc valoarea
											// acesteia cu noua valoare
											hashMap = inlocuire(hashMap, cheie, valoare_noua, index);
										}

										free(cheie);
										free(valoare_noua);
									} else {
										afisare(linie, fout);
									}
									gasit = 1;
								} else if (valoare[0] != '0' && isdigit(valoare[0])) {
									linie = strtok(NULL, "\n");
									afisare(linie, fout);
									gasit = 1;
								} else if (strcmp(valoare, "0") == 0) {
									linie = strtok(NULL, "\n");
								} else if (strcmp(valoare, "") == 0) {
									linie = strtok(NULL, "\n");
									afisare(linie, fout);
									gasit = 1;
								}
							} else {
								linie = strtok(NULL, "\n");
							}
						}
					}
					linie = strtok(NULL, "\n");
				}
			} else if (strstr(linie, "#ifdef ") != NULL) {
				argument = strstr(linie, " ") + 1;
				valoare = cautare(hashMap, index, argument);
				if (valoare != NULL) {
					linie = strtok(NULL, "\n");

					if (strstr(linie, "#undef ") != NULL) {
						hashMap = stergere(hashMap, argument, &index);
					} else {
						if (strstr(linie, "#define") != NULL) {
							cheie = calloc(50, sizeof(char));
							if (cheie == NULL)
								return 12;

							valoare_noua = calloc(50, sizeof(char));
							if (valoare_noua == NULL)
								return 12;

							subsir = strstr(linie, " ") + 1;
							valoare_noua = implementare_define(subsir, valoare, dim, cheie, 
												valoare_noua, linie, hashMap, index);
							if (cautare(hashMap, index, cheie) == NULL) {
								// daca nu, atunci adaug o pereche moua in hashMap
								hashMap = adaugare(hashMap, cheie, valoare_noua, &index);
								if (hashMap == NULL)
									return 12;
							} else {
								// daca am deja cheia in hashMap, inlocuiesc valoarea
								// acesteia cu noua valoare
								hashMap = inlocuire(hashMap, cheie, valoare_noua, index);
							}

							free(cheie);
							free(valoare_noua);
						} else {
							afisare(linie, fout);
						}
					}
				} else {
					// trec pe linia urmatoare
					linie = strtok(NULL, "\n");
					if (strcmp(linie, "#else") == 0) {
						linie = strtok(NULL, "\n");
						// verific daca urmeaza un define
						if (strstr(linie, "#define ") != NULL) {
							cheie = calloc(50, sizeof(char));
							if (cheie == NULL)
								return 12;

							valoare_noua = calloc(50, sizeof(char));
							if (valoare_noua == NULL)
								return 12;

							subsir = strstr(linie, " ") + 1;
							// retin valoarea cheii in variabila valoare_noua
							valoare_noua = implementare_define(subsir, valoare, 
															dim, cheie, 
															valoare_noua, linie,
															hashMap, index);
							if (cautare(hashMap, index, cheie) == NULL) {
								// daca nu am cheia in hashMap, atunci adaug o pereche noua
								hashMap = adaugare(hashMap, cheie, valoare_noua, &index);
								if (hashMap == NULL)
									return 12;
							} else {
								// daca am deja cheia in hashMap, inlocuiesc valoarea
								// acesteia cu noua valoare
								hashMap = inlocuire(hashMap, cheie, valoare_noua, index);
							}

							free(cheie);
							free(valoare_noua);
						} else {
							afisare(linie, fout);
						}
					}
				}

				// termin de incheiat ciclul
				while (strstr(linie, "#endif") == NULL)
					linie = strtok(NULL, "\n");
			} else if (strstr(linie, "#ifndef ") != NULL) {
				// Implementez directia "#ifndef"
				linie = strtok(NULL, "\n");

				if (strstr(linie, "#define") != NULL) {
					argument = strstr(linie, " ") + 1;
					cheie = calloc(50, sizeof(char));
					if (cheie == NULL) {
						return 12;
					}

					if (strstr(argument, " ") != NULL) {
						valoare = strstr(argument, " ");
						memcpy(cheie, argument, strlen(argument) - strlen(valoare));
					} else {
						valoare = NULL;
					}

					hashMap = adaugare(hashMap, cheie, valoare, &index);
					if (hashMap == NULL)
						return 12;

					free(cheie);
				}
			} else if (strstr(linie, "#endif") == NULL) {
				// Implementez directiva "#endif"
				nr_valori = 0;
				context_lit = 0;
				linie_noua = calloc(50, sizeof(char));
				if (linie_noua == NULL) {
					return 12;
				}

				cautare_in_sir(hashMap, linie, index, &nr_valori, pozitii);

				if (nr_valori > 0) {
					// Determin contextul literal, ce se afla intre ""
					subsir1 = strchr(linie, '"');
					idx = linie;
					prev = linie;
					if (subsir1 != NULL) {
						context_literal = calloc(50, sizeof(char));
						if (context_literal == NULL) {
							return 12;
						}

						subsir2 = strchr(subsir1 + 1, '"');
						dim = strlen(linie) - strlen(subsir2) + 1;
						memcpy(context_literal, linie, dim);
					}
					
					// Pentru perechile gasite
					for (j = 0; j < nr_valori; j++) {
						pozitie = pozitii[j];
						simbol = hashMap[pozitie].cheie;
						val    = hashMap[pozitie].valoare;
						idx = strstr(idx, simbol);

						// verific daca simbolul curent apare in contextul literal
						if (context_literal != NULL &&
							strstr(context_literal, simbol) != NULL) {
							// daca da, atunci il copiez direct in linia nou formata
							strncat(linie_noua, prev, strlen(prev) - strlen(idx));
							strncat(linie_noua, simbol, strlen(simbol));
							context_lit = 1;
						}

						// verific daca simbolul curent nu apare intr-un context literal
						if (context_literal == NULL || 
							(context_literal != NULL &&
							strstr(context_literal, simbol) == NULL)) {
							// il inlocuiesc in linia noua cu valoarea lui
							strncat(linie_noua, prev, strlen(prev) - strlen(idx));
							strncat(linie_noua, val, strlen(val));
						}

						// verific daca simbolul curent apare si in contextul nonliteral
						if (context_lit == 1 && subsir2 != NULL && 
							strstr(subsir2 + 1, simbol) != NULL) {
							prev = strstr(prev, simbol) + strlen(simbol);
							idx = idx + strlen(simbol);
							idx = strstr(idx, simbol);
							strncat(linie_noua, prev, strlen(prev) - strlen(idx));
							strncat(linie_noua, val, strlen(val));
						}
						prev = strstr(prev, simbol);
						prev = prev + strlen(simbol);
					}
					strncat(linie_noua, prev, strlen(prev));
					afisare(linie_noua, fout);
				} else {
					afisare(linie, fout);
				}

				free(linie_noua);
			}
			linie = strtok(NULL, "\n");
		}
	}
	
	eliberare_memorie(buffer, string, hashMap, paths, pozitii, 
						cale_curenta, context_literal, fout);

	return 0;
}




