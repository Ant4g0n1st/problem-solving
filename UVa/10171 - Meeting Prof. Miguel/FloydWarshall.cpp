#include <algorithm>
#include <iostream>
#include <memory>
#include <limits>
#include <vector>

using std::numeric_limits;
using std::unique_ptr;
using std::vector;
using std::cout;
using std::sort;
using std::cin;
using std::min;
using std::ws;

typedef long long int Long;
typedef vector<Long> vLong;

const Long oo = numeric_limits<int>::max();
const int M = -1;
const int S = 26;

struct Graph{

    vector<vLong> g;
    int n;

    Graph(int N): g(N, vLong(N, oo)), n(N) {}

    void FloydWarshall(){
        for(int w = 0; w < n; w++)
            for(int u = 0; u < n; u++)
                for(int v = 0; v < n; v++)
                    g[u][v] = min(g[u][v],
                        g[u][w] +  g[w][v]);
    }

    void AddEdge(int u, int v, Long w){
        g[u][v] = min(g[u][v], w);
    }

    bool Reachable(int u, int v){
        return g[u][v] < oo;
    }

    Long Weight(int u, int v){
        return g[u][v];
    }

};

struct Edge{

    int b,u,v,y;
    Long w;

    Edge(): b(), u(), v(), w(), y() {}

};

char ReadLetter(){ char c;
    cin >> ws >> c;
    return c;
}

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int m; 
    while(cin >> m and m){ int n = 0;
        vector<int> node(S, M); 
        vector<Edge> edges(m);
        unique_ptr<Graph> a;
        unique_ptr<Graph> b;
        for(auto& e : edges){
            e.y = (ReadLetter() == 'Y'),
            e.b = (ReadLetter() == 'B'),
            e.u = ReadLetter() - 'A',
            e.v = ReadLetter() - 'A';
            if(node[e.u] == M)
                node[e.u] = n++;
            if(node[e.v] == M)
                node[e.v] = n++;
            cin >> e.w;
        }
        a.reset(new Graph(n)),
        b.reset(new Graph(n));
        vector<char> c(n);
        for(auto& e : edges){
            c[node[e.u]] = 'A' + e.u,
            c[node[e.v]] = 'A' + e.v,
            e.u = node[e.u],
            e.v = node[e.v];
            if(e.y){
                a->AddEdge(e.u, e.v, e.w);
                if(!e.b) continue;
                a->AddEdge(e.v, e.u, e.w);
            }else{
                b->AddEdge(e.u, e.v, e.w);
                if(!e.b) continue;
                b->AddEdge(e.v, e.u, e.w);   
            }
        }
        for(int u = 0; u < n; u++)
            a->AddEdge(u, u, 0),
            b->AddEdge(u, u, 0);
        a->FloydWarshall(), 
        b->FloydWarshall();
        int u = node[ReadLetter() - 'A'];
        int v = node[ReadLetter() - 'A'];
        Long mn = oo; bool meet = false;
        if(u != M and v != M){
            for(int w = 0; w < n; w++){
                if(!a->Reachable(u, w))
                    continue;
                if(!b->Reachable(v, w))
                    continue; 
                auto uw = a->Weight(u, w);
                auto vw = b->Weight(v, w);
                mn = min(mn, uw + vw),
                meet = true;
            }
        }
        if(!meet){
            cout << "You will never",
            cout << " meet.\n";
            continue;
        } 
        vector<int> best;
        for(int w = 0; w < n; w++){
            auto uw = a->Weight(u, w);
            auto vw = b->Weight(v, w);
            if(uw + vw == mn)
                best.push_back(w);
        }
        sort(best.begin(), best.end());
        cout << mn;
        for(auto& w : best)
            cout << ' ' << c[w];
        cout << '\n';
    }
    return 0;
}