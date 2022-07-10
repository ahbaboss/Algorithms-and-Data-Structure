#include <iostream>

using namespace std;
template <typename Tip>
class Red
{
    struct Cvor {
        Tip element;
        Cvor* sljedeci;
        Cvor (const Tip &e, Cvor *s) : element(e), sljedeci(s) {}
    };
    Cvor *pocetak;
    Cvor *kraj;
    int velicina;
    void Unisti()
    {
        while (brojElemenata()) skini();
    }
    void Kopiraj (const Red &q)
    {
        Cvor *p(q.pocetak);
        while (p!=0) {
            stavi(p->element);
            p=p->sljedeci;
        }
    }
public:
    Red() : pocetak(0),kraj(0),velicina(0) {}
    Red(const Red &q) : pocetak(0),kraj(0),velicina(0)
    {
        Kopiraj(q);
    }
    ~Red()
    {
        Unisti();
    }
    Red &operator =(const Red &q)
    {
        if (&q!=this) {
            Unisti();
            Kopiraj(q);
        }
        return *this;
    }
    void stavi (const Tip& element)
    {
        Cvor *n=new Cvor(element,0);
        if (pocetak==nullptr) pocetak=kraj=n;
        else {
            kraj->sljedeci=n;
            kraj=n;
        }
        velicina++;
    }
    Tip skini()
    {
        if (velicina==0) throw ("Red je prazan");
        Tip elem(pocetak->element);
        Cvor *p=pocetak;
        if (pocetak==kraj) pocetak=kraj=0;
        else pocetak=pocetak->sljedeci;
        delete p;
        velicina--;
        return elem;
    }
    void brisi()
    {
        Unisti();
    }
    int brojElemenata() const
    {
        return velicina;
    }
    Tip &celo() 
    {
        if (pocetak==0) throw "Prazan red";
        return pocetak->element;
    }

};
int main()
{
    Red<int> r1;
    for (int i = 1; i <= 5; i++) r1.stavi(i);
    for (int i = 1; i <= 5; i++) cout << r1.skini() << " ";
    cout<<endl;
    for (int i = 1; i <= 10; i++) r1.stavi(i);
    Red<int> r2(r1);
    for (int i = 1; i <= 10; i++) cout << r1.skini() << " ";
    Red<int> r3;
    cout << endl << (r3=r2).brojElemenata() << " ";
    cout << endl << r3.celo();
    r3.celo() = 10;
    cout << endl << r3.celo();
    r2.brisi();
    
}
