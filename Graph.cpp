#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;

template <typename TipOznake>
class UsmjereniGraf;

template <typename TipOznake>
class GranaIterator;

template <typename TipOznake>
class Cvor 
{
    int r_broj;
    TipOznake oznaka;
    UsmjereniGraf<TipOznake> *pok;
public:
    Cvor() {}
    Cvor(UsmjereniGraf<TipOznake> *ug, int rbr) : pok(ug), r_broj(rbr) {}
    int dajRedniBroj() const { return r_broj; }
    void postaviOznaku (TipOznake o) { oznaka = o; }
    TipOznake dajOznaku () const { return oznaka; }
    
};

template <typename TipOznake>
class Grana
{
    float tezina;
    TipOznake oznaka;
    Cvor<TipOznake> polazni, dolazni;
public:
    Grana() {}
    float dajTezinu() const { return tezina; }
    void postaviTezinu (float t) { tezina = t; }
    TipOznake dajOznaku() const { return oznaka; }
    void postaviOznaku(TipOznake o) { oznaka = o; }
    Cvor<TipOznake> dajPolazniCvor () const { return polazni; }
    Cvor<TipOznake> dajDolazniCvor () const { return dolazni; }
    void postaviPolazniCvor (Cvor<TipOznake> cvor) { polazni = cvor; }
    void postaviDolazniCvor (Cvor<TipOznake> cvor) { dolazni = cvor; }
    
};

template <typename TipOznake>
class UsmjereniGraf 
{
    
public:
    virtual ~UsmjereniGraf() = default;
    virtual int dajBrojCvorova () const = 0;
    virtual void postaviBrojCvorova (int broj_cvorova) = 0;
    virtual void dodajGranu (int polazni, int dolazni, float tezina=0) = 0;
    virtual void obrisiGranu (int polazni, int dolazni) = 0;
    virtual void postaviTezinuGrane (int polazni, int dolazni, float tezina) = 0;
    virtual float dajTezinuGrane (int polazni, int dolazni) = 0;
    virtual bool postojiGrana (int polazni, int dolazni) = 0;
    virtual void postaviOznakuCvora (int cvor, TipOznake o) = 0;
    virtual TipOznake dajOznakuCvora (int cvor) = 0;
    virtual void postaviOznakuGrane (int polazni, int dolazni, TipOznake o) = 0;
    virtual TipOznake dajOznakuGrane (int polazni, int dolazni) = 0;
    virtual Cvor<TipOznake> &dajCvor (int rbr) = 0;
    virtual Grana<TipOznake> &dajGranu (int polazni, int dolazni) = 0;
    virtual GranaIterator<TipOznake> dajGranePocetak() = 0;
    virtual GranaIterator<TipOznake> dajGraneKraj() = 0;
    virtual int dajBrojGrana() const = 0;
    virtual void ispisiGrane() = 0;
};



template <typename TipOznake>
class MatricaGraf : public UsmjereniGraf<TipOznake>
{
    int brojCvorova, brojGrana;
    vector<Cvor<TipOznake>> cvorovi;
    vector<Grana<TipOznake>> grane;
public:
    MatricaGraf(int broj_cvorova) : brojCvorova(broj_cvorova), brojGrana(0) {
        for (int i = 0; i < brojCvorova; i++) {
            Cvor<TipOznake> c(this,i);
            c.postaviOznaku(TipOznake());
            cvorovi.push_back(c);
        }        
    }
    ~MatricaGraf() {
        cvorovi.resize(0);
        grane.resize(0);
    };
    int dajBrojCvorova () const { return brojCvorova; }
    void postaviBrojCvorova (int broj_cvorova) {
        if (broj_cvorova < brojCvorova) throw "Izuzetak";
        for (int i = brojCvorova; i < broj_cvorova; i++) {
            Cvor<TipOznake> c(this,i);
            c.postaviOznaku(TipOznake());
            cvorovi.push_back(c);
        }
    }
    void dodajGranu (int polazni, int dolazni, float tezina=0) {
        Grana<TipOznake> g;
        g.postaviPolazniCvor(cvorovi.at(polazni));
        g.postaviDolazniCvor(cvorovi.at(dolazni));
        g.postaviTezinu(tezina);
        grane.push_back(g);
        sort(grane.begin(),grane.end(),[](Grana<TipOznake> &g1, Grana<TipOznake> &g2) {
            if (g1.dajPolazniCvor().dajRedniBroj() == g2.dajPolazniCvor().dajRedniBroj()) 
                return g1.dajDolazniCvor().dajRedniBroj() < g2.dajDolazniCvor().dajRedniBroj();
            return g1.dajPolazniCvor().dajRedniBroj() < g2.dajPolazniCvor().dajRedniBroj();
        });
    }
    void obrisiGranu (int polazni, int dolazni) {
        for (auto it = grane.begin(); it != grane.end(); it++) {
            if ((it->dajPolazniCvor()).dajRedniBroj() == polazni && (it->dajDolazniCvor()).dajRedniBroj() == dolazni)
                grane.erase(it);
        }
    }
    void postaviTezinuGrane (int polazni, int dolazni, float tezina) {
        for (auto it = grane.begin(); it != grane.end(); it++) {
            if ((it->dajPolazniCvor()).dajRedniBroj() == polazni && (it->dajDolazniCvor()).dajRedniBroj() == dolazni)
                it->postaviTezinu(tezina);
        }
    }
    float dajTezinuGrane (int polazni, int dolazni) {
        for (auto it = grane.begin(); it != grane.end(); it++) {
            if ((it->dajPolazniCvor()).dajRedniBroj() == polazni && (it->dajDolazniCvor()).dajRedniBroj() == dolazni)
                return it->dajTezinu();
        }
        return -1;
    }
    bool postojiGrana (int polazni, int dolazni) {
        for (auto it = grane.begin(); it != grane.end(); it++) {
            if ((it->dajPolazniCvor()).dajRedniBroj() == polazni && (it->dajDolazniCvor()).dajRedniBroj() == dolazni)
                return true;
        }
        return false;
    }
    void postaviOznakuCvora (int cvor, TipOznake o) {
        cvorovi.at(cvor).postaviOznaku(o);
    }
    TipOznake dajOznakuCvora (int cvor) {
        return cvorovi.at(cvor).dajOznaku();
    }
    void postaviOznakuGrane (int polazni, int dolazni, TipOznake o) {
        for (auto it = grane.begin(); it != grane.end(); it++) {
            if ((it->dajPolazniCvor()).dajRedniBroj() == polazni && (it->dajDolazniCvor()).dajRedniBroj() == dolazni)
                it->postaviOznaku(o);
        }
        
    }
    TipOznake dajOznakuGrane (int polazni, int dolazni) {
        for (auto it = grane.begin(); it != grane.end(); it++) {
            if ((it->dajPolazniCvor()).dajRedniBroj() == polazni && (it->dajDolazniCvor()).dajRedniBroj() == dolazni)
                return it->dajOznaku();
        }
        return TipOznake();
    }    
    Cvor<TipOznake> &dajCvor (int rbr) {
        return cvorovi.at(rbr);
    }
    Grana<TipOznake> &dajGranu (int polazni, int dolazni) {
        for (auto it = grane.begin(); it != grane.end(); it++) {
            if ((it->dajPolazniCvor()).dajRedniBroj() == polazni && (it->dajDolazniCvor()).dajRedniBroj() == dolazni)
                return *it;
        }
    }
    GranaIterator<TipOznake> dajGranePocetak() {
        GranaIterator<TipOznake> gg(grane.at(0));
        return gg;
    }
    GranaIterator<TipOznake> dajGraneKraj() {
        GranaIterator<TipOznake> gg(grane.at(0));
        for (int i = 0; i < grane.size(); i++) ++gg;
        return gg;
    }
    int dajBrojGrana() const { return grane.size(); }
    friend class GranaIterator<TipOznake>;
    void ispisiGrane() {
        for (int i = 0; i < grane.size(); i++) {
            cout << grane.at(i).dajPolazniCvor().dajRedniBroj() << " " << grane.at(i).dajDolazniCvor().dajRedniBroj() << endl;
        }
    }
};

template <typename TipOznake>
class GranaIterator {
    Grana<TipOznake> *g;
public:
    GranaIterator(Grana<TipOznake> &gg) {
        g = &gg;
    }
    Grana<TipOznake> operator*() {
        return *g;
    }
    bool operator==(const GranaIterator &iter) const {
        return g == iter.g;
    }
    bool operator!=(const GranaIterator &iter) const {
        return g != iter.g;
    }
    GranaIterator& operator++() {
        ++g;
        return *this;
    }
    GranaIterator operator++(int) {
        auto tmp(*this);
        operator++();
        return *tmp;
    }
    
};

int main() {
    
    return 0;
}
