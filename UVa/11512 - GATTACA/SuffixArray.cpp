#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using std::ios_base;
using std::string;
using std::vector;
using std::cout;
using std::iota;
using std::pair;
using std::cin;

namespace Constants{

    const int S = -1;

};

using namespace Constants;

namespace Strings{

    vector<int> SuffixArray(const string& w){
        const int n = w.size();
        vector<int> rank(n);
        vector<int> sa(n);
        vector<int> z(n);
        iota(z.rbegin(), z.rend(), int());
        auto f = [&w] (int u, int v){
            if(w[u] == w[v]){
                return u < v;
            }
            return w[u] < w[v];
        };
        std::sort(z.begin(), z.end(), f); 
        for(int k = 0; k < n; k++){
            rank[k] = w[k];
            sa[k] = z[k];
        } 
        vector<int> a(n);
        vector<int> r(n);
        for(int l = 1; l < n; l <<= 1){
            const int h = l >> 1;
            r = rank, a = sa;
            for(int k = 0; k < n; k++){
                rank[sa[k]] = k;
                if(k == 0) continue;
                int u = sa[k - 1];
                int v = sa[k];
                if(r[u] != r[v])
                    continue;
                if(u + l >= n)
                    continue;
                if(r[u + h] == r[v + h]){
                    rank[v] = rank[u];
                }
            }
            iota(z.begin(), z.end(), int());
            for(int k = 0; k < n; k++){
                int x = a[k] - l;
                if(x >= 0){
                    sa[z[rank[x]]++] = x;
                }
            }
        }
        return sa;
    }

    vector<int> LCP(const string& w,
                    const vector<int>& sa){
        const int n = w.size();
        vector<int> lcp(n - 1);
        vector<int> rank(n);
        for(int k = 0; k < n; k++){
            rank[sa[k]] = k;
        }
        for(int h = 0, u = 0; u < n; u++){
            if(rank[u] >= n - 1) continue;
            int v = sa[rank[u] + 1];
            while(std::max(u, v) + h < n){
                if(w[u + h] != w[v + h])
                    break;
                h++;
            }
            lcp[rank[u]] = h;
            if(h > 0) h--;
        }
        return lcp;
    }

    pair<int,int> MaxRepetition(int& count, 
                                const string& w){
        const auto sa = SuffixArray(w);
        const auto lcp = LCP(w, sa);
        int index = S, max = int();
        const int n = w.size();
        for(int k = 1; k < n; k++){
            if(lcp[k - 1] > max){
                max = lcp[k - 1];
                index = sa[k];
                count = int();
                for(; k < n; k++){
                    if(lcp[k - 1] == max){
                        count++; 
                    }else break;
                }
                count++;
                k--;
            }
        }
        return { index, max };
    }

};

using Strings::MaxRepetition;

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    int t; cin >> t;
    for(int c = 0; c < t; c++){
        string w; int count; 
        cin >> std::ws;
        cin >> w;
        auto p = MaxRepetition(count, w);
        if(p.first == S){
            cout << "No repetitions",
            cout << " found!\n";
            continue;
        }
        for(int k = 0; k < p.second; k++){
            cout << w[p.first + k];
        }
        cout << ' ' << count,
        cout << '\n';
    }
    return 0;
}
