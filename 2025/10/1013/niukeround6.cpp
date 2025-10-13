//https://ac.nowcoder.com/acm/contest/62622
//每日一题 https://www.nowcoder.com/practice/8afb3cefb73c4918863f73ffec7782e4?channelPut=w251acm
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
i64 a[101010];
int main() {
    int n,k,i,j;cin>>n>>k;
    k = min(k, n - k);
    for(int i = 0; i < n; i++) cin>>a[i];
    sort(a, a + n);
    i64 ans = 0;
    for(i = 0,j= n-1; i < k; i++,j--) ans += a[j] - a[i];
    cout<<ans<<'\n';
    return 0;
}
//A
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
int main() {
    string s;cin>>s;
    int cnt = 0;
    for(int i = 0; i < s.size(); i++) {
        if(isdigit(s[i])) {
            int t = s[i] - '0';
            if(t == 0 || t == 6 || t == 9) cnt++;
            else if(t == 8) cnt += 2;
        }
    }cout<<cnt<<'\n';
    return 0;
}
//B
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
int main() {
    int t;cin>>t;
    while(t--) {
        i64 a,b;cin>>a>>b;
        int sum = 0;
        while(b > 0) {
            sum += b % 10;
            b /= 10;
        }
        cout<<a*sum<<'\n';
    }
    return 0;
}
//C
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

template<class T> bool chmin(T &a, T b) {
    if (a <= b) return false;
    a = b; return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll N;
    cin >> N;

    ll best_diff = LLONG_MAX;
    ll best_x = -1, best_y = -1;
    ll fact = 1;

    auto check = [&](int nx, ll ny, ll diff) {
        if (nx != 2 && ny != 2 && ny > 0 && chmin(best_diff, diff)) {
            best_x = nx;
            best_y = ny;
        }
    };

    for (int i = 1; i <= 13; ++i) {
        fact *= i;
        if (i == 1) {
            check(1, 1, N);
        } else if (i != 2) {
            ll denom = fact - 1;
            if (denom == 0) continue;
            ll y = N / denom;
            check(i, y, llabs(N - y * denom));
            check(i, y + 1, llabs(N - (y + 1) * denom));
        }
    }

    cout << best_x << " " << best_y << "\n";
    return 0;
}
//D
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
int main() {
    int t;cin>>t;while(t--) {
        int n,k;i64 x;cin>>n>>k>>x;
        vector<int> v(n);
        for(auto& e : v) cin>>e;
        vector<int> mx(k, 0), c(k, 0);
        for(int i = 0; i < k; i++) {
            i64 sum = 0;
            for(int j = i; j < n; j += k) {
                mx[i] = max(mx[i], v[j]);
                sum += v[j]; c[i]++;
            }
            x -= 1LL * mx[i] * c[i] - sum;
        }
        if(x<0) {
            cout<<"-1\n";
            continue;
        }
        i64 ans = 0;
        for(int i = 0; i < k; i++) {
            ans = max(ans, (i64)mx[i] + x / c[i]);
        }
        cout<<ans<<'\n';
    }
    return 0;
}
//E
#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
int c2, c5;
i64 p, q;

bool check1() {
    while (q % 2 == 0) q /= 2, ++ c2;
    while (q % 5 == 0) q /= 5, ++ c5;
    return q == 1;
}

i64 qpow(__int128_t a, __int128_t b, __int128_t p) {
    a %= p;
    __int128_t ret = 1 % p;
    for (; b; b >>= 1) {
        if (b & 1) ret = ret * a % p;
        a = a * a % p;
    }
    return ret;
}

i64 phi(i64 x) {
    i64 res = x;
    for (i64 i = 2; i * i <= x; i ++) if (x % i == 0) {
        while (x % i == 0) x /= i;
        res = res / i * (i - 1);
    }
    if (x > 1) res = res / x * (x - 1);
    return res;
}

void solve(i64 p, i64 q) {
        

    i64 phiv = phi(q);
    i64 mn_ans = phiv;
    auto check_ans = [&](i64 v) {
        if (qpow(10, v, q) == 1) {
            mn_ans = min(mn_ans, v);
        }
    };
    for (i64 d = 1; d * d <= phiv; d ++) if (phiv % d == 0) {
        check_ans(d);
        if (phiv / d != d) check_ans(phiv / d);
    }
    cout << max(c2, c5) << " " << mn_ans;
}

int main() {
    cin >> p >> q;
    i64 d = __gcd(p, q);
    p /= d, q /= d;
    if (check1()) {
        cout << "-1";
        return 0;
    }
    solve(p, q);
    return 0;
}

