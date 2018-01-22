#include <iostream>
#include <iomanip>
#include <vector>
#include <map>

using namespace std;

typedef long long int Long;
typedef pair<int,int> Pair;
typedef vector<Long> vLong;
typedef vector<Pair> vPair;

int main(){
	cout << fixed << setprecision(3);
	ios_base::sync_with_stdio(0),
	cout.tie(0), cin.tie(0);
	int k,u,v; k = 0;
	while(cin >> u >> v and u){ int n = 0;
		vPair p; p.push_back({u, v});
		while(cin >> u >> v and u)
			p.push_back({u, v});
		map<int,int> m;
		for(auto& q : p){
			if(!m.count(--q.second))
				m[q.second] = n++;
			if(!m.count(--q.first))
				m[q.first] = n++;
			q.second = m[q.second],
			q.first = m[q.first];
		}
		const Long oo = n + 1;
		vector<vLong> g(n, vLong(n, oo));
		for(auto& q : p)
			g[q.first][q.second] = 1;
		for(int w = 0; w < n; w++) 
			g[w][w] = 0;
		for(int z = 0; z < n; z++)
			for(int x = 0; x < n; x++)
				for(int y = 0; y < n; y++)
					g[x][y] = min(g[x][y],
						g[x][z] + g[z][y]);
		double num = 0;
		for(int x = 0; x < n; x++)
			for(int y = 0; y < n; y++)
				num += g[x][y];
		double den = n * (n - 1);
		cout << "Case " << ++k << ": ",
		cout << "average length ",
		cout << "between pages =",
		cout << ' ' << num / den,
		cout << " clicks\n";
	}
	return 0;
}