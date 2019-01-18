#include <iostream>
#include <string>
#include <vector>
#include <array>

using std::ios_base;
using std::string;
using std::vector;
using std::cout;
using std::cin;

namespace Constants{

    const int N = 1e6 + 1;
    const int S = -1;

};

using namespace Constants;

namespace Strings{

    std::array<int, N> fail;

    void Failure(const string& w){
        int v = fail.front() = 0;
        const int n = w.size();
        for(int u = 1; u < n; u++){
            while(v > 0){
                if(w[u] == w[v]){
                    break;
                }
                v = fail[v - 1];
            }
            if(w[u] == w[v]) v++;
            fail[u] = v;
        }
    }

    vector<int> Periods(const string& w){
        const unsigned int n = w.size();
        vector<int> p(n, S); Failure(w);
        for(int k = 1; k < n; k++){
            int x = (k + 1) - fail[k];
            if((k + 1) % x == 0){
                x = (k + 1) / x;
                if(x > 1){
                    p[k] = x;
                }
            }
        }
        return p;
    }

};

using Strings::Periods;

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); 
    int kase = int(); int n;
    while(cin >> n and n){
        string w; 
        cin >> std::ws; 
        cin >> w;
        cout << "Test case #",
        cout << ++kase,
        cout << '\n';
        auto p = Periods(w);
        for(int k = 0; k < n; k++){
            if(p[k] == S) continue;
            cout << (k + 1) << ' ',
            cout << p[k] << '\n';
        }
        cout << '\n';
    }
    return 0;
}
