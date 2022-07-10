#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
void countSort(vector<int> &a, int n, int e)
{
    vector<int> output(n,0);
    int i;
    vector<int> count(10,0);

    for (i = 0; i < n; i++) count[(a[i]/e)%10]++;
    for (i = 1; i < 10; i++) count.at(i) += count.at(i-1);
    for (i = n-1; i>=0; i--) {
        output.at(count.at((a.at(i)/e)%10)-1) = a.at(i);
        count.at((a.at(i)/e)%10)--;
    }
    for (i = 0; i < n; i++)
        a.at(i) = output.at(i);

}
void radixSort(vector<int> &a)
{
    int max = a[0];
    for (int i = 1; i < a.size(); i++)
        if (a[i]>max)
            max=a[i];
    for (int e = 1; max/e>0; e*=10)
        countSort(a,a.size(),e);
}

void PopraviDolje (vector<int> &a, int velicina, int i) {
    while (!(i>=velicina/2 && i<velicina)) {
        int veci = 2*i+1;
        int dd = 2*i+2;
        if ((dd<velicina) && (a.at(dd)>a.at(veci)))
            veci=dd;
        if (a.at(i)>a.at(veci)) return;
        swap(a.at(i),a.at(veci));
        i=veci;
    }
}
void stvoriGomilu(vector<int> &a) {
    int velicina(a.size());
    for (int i = velicina/2; i>=0; i--)
        PopraviDolje(a,velicina,i);
}
void PopraviGore(vector<int> &a, int i) {
    while (i!=0 && a.at(i)>a.at((i-1)/2)) {
        swap(a.at(i),a.at((i-1)/2));
        i = (i-1)/2;
    }
}
void umetniUGomilu(vector<int> &a, int umetnuti, int &velicina) {
    a.push_back(umetnuti);
    PopraviGore(a,velicina++);
}
int izbaciPrvi(vector<int> &a, int &velicina) {
    if (velicina==0) throw domain_error("Gomila je prazna\n");
    swap(a.at(0),a.at(--velicina));
    if (velicina!=0)
        PopraviDolje(a,velicina,0);
    return a.at(velicina);
}
void gomilaSort(vector<int> &a) {
    stvoriGomilu(a);
    for (int i = a.size() - 1; i > 0; i--) {
        swap(a.at(0),a.at(i));
        PopraviDolje(a,i,0);
    }
}

int main()
{
    vector<int> a;
    a.push_back(546);
    a.push_back(1);
    a.push_back(31);
    a.push_back(1001);
    a.push_back(104351);
    a.push_back(7);
    a.push_back(6);
    a.push_back(60045);
    gomilaSort(a);
    for(int i(0);i<a.size();i++){
        cout<<a[i]<<" ";
    }
}
