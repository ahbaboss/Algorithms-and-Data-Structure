#include <iostream>
#include <stdexcept>
#include <string>
#include <limits>
using namespace std;

template <typename Tip>
class Iterator;

template <typename Tip>
class Lista
{
public:
    Lista() {};
    virtual ~Lista() = default;
    virtual int brojElemenata() const = 0;
    virtual Tip trenutni() const = 0;
    virtual Tip& trenutni() = 0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual void obrisi() = 0;
    virtual void dodajIspred (const Tip& el) = 0;
    virtual void dodajIza (const Tip& el) = 0;
    virtual Tip operator [] (int i) const = 0;
    virtual Tip &operator [] (int i) = 0;
};


template <typename Tip>
class NizLista : public Lista<Tip>
{
    int tekuci;
    int velicina;
    int granica;
    Tip** L;
    void prosiri()
    {
        granica *= 2;
        auto Lpom = new Tip* [granica] {};
        for (int i=0; i<velicina+1; i++) {
            Lpom[i] = L[i];
        }
        delete[] L;
        L = Lpom;
    }
public:
    NizLista()
    {
        velicina = 0;
        tekuci=-1;
        L = new Tip* [100] {};
        granica = 100;
    }
    ~NizLista()
    {
        if (L != nullptr) {
            for (int i = 0; i < velicina; i++) delete L[i];
            delete[] L;
        }
    }
    NizLista(const NizLista &lista)
    {
        L = new Tip* [lista.granica] {};
        for (int i = 0; i < lista.velicina; i++) L[i] = new Tip (*(lista.L[i]));
        velicina = lista.velicina;
        tekuci = lista.tekuci;
        granica = lista.granica;
    }
    NizLista &operator =(const NizLista &lista)
    {
        for (int i = 0; i < velicina; i++) delete L[i];
        delete[] L;
        L = new Tip* [lista.granica] {};
        for (int i = 0; i < velicina; i++) L[i] = new Tip (*(lista.L[i]));
        velicina = lista.velicina;
        tekuci = lista.tekuci;
        granica = lista.granica;
        return *this;
    }
    int brojElemenata() const override
    {
        return velicina;
    }
    Tip trenutni() const override
    {
        return *(L[tekuci]);
    }
    Tip& trenutni() override
    {
        return *(L[tekuci]);
    }
    bool prethodni() override
    {
        if (velicina==0 || tekuci == 0) return false;
        tekuci -= 1;
        return true;
    }
    bool sljedeci() override
    {
        if (velicina==0 || tekuci == velicina-1) return false;
        tekuci += 1;
        return true;
    }
    void dodajIspred (const Tip& el) override
    {
        if (velicina >= granica) {
            granica *= 2;
            auto Lpom = new Tip* [granica] {};
            for (int i = 0; i<velicina+1; i++) {
                if (i>tekuci) Lpom[i]=L[i-1];
                else if (i==tekuci) Lpom[i]=new Tip (el);
                else Lpom[i]=L[i];
            }
            tekuci++;
            velicina++;
            delete[] L;
            L = Lpom;
        } else if (velicina == 0) {
            L[0] = new Tip(el);
            tekuci++;
            velicina++;
        } else {
            for (int i = velicina; i > tekuci; i--) L[i] = L[i-1];
            L[tekuci] = new Tip (el);
            tekuci++;
            velicina++;
        }
    }
    void dodajIza (const Tip& el) override
    {
        if (velicina > 0 && tekuci==velicina-1) {
            if (velicina-1 == granica) prosiri();
            L[velicina] = new Tip (el);
            velicina++;
        } else if (velicina > 0 && tekuci != velicina-1) {
            if (velicina-1 == granica) prosiri();
            for (int i = velicina; i >= tekuci+1; i--) L[i] = L[i-1];
            L[tekuci+1] = new Tip (el);
            velicina++;
        } else {
            if (velicina-1 == granica) prosiri();
            tekuci++;
            velicina++;
            L[tekuci] = new Tip (el);
        }
    }
    void obrisi() override
    {
        if (velicina > 1) {
            delete L[tekuci];
            for (int i = tekuci; i < velicina-1; i++) L[i] = L[i+1];
            velicina--;
            if (tekuci == velicina) tekuci--;
        } else {
            delete L[tekuci];
            velicina--;
            tekuci--;
        }

    }
    void pocetak() override
    {
        if (velicina > 0) tekuci = 0;
    }
    void kraj() override
    {
        if (velicina > 0) tekuci = velicina-1;
    }
    Tip operator [] (int i) const override
    {
        return *L[i];
    }
    Tip &operator [] (int i) override
    {
        return *L[i];
    }
    friend class Iterator<Tip>;

};

template <typename Tip>
class JednostrukaLista : public Lista<Tip>
{
    struct Cvor {
        Tip element;
        Cvor* sljedeci;
    };
    int velicina;
    Cvor *start;
    Cvor *end;
    Cvor *tekuci;
public:
    JednostrukaLista()
    {
        start = nullptr;
        end = nullptr;
        tekuci = nullptr;
        velicina = 0;
    }
    JednostrukaLista(const JednostrukaLista &lista)
    {
        start = new Cvor ({lista.start->element, nullptr});
        auto pom = start;
        for (auto pok = lista.start; pok != lista.end; pok = pok->sljedeci) {
            pom -> sljedeci = new Cvor ({pok->element,nullptr});
            pom = pom->sljedeci;
        }
        velicina = lista.velicina;
        end = pom;
    }
    JednostrukaLista &operator = (const JednostrukaLista &lista)
    {
        start = new Cvor {lista.start->element,nullptr};
        auto pom = start;
        for (auto pok = lista.start; pok != lista.end; pok = pok->sljedeci) {
            pom-> sljedeci = new Cvor ({pok->element,nullptr});
            pom = pom->sljedeci;
        }
        end = pom;
        velicina = lista.velicina;
        return *this;
    }
    ~JednostrukaLista()
    {
        while (start!=end) {
            tekuci = start->sljedeci;
            delete start;
            start = tekuci;
        }
        delete start;

    }
    int brojElemenata() const override
    {
        return velicina;
    }
    Tip trenutni() const override
    {
        return tekuci->element;
    }
    Tip& trenutni() override
    {
        return tekuci->element;
    }
    bool prethodni() override
    {
        if (velicina == 0 || tekuci == start) return false;
        auto pok = start;
        while (pok->sljedeci!=tekuci) pok = pok->sljedeci;
        tekuci = pok;
        return true;
    }
    bool sljedeci() override
    {
        if (velicina == 0 || tekuci == end) return false;
        tekuci = tekuci->sljedeci;
        return true;
    }
    void pocetak() override
    {
        tekuci = start;
    }
    void kraj() override
    {
        tekuci = end;
    }
    void obrisi() override
    {
        if (velicina > 1) {
            if (tekuci == start) {
                start = start->sljedeci;
                delete tekuci;
                tekuci = start;
                velicina--;
            } else if (tekuci == end) {
                auto pok = start;
                while (pok -> sljedeci != end) pok = pok->sljedeci;
                delete end;
                end = tekuci = pok;
                velicina --;
            } else {
                auto pok = start;
                while (pok->sljedeci!=tekuci) pok = pok->sljedeci;
                pok->sljedeci = tekuci -> sljedeci;
                delete tekuci;
                tekuci = pok->sljedeci;
                velicina--;
            }
        } else if (velicina == 1) {
            delete tekuci;
            start = end = tekuci = nullptr;
            velicina--;
        } else if (velicina==0) return;

    }
    void dodajIspred (const Tip& el) override
    {
        if (tekuci != start) {
            auto pok = start;
            while (pok->sljedeci != tekuci) pok = pok->sljedeci;
            pok->sljedeci = new Cvor ({el, tekuci});
            velicina++;
        } else if (velicina > 0 && tekuci == start) {
            auto pom = new Cvor ({el,start});
            start = pom;
            velicina++;
        } else if (velicina == 0) {
            tekuci = new Cvor ({el,start});
            velicina++;
            start = end = tekuci;
        }
    }
    void dodajIza (const Tip& el) override
    {
        if (tekuci != end) {
            tekuci->sljedeci = new Cvor ({el, tekuci->sljedeci});
            velicina++;
        } else if (velicina > 0 && tekuci == end) {
            end->sljedeci = new Cvor ({el, nullptr});
            velicina++;
            end = end->sljedeci;
        } else if (velicina == 0) {
            start = new Cvor {el,nullptr};
            tekuci = start;
            end = start;
            velicina++;
        }
    }
    Tip operator [] (int i) const override
    {
        if (i < 0 || i >= velicina) throw "Ne moze";
        auto pokazivac = start;
        for (int brojac = 0; brojac < i; brojac++) pokazivac = pokazivac->sljedeci;
        return pokazivac->element;
    }
    Tip &operator [] (int i) override
    {
        if (i < 0 || i >= velicina) throw "Ne moze";
        auto pokazivac = start;
        for (int brojac = 0; brojac < i; brojac++) pokazivac = pokazivac->sljedeci;
        return pokazivac->element;
    }
    friend class Iterator<Tip>;
};
template <typename Tip>
class DvostrukaLista : public Lista<Tip>
{
    struct Cvor {
        Tip element;
        Cvor *sljedeci;
        Cvor *prethodni;
        Cvor (const Tip &element, Cvor *prevp, Cvor *nextp=0) : element(element), prethodni(prevp),sljedeci(nextp)
        {
            if (prethodni!=0) prethodni->sljedeci=this;
            if (sljedeci!=0) sljedeci->prethodni=this;
        }
        Cvor (Cvor *prevp=0, Cvor* nextp=0) : prethodni(prevp), sljedeci(nextp)
        {
            if (prethodni!=0) prethodni->sljedeci=this;
            if (sljedeci !=0) sljedeci->prethodni=this;
        }
    };
    Cvor* start;
    Cvor *end;
    Cvor *tekuci;
    int duzina;
    void Iniciraj()
    {
        tekuci = start = new Cvor(0,0);
        start->sljedeci = end = new Cvor (start,0);
        duzina = 0;
    }
    void Unisti()
    {
        while (start!=0) {
            tekuci=start;
            start=start->sljedeci;
            delete tekuci;
        }
        end=0;
        tekuci=0;
    }
public:
    DvostrukaLista()
    {
        start = end = tekuci = nullptr;
        duzina=0;
    }
    ~DvostrukaLista()
    {
        Unisti();
    }
    DvostrukaLista(const DvostrukaLista &dl)
    {
        start = new Cvor (dl.start->element,nullptr,nullptr);
        auto pom = start;
        for (auto pok = dl.start->sljedeci; pok != dl.end; pok=pok->sljedeci) {
            pom->sljedeci = new Cvor (pok->element,pom,nullptr);
            pom = pom->sljedeci;
        }
        duzina = dl.duzina;
        end = pom;
    }
    DvostrukaLista &operator = (const DvostrukaLista &dl)
    {
        if (&dl != this) {
            Unisti();
            start = new Cvor (dl.start->element,nullptr,nullptr);
            auto pom = start;
            for (auto pok = dl.start->sljedeci; pok != dl.end; pok=pok->sljedeci) {
                pom->sljedeci = new Cvor (pok->element,pom,nullptr);
                pom = pom->sljedeci;
            }
            duzina = dl.duzina;
            end = pom;
        }

        return *this;
    }
    int brojElemenata() const override
    {
        return duzina;
    }
    Tip trenutni() const override
    {
        return tekuci->element;
    }
    Tip &trenutni() override
    {
        return tekuci->element;
    }
    bool prethodni() override
    {
        if (duzina==0 || tekuci==start) return false;
        tekuci=tekuci->prethodni;
        return true;
    }
    bool sljedeci() override
    {
        if (duzina==0 || tekuci==end) return false;
        tekuci=tekuci->sljedeci;
        return true;
    }
    void pocetak() override
    {
        tekuci=start;
    }
    void kraj() override
    {
        tekuci=end;
    }
    void obrisi() override
    {
        if (duzina>1) {
            if (tekuci==start) {
                start=start->sljedeci;
                start->prethodni=nullptr;
                delete tekuci;
                tekuci=start;
                duzina--;
            } else if (tekuci==end) {
                end=end->prethodni;
                end->sljedeci=nullptr;
                delete tekuci;
                tekuci=end;
                duzina--;
            } else {
                tekuci->prethodni->sljedeci = tekuci->sljedeci;
                tekuci->sljedeci->prethodni=tekuci->prethodni;
                auto pok = tekuci->sljedeci;
                delete tekuci;
                tekuci=pok;
                duzina--;
            }
        } else if (duzina==1) {
            delete tekuci;
            start = end = tekuci = nullptr;
            duzina--;
        } else if (duzina==0) return;
    }
    void dodajIspred (const Tip &el) override
    {
        if (tekuci != start) {
            tekuci->prethodni = new Cvor(el,tekuci->prethodni,tekuci);
            tekuci->prethodni->prethodni->sljedeci = tekuci->prethodni;
            duzina++;
        } else if (duzina>0 && tekuci == start) {
            auto pom = new Cvor (el,nullptr,start);
            start->prethodni=pom;
            start=pom;
            duzina++;
        } else if (duzina==0) {
            tekuci = new Cvor (el,nullptr,nullptr);
            duzina++;
            start=end=tekuci;
        }
    }
    void dodajIza(const Tip &el) override
    {
        if (duzina==0) {
            start = new Cvor (el,nullptr,nullptr);
            tekuci = end = start;
            duzina++;
        }
        else if (duzina>0 && tekuci==end) {
            auto pom = new Cvor (el, end, nullptr);
            end->sljedeci=pom;
            end = pom;
            duzina++;
        } 
        else if (tekuci!=end) {
            auto pom = new Cvor (el, tekuci, tekuci->sljedeci);
            tekuci->sljedeci->prethodni->prethodni=pom;
            tekuci->sljedeci=pom;
            duzina++;
        } 
    }
    Tip operator [] (int i) const override
    {
        if (i<0 || i>=duzina) throw "Ne moze";
        auto pok = start;
        {
            for (int brojac=0; brojac<i; brojac++) pok=pok->sljedeci;
        }
        return pok->element;
    }
    Tip &operator [] (int i) override
    {
        if (i<0 || i>=duzina) throw "Ne moze";
        auto pok = start;
        {
            for (int brojac=0; brojac<i; brojac++) pok=pok->sljedeci;
        }
        return pok->element;
    }
    friend class Iterator<Tip>;
};

template <typename Tip>
class Iterator
{
    const DvostrukaLista<Tip> *lista;
    typename DvostrukaLista<Tip>::Cvor *trenutniLista;
    bool parametar;
public:
    
    Iterator (const Lista<Tip> &l)
    {
        
        lista = (DvostrukaLista<Tip>*)&l;
        trenutniLista=lista->start;
    }
    Tip trenutni() const
    {
        return trenutniLista->element;
    }
    Tip &trenutni()
    {
        return trenutniLista->element;
    }
    void pocetak ()
    {
        trenutniLista=lista->start;
    }
    void kraj ()
    {
        trenutniLista=lista->end;
    }
    bool sljedeci()
    {
        if (lista->duzina==0 || trenutniLista==lista->end) return false;
        trenutniLista = trenutniLista->sljedeci;
        return true;
    }
    bool prethodni()
    {
        if (lista->duzina==0 || trenutniLista==lista->start) return false;
        trenutniLista = trenutniLista->prethodni;
        return true;
    }
};
template<typename Tip>
Tip dajMaksimum(const Lista<Tip>& n)
{
    Iterator<Tip> it(n);
    Tip max = numeric_limits<Tip>::min();
    it.pocetak();
    while (it.sljedeci()) {
        if (it.trenutni()>max) max = it.trenutni();
    }
    return max;
}
int main ()
{
    // DvostrukaLista<int> DLista;
    // for (int i = 1; i <= 5; i++) { DLista.dodajIza(i); DLista.kraj(); }
    // DLista.pocetak();
    // for (int i = 0; i < 5; i++) {
    //     cout << DLista.trenutni() << " ";
    //     DLista.sljedeci();
    // }
    // cout << endl;
    // DLista.pocetak();
    // for (int i = 1; i <= 5; i++) {
    //     DLista.dodajIspred(-i);
    //     DLista.pocetak();
    // }
    // for (int i = 0; i <10; i++) {
    //     cout << DLista.trenutni() << " ";
    //     DLista.sljedeci();
    // }
    // cout << DLista.brojElemenata() << endl;
    // for (int i = 0; i < 10; i++) cout << DLista[i] << " ";
    // cout << endl;
    // DLista[5]=10;
    // DLista.trenutni()=2;
    
    // for (int i = 0; i < 10; i++) cout << DLista[i] << " ";
    // DLista.pocetak();
    // for (int i = 0; i < 10; i++) DLista.obrisi();
    // return 0;
    DvostrukaLista<int> l;
    l.dodajIspred(1);
    cout<<l.trenutni()<<" ";
    l.obrisi();
    l.dodajIza(1);
    l.dodajIza(2);
    l.dodajIza(8);
    cout<<l.trenutni()<<" ";
    l.dodajIza(3);
    l.dodajIza(4);
    l.dodajIza(5);
    l.pocetak();
    l.kraj();
    cout<<l.trenutni()<<" ";
    l.pocetak();
    cout<<l.trenutni()<<" ";
    l.sljedeci();
    cout<<l.trenutni()<<" ";
    l.prethodni();
    cout<<l.trenutni()<<" ";
    cout<<l[1]<<" ";
    cout<<l.brojElemenata()<<" ";
    l.pocetak();
    for(int i=0;i<l.brojElemenata();i++){
        cout<<l.trenutni()<<" ";
        l.sljedeci();
    }
}
