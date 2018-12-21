#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::cout;
using std::swap;
using std::cin;

namespace Constants{

    const string Z = "@";

};

using namespace Constants;

namespace Strings{

    vector<int> SuffixArray(const string& w){
        const int n = w.size();
        vector<int> rank(n);
        vector<int> sa(n);
        vector<int> z(n);
        for(int k = 0; k < n; k++){
            z[k] = n - k - 1;
        }
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
            for(int k = 0; k < n; k++){
                z[k] = k;
            }
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
    
    vector<int> Longest(int& maxLcs,
                    const string& a, 
                    const string& b){
        const string w = a + Z + b;
        const int m = a.size();
        const auto sa = SuffixArray(w);
        const auto lcp = LCP(w, sa);
        const int n = w.size();
        vector<int> best; 
        int max = 1;
        for(int k = 1; k < n; k++){
            int u = sa[k - 1];
            int v = sa[k];
            if(u == m or v == m)
                continue;
            if(u < m and v < m) 
                continue;
            if(u > m and v > m) 
                continue;
            if(lcp[k - 1] < max)
                continue;
            if(lcp[k - 1] > max){
                max = lcp[k - 1];
                best.clear();
            }
            if(v < u) swap(u, v);
            best.emplace_back(u);
        } 
        maxLcs = max;
        return best;
    }

};

using Strings::Longest;

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    string a; int t = 0; 
    while(cin >> std::ws >> a){
        int max = 0;
        string b;
        cin >> std::ws;
        cin >> b;
        auto lcs = Longest(max, a, b);
        if(t++) cout << '\n';
        if(lcs.empty()){
            cout << "No common ",
            cout << "sequence.",
            cout << '\n';
            continue;
        }
        string previous;
        for(const auto& x : lcs){
            string s;
            for(int k = 0; k < max; k++){
                s.push_back(a[x + k]);
            }
            if(s != previous){
                cout << s << '\n'; 
            }
            swap(s, previous);
        }
    } 
    return 0;
}
