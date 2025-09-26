//https://ac.nowcoder.com/acm/contest/112543
//A
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    vector<int> a(5), b(5);
    for(int i = 0; i < 5; i++) cin >> a[i];
    for(int i = 0; i < 5; i++) cin >> b[i];
    
    // 计算我们选择每个角色当队长时的总实力
    vector<ll> our_strength(5);
    for(int i = 0; i < 5; i++) {
        ll sum = 0;
        for(int j = 0; j < 5; j++) {
            if(i == j) sum += 2 * a[j];  // 队长战力翻倍
            else sum += a[j];           // 队员战力正常计算
        }
        our_strength[i] = sum;
    }
    
    // 计算好友选择每个角色当队长时的总实力
    vector<ll> friend_strength(5);
    for(int i = 0; i < 5; i++) {
        ll sum = 0;
        for(int j = 0; j < 5; j++) {
            if(i == j) sum += 2 * b[j];  // 队长战力翻倍
            else sum += b[j];           // 队员战力正常计算
        }
        friend_strength[i] = sum;
    }
    
    // 检查是否存在我们的某种选择能胜过好友的某种选择
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            if(our_strength[i] > friend_strength[j]) {
                cout << "YES" << endl;
                return 0;
            }
        }
    }
    
    // 如果没有任何一种组合能胜过，则输出NO
    cout << "NO" << endl;
    return 0;
}
//B
#include <bits/stdc++.h>
using namespace std;

long long gcd(long long a, long long b) {
    return b == 0 ? a : gcd(b, a % b);
}

int main() {
    ios::sync_with_stdio(false);
    // cin.tie(nullptr);  // 去掉，避免部分 OJ 环境 SIGTERM

    int T;
    if (!(cin >> T)) return 0;  // 防止输入异常
    while (T--) {
        int a[5], k;
        for (int i = 0; i < 5; i++) cin >> a[i];
        cin >> k;

        int cnt = 0, m = 0;
        for (int i = 0; i < 5; i++) {
            if (a[i] != 0) {
                m++;
                if (a[i] == k) cnt++;
            }
        }

        long long num, den;
        if (m == 0) {
            num = 1; den = 1000;
        } else {
            if (cnt == 0) {
                num = 0; den = 1;
            } else {
                num = cnt; den = m;
            }
        }

        long long g = gcd(num, den);
        num /= g; den /= g;
        cout << num << "/" << den << "\n";
    }
    return 0;
}
//C
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int i = 0; i < n; i++) cin >> a[i];

        // 特殊情况：所有人回答相同
        if (count(a.begin(), a.end(), a[0]) == n) {
            if (a[0] * 2 <= n || a[0] == n - 1)
                cout << "Other\n";
            else
                cout << "Lie\n";
            continue;
        }

        // 一般情况
        int mx = *max_element(a.begin(), a.end());
        int cnt_mx = count(a.begin(), a.end(), mx);
        int cnt_mx_minus1 = count(a.begin(), a.end(), mx - 1);

        if (cnt_mx == n - mx && cnt_mx_minus1 == mx)
            cout << "Other\n";
        else
            cout << "Lie\n";
    }

    return 0;
}
//D
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
int main() {
    int t;cin>>t;while(t--) {
        int n;cin>>n;
        vector<vector<int>> g(n+1);
        vector<int> deg(n+1, 0);
        for(int i = 0; i < n - 1; i++) {
            int u, v;cin>>u>>v;
            g[u].push_back(v);
            g[v].push_back(u);
            deg[u]++;
            deg[v]++;
        }
        vector<int> sekai;
        for(int i = 1; i <= n; i++) {
            if(deg[i] == 1) sekai.push_back(i);
        }
        vector<int> dist(n+1, -1);
        queue<int> q;
        for(int u : sekai) {
            dist[u] = 0;
            q.push(u);
        }
        while(!q.empty()) {
            int u = q.front(); q.pop();
            for(int v : g[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }
        int ma = 0;
        for(int i = 1; i <= n; i++) {
            if(deg[i] > 1) ma = max(ma, dist[i]);
        }
        vector<int> miku;
        for(int i = 1; i <= n; i++) {
            if(deg[i] > 1 && dist[i] == ma) miku.push_back(i);
        }
        cout<<miku.size()<<"\n";
        for(int i = 0 ; i < miku.size(); i++) {
            if(i) cout<<" ";
            cout<<miku[i];
        }
        cout<<"\n";
    }
    return 0;
}
//E
#include <bits/stdc++.h>
using namespace std;

const int N = 2e5 + 5;
vector<int> d[N];  // 存每个数的因数
int f[N], g[N];     // dp数组

template<class T1, class T2>
bool cmax(T1 &x, const T2 &y) {
    if (x < y) { x = y; return true; }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout << fixed << setprecision(15);

    // 预处理所有数的因数
    for (int i = 1; i < N; i++) {
        for (int j = i; j < N; j += i) {
            d[j].push_back(i);
        }
    }

    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int i = 0; i < n; i++) cin >> a[i];

        int ans = 0;
        for (int x : a) {
            int cur = g[x]; // 当前以x结尾的最大长度
            for (int y : d[x]) cmax(cur, f[y]); // 找能接上的最长序列
            cur++;
            cmax(f[x], cur);
            for (int y : d[x]) cmax(g[y], cur); // 更新因数对未来的贡献
            cmax(ans, cur);
        }

        cout << ans << "\n";

        // 清理状态，避免影响下一组数据
        for (int x : a) {
            f[x] = 0;
            for (int y : d[x]) g[y] = 0;
        }
    }

    return 0;
}
//F
//这回只花了114514min就打完了。
//真好。记得多手造几组。ACM拍什么拍。 
#include "bits/stdc++.h"
using namespace std;
using ui = unsigned; using db = long double; using ll = long long; using ull = unsigned long long;
template<class T1, class T2> istream &operator>>(istream &cin, pair<T1, T2> &a) { return cin >> a.first >> a.second; }
template <std::size_t Index = 0, typename... Ts> typename std::enable_if<Index == sizeof...(Ts), void>::type tuple_read(std::istream &is, std::tuple<Ts...> &t) { }
template <std::size_t Index = 0, typename... Ts> typename std::enable_if < Index < sizeof...(Ts), void>::type tuple_read(std::istream &is, std::tuple<Ts...> &t) { is >> std::get<Index>(t); tuple_read<Index + 1>(is, t); }
template <typename... Ts>std::istream &operator>>(std::istream &is, std::tuple<Ts...> &t) { tuple_read(is, t); return is; }
template<class T1> istream &operator>>(istream &cin, valarray<T1> &a);
template<class T1> istream &operator>>(istream &cin, vector<T1> &a) { for (auto &x : a) cin >> x; return cin; }
template<class T1> istream &operator>>(istream &cin, valarray<T1> &a) { for (auto &x : a) cin >> x; return cin; }
template<class T1, class T2> bool cmin(T1 &x, const T2 &y) { if (y < x) { x = y; return 1; } return 0; }
template<class T1, class T2> bool cmax(T1 &x, const T2 &y) { if (x < y) { x = y; return 1; } return 0; }
template<class T1> vector<T1> range(T1 l, T1 r, T1 step = 1) { assert(step > 0); int n = (r - l + step - 1) / step, i; vector<T1> res(n); for (i = 0; i < n; i++) res[i] = l + step * i; return res; }
template<class T1> basic_string<T1> operator*(const basic_string<T1> &s, int m) { auto r = s; m *= s.size(); r.resize(m); for (int i = s.size(); i < m; i++) r[i] = r[i - s.size()]; return r; }
using lll = __int128;
istream &operator>>(istream &cin, lll &x) { bool flg = 0; x = 0; static string s; cin >> s; if (s[0] == '-') flg = 1, s = s.substr(1); for (char c : s) x = x * 10 + (c - '0'); if (flg) x = -x; return cin; }
ostream &operator<<(ostream &cout, lll x) { static char s[60]; if (x < 0) cout << '-', x = -x; int tp = 1; s[0] = '0' + (x % 10); while (x /= 10) s[tp++] = '0' + (x % 10); while (tp--) cout << s[tp]; return cout; }
#if !defined(ONLINE_JUDGE)&&defined(LOCAL)
#include "my_header/debug.h"
#else
#define dbg(...) ;
#endif
template<class T1, class T2> ostream &operator<<(ostream &cout, const pair<T1, T2> &a) { return cout << a.first << ' ' << a.second; }
template<class T1, class T2> ostream &operator<<(ostream &cout, const vector<pair<T1, T2>> &a) { for (auto &x : a) cout << x << '\n'; return cout; }
template<class T1> ostream &operator<<(ostream &cout, vector<T1> a) { int n = a.size(); if (!n) return cout; cout << a[0]; for (int i = 1; i < n; i++) cout << ' ' << a[i]; return cout; }
template<class T1> ostream &operator<<(ostream &cout, const valarray<T1> &a) { int n = a.size(); if (!n) return cout; cout << a[0]; for (int i = 1; i < n; i++) cout << ' ' << a[i]; return cout; }
template<class T1> ostream &operator<<(ostream &cout, const vector<valarray<T1>> &a) { int n = a.size(); if (!n) return cout; cout << a[0]; for (int i = 1; i < n; i++) cout << '\n' << a[i]; return cout; }
template<class T1> ostream &operator<<(ostream &cout, const vector<vector<T1>> &a) { int n = a.size(); if (!n) return cout; cout << a[0]; for (int i = 1; i < n; i++) cout << '\n' << a[i]; return cout; }
#define all(x) (x).begin(),(x).end()
#define print(...) cout<<format(__VA_ARGS__)
#define println(...) cout<<format(__VA_ARGS__)<<'\n'
#define err(...) cerr<<format(__VA_ARGS__)
#define errln(...) cerr<<format(__VA_ARGS__)<<'\n'
ll f(ll x) { return x * (x + 1) >> 1; }
int main()
{
	ios::sync_with_stdio(0); cin.tie(0);
	cout << fixed << setprecision(15);
	int T; cin >> T;
	while (T--)
	{
		int n, m, i, j;
		cin >> n >> m;
		ll ans = 0;
		priority_queue<tuple<ll, ll, int, int>> q;
		{
			string s;
			cin >> s;
			for (i = j = 0; i < n; i = j)
			{
				while (s[i] == s[j]) ++j;
				if (s[i] == 'o')
				{
					int len = j - i;
					q.push({f(len) - f(len >> 1) - f(len - 1 >> 1), f(len >> 1) + f(len - 1 >> 1), len, 1});
					ans += f(len);
				}
			}
		}
		while (q.size() && m--)
		{
			auto [d, cur, n, k] = q.top(); q.pop();
			ans -= d; ++k;
			if (k > n) continue;
			n -= k;
			int sh = n / (k + 1), lar = sh + 1, nl = n % (k + 1), ns = k + 1 - nl;
			ll now = f(sh) * ns + f(lar) * nl;
			q.push({cur - now, now, n + k, k});
		}
		cout << ans << '\n';
	}
}
