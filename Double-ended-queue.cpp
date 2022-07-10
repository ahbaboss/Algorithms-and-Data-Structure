#include <iostream>
using namespace std;
template <typename Tip>
class DvostraniRed
{
    struct Cvor {
        Tip element;
        Cvor* prethodni;
        Cvor* sljedeci;
        Cvor (const Tip& element, Cvor *prethodni, Cvor *sljedeci) : element(element), prethodni(prethodni), sljedeci(sljedeci) {}
    };
    int velicina;
    Cvor* pocetak;
    Cvor* kraj;
    void unisti()
    {
        while (brojElemenata()) skiniSaCela();
    }
    void kopiraj(const DvostraniRed &dr)
    {
        Cvor *p(dr.pocetak);
        while (p!=0) {
            staviNaVrh(p->element);
            p=p->sljedeci;
        }
    }
public:
    DvostraniRed() : velicina(0), pocetak(0), kraj(0) {}
    ~DvostraniRed()
    {
        unisti();
    }
    DvostraniRed(const DvostraniRed &dr) : velicina(0), pocetak(0), kraj(0)
    {
        kopiraj(dr);
    }
    DvostraniRed &operator = (const DvostraniRed &dr)
    {
        if (&dr!=this) {
            unisti();
            kopiraj(dr);
        }
        return *this;
    }
    int brojElemenata()
    {
        return velicina;
    }
    void brisi()
    {
        unisti();
    }
    Tip skiniSaCela()
    {
        if (velicina==0) throw ("Red je prazan");
        Tip elem(pocetak->element);
        Cvor *p=pocetak;
        if (pocetak==kraj) pocetak=kraj=0;
        else {
            pocetak=pocetak->sljedeci;
            pocetak->prethodni=nullptr;
        }
        delete p;
        velicina--;
        return elem;
    }
    void staviNaVrh (const Tip& el)
    {
        Cvor *n=new Cvor(el,0,0);
        if (pocetak==nullptr) pocetak=kraj=n;
        else if (pocetak==kraj) {
            kraj->sljedeci=n;
            kraj=n;
            kraj->prethodni=pocetak;
        } else {
            kraj->sljedeci=n;
            n->prethodni=kraj;
            kraj=n;
        }
        velicina++;
    }

    Tip &celo ()
    {
        if (pocetak==0) throw "Prazan red";
        return pocetak->element;
    }
    Tip &vrh ()
    {
        if (kraj==0) throw "Prazan red";
        return kraj->element;
    }
    Tip skiniSaVrha()
    {
        if (velicina==0) throw "Prazan red";
        Tip elem(kraj->element);
        Cvor *p=kraj;
        if (pocetak==kraj) pocetak=kraj=0;
        else {
            kraj=kraj->prethodni;
            kraj->sljedeci=nullptr;
        }
        delete p;
        velicina--;
        return elem;
    }
    void staviNaCelo (const Tip &el)
    {
        Cvor *n = new Cvor (el,0,0);
        if (pocetak==nullptr) pocetak=kraj=n;
        else if (pocetak==kraj) {
            pocetak->prethodni=n;
            pocetak=n;
            pocetak->sljedeci=kraj;
        } else {
            pocetak->prethodni=n;
            n->sljedeci=pocetak;
            pocetak=n;
        }
        velicina++;
    }

};
int main()
{
    DvostraniRed<int> r1;
    for (int i = 1; i <= 5; i++) r1.staviNaVrh(i);
    for (int i = 1; i <= 5; i++) cout << r1.skiniSaCela() << " ";
    cout<<endl;
    for (int i = 1; i <= 10; i++) r1.staviNaCelo(i);
    DvostraniRed<int> r2(r1);
    for (int i = 1; i <= 10; i++) cout << r1.skiniSaVrha() << " ";
    DvostraniRed<int> r3;
    cout << endl << (r3=r2).brojElemenata() << " ";
    cout << endl << r3.celo();
    r3.celo() = 10;
    cout << endl << r3.celo();
    r2.brisi();
    return 0;
}
