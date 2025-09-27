//https://ac.nowcoder.com/acm/contest/111666#question
//A
#include <bits/stdc++.h>
using namespace std;
int main() {
    int x, y;cin>>x>>y;
    if(x == y) cout<<"Draw\n";
    else if((x+1)%3==y) cout<<"Hongwins\n";
    else cout<<"chengwins\n";
    return 0;
}
//B
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;
        vector<ll> a(n);
        for (int i = 0; i < n; i++) cin >> a[i];

        int m = n - 1;
        vector<int> b(m);
        for (int i = 0; i < m; i++) {
            b[i] = __gcd(a[i], a[i + 1]) != 1;  // 1 表示不互质
        }

        int ans = 0;
        for (int i = 0, j = 0; i < m; i = j) {
            while (j < m && b[i] == b[j]) j++;  // 找到连续段
            if (b[i]) ans += (j - i - 1) / 2 + 1;  // 连续冲突段最少修改次数
        }

        cout << ans << "\n";
    }

    return 0;
}
//C
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const ll MOD = 1e9 + 7;

// 快速幂，计算 a^b % MOD
ll power(ll a, ll b) {
    ll res = 1;
    a %= MOD;
    while (b > 0) {
        if (b & 1) res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        ll n;
        string s;
        cin >> n >> s;

        ll len = s.size();
        // 用费马小定理优化大指数：2^(n*len) ≡ 2^((n*len)%(MOD-1)) (mod MOD)
        ll exp = (n % (MOD - 1)) * len % (MOD - 1);
        ll ans = (power(2, exp) - 1 + MOD) % MOD; // 非空子序列 = 2^(长度) - 1
        cout << ans << '\n';
    }

    return 0;
}
