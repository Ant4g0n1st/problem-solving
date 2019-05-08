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
        unsigned int m;

        AhoCorasick(const vector<string>& patterns){
            unsigned int size = int(); 
            for(const auto& p : patterns){
                size = size + p.size();
            }
            fail.resize(++size);
            trie.resize(size);
            out.resize(size);
            this->n = int();
            this->m = int();
            for(const auto& p : patterns){
                this->InsertString(p);
            }
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
                    q.push(v);
                }
            }
        }

        void UpdateOutput(int u){
            auto& f = out[fail[u]];
            out[u].insert(f.begin(), f.end());
        }

        vector<bool> Matches(const string& s){
            vector<bool> found(n, false);
            auto u = int();
            for(const auto& c : s){
                u = Failure(u, c);
                for(auto& v : out[u]){
                    found[v] = true;
                }
            }
            return found;
        }

        void InsertString(const string& s){
            unsigned int u = int();
            for(const auto& c : s){
                auto f = trie[u].find(c);
                if(f == trie[u].end()){
                    u = trie[u][c] = ++m;
                }else{
                    u = f->second;
                }
            }
            out[u].insert(n++);
        }

        int Failure(int u, char c){
            while(!trie[u].count(c)){
                if(u == 0) break;
                u = fail[u];
            }
            auto x = trie[u].find(c);
            if(x != trie[u].end()){
                return x->second;
            }
            return u;
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
