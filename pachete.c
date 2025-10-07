#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
// declarare structurile;

// structura cartier
typedef struct
{
    int id;
    char *nume;
} cartier;

// structura pachet
typedef struct
{
    int id;
    int adresa[18];
    int idCartier;
    int strada;
    int numar;
    int prioritate;
    float greutate;
    char *mesaj;
    int codificare_mesaj;
} pachet;

// structura poștaș
typedef struct
{
    int id;
    int nrPachete;
    int v[50];
} postas;

void citire(int *nrC, cartier c[], int *nrP, pachet p[])
// cu ajutorul acestei funcții citim datele de intrare;
{
    scanf("%d", nrC); // introduc de la tastatură numărul de cartiere
    getchar();
    int i, j; // consumă caracterul de newline rămas în bufferul de intrare
    for (i = 0; i < *nrC; i++)
    {
        c[i].id = i;   // setez id ul cartierului
        char aux[110]; // introduc de la tastatură șirul
        fgets(aux, sizeof(aux), stdin);
        aux[strcspn(aux, "\n")] = '\0';              // elimină carcacterul de newline
        c[i].nume = (char *)malloc(strlen(aux) + 1); // aloc dinamic memorie;
        strcpy(c[i].nume, aux);
    }
    scanf("%d", nrP); // introduc de la tastatură numărul de pachete
    getchar();
    for (i = 0; i < *nrP; i++)
    {
        p[i].id = i; // setez id ul pachetului
        for (j = 0; j < 18; j++)
        {
            scanf("%d", &p[i].adresa[j]); // introduc de la tastatură adresa pachetului
            getchar();
        }
        // introduc de la tastatură prioritatea pachetului
        scanf("%d", &p[i].prioritate);
        getchar();
        // introduc de la tastatură greutatea pachetului
        scanf("%f", &p[i].greutate);
        getchar();
        char ajutor[110];
        // citesc de la tastatură șirul
        fgets(ajutor, sizeof(ajutor), stdin);
        // elimină carcacterul de newline
        ajutor[strcspn(ajutor, "\n")] = '\0';
        p[i].mesaj = (char *)malloc(strlen(ajutor) + 1); // aloc dinamic memorie
        strcpy(p[i].mesaj, ajutor);
    }
}
void extragere(pachet p[], int n)
// află idCartier,număr și strada pentru un pachet
{
    int i;
    p[n].idCartier = 0; // Resetare înainte de calcul
    p[n].strada = 0;
    p[n].numar = 0;
    for (i = 0; i < 5; i++)
        p[n].idCartier += pow(2, i) * p[n].adresa[4 - i];
    // trec din binar in zecimal
    for (i = 0; i < 5; i++)
        p[n].strada += pow(2, i) * p[n].adresa[9 - i];
    // trec din binar in zecimal
    for (i = 0; i < 8; i++)
        p[n].numar += pow(2, i) * p[n].adresa[17 - i];
    // trec din binar in zecimal
}
void distribuire(postas po[], pachet p[], int nrC, int nrP)
// distribui pachetele poștașilor
{
    int i;
    for (i = 0; i < nrC; i++)
    {
        po[i].id = i; // setez id ul poștașului
        po[i].nrPachete = 0;
        // inițial acesta nu are de distribuit niciun pachet
        int j;
        for (j = 0; j < nrP; j++)
        {
            extragere(p, j);
            if (po[i].id == p[j].idCartier) // verific dacă cele două id coincid
            {
                po[i].v[po[i].nrPachete] = p[j].id; // creez vectorul de pachete
                po[i].nrPachete++;
            }
        }
    }
}
void sortare(pachet p[], postas po[], int n)
{
    int i, j;
    for (i = 0; i < po[n].nrPachete - 1; i++)
    // sortez vectorul de pachete,verificând fiecare element din vector cu următoarele
    {
        for (j = i + 1; j < po[n].nrPachete; j++)
        {
            int index1 = po[n].v[i];
            int index2 = po[n].v[j];
            // compar după prioritate sau dacă cele două sunt egale , compar după greutate
            if (p[index1].prioritate < p[index2].prioritate ||
            (p[index1].prioritate == p[index2].prioritate && p[index1].greutate < p[index2].greutate))
            {
                int aux = po[n].v[i];    // folosim o variabilă auxiliară pentru interschimbare
                po[n].v[i] = po[n].v[j]; // interschimbare id-urile în vectorul poștașului
                po[n].v[j] = aux;
            }
        }
    }
}
void inversare(pachet p[], int n)
{
    int j;
    char *cautare = strtok(p[n].mesaj, " .,!?:");
    char aux[110] = "";    // Inițializăm aux ca un șir gol
    char cuvinte[32][100]; // Tablou de cuvinte
    int i = 0;
    while (cautare) // caut în șir
    {
        strcpy(cuvinte[i], cautare); // Salvăm fiecare cuvânt
        i++;
        cautare = strtok(NULL, " .,!?:");
    }
    for (j = i - 1; j >= 0; j--)
        strcat(aux, cuvinte[j]);
    // formez noul șir unde ordinea cuvintele este inversată de aceea for-ul îl parcurg de la coadă la cap
    strcpy(p[n].mesaj, aux); // copiez șirul în variabila p[n].mesaj
}
void calcul(pachet p[], int n, long int *cod)
{
    *cod = 0;
    int i;
    extragere(p, n); // aflam numarul strazii si numarul casei
    inversare(p, n); // creez noul mesaj inversat
    for (i = 0; i < strlen(p[n].mesaj); i++)
        *cod += i * (int)p[n].mesaj[i];           // calculez codul
    *cod = *cod % (p[n].numar * p[n].strada + 1); // calculez codul
}
int modif(int id, long int cod) // modifică codul după cerințele specificate
{
    int d;
    if (id == 0)
        // Dacă id este 0, se face un XOR între cod și 1 (schimbă primul bit din cod)
        cod = cod ^ 1;
    else if (id == 1)
        // Dacă id este 1, se face un XOR între cod și 2^1 (schimbă al doilea bit din cod)
        cod = cod ^ (1 << 1);
    else
    {
        d = 2;
        while (id > 1 && d < 32)
        // algortim de descompunere a unui număr în factor primi;
        {
            if (id % d == 0)
            {
                cod = cod ^ (1 << d);
                // Dacă d este divizor al id, se face un XOR între cod și 2^d
                while (id % d == 0)
                    id /= d;
            }
            d++;
        }
    }
    return cod; // Se returnează noul cod modificat
}
void alterare(int id, long int *cod)
{
    int ok = 0, a, b, aux = *cod;
    if (id > 9)
    // dacă id ul are două cifre, țin minte ambele cifre în două variabile
    {
        a = id % 10;
        b = id / 10;
    }
    else
    {
        a = id;
        b = -1; // id are doar o cifră așa că b ia valoare -1
    }
    ok = 0;
    while (aux)
    {
        if (aux % 10 == a || aux % 10 == b)
            // verific dacă se regăsește vreo cifră din id-ul poștașului în cod
            ok = 1;
        aux /= 10;
    }
    if (ok == 1)                // dacă se găsește alterez codul
        *cod = modif(id, *cod); // noua valoare a codului
}
void punctaj(postas po[], pachet p[], int nrC, int nrP, long int cod)
{
    int i, j;
    distribuire(po, p, nrC, nrP); // distribui pachetele
    for (i = 0; i < nrC; i++)
    {
        printf("%d ", po[i].id); // afișesz id-ul poștașului
        sortare(p, po, i);       // sortez pachetele
        if (po[i].nrPachete > 0) // verific dacă are pachete de distribuit sau nu
        {
            float cnt = 0;
            float rez = 0;
            for (j = 0; j < po[i].nrPachete; j++)
            // verific pentru fiecare pachet în parte dacă a fost distribuit corect
            {

                calcul(p, po[i].v[j], &cod);
                float aux = cod;
                alterare(po[i].id, &cod);
                if (aux == cod)
                    cnt++; // contorieze pachetele livrate corect
            }
            rez = (float)cnt / po[i].nrPachete; // calculez punctajul
            printf("%.3f\n", rez);              // afișez punctajul
        }
        else
            printf("0.000\n"); // dacă nu a avut pachete de distribuit, scorul este 0
    }
}
int main()
{
    int nrC, nrP, nrcerinta, i, j; // inițializez variabilele
    long int cod;
    cartier c[33];
    pachet p[51];
    postas po[33];
    scanf("%d", &nrcerinta);
    // citim de la tastatură numărul cerinței
    citire(&nrC, c, &nrP, p);
    // citim datele de intrare
    if (nrcerinta == 1) // luăm pe cazuri
    {
        for (i = 0; i < nrC; i++)
            printf("%d %s\n", c[i].id, c[i].nume);
        // afișăm id-ul și numele cartierului
        for (i = 0; i < nrP; i++)
        {
            printf("%d\n", p[i].id);
            for (j = 0; j < 17; j++)
                printf("%d ", p[i].adresa[j]);
            printf("%d\n", p[i].adresa[17]);                     // afișăm adresa
            printf("%d %.3f\n", p[i].prioritate, p[i].greutate); // afișăm prioritatea și greutatea
            printf("%s\n", p[i].mesaj);                          // afișăm mesajul
        }
    }
    if (nrcerinta == 2)
    {
        for (i = 0; i < nrP; i++)
        {
            extragere(p, i);
            // află idCartier,număr și strada pentru un pachet
            printf("%d\n", p[i].id);
            // afișez id pachet
            printf("%d %d %d\n", p[i].idCartier, p[i].strada, p[i].numar);
            // afișez idcartier,strada si numărul pachetului
        }
    }
    if (nrcerinta == 3)
    {
        distribuire(po, p, nrC, nrP); // distribuie pachetele fiecărui poștaș în parte
        for (i = 0; i < nrC; i++)
        {
            printf("%d %d\n", po[i].id, po[i].nrPachete);
            // afișăm id-ul poștașului și numărul de pachete pe care le are de distribuit
            if (po[i].nrPachete > 0)
            // verificăm dacă are pachete de distribuit
            {
                for (j = 0; j < po[i].nrPachete - 1; j++)
                    printf("%d ", po[i].v[j]);
                printf("%d\n", po[i].v[po[i].nrPachete - 1]);
                // afișăm id ul fiecărui pachet pe care îl are de distribuit
            }
        }
    }
    if (nrcerinta == 4)
    {
        distribuire(po, p, nrC, nrP); // distribuim pachetele fiecărui poștaș
        for (i = 0; i < nrC; i++)
        {
            printf("%d %d\n", po[i].id, po[i].nrPachete);
            // afișăm id-ul poștașului și numărul de pachete pe care le are de distribuit
            sortare(p, po, i);
            // sortăm pachetele în funcție de prioritate sau greutate
            if (po[i].nrPachete > 0)
            {
                for (j = 0; j < po[i].nrPachete - 1; j++)
                    printf("%d ", po[i].v[j]);
                printf("%d\n", po[i].v[po[i].nrPachete - 1]);
                // afișăm id ul fiecărui pachet pe care îl are de distribuit
            }
        }
    }
    if (nrcerinta == 5)
    {
        distribuire(po, p, nrC, nrP);
        // distribuim pachetele fiecărui poștaș
        for (i = 0; i < nrC; i++)
        {
            printf("%d %d\n", po[i].id, po[i].nrPachete);
            // afișăm id-ul poștașului și numărul de pachete pe care le are de distribuit
            sortare(p, po, i); // sortăm pachetele în funcție de prioritate sau greutate
            if (po[i].nrPachete > 0)
            {
                for (j = 0; j < po[i].nrPachete; j++)
                {
                    printf("%d ", po[i].v[j]);
                    // afișăm id ul fiecărui pachet pe care îl are de distribuit
                    calcul(p, po[i].v[j], &cod);
                    // calculăm codul
                    printf("%ld\n", cod);
                    // afișăm codul respectivului pachet
                }
            }
        }
    }
    if (nrcerinta == 6)
    {
        distribuire(po, p, nrC, nrP); // distribuim pachetele fiecărui poștaș
        for (i = 0; i < nrC; i++)
        {
            printf("%d %d\n", po[i].id, po[i].nrPachete);
            // afișăm id-ul poștașului și numărul de pachete pe care le are de distribuit
            sortare(p, po, i);
            // sortăm pachetele în funcție de prioritate sau greutate
            if (po[i].nrPachete > 0)
            {
                for (j = 0; j < po[i].nrPachete; j++)
                {
                    printf("%d ", po[i].v[j]);
                    // afișăm id ul fiecărui pachet pe care îl are de distribuit
                    calcul(p, po[i].v[j], &cod);
                    // calculăm codul
                    alterare(po[i].id, &cod);
                    // alterez codul dacă este cazul
                    printf("%ld\n", cod);
                    // afișăm codul respectivului pachet
                }
            }
        }
    }
    if (nrcerinta == 7)
    {
        punctaj(po, p, nrC, nrP, cod);
        // afișează id ul fiecărui poștaș urmat de scorul obținut de acesta
    }
    return 0;
}
