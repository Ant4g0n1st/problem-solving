#include <forward_list>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <queue>

using std::forward_list;
using std::unique_ptr;
using std::ios_base;
using std::string;
using std::vector;
using std::queue;
using std::cout;
using std::cin;

namespace Constants{

    const int D = 26;
    const int S = -1;

};

using namespace Constants;

struct Graph{

    typedef forward_list<int> List;

    vector<List> edges;
    vector<int> out;
    vector<int> in;
    int n;

    Graph(int N): edges(N), in(N), n(N), out(N) {}

    bool IsEulerian(){ int x = 0, y = 0;
        for(int u = 0; u < n; u++){
            if(out[u] - in[u] > 1)
                return false;
            if(in[u] - out[u] > 1)
                return false;
            if(out[u] - in[u] > 0) ++x;
            if(in[u] - out[u] > 0) ++y;
        }
        if(x and y)
            return x == 1 and y == 1;
        return !x and !y;
    }

    bool IsConnected(){ int s = 0;
        for(int u = 0; u < n; u++){
            if(in[u]) continue;
            s = u; break;
        }
        queue<int> q; int m = 0;
        vector<bool> seen(n);
        seen[s] = true;
        q.push(s);
        while(!q.empty()){
            auto u = q.front();
            for(auto& v : edges[u])
                if(!seen[v])
                    seen[v] = true,
                    q.push(v), ++m;
            q.pop();
        }
        return ++m == n;
    }

    void AddEdge(int u, int v){
        edges[u].push_front(v),
        ++out[u], ++in[v];
    }

};

struct Edge{

    int u,v;

    Edge(): u(), v() {}

};

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    int z; cin >> z;
    for(int y = 0; y < z; y++){
        vector<int> node(D, S);
        unique_ptr<Graph> g;
        vector<Edge> edges;
        int m; cin >> m;
        edges.resize(m);
        int n = 0;
        for(auto& e : edges){
            string w; cin >> w;
            auto a = w.front();
            auto b = w.back();
            e.u = a - 'a';
            e.v = b - 'a';
            if(node[e.u] == S)
                node[e.u] = n++;
            if(node[e.v] == S)
                node[e.v] = n++;
            e.u = node[e.u],
            e.v = node[e.v];
        }
        g.reset(new Graph(n));
        for(auto& e : edges)
            g->AddEdge(e.u , e.v);
        bool b = g->IsConnected();
        b = b and g->IsEulerian();
        if(b){
            cout << "Ordering is",
            cout << " possible.",
            cout << '\n';
            continue;
        }
        cout << "The door cannot",
        cout << " be opened.",
        cout << '\n';
    }
    return 0;
}