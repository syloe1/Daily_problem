//https://ac.nowcoder.com/acm/contest/69117
//A
int main() {
    int n,k;cin>>n>>k;
    string s;cin>>s;
    int i = 0;
    for(; i <k; i++) {
        s[i] = toupper(s[i]);
    }
    for(; i < n; i++) {
        s[i] = tolower(s[i]);
    }
    cout<<s<<'\n'; return 0;
}
//B
int main() {
    int a,b,x,y;cin>>a>>b>>x>>y;
    int c = INT_MAX;
    int ms = max((a+y-1)/2, (b+y-1)/y)+2;
    for(int i = 0; i <= ms; i++) {
        int tot = i;
        int ra = max(0, a - i * y);
        if(ra > 0) tot += (ra + x - 1) / x;
        int rb = max(0, b - i * y);
        if(rb > 0) tot += (rb + x - 1) / x;
        c = min(c, tot);
    }
    cout<<c<<'\n';
    return 0;
}
//C
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 100000;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> a(n);
    for (int &x : a) cin >> x;

    vector<int> f(MAXN + 1, INT_MAX);
    f[1] = 0;

    for (int i = 2; i <= MAXN; i++) {
        // 操作1: 分裂为 (1, i-1)
        f[i] = f[i - 1] + 1;

        // 操作2: 分裂为 (a, b)，a*b = i
        for (int j = 2; j * j <= i; j++) {
            if (i % j == 0) {
                int a1 = j, a2 = i / j;
                f[i] = min(f[i], f[a1] + f[a2] + 1);
            }
        }
    }

    long long ans = 0;
    for (int x : a) ans += f[x];

    cout << ans << "\n";
    return 0;
}
//D
