#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <queue>
#include <tuple>
#include <map>
#include <set>

using std::ios_base;
using std::string;
using std::vector;
using std::array;
using std::tuple;
using std::cout;
using std::cin;

namespace Constants{

    const int M = 1e6 + 1;
    const int N = 1e3 + 1;

};

using namespace Constants;

namespace AhoCorasick{

    typedef tuple<int,int,char> Triplet;
    typedef std::map<char,int> Map;
    typedef std::set<int> Set;

    array<int, M> fail;
    array<Map, M> trie;
    array<int, N> size;
    array<Set, M> out;

    unsigned int n = 0;
    unsigned int m = 0;

    void UpdateOutput(int u){
        const auto& f = out[fail[u]];
        out[u].insert(f.begin(), f.end());
    }

    void AddString(const string& s){
        unsigned int u = int();
        for(const auto& c : s){
            auto f = trie[u].find(c);
            if(f == trie[u].end()){
                u = trie[u][c] = ++m;
            }else{
                u = f->second; 
            } 
        } 
        size[n] = s.size() - 1;
        out[u].insert(n++);
    } 
    
    void Reset(){
        for(int u = 0; u < m; u++){
            fail[u] = int();
            trie[u].clear();
            out[u].clear();
        }
        for(int u = 0; u < n; u++){
            size[u] = int();
        }
        AhoCorasick::m = int();
        AhoCorasick::n = int();
    }
    
    int Failure(int u, char c){
        while(!trie[u].count(c)){
            if(u == 0) break;
            u = fail[u];
        }
        auto f = trie[u].find(c);
        if(f != trie[u].end()){
            return f->second;
        }
        return u; 
    }

    void Build(){
        std::queue<int> q;
        for(auto& e : trie.front()){
            q.emplace(e.second);
        }
        while(!q.empty()){
            auto u = q.front(); q.pop(); 
            for(auto& e : trie[u]){
                auto& v = e.second;
                auto& c = e.first;
                auto& f = fail[u];
                fail[v] = Failure(f, c);
                UpdateOutput(v);
                q.emplace(v);
            }
        }
    }

    template<class Matrix>
    vector<Triplet> FindWords(const Matrix& p){
        const auto c = p.front().size();
        const auto r = p.size();
        vector<Triplet> f(n);
        vector<bool> seen(n);
        for(int x = 0; x < c; x++){
            unsigned int u = int();
            for(int y = r - 1; y >= 0; y--){
                u = Failure(u, p[y][x]);
                for(const auto& z : out[u]){
                    if(seen[z]) continue;
                    seen[z] = true;
                    f[z] = { y + size[z], x, 'A' };
                }
            }
        }
        for(int k = r - 1; k >= 0; k--){
            unsigned int u = int();
            for(int x = k, y = 0; ; x--, y++){
                if(x < 0 or y >= c) break;
                u = Failure(u, p[x][y]);
                for(const auto& z : out[u]){
                    if(seen[z]) continue;
                    seen[z] = true;
                    f[z] = { 
                        x + size[z], 
                        y - size[z],
                        'B' 
                    };
                }
            }
        }
        for(int k = 0; k < c; k++){
            unsigned int u = int();
            for(int x = r - 1, y = k; ; x--, y++){
                if(x < 0 or y >= c) break;
                u = Failure(u, p[x][y]);
                for(const auto& z : out[u]){
                    if(seen[z]) continue;
                    seen[z] = true;
                    f[z] = { 
                        x + size[z], 
                        y - size[z],
                        'B' 
                    };
                }
            }
        }
        for(int x = 0; x < r; x++){
            unsigned int u = int();
            for(int y = 0; y < c; y++){
                u = Failure(u, p[x][y]); 
                for(const auto& z : out[u]){
                    if(seen[z]) continue;
                    seen[z] = true;
                    f[z] = { x, y - size[z], 'C' };
                }
            } 
        }
        for(int k = 0; k < r; k++){
            unsigned int u = int();
            for(int x = k, y = 0; ; x++, y++){
                if(x >= r or y >= c) break;
                u = Failure(u, p[x][y]);
                for(const auto& z : out[u]){
                    if(seen[z]) continue;
                    seen[z] = true;
                    f[z] = { 
                        x - size[z], 
                        y - size[z],
                        'D' 
                    };
                }
            }
        }
        for(int k = 0; k < c; k++){
            unsigned int u = int();
            for(int x = 0, y = k; ; x++, y++){
                if(x >= r or y >= c) break;
                u = Failure(u, p[x][y]);
                for(const auto& z : out[u]){
                    if(seen[z]) continue;
                    seen[z] = true;
                    f[z] = { 
                        x - size[z], 
                        y - size[z],
                        'D' 
                    };
                }
            }
        }
        for(int x = 0; x < c; x++){
            unsigned int u = int();
            for(int y = 0; y < r; y++){
                u = Failure(u, p[y][x]);
                for(const auto& z : out[u]){
                    if(seen[z]) continue;
                    seen[z] = true;
                    f[z] = { y - size[z], x, 'E' };
                }
            }
        }
        for(int k = r - 1; k >= 0; k--){
            unsigned int u = int();
            for(int x = k, y = c - 1; ; x++, y--){
                if(x >= r or y < 0) break;
                u = Failure(u, p[x][y]);
                for(const auto& z : out[u]){
                    if(seen[z]) continue;
                    seen[z] = true;
                    f[z] = { 
                        x - size[z], 
                        y + size[z],
                        'F' 
                    };
                }
            }
        }
        for(int k = c - 1; k >= 0; k--){
            unsigned int u = int();
            for(int x = 0, y = k; ; x++, y--){
                if(x >= r or y < 0) break;
                u = Failure(u, p[x][y]);
                for(const auto& z : out[u]){
                    if(seen[z]) continue;
                    seen[z] = true;
                    f[z] = { 
                        x - size[z], 
                        y + size[z],
                        'F' 
                    };
                }
            }
        }
        for(int x = 0; x < r; x++){
            unsigned int u = int();
            for(int y = c - 1; y >= 0; y--){
                u = Failure(u, p[x][y]);
                for(const auto& z : out[u]){
                    if(seen[z]) continue;
                    seen[z] = true;
                    f[z] = { x, y + size[z], 'G' };
                }
            }
        }
        for(int k = 0; k < r; k++){
            unsigned int u = int();
            for(int x = k, y = c - 1; ; x--, y--){
                if(x < 0 or y < 0) break;
                u = Failure(u, p[x][y]);
                for(const auto& z : out[u]){
                    if(seen[z]) continue;
                    seen[z] = true;
                    f[z] = { 
                        x + size[z], 
                        y + size[z],
                        'H' 
                    };
                }
            }
        }
        for(int k = c - 1; k >= 0; k--){
            unsigned int u = int();
            for(int x = r - 1, y = k; ; x--, y--){
                if(x < 0 or y < 0) break;
                u = Failure(u, p[x][y]);
                for(const auto& z : out[u]){
                    if(seen[z]) continue;
                    seen[z] = true;
                    f[z] = { 
                        x + size[z], 
                        y + size[z],
                        'H' 
                    };
                }
            }
        }
        return f;
    }

};

using AhoCorasick::AddString;
using AhoCorasick::FindWords;

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    int tc; cin >> tc;
    for(int cs = 0; cs < tc; cs++){
        if(cs > 0) cout << "\n";
        AhoCorasick::Reset();
        int r; cin >> r;
        int c; cin >> c;
        int n; cin >> n;
        vector<string> puzzle(r);
        for(auto& s : puzzle){
            cin >> std::ws;
            cin >> s;
        }
        for(int k = 0; k < n; k++){
            cin >> std::ws;
            string w; cin >> w;
            AddString(w);
        } 
        AhoCorasick::Build();
        auto m = FindWords(puzzle);
        for(const auto& t : m){
            cout << std::get<0>(t) << ' ';
            cout << std::get<1>(t) << ' ';
            cout << std::get<2>(t);
            cout << '\n';
        }
    } 
    return 0;
}
