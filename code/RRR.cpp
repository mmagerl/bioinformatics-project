#include <vector>
using namespace std;

class RRR {
  // superblokovi su 0-indeksirani
  // blokovi su 0-indeksirani
  private:
    int b; // velicina bloka
    int f; // broj blokova u superbloku
    int sumsb[n/(b*f)]; // suma jedinica do i-tog superbloka, ukljucivo
    int c[n/b]; // klasa za svaki blok
    int o[n/b]; // offset za svaki blok
    int cum[c,o][pos]; // kumulativna suma bloka indeksiranog s c, o; 0-indeksirano

    int nb = (n-1)/b+1; // broj blokova
    int nsb = (n-1)/(b*f)+1; // broj superblokova
    
    // vraca zadnji superblok u kojem se nalazi i-ta jedinica
    int sb_search(int i) {
      int lo = 0;
      int hi = nsb; 
      while(lo < hi) {
        int mid = (lo+hi)/2;
        if (sumsb[mid] < i) {
          lo = mid+1;
        } else {
          hi = mid;
        }
      }
      return lo;
    }

  public:
    RRR(vector<bool> bits, int _b, int _f): b(_b), f(_f) {

    }

    // broj nula do pozicije i (0-indeksiran), ukljucivo 
    int rank0(int i) {
      return i+1-rank1(i);
    }

    // broj jedinica do pozicije i (0-indeksiran), ukljucivo 
    int rank1(int i) {
      int ib = i/b; // indeks bloka te pozicije
      int is = ib/f; // indeks superbloka te pozicije

      int result = (is > 0) ? sumsb[is-1] : 0; // suma jedinica do is-tog superbloka
       
      // prodji po svim blokovima od pocetka ovog superbloka do trenutnog bloka
      for (int i = is*f; i < ib; ++i) { 
        result += c[i]; // dodaj broj jedinica u tom bloku
      }
      result += cum[c[ib], o[ib]][i-ib*b] // dodaj broj jedinica u zadnjem bloku
      return result;
    }

    // vraca poziciju i-te jedinice, 0-indeksirano
    int select(int i) {
      int is = sb_search(i); // superblok u kojem se nalazi i-ta jedinica
      int cnt = (is > 0) ? sumsb[is-1] : 0; // broj jedinica do trenutnog superbloka

      int ib = is*f; // trenutni blok
      // hodaj po blokovima dok ne dostignes sumu
      while (cnt < i) {
        cnt += c[ib];
        ++ib; 
      }
      
      // vrati se na prethodni blok
      --ib; 
      cnt -= c[ib];

      // binary search unutar bloka
      int lo = 0;
      int hi = b;
      while(lo < hi) {
        int mid = (lo+hi)/2;
        if (cum[c[ib], o[ib]][mid]+cnt < i) {
          lo = mid+1;
        } else {
          hi = mid;
        }
      }

      return ib*b + lo;
    }

    // vraca bit na i-toj poziciji
    int access(int i) {
      return (i > 0) ? rank1(i)-rank1(i-1) : rank1(i);
    }
};

int main(void) {

  return 0;
}
