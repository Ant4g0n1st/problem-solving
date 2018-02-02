#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <vector>
#include <queue>
#include <set>

using std::stringstream;
using std::unique_ptr;
using std::ios_base;
using std::getline;
using std::string;
using std::vector;
using std::queue;
using std::cout;
using std::cin;
using std::set;
using std::ws;

typedef vector<int> vInt;

const int M = -1;
const int S = 26;

struct Graph{

    vector<vInt> orderings; 
    vector<int> closure;
    vector<vInt> edges;
    vector<int> order;
    int n,p;

    Graph(int N): p(M), 
        n(N), edges(N), order(N), closure(N) {}

    bool HasCycle(){ int m = 0;
        vector<int> in(n); queue<int> q;
        for(int u = 0; u < n; u++)
            for(auto& v : edges[u]) ++in[v];
        for(int u = 0; u < n; u++)
            if(!in[u]) q.push(u);
        while(!q.empty()){
            int u = q.front(); q.pop(), ++m;
            for(auto& v : edges[u])
                if(--in[v] == 0) q.push(v);
        }
        return m < n;
    }

    void GenerateOrderings(){ 
        for(int u = 0; u < n; u++){
            queue<int> q; q.push(u);
            vector<bool> seen(n);
            seen[u] = true;
            while(!q.empty()){
                int v = q.front(); q.pop();
                for(auto& w : edges[v]){
                    if(seen[w]) continue;
                    closure[u] |= (1 << w),
                    seen[w] = true,
                    q.push(w);
                }
            }
        } Generate();
    }

    void Generate(int mask = 0){
        if(mask == (1 << n) - 1){
            orderings.push_back(order);
            return;
        } 
        for(int u = 0; u < n; u++){
            if(mask & (1 << u)) continue;
            if(mask & closure[u]) return;
            order[++p] = u,
            Generate(mask | (1 << u)), 
            order[p--] = u;
        }
    }

    void AddEdge(int u, int v){
        edges[u].push_back(v);
    }

};

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    int t; cin >> t;
    for(int c = 0; c < t; c++){
        set<char> s; string in,rs; 
        cin >> ws, getline(cin, in);
        vector<int> m(S, M); char r;
        stringstream st(in);  
        while(st >> ws >> r) 
            s.insert(r);
        vector<char> l; int n = 0; 
        for(auto& v : s)
            m[v - 'A'] = n++,
            l.push_back(v);
        cin >> ws, getline(cin, in);
        stringstream rt(in);
        unique_ptr<Graph> g;
        g.reset(new Graph(n));
        while(rt >> ws >> rs){
            int u = m[rs[0] - 'A'];
            int v = m[rs[2] - 'A'];
            g->AddEdge(u, v);
        }
        if(c) cout << '\n';
        if(g->HasCycle()){
            cout << "NO\n";
            continue;
        }
        g->GenerateOrderings();
        for(auto& order : g->orderings){
            cout << l[order.front()];
            for(int u = 1; u < n; u++)
                cout << ' ' << l[order[u]];
            cout << '\n';
        }
    }
    return 0;
}