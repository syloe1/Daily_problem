//https://ac.nowcoder.com/acm/contest/65821
//A
int main() {
    int n; string s;cin>>n>>s; 
    list<char> q;
    i64 ans = 0;
    for(char c : s) {
        if(!q.empty() && q.back() == c) {
            ans += 2;
            q.pop_back();
        } else {
            q.push_back(c);
        }
    }
    while(q.size() > 1 && q.front() == q.back()) {
        ans += 2;
        q.pop_front();
        q.pop_back();
    }
    cout<<ans<<"\n";
    return 0;
}
//B
int main() {
    int t;
    cin >> t;
    while (t--) {
        long long X, Y;
        cin >> X >> Y;

        int c5 = 0, c6 = 0, d5 = 0, d6 = 0;

        while (X % 5 == 0) c5++, X /= 5;
        while (X % 6 == 0) c6++, X /= 6;
        while (Y % 5 == 0) d5++, Y /= 5;
        while (Y % 6 == 0) d6++, Y /= 6;

        int ans;
        if (c5 > d5 || c6 < d6 || X != Y)
            ans = -1;
        else
            ans = (d5 - c5) + (c6 - d6);

        cout << ans << '\n';
    }
    return 0;
}
//C
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll count_at_most(const string &s, int k) {
    if (k == 0) return 0;
    int n = s.size();
    vector<int> cnt(26, 0);
    int distinct = 0;
    ll ans = 0;
    int l = 0;

    for (int r = 0; r < n; ++r) {
        if (++cnt[s[r] - 'a'] == 1)
            distinct++;

        while (distinct > k) {
            if (--cnt[s[l] - 'a'] == 0)
                distinct--;
            l++;
        }
        ans += (r - l + 1);
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, l, r;
    string s;
    cin >> n >> l >> r >> s;

    ll ans = count_at_most(s, r) - count_at_most(s, l - 1);
    cout << ans << '\n';
    return 0;
}
//D
#include <bits/stdc++.h>
using namespace std;
using int64 = long long;
const int64 MOD = 1'000'000'007;

int64 modpow(int64 a, int64 b) {
    int64 res = 1;
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

    long long n;
    cin >> n;

    if (n < 3) {
        cout << 0 << '\n'; // 长度 < 3 没有长度为3的子串
        return 0;
    }

    long long ans = ( (n - 2) % MOD ) * modpow(26, n - 1) % MOD;
    cout << ans << '\n';
    return 0;
}
