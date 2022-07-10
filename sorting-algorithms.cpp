#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <ctime>

using namespace std;

template <typename Tip>
void bubble_sort(Tip* niz, int vel)
{
    for (int i = vel-1; i > 0; i--)
        for (int j=1; j <= i; j++)
            if (niz[j-1]>niz[j])
                swap(niz[j-1],niz[j]);
}
template <typename Tip>
void selection_sort(Tip* niz, int vel)
{
    for (int i = 0; i < vel-1; i++) {
        Tip min = niz[i];
        int pmin=i;
        for (int j = i+1; j < vel; j++) {
            if (niz[j]<min) {
                min=niz[j];
                pmin=j;
            }
        }
        niz[pmin]=niz[i];
        niz[i]=min;
    }
}
template <typename Tip>
int particija (Tip* niz, int prvi, int zadnji)
{
    Tip pivot = niz[prvi];
    int p = prvi+1;
    while (p<=zadnji && niz[p]<pivot) p++;
    for (int i = p+1; i <= zadnji; i++) {
        if (niz[i]<pivot) {
            swap(niz[i],niz[p]);
            p++;
        }
    }
    swap(niz[prvi],niz[p-1]);
    return p-1;
}
template <typename Tip>
void quick (Tip* niz, int prvi, int zadnji)
{
    if (prvi<zadnji) {
        int j=particija(niz,prvi,zadnji);
        quick(niz,prvi,j-1);
        quick(niz,j+1,zadnji);
    }
}
template <typename Tip>
void quick_sort(Tip* niz, int vel)
{
    quick(niz,0,vel-1);
}
template <typename Tip>
void merge (Tip* niz, int l, int p, int q, int u)
{
    int i = 0;
    int j = q-l;
    int k = l;
    Tip* niz2 = new Tip[u-l+1] {};
    for (int m=0; m<=u-l; m++) {
        niz2[m]=niz[l+m];
    }
    while (i<=p-l && j <= u-l) {
        if (niz2[i]<niz2[j]) {
            niz[k]=niz2[i];
            i++;
        } else {
            niz[k]=niz2[j];
            j++;
        }
        k++;
    }
    while (i<=p-l) {
        niz[k]=niz2[i];
        k++;
        i++;
    }
    while (j<=u-l) {
        niz[k]=niz2[j];
        k++;
        j++;
    }
    delete[] niz2;
}
template <typename Tip>
void mergesort(Tip* niz, int l, int u)
{
    if (u>l) {
        int p=(int)(l+u-1)/2;
        int q=p+1;
        mergesort(niz,l,p);
        mergesort(niz,q,u);
        merge(niz,l,p,q,u);
    }
}
template <typename Tip>
void merge_sort(Tip* niz, int vel)
{
    mergesort(niz,0,vel-1);
}

void ucitaj(string filename, int*& niz, int &vel)
{
    int velicina(0);
    ifstream ulazni_tok(filename);
    vector<int> pom;
    if (!ulazni_tok) cout << "Otvaranje datoteke nije uspjelo!\n";
    else {
        int br;
        char znak;
        while (ulazni_tok >> br >> znak) {
            pom.push_back(br);
            velicina++;
        }
    }
    vel=velicina;
    niz = new int[vel];
    for (int i = 0; i < vel; i++) niz[i]=pom.at(i);
}
void generisi(string filename, int vel)
{
    ofstream izlazni_tok(filename);
    if (!izlazni_tok) cout << "Kreiranje datoteke nije uspjelo!\n";
    else {
        for (int i = 0; i < vel; i++) izlazni_tok << rand()%10000 << " ";
    }
}
void pom (string filename)
{
    int* niz;
    int velicina;
    ucitaj(filename,niz,velicina);
    int opcija;
    do {
        cout << "Unesite opciju: " << endl;
        cout << "1. Bubble sort" << endl;
        cout << "2. Selection sort" << endl;
        cout << "3. Quick sort" << endl;
        cout << "4. Merge sort" << endl;
        cin.clear();
        cin.ignore(1000,'\n');
        cin>>opcija;
    } while (!cin || opcija<1 || opcija>4);
    if (opcija==1) {
        clock_t vrijeme1=clock();
        bubble_sort(niz,velicina);
        clock_t vrijeme2=clock();
        int ukvrijeme = (vrijeme2-vrijeme1) / (CLOCKS_PER_SEC/1000);
        cout << "Vrijeme izvrsenja: " << ukvrijeme << " ms." << endl;

    } else if (opcija==2) {
        clock_t vrijeme1=clock();
        selection_sort(niz,velicina);
        clock_t vrijeme2=clock();
        int ukvrijeme = (vrijeme2-vrijeme1) / (CLOCKS_PER_SEC/1000);
        cout << "Vrijeme izvrsenja: " << ukvrijeme << " ms." << endl;
    } else if (opcija==3) {
        clock_t vrijeme1=clock();
        quick_sort(niz,velicina);
        clock_t vrijeme2=clock();
        int ukvrijeme = (vrijeme2-vrijeme1) / (CLOCKS_PER_SEC/1000);
        cout << "Vrijeme izvrsenja: " << ukvrijeme << " ms." << endl;
    } else if (opcija==4) {
        clock_t vrijeme1=clock();
        merge_sort(niz,velicina);
        clock_t vrijeme2=clock();
        int ukvrijeme = (vrijeme2-vrijeme1) / (CLOCKS_PER_SEC/1000);
        cout << "Vrijeme izvrsenja: " << ukvrijeme << " ms." << endl;
    }
    bool istina=true;
    for (int i = 0; i < velicina-1; i++) {
        if (niz[i]>niz[i+1]) istina=false;
    }
    if (istina) cout << "Niz je sortiran";
    else cout<<"Niz nije sortiran";
    
    ofstream izlaz ("izlaz.txt");
    for (int i = 0; i < velicina; i++)
        izlaz << niz[i] << " ";
    delete[] niz;
}
int main()
{
    generisi("autotest.txt", 30);
    pom("autotest.txt");
    cout << endl << "OK";
}
