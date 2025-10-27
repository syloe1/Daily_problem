//https://ac.nowcoder.com/acm/contest/69695
//A
int main() {
    int n;cin>>n;
    vector<i64> a(n);
    for(auto&e : a) cin>>e;
    i64 mx = *max_element(a.begin(), a.end());
    int cnt = count(a.begin(), a.end(), mx);
    cout<<n-cnt<<'\n';
    return 0;
}
//B
int main() {
    string s;cin>>s; 
    auto longest = [&](char c) {
        int cnt = 0, res = 0;
        for(char x : s) {
            if(x == c || x == '?') cnt++;
            else cnt = 0;
            res = max(res, cnt);
        }
        return res;
    };
    cout<<max(longest('0'), longest('1'));
    return 0;
}
//C
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n, k, t;
    cin >> n >> k >> t;

    // 特殊情况：全为 0 或无法构造
    if (k == 0) {
        if (t == 0) {
            cout << string(n, '0') << '\n';
        } else {
            cout << -1 << '\n';
        }
        return 0;
    }

    // 1. 基本合法性检查
    if (t > k - 1 || t < 0) {
        cout << -1 << '\n';
        return 0;
    }

    long long blocks = k - t;  // 连续块数
    if (blocks > n) {
        cout << -1 << '\n';
        return 0;
    }

    long long needLen = k + (blocks - 1);
    if (needLen > n) {
        cout << -1 << '\n';
        return 0;
    }

    string res;
    // 先放一个连续块（长度 t+1）
    res += string(t + 1, '1');
    k -= (t + 1);
    blocks--;

    // 每个剩余块放 “0” + “1”
    while (blocks > 0 && (int)res.size() < n) {
        res += "01";
        k--;
        blocks--;
    }

    // 剩余的 1（如果有），放在结尾
    res += string(k, '1');

    // 补 0 到长度 n
    if ((int)res.size() < n)
        res += string(n - res.size(), '0');

    cout << res.substr(0, n) << '\n';
    return 0;
}
//D
#include <bits/stdc++.h>
using namespace std;

long long toNum(const string &s) {
    if (s.empty()) return -1;
    // 去除前导0（"000"→"0"）
    int i = 0;
    while (i + 1 < (int)s.size() && s[i] == '0') i++;
    return stoll(s.substr(i));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string a, b;
    cin >> a >> b;
    int n = a.size(), m = b.size();

    int ans = INT_MAX;

    vector<long long> sa;
    vector<int> da;
    for (int mask = 1; mask < (1 << n); ++mask) {
        string s;
        for (int i = 0; i < n; ++i)
            if (mask & (1 << i))
                s.push_back(a[i]);
        long long num = toNum(s);
        if (num != -1) {
            sa.push_back(num);
            da.push_back(n - (int)s.size());
        }
    }

    vector<long long> sb;
    vector<int> db;
    for (int mask = 1; mask < (1 << m); ++mask) {
        string s;
        for (int i = 0; i < m; ++i)
            if (mask & (1 << i))
                s.push_back(b[i]);
        long long num = toNum(s);
        if (num != -1) {
            sb.push_back(num);
            db.push_back(m - (int)s.size());
        }
    }

    for (int i = 0; i < (int)sa.size(); ++i) {
        for (int j = 0; j < (int)sb.size(); ++j) {
            long long x = sa[i], y = sb[j];
            if (x == 0 || y == 0) continue;
            if (x % y == 0 || y % x == 0)
                ans = min(ans, da[i] + db[j]);
        }
    }

    if (ans == INT_MAX) cout << -1 << '\n';
    else cout << ans << '\n';
}
//E
#include <bits/stdc++.h>
using namespace std;
const int MOD = 1e9 + 7;

// 计算 pattern 的自动机转移表
vector<vector<int>> buildTrans(const string &pat) {
    int m = pat.size();
    vector<vector<int>> nxt(m + 1, vector<int>(26, 0));
    for (int i = 0; i <= m; i++) {
        for (char c = 'a'; c <= 'z'; c++) {
            string s = pat.substr(0, i) + c;
            int k = min(m, i + 1);
            while (k > 0 && pat.substr(0, k) != s.substr(s.size() - k))
                k--;
            nxt[i][c - 'a'] = k;
        }
    }
    return nxt;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    string red = "red", der = "der";
    auto tr1 = buildTrans(red);
    auto tr2 = buildTrans(der);

    // dp[r][d][h]
    vector<vector<vector<int>>> dp(4, vector<vector<int>>(4, vector<int>(2, 0)));
    dp[0][0][0] = 1;

    for (int i = 0; i < n; i++) {
        vector<vector<vector<int>>> ndp(4, vector<vector<int>>(4, vector<int>(2, 0)));
        for (int r = 0; r < 4; r++) {
            for (int d = 0; d < 4; d++) {
                for (int h = 0; h < 2; h++) {
                    int cur = dp[r][d][h];
                    if (!cur) continue;
                    for (int c = 0; c < 26; c++) {
                        int nr = tr1[r][c];
                        int nd = tr2[d][c];
                        if (nd == 3) continue; // 出现了 "der"，非法
                        int nh = h | (nr == 3);
                        ndp[nr][nd][nh] = (ndp[nr][nd][nh] + cur) % MOD;
                    }
                }
            }
        }
        dp.swap(ndp);
    }

    long long ans = 0;
    for (int r = 0; r < 4; r++)
        for (int d = 0; d < 4; d++)
            ans = (ans + dp[r][d][1]) % MOD;

    cout << ans << '\n';
    return 0;
}
//F
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

int count_factor(long long x, int p) {
    int cnt = 0;
    while (x % p == 0) {
        x /= p;
        cnt++;
    }
    return cnt;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n; cin >> n;
    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];

    vector<i64> pre2(n + 1, 0), pre5(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        pre2[i] = pre2[i - 1] + count_factor(a[i], 2);
        pre5[i] = pre5[i - 1] + count_factor(a[i], 5);
    }

    i64 ans = 0;
    // prefix sum of pre2 and pre5 to compute contributions faster
    for (int r = 1; r <= n; r++) {
        for (int l = 1; l <= r; l++) {
            i64 cnt2 = pre2[r] - pre2[l - 1];
            i64 cnt5 = pre5[r] - pre5[l - 1];
            ans += min(cnt2, cnt5);
        }
    }

    cout << ans << '\n';
    return 0;
}
