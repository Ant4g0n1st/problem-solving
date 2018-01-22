#include <iostream>
#include <limits>
#include <vector>

using namespace std;

typedef long long int Long;
typedef vector<char> vChar;
typedef vector<Long> vLong;
typedef vector<int> vInt;

const Long oo = numeric_limits<int>::max();
const int M = -1;
const int S = 26;

char ReadLetter(){ char c;
	cin >> ws, cin >> c;
	return c - 'A';
}

int main(){
	ios_base::sync_with_stdio(0),
	cout.tie(0), cin.tie(0);
	int m,n; cin >> n >> m;
	vInt node(S, M);
	vChar a(n);
	vector<vLong> f(n, vLong(n, oo));
	vector<vLong> g(n, vLong(n, oo));
	vector<vInt> p(n, vInt(n));
	for(int u = 0; u < n; u++){
		f[u][u] = g[u][u] = 0;
		for(int v = 0; v < n; v++)
			p[u][v] = u;
	} int k = 0;
	for(int i = 0; i < m; i++){
		char u = ReadLetter();
		char v = ReadLetter();
		if(node[u] == M)
			a[k] = 'A' + u,
			node[u] = k++;
		if(node[v] == M)
			a[k] = 'A' + v,
			node[v] = k++;
		Long w; cin >> w;
		u = node[u], v = node[v],
		g[u][v] = g[v][u] = w,
		f[u][v] = 1; 
	}
	for(int w = 0; w < n; w++)
		for(int u = 0; u < n; u++)
			for(int v = 0; v < n; v++){
					if(g[u][w] + g[w][v] == g[u][v])
						if(f[u][w] + f[w][v] < f[u][v])
							f[u][v] = f[u][w] + f[w][v],
							p[u][v] = p[w][v];
					if(g[u][w] + g[w][v] < g[u][v])
						f[u][v] = f[u][w] + f[w][v],
						g[u][v] = g[u][w] + g[w][v],
						p[u][v] = p[w][v];
				}
	int q; cin >> q;
	vInt path(n);
	for(int i = 0; i < q; i++){
		char u = ReadLetter();
		char v = ReadLetter();
		u = node[u], v = node[v];
		int t = 0;
		while(v != u) 
			path[t++] = v, 
			v = p[u][v];
		cout << a[u];
		for(--t; t >= 0; t--)
			cout << ' ' << a[path[t]];
		cout << '\n';
	}
	return 0; 
}