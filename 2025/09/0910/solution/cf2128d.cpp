#include <bits/stdc++.h>
#define TESTS int t; cin >> t; while (t--)
using namespace std;
using ll = long long;
int main() {
    ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);
    TESTS{
        int n;cin>>n;
        vector<int> p(n+1);
        for(int i = 1; i <= n; i++) cin>>p[i];
        ll ans = 1LL * n * (n + 1) / 2;
        for(int i = 1; i <= n - 1; i++) {
            if(p[i] > p[i+1])  ans += 1LL * i * (n - i);
        }
        cout<<ans<<"\n";
    }
    return 0;
}
