//每日一题 https://www.nowcoder.com/practice/761f78551b854037abe412c2de615c05?channelPut=w251acm
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    if (!(cin >> s)) return 0;
    // 字符是大小写字母混合，直接用 bool[128] 统计
    vector<bool> seen(128, false);
    int cnt = 0;
    for (char c : s) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (!seen[uc]) {
            seen[uc] = true;
            ++cnt;
        }
    }
    cout << cnt << '\n';
    return 0;
}
//https://ac.nowcoder.com/acm/contest/64272
//A
int main() {
    int n; cin>>n;
    vector<i64> a(n);
    for(int i = 0; i < n; i++) {
        cin>>a[i];
    }
    int ans = 1, l = 0;
    for(int r = 1; r < n; r++) {
        if(abs(a[r] - a[r-1]) > 1) {
            l = r;
        }
        ans = max(ans, r - l + 1);
    }cout<<ans<<'\n';
    return 0;
}
//B
bool is(string s) {
    for(int i = 1; i < s.size(); i++) {
        if(s[i] == s[i-1]) return false; 
    }
    return true;
}
int main() {
    string s;cin>>s; 
    sort(s.begin(), s.end());
    i64 ans = 0;
    do {
        if(is(s)) ans++;
    } while(next_permutation(s.begin(), s.end()));
    cout<<ans<<'\n';

    return 0;
}
//C
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    double v0, x, y;
    cin >> v0 >> x >> y;

    double t_min;
    double limit = sqrt(y * x);
    if (v0 >= limit) {
        // 不用加油，直接跑
        t_min = y / v0;
    } else {
        // 先加油再跑
        double t = (limit - v0) / x;
        t_min = t + y / limit;
    }

    cout << fixed << setprecision(7) << t_min << "\n";
    return 0;
}
//D
#include<bits/stdc++.h>
#define int long long
#define mod 1000000007
using namespace std;

const int maxn = 200005;
int a[maxn];

void solve() {
    int n;
    cin >> n;
    int ans = 0;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        ans = (ans + (1 + a[i]) * a[i] / 2) % mod;
    }

    for (int i = 2; i < n; i++) {
        int sum = 0;
        for (int j = 1; i - j >= 1 && i + j <= n; j++) {
            if (a[i - j] != a[i + j]) {
                sum += min(a[i - j], a[i + j]);
                break;
            } else {
                sum += a[i - j];
            }
        }
        ans = (ans + sum) % mod;
    }
    cout << ans;
}

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    solve();
    return 0;
}
