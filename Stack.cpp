#include <iostream>
#include <stdexcept>
using namespace std;

template <typename Tip>
class Stek
{
    struct Cvor {
        Tip element;
        Cvor *sljedeci;
        Cvor (const Tip &element, Cvor *sljedeci) : element(element), sljedeci(sljedeci) {}
    };
    Cvor *top;
    int velicina;
    void kopiraj (const Stek &stek)
    {
        Cvor *q, *p(stek.top);
        top=nullptr;
        velicina=stek.velicina;
        while (p!=0) {
            Cvor *n(new Cvor (p->element,nullptr));
            if (!top) top = n;
            else q->sljedeci = n;
            p=p->sljedeci;
            q=n;
        }
    }
public:
    Stek()
    {
        top=nullptr;
        velicina = 0;
    }
    ~Stek()
    {
        brisi();
    }
    Stek (const Stek &stek)
    {
        kopiraj(stek);
    }
    Stek &operator =(const Stek &stek)
    {
        if (&stek==this) return *this;
        brisi();
        kopiraj(stek);
        return *this;
    }
    Tip skini()
    {
        if (top==nullptr) throw ("Prazan stek");
        Tip novi = top->element;
        auto p = top->sljedeci;
        delete top;
        top = p;
        velicina--;
        return novi;
    }
    int brojElemenata() const
    {
        return velicina;
    }
    Tip &vrh()
    {
        if (!brojElemenata()) throw ("Prazan stek");
        return top->element;
    }
    void brisi()
    {
        while (brojElemenata()) skini();
    }
    void stavi (const Tip &element)
    {
        top = new Cvor (element, top);
        velicina++;
    }

};
int main()
{
    return 0;
}
