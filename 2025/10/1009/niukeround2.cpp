//https://ac.nowcoder.com/acm/contest/60456
//A
#include <bits/stdc++.h>
using namespace std;
int main() {
    string s, t;cin>>s>>t;
    int sl = s.size(), tl = t.size();
    if(sl < tl) {
        cout<<"-1\n"; return 0;
    }
    s += s;
    int cnt = 0;
    for(int i = 0; i < sl; i++) {
        string sub = s.substr(i, tl);
        if(sub == t) cnt++;
    }cout<<cnt<<'\n';
    return 0;
}//O(sl*ml)
//KMP优化 O(sl+ml)
#include <bits/stdc++.h>
using namespace std;
vector<int> build(const string &t) {
    int m = t.size();
    vector<int> lps(m, 0);
    for(int i = 1, len = 0; i < m; ) {
        if(t[i] == t[len]) {
            lps[i++] = ++len;
        } else if(len) {
            len = lps[len - 1];
        } else {
            lps[i++] = 0;
        }
    }
    return lps;
}
int main() {
    string s, t;cin>>s>>t;
    int n = s.size(), m = t.size();
    string ss = s + s.substr(0, m - 1);
    vector<int> lps = build(t);
    int cnt = 0;
    for(int i = 0, j = 0; i < ss.size(); ) {
        if(ss[i] == t[j])  {
            ++i; ++j;
            if(j == m) {
                int st  = i - m;
                if(st < n) cnt++;
                j = lps[j-1];
            }
        } else if(j > 0) {
            j = lps[j-1];
        } else i++;
    }cout<<cnt<<'\n'; return 0;
}
//B
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<long long> a(n + 1);
    for (int i = 1; i <= n; ++i) cin >> a[i];
    string s;
    cin >> s;
    s = " " + s; // 对齐索引

    vector<vector<long long>> dp(n + 1, vector<long long>(2, 0));

    for (int i = 2; i <= n; ++i) {
        dp[i][0] = max(dp[i - 1][0], dp[i - 1][1]);
        if (s[i] != s[i - 1])
            dp[i][1] = max(dp[i-2][0], dp[i-2][1]) + a[i] + a[i-1];
    }

    cout << max(dp[n][0], dp[n][1]) << "\n";
    return 0;
}//O(n) O(n)
//优化空间复杂度 1 dimension dp 
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<long long> a(n + 1);
    for (int i = 1; i <= n; ++i) cin >> a[i];
    string s;
    cin >> s;
    s = " " + s;  // 下标从1开始对齐

    // dp[i][0], dp[i][1] 压缩到几个变量
    long long dp0_prev2 = 0, dp1_prev2 = 0;  // i-2层
    long long dp0_prev1 = 0, dp1_prev1 = 0;  // i-1层
    long long dp0_cur = 0, dp1_cur = 0;      // 当前层

    for (int i = 2; i <= n; ++i) {
        // 当前未选
        dp0_cur = max(dp0_prev1, dp1_prev1);

        // 当前选
        if (s[i] != s[i - 1])
            dp1_cur = max(dp0_prev2, dp1_prev2) + a[i] + a[i - 1];
        else
            dp1_cur = 0;

        // 更新滚动
        dp0_prev2 = dp0_prev1;
        dp1_prev2 = dp1_prev1;
        dp0_prev1 = dp0_cur;
        dp1_prev1 = dp1_cur;
    }

    cout << max(dp0_cur, dp1_cur) << "\n";
    return 0;
}
//C
#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;

int main() {
    // 快速IO
    cin.tie(nullptr)->sync_with_stdio(false);

    int n; // 方块数量
    cin >> n;

    // 定义四种旋转下方块的相对坐标
    // 每个方块由 4 个格子组成，(x, y) 表示相对左下角的偏移
    array<array<pair<int, int>, 4>, 4> p = {{
        // 0°旋转
        {{{0, 0}, {0, 1}, {0, 2}, {1, 2}}},
        // 90°旋转
        {{{0, 0}, {0, 1}, {1, 0}, {2, 0}}},
        // 180°旋转
        {{{0, 0}, {1, 0}, {1, 1}, {1, 2}}},
        // 270°旋转
        {{{0, 1}, {1, 1}, {2, 0}, {2, 1}}}
    }};

    // 列高度数组 h[1..8]，h[i] 表示第 i 列的当前最高高度
    // h[0] 不用，方便直接用 1~8 索引
    vector<int> h(9);

    // 遍历每一个方块
    for (int i = 0; i < n; i += 1) {
        int a, b; 
        cin >> a >> b; // a: 旋转角度，b: 方块左端落在哪一列

        a /= 90; // 将角度转换成 0~3 的索引，对应 p 数组

        int hi = 0; // 方块落下的最终最大高度
        // 计算方块落下时，不碰已有方块的最大高度
        for (auto [x, y] : p[a]) {
            // b+x: 对应全局列
            // h[b+x] + y + 1: 当前列高度 + 方块相对高度 + 1
            hi = max(hi, h[b + x] + y + 1);
        }

        // 更新落下后的列高度
        for (auto [x, y] : p[a]) {
            // 每个格子在列上的高度 = hi - y
            // max 防止降低已有列高度
            h[b + x] = max(h[b + x], hi - y);
        }
    }

    // 输出最终 8 列的高度
    for (int i = 1; i <= 8; i += 1) {
        cout << h[i] << " ";
    }
}

//D
#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
int main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    i64 n, h, k;
    cin >> n >> h >> k;
    vector<i64> s(n);
    for (i64 &si : s) {
        i64 a, b;
        cin >> a >> b;
        i64 t = a / 4 * 3 + (a % 4 - (a % 4 >= 2));
        si = (t - 1) * b;
    }
    sort(s.begin(), s.end());
    partial_sum(s.begin(), s.end(), s.begin());
    int q;
    for (cin >> q; q; q -= 1) {
        i64 x;
        cin >> x;
        x = x * k + h;
        cout << lower_bound(s.begin(), s.end(), x) - s.begin() << " ";
    }
}