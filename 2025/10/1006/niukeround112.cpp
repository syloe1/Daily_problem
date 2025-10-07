//https://ac.nowcoder.com/acm/contest/118247
//A
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
int main() {
    int n,k;cin>>n>>k;
    int gold = ceil(n*0.1);
    int yin = gold * 2;
    int tong = gold * 3;
    if(k<=gold) {
        cout<<"Gold Medal\n";
    }else if(k<=gold+yin) {
        cout<<"Silver Medal\n";
    }else if(k<=gold+yin+tong){
        cout<<"Bronze Medal\n";
    }else{
        cout<<"Da Tie\n";
    }
    return 0;
}
//B
#include <bits/stdc++.h>
using namespace std;
int main() {
    int t;cin>>t;while(t--){
        int n;cin>>n;vector<int> a(n);
        int ax = -1;
        for(int i = 0; i < n; i++) {
            cin>>a[i]; ax = max(ax, a[i]);
        }
        cout<<max(ax, n)<<'\n';
    }
    return 0;
}
//C
#include <bits/stdc++.h>
using namespace std;
bool is(string s) {
    int l = 0, r = s.size() - 1;
    while (l < r) {
        if(s[l] != s[r]) return false;
        l++; r--;
    }return true;
}
int main() {
    int t;cin>>t;while(t--){
        string s;cin>>s;
        if(is(s)) cout<<1<<'\n';
        else cout<<2<<'\n';
    }
    return 0;
}
//D
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
int main() {
    int t;cin>>t;while(t--) {
        int n;cin>>n;
        vector<i64> a(2*n);
        for(auto& x : a) cin>>x;
        i64 ans = 0;
        for(int bit = 60; bit >= 0; bit--) {
            i64 test = ans | (1LL<<bit);
            int cnt = 0;
            for(auto x : a) {
                if((x & test) == test) cnt++;
            }
            if(cnt >= n) ans = test;
        }
        cout<<ans<<'\n';
    }
    return 0;
}
//E
#include <bits/stdc++.h>
using namespace std;
const int MOD = 998244353;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    int MAXN = 200000;
    vector<ll> pow2(MAXN + 5, 1);
    for (int i = 1; i <= MAXN; ++i)
        pow2[i] = (pow2[i - 1] * 2) % MOD;

    while (T--) {
        int n;
        cin >> n;
        vector<int> a(n);
        for (auto &x : a) cin >> x;

        vector<int> cnt(n + 1, 0);
        for (auto x : a) cnt[x]++;

        vector<ll> F(n + 2, 0), G(n + 2, 0);

        // Step 1: compute F(g)
        for (int g = n; g >= 1; --g) {
            int mult = 0;
            for (int k = g; k <= n; k += g)
                mult += cnt[k];
            ll total = (pow2[mult] - 1 + MOD) % MOD;
            for (int k = 2 * g; k <= n; k += g)
                total = (total - F[k] + MOD) % MOD;
            F[g] = total;
        }

        // Step 2: compute G(g) (exclude g itself)
        for (int g = n; g >= 1; --g) {
            int mult_no_self = 0;
            for (int k = 2 * g; k <= n; k += g)
                mult_no_self += cnt[k];
            ll total = (pow2[mult_no_self] - 1 + MOD) % MOD;
            for (int k = 2 * g; k <= n; k += g)
                total = (total - F[k] + MOD) % MOD;
            G[g] = total;
        }

        ll ans = 0;
        for (int g = 1; g <= n; ++g)
            ans = (ans + G[g]) % MOD;

        cout << ans << "\n";
    }
    return 0;
}
