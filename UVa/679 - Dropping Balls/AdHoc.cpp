#include <iostream>

using std::ios_base;
using std::cout;
using std::cin;

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    int z; cin >> z;
    for(int y = 0; y < z; y++){
        int d; cin >> d, --d;
        int x; cin >> x;
        int n = 1;
        for(int k = 0; k < d; k++)
            n <<= 1, n += !(x & 1),
            x = (x + 1) >> 1;
        cout << n << '\n';
    }
    return 0;
}