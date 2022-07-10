#include <iostream>
#include <utility>
#include <stdexcept>

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


void provjera1()
{
    NizMapa<string,string> m;
    m["Djed"] = "Osman";
    m["Nana"] = "Nurija";
    {
        NizMapa<string,string> m2(m);
        NizMapa<string,string> m3;
        m3=m;
        cout << m2.brojElemenata() << " " << m3.brojElemenata() << endl;
        cout << (m=m).brojElemenata() << endl;
    }
    cout << m.brojElemenata() << endl;
}

void provjera2()
{
    NizMapa<int,int> m;
    m[1] = 150;
    m[2] = 250;
    cout << m.brojElemenata() << endl;
    m.obrisi(2);
    cout << m.brojElemenata() << endl;
    m.obrisi();
    cout << m.brojElemenata() << endl;
}
void provjera3()
{
    NizMapa<double,string> m;
    m[1.50] = "Marka i po";
    m[200] = "Andric";
    m[100] = "Nikola";
    cout << m[1.50] << " " << m[100] << " " << m[200] << endl;
    m.obrisi();
    cout << m.brojElemenata();
}
void provjera4()
{
    NizMapa<string,string> m;
    m["Ahmedin"] = "Hasanovic";
    m["Emir"] = "Cogo";
    m["Haris"] = "Supic";
    NizMapa<string,string> m2(m);
    NizMapa<string,string> m3;
    m3=m;
    cout << endl << m["Ahmedin"] << " " << m2["Emir"] << " " << m3["Haris"] << endl;
}
void provjera5()
{
    NizMapa <int, int> m;
    for (int i(1000); i>-1000; i--)
        m[i] = i*i;
    cout << m[-100];
}
int main()
{
    provjera1();
    provjera2();
    provjera3();
    provjera4();
    provjera5();
}
