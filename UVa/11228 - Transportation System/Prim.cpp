#include <forward_list>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <cmath>
#include <queue>

using std::numeric_limits;
using std::forward_list;
using std::unique_ptr;
using std::ios_base;
using std::vector;
using std::hypot;
using std::queue;
using std::round;
using std::cout;
using std::cin;

typedef forward_list<int> List;
typedef vector<double> vDouble;
typedef vector<bool> vBool;

const double oo = numeric_limits<int>::max();
const int S = -1;

struct Graph{

    vector<List> edges;
    vector<vDouble> g;
    vector<vBool> s;
    int n;

    Graph(int N): n(N),
        edges(N), g(N, vDouble(N, oo)), s(N, vBool(N)) {}

    void AddEdge(int u, int v, double w, bool r = false){
        s[u][v] = s[v][u] = r,
        g[u][v] = g[v][u] = w;
        if(r) return;
        edges[u].push_front(v),
        edges[v].push_front(u);
    }

    int MST(double& a, double& b, int f = 0){
        vector<int> super(n, S); int c = 0;
        for(int w = 0; w < n; w++){
            if(super[w] != S) continue;
            super[w] = c++;
            queue<int> q; 
            q.push(w);
            while(!q.empty()){
                int u = q.front(); q.pop();
                for(auto& v : edges[u]){
                    if(super[v] != S)
                        continue;
                    super[v] = super[u],
                    q.push(v);
                }
            }
        }
        vector<double> w(n, oo);
        vector<int> parent(n);
        vector<bool> seen(n);
        w[f] = 0, parent[f] = S;
        for(int k = 1; k < n; k++){
            double min = oo; int u;
            for(int v = 0; v < n; v++){
                if(seen[v]) continue;
                if(w[v] < min)
                    min = w[u = v];
            }
            seen[u] = true;
            for(int v = 0; v < n; v++){
                if(seen[v]) continue;
                if(g[u][v] < w[v])
                    w[v] = g[u][v],
                    parent[v] = u;
            }
        }
        for(int u = 0; u < n; u++){
            if(u == f) continue;
            auto& p = parent[u];
            if(s[u][p]) 
                b += g[u][p];
            else
                a += g[u][p];
        }
        return c;
    }

};

namespace Math{

    using std::abs;
    
    const double E = 1e-9;

    bool Equal(double a, double b){
        return abs(a - b) < E;
    }

};

struct Point{

    double x,y;

    Point(): x(), y() {}

    double Distance(const Point& other){
        return hypot(x - other.x, y - other.y);
    }

    bool operator<(const Point& other) const{
        if(Math::Equal(x, other.x)) 
            return y < other.y;
        return x < other.x;
    }

};

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); 
    int t; cin >> t;
    for(int c = 1; c <= t; c++){
        int n; double r;
        cin >> n >> r;
        vector<Point> city(n);
        unique_ptr<Graph> g;
        for(auto& p : city)
            cin >> p.x >> p.y;
        g.reset(new Graph(n));
        for(int u = 0; u < n; u++){
            auto& p = city[u];
            for(int v = u + 1; v < n; v++){
                auto z = p.Distance(city[v]);
                if(r < z)
                    g->AddEdge(u, v, z, true);
                else g->AddEdge(u, v, z);
            }
        }
        double rails = 0, roads = 0;
        cout << "Case #" << c << ": ",
        cout << g->MST(roads, rails),
        cout << ' ' << round(roads),
        cout << ' ' << round(rails),
        cout << '\n';
    }
    return 0;
}