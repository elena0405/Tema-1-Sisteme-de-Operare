IONESCU ELENA 336CA, semigrupa 1

TEMA 1 SO

Pentru rezolvarea temei, am inplementat un hashMap ca fiind un vector de elemente alocate dinamic.
Elementele sunt de tipul struct si au doua campuri: cheie si valoare. Pentru adaugarea, stergerea,
cautarea unui element intr-un hashMap, am implementat cate o functie, dupa cum am specificat si
in fisierul so-cpp.c. Cele doua Makefile-uri le-am scris conform exemplelor din laboratorul 1.
Pentru prima parte a temei, am interpretat parametrii din linia de comanda si am extras informatiile
necesare: daca am calea catre fisierul de intrare citesc datele din el, daca am cai catre fisiere header
le retin intr-un vector de stringuri, daca am varibile definite prin optiunea -D, le prelucrez si le
adaug in hashMap, iar daca am un fisier de iesire, il deschid. In continuare, verific daca am un 
fisier header de inclus; daca da, atunci il deschis si atasez datele acestuia la datele retinute
din fisierul de intrare, iar daca nu, las buffer-ul in care am datele respective nemodificat.
Intrepretez buffer-ul linie cu linie si in functie de parametrii cheie de care ma lovesc (define, 
undef, ifndef, ifdef, endif), prelucrez sirurile de caractere si afisez in fisierul de iesire (daca
ni se ofera) sau la consola. La final, eliberez toata memoria alocata.

Link-uri folosite in rezolvarea temei:
https://www.tutorialspoint.com/c_standard_library/c_function_strstr.htm
https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
https://www.programiz.com/c-programming/library-function/string.h/strcat
https://ocw.cs.pub.ro/courses/so/laboratoare/laborator-01

Link catre git:
https://gitlab.cs.pub.ro/elena.ionescu0405/l3-so-assignments
(codul sursa al temei este la calea tema1/so-cpp.c)