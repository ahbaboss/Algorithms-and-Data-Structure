#include <iostream>
#include <vector>
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
int BinarnaPretraga (vector<int> &v, int prvi, int posljednji, int trazeni)
{
    if (prvi<=posljednji) {
        int mid = (prvi+posljednji)/ 2;
        if (v.at(mid)==trazeni) return mid;
        if (v.at(mid)>trazeni) return BinarnaPretraga(v,prvi,mid-1,trazeni);
        if (v.at(mid)<trazeni) return BinarnaPretraga(v,mid+1,posljednji,trazeni);
    }
    return -1;
}
void pretraga(Stek<vector<int>>& s, int trazeni)
{
    static int velicina(s.brojElemenata());
    if (!velicina) {
        cout << "Nema elementa" << endl;
        return;
    }
    static int index;
    vector<int> pomocni;
    if (s.brojElemenata()) {
        pomocni = s.skini();
        if (pomocni.size()>0 && trazeni>=pomocni.at(0) && trazeni<=pomocni.at(pomocni.size()-1)) {
            index = BinarnaPretraga(pomocni,0,pomocni.size()-1,trazeni);
            if (index == -1) {
                cout << "Nema elementa";
                s.stavi(pomocni);
                return;
            } else {
                cout << index << " " << s.brojElemenata();
                s.stavi(pomocni);
            }
        } else {
            pretraga(s,trazeni);
            s.stavi(pomocni);
        }
    } else {
        cout << "Nema elementa";
        s.stavi(pomocni);
    }
}

int main()
{
    Stek<vector<int> > s;
    vector<int> a;
    for (int i(0); i<5; i++)
        a.push_back(i);
    vector<int> b;
    vector<int> c;
    vector<int> d;
    for (int i(5); i<7; i++)
        b.push_back(i);
    s.stavi(a);
    s.stavi(b);
    c.push_back(8);
    s.stavi(c);
    d.push_back(9);
    s.stavi(d);
    pretraga(s,2);
    cout<<" ";
    while(s.brojElemenata()!=0) {
        vector<int> k = s.skini();
        for(int i(0); i<k.size(); i++) {
            cout<<k[i]<<" ";
        }
    }
}
