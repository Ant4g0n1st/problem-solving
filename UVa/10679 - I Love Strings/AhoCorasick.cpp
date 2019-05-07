#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <set>

using std::ios_base;
using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::set;

namespace Strings{

    struct AhoCorasick{

        typedef std::map<char,int> List;

        vector<set<int>> out;
        vector<List> trie;
        vector<int> fail;
        unsigned int n;

        AhoCorasick(const vector<string>& patterns){
            unsigned int size = int(); 
            for(const auto& p : patterns){
                size = size + p.size();
            }
            fail.resize(++size);
            trie.resize(size);
            out.resize(size);
            this->n = int();
            auto m = int();
            for(const auto& p : patterns){
                unsigned int u = int();
                for(const auto& c : p){
                    auto f = trie[u].find(c);
                    if(f == trie[u].end()){
                        u = trie[u][c] = ++m;
                    }else{
                        u = f->second;
                    }
                }
                out[u].insert(n++);
            }
            std::queue<int> q;
            for(auto& e : trie.front()){
                q.emplace(e.second);    
            }
            while(!q.empty()){
                auto u = q.front(); q.pop();
                for(auto& e : trie[u]){
                    const auto& c = e.first;
                    auto f = fail[u];
                    while(!trie[f].count(c)){
                        if(f == 0) break;
                        f = fail[f];
                    } 
                    const auto& v = e.second;
                    auto x = trie[f].find(c);
                    if(x != trie[f].end()){
                        f = x->second;
                    }
                    fail[v] = f;
                    q.push(v);
                }
            }
            for(int u = 0; u < m; u++){
                auto& f = out[fail[u]];
                out[u].insert(f.begin(), f.end());
            }
        }

        vector<bool> Matches(const string& s){
            vector<bool> found(n, false);
            auto u = int();
            for(const auto& c : s){
                u = Next(u, c);
                for(auto& v : out[u]){
                    found[v] = true;
                }
            }
            return found;
        }

        int Next(int u, char c){
            auto next = u;
            while(!trie[next].count(c)){
                if(next == 0) break;
                next = fail[next];
            }
            auto x = trie[next].find(c);
            if(x != trie[next].end()){
                return x->second;
            }
            return next;
        }

    };

};

using Strings::AhoCorasick;

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    int t; cin >> t; 
    for(int cs = 0; cs < t; cs++){
        string text; 
        cin >> std::ws;
        cin >> text;
        unsigned int n; cin >> n;
        vector<string> qs(n);
        for(auto& s : qs){
            cin >> std::ws;
            cin >> s;
        }
        std::unique_ptr<AhoCorasick> t;
        t.reset(new AhoCorasick(qs));
        auto f = t->Matches(text);
        for(int k = 0; k < n; k++){
            cout << (f[k] ? 'y' : 'n');
            cout << '\n';
        }
    }
    return 0;
}
