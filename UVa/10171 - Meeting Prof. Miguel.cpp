#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

using std::numeric_limits;
using std::ios_base;
using std::vector;
using std::cout;
using std::sort;
using std::cin;
using std::min;
using std::ws;

typedef long long int Long;
typedef vector<char> vChar;
typedef vector<Long> vLong;
typedef vector<int> vInt;

const Long oo = numeric_limits<int>::max();
const int M = -1;
const int S = 26;

struct Edge{

	bool b,y;
	int u,v;
	Long w;

};

typedef vector<Edge> vEdge;

void AddEdge(vector<vLong>& g,
		int u, int v, Long w){
	g[u][v] = min(g[u][v], w);
}

void AddEdge(vector<vInt>& g,
		int u, int v){
	g[u][v] = 1;
}

char ReadLetter(){ char c;
	cin >> ws, cin >> c;
	return c;
}

int main(){
	ios_base::sync_with_stdio(0),
	cout.tie(0), cin.tie(0); int m; 
	while(cin >> m and m){
		vInt node(S, M);
		vEdge edges(m);
		int n = 0;
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
		vector<vLong> go(n, vLong(n, oo));
		vector<vLong> gy(n, vLong(n, oo));
		vector<vInt> to(n, vInt(n, 0));
		vector<vInt> ty(n, vInt(n, 0));
		vChar a(n);
		for(auto& e : edges){
			a[node[e.u]] = 'A' + e.u,
			a[node[e.v]] = 'A' + e.v,
			e.u = node[e.u],
			e.v = node[e.v];
			if(e.y){
				AddEdge(gy, e.u, e.v, e.w),
				AddEdge(ty, e.u, e.v);
				if(e.b)
					AddEdge(gy, e.v, e.u, e.w),
					AddEdge(ty, e.v, e.u);
				continue;
			}
			AddEdge(go, e.u, e.v, e.w),
			AddEdge(to, e.u, e.v);
			if(e.b)
				AddEdge(go, e.v, e.u, e.w),
				AddEdge(to, e.v, e.u);
		}
		for(int u = 0; u < n; u++)
			go[u][u] = gy[u][u] = 0,
			to[u][u] = ty[u][u] = 1;
		for(int w = 0; w < n; w++)
			for(int u = 0; u < n; u++)
				for(int v = 0; v < n; v++)
					to[u][v] |= (to[u][w] & to[w][v]),
					ty[u][v] |= (ty[u][w] & ty[w][v]),
					go[u][v] = min(go[u][v],
						go[u][w] + go[w][v]),
					gy[u][v] = min(gy[u][v],
						gy[u][w] + gy[w][v]);
		int u = node[ReadLetter() - 'A'];
		int v = node[ReadLetter() - 'A'];
		Long mn = oo; bool meet = false;
		if(u != M and v != M){
			for(int w = 0; w < n; w++){
				if(!(ty[u][w] & to[v][w]))
					continue; meet = true;
				mn = min(mn, gy[u][w] + go[v][w]);
			}
		}
		if(!meet){
			cout << "You will never meet.\n";
			continue;
		} vInt best;
		for(int w = 0; w < n; w++)
			if(gy[u][w] + go[v][w] == mn)
				best.push_back(w);
		sort(best.begin(), best.end());
		cout << mn;
		for(auto& u : best)
			cout << ' ' << a[u];
		cout << '\n';
	}
	return 0;
}