#include <iostream>
#include <utility>
#include <stdexcept>
#include <string>
#include <ctime>

using namespace std;

template <typename TipKljuca, typename TipVrijednosti>
class Mapa
{
public:
    Mapa() {};
    virtual ~Mapa() = default;
    virtual TipVrijednosti operator [] (TipKljuca k) const = 0;
    virtual TipVrijednosti &operator [] (TipKljuca k) = 0;
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuca &kljuc) = 0;
};

template <typename TipKljuca, typename TipVrijednosti>
class NizMapa : public Mapa<TipKljuca,TipVrijednosti>
{
    pair<TipKljuca,TipVrijednosti> *pointer;
    int size;
    int max_size;
    void extend()
    {
        max_size*=2;
        auto helper = new pair<TipKljuca,TipVrijednosti>[max_size] {};
        for (int i = 0; i < size; i++)
            helper[i] = pointer[i];
        delete[] pointer;
        pointer = helper;
    }
public:
    NizMapa() : max_size(50), size(0)
    {
        pointer = new pair<TipKljuca,TipVrijednosti>[max_size] {};
    }
    ~NizMapa()
    {
        delete[] pointer;
    }
    NizMapa(const NizMapa<TipKljuca,TipVrijednosti> &nm) : size(nm.size), max_size(nm.max_size)
    {
        pointer = new pair<TipKljuca,TipVrijednosti>[max_size] {};
        for (int i = 0; i < size; i++)
            pointer[i]=nm.pointer[i];
    }
    NizMapa &operator = (const NizMapa<TipKljuca,TipVrijednosti> &nm)
    {
        if (&nm!=this) {
            size = nm.size;
            max_size = nm.max_size;
            delete[] pointer;
            pointer = new pair<TipKljuca,TipVrijednosti>[max_size] {};
            for (int i = 0; i < size; i++)
                pointer[i]=nm.pointer[i];
        }
        return *this;
    }
    TipVrijednosti operator [] (TipKljuca k) const
    {
        for (int i = 0; i < size; i++)
            if (pointer[i].first == k)
                return pointer[i].second;
        return TipVrijednosti();

    }
    TipVrijednosti &operator [] (TipKljuca k)
    {
        for (int i = 0; i < size; i++)
            if (pointer[i].first == k)
                return pointer[i].second;
        if (size==max_size) extend();
        pointer[size].first = k;
        pointer[size].second = TipVrijednosti();
        return pointer[size++].second;
    }
    int brojElemenata() const
    {
        return size;
    }
    void obrisi()
    {
        size=0;
    }
    void obrisi (const TipKljuca &kljuc)
    {
        bool refuse = false;
        for (int i = 0; i < size; i++) {
            if (pointer[i].first == kljuc) {
                refuse = true;
                for (int j = i; j < size-1; j++) pointer[j]=pointer[j+1];
                size--;
                return;
            }
        }
        if (!refuse) throw domain_error("Nema tog elementa");
    }
};

template <typename TipKljuca, typename TipVrijednosti>
struct Cvor
{
    Cvor *l_dijete, *d_dijete;
    TipKljuca kljuc;
    TipVrijednosti vrijednost;
    Cvor() { l_dijete = d_dijete = nullptr; }
    Cvor (TipKljuca k, Cvor *l, Cvor *d) {
        kljuc = k, l_dijete = l, d_dijete = d;
    }
};

template <typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa : public Mapa<TipKljuca,TipVrijednosti>
{
    int velicina;
    Cvor<TipKljuca,TipVrijednosti> *glavni;
    void Brisi(Cvor<TipKljuca,TipVrijednosti> *p) {
        if (p) {
            Brisi(p -> l_dijete);
            Brisi(p -> d_dijete);
            delete p;
        }
    }
    Cvor<TipKljuca,TipVrijednosti>* search(Cvor<TipKljuca,TipVrijednosti> *p, TipKljuca &k) const {
        if (p == 0 || p->kljuc==k) return p;
        else if (k < p->kljuc) return search(p->l_dijete, k);
        else return search(p->d_dijete, k); 
    }
    bool delement(Cvor<TipKljuca,TipVrijednosti> *r, TipKljuca k) {
        Cvor<TipKljuca,TipVrijednosti> *p = r, *q = 0, *tmp = 0, *pp = 0, *rp = 0;
        while (p != 0 && k != p->kljuc) {
            q = p;
            if (k < p->kljuc) p = p->l_dijete;
            else p = p->d_dijete;
        }
        if (p == nullptr) return false;
        if (!(p->l_dijete)) rp = p->d_dijete;
        else if (!(p->d_dijete)) rp = p->l_dijete;
        else {
            pp = p;
            rp = p->l_dijete;
            tmp= rp->d_dijete;
            while (tmp) {
                pp = rp;
                rp = tmp;
                tmp = rp->d_dijete;
            }
            if (pp != p) {
                pp->d_dijete = rp->l_dijete;
                rp->l_dijete = p->l_dijete;
            }
            rp->d_dijete = p->d_dijete;
        }
        if (!q) glavni = rp;
        else if (p == q->l_dijete) q->l_dijete = rp;
        else q->d_dijete = rp;
        delete p;
        return true;
    }
    Cvor<TipKljuca,TipVrijednosti>* ubaci(const TipKljuca &k) {
        Cvor<TipKljuca,TipVrijednosti> *pok = new Cvor<TipKljuca,TipVrijednosti>(k,nullptr,nullptr);
        pok->vrijednost = TipVrijednosti();
        return pok;
    }
    Cvor<TipKljuca,TipVrijednosti>* Umetni(Cvor<TipKljuca,TipVrijednosti>* &r, const TipKljuca &x) {
        if (!r) return r = ubaci(x);
        else if (x < r->kljuc) return Umetni(r->l_dijete,x);
        else return Umetni(r->d_dijete,x);
    }
    void copy_helper(Cvor<TipKljuca,TipVrijednosti>* &copy_to, Cvor<TipKljuca,TipVrijednosti>* copy_from) const {
        if (!copy_from) copy_to = nullptr;
        else {
            copy_to = new Cvor<TipKljuca,TipVrijednosti>(copy_from->kljuc,nullptr,nullptr);
            copy_to->vrijednost = copy_from->vrijednost;
            copy_helper(copy_to->l_dijete, copy_from->l_dijete); 
            copy_helper(copy_to->d_dijete, copy_from->d_dijete); 
        }
    }
public:
    BinStabloMapa() {
        glavni = nullptr;
        velicina = 0;
    }
    ~BinStabloMapa() { obrisi(); }
    BinStabloMapa(const BinStabloMapa<TipKljuca,TipVrijednosti> &bsm) : velicina(bsm.velicina) {
        copy_helper(this->glavni, bsm.glavni);
    }
    BinStabloMapa &operator = (const BinStabloMapa<TipKljuca,TipVrijednosti> &bsm) {
        if (&bsm!=this) { 
            obrisi(); 
            copy_helper(this->glavni, bsm.glavni); 
            velicina=bsm.velicina;
        }
        return *this;
    }
    TipVrijednosti operator [] (TipKljuca k) const {
        Cvor<TipKljuca,TipVrijednosti> *pok = search(glavni,k);
        if (pok != nullptr) return pok->vrijednost;
        return TipVrijednosti();
    }
    TipVrijednosti &operator [] (TipKljuca k) {
        Cvor<TipKljuca,TipVrijednosti> *pok = search(glavni,k);
        if (pok) return pok->vrijednost;
        pok = Umetni(glavni,k);
        velicina++;
        return pok->vrijednost;
    }
    int brojElemenata() const { return velicina; }
    void obrisi() {
        Brisi(glavni);
        velicina = 0;
        glavni = nullptr;
    }
    void obrisi(const TipKljuca &kljuc) {
        delement(glavni,kljuc);
        velicina--;
    }
};

int main() {
    NizMapa<int,int> NMapa;
    BinStabloMapa<int,int> BMapa;
    int vel = 1000;
    for (int i = 1; i <= vel; i++) {
        int temp(rand()%(2*vel)-vel);
        NMapa[temp] = i;
        BMapa[temp] = i;
    }
    {
        clock_t vrijeme1 = clock();
        cout << BMapa[999] << endl;
        clock_t vrijeme2 = clock();
        int ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
        std::cout << "Vrijeme izvrsenja: " << ukvrijeme << " ms." << std::endl;
    }
    {
        clock_t vrijeme1 = clock();
        NMapa.obrisi(255);
        clock_t vrijeme2 = clock();
        int ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
        std::cout << "Vrijeme izvrsenja: " << ukvrijeme << " ms." << std::endl;
    }
    {
        clock_t vrijeme1 = clock();
        BMapa.obrisi(255);
        clock_t vrijeme2 = clock();
        int ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
        std::cout << "Vrijeme izvrsenja: " << ukvrijeme << " ms." << std::endl;
    }
    return 0;
}
