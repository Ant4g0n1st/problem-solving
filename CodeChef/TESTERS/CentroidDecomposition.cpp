#include <forward_list>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <array>

using std::vector;
using std::cout;
using std::cin;

namespace Constants{

    const int oo = std::numeric_limits<int>::max();
    const int S = 1e6 + 5e5 + 1;

};

using namespace Constants;

namespace Math{

    typedef long long int Long;    

    std::array<int, S> sieve;

    void FillSieve(){
        sieve.fill(int());
        for(int k = 4; k < S; k += 2) sieve[k] = 2;
        for(int u = 3; u * u < S; u += 2){
            if(sieve[u]) continue;
            for(int v = u * u; v < S; v += 2 * u){
                if(!sieve[v]) sieve[v] = u;
            }
        }
    }

    bool IsBeautiful(int v){
        while(sieve[v]){
            int p = sieve[v], k = 0;
            while(v % p == 0){
                v /= p, k++;
            }
            if((p & 1) and (k & 1)){
                return false;
            }
        }
        return v <= 2;
    }

};

int main(){
    //std::ios_base::sync_with_stdio(0),
    //cout.tie(0), cin.tie(0);
    Math::FillSieve();
    return 0;
}
