#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;
const int MAXN = 300005;

long long fac[MAXN], invfac[MAXN];

// 快速幂
long long qpow(long long a, long long b) {
    long long res = 1;
    while(b) {
        if(b & 1) res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}

// 预处理阶乘和逆元阶乘
void init(int n) {
    fac[0] = 1;
    for(int i=1; i<=n; i++) fac[i] = fac[i-1]*i%MOD;
    invfac[n] = qpow(fac[n], MOD-2);
    for(int i=n-1; i>=0; i--) invfac[i] = invfac[i+1]*(i+1)%MOD;
}

// 组合数
long long C(int n, int k) {
    if(k<0 || k>n) return 0;
    return fac[n]*invfac[k]%MOD*invfac[n-k]%MOD;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    int m = n/3;
    vector<int> w(n);
    for(int i=0;i<n;i++) cin >> w[i];

    init(m); // 预处理组合数到 m

    long long ways = 1;
    long long W = 0;

    for(int i=0;i<m;i++) {
        int a = w[i*3], b = w[i*3+1], c = w[i*3+2];
        int sum1 = a+b, sum2 = a+c, sum3 = b+c;
        int maxv = max({sum1,sum2,sum3});
        int k = 0;
        if(sum1==maxv) k++;
        if(sum2==maxv) k++;
        if(sum3==maxv) k++;
        W += maxv;
        ways = ways * k % MOD;
    }

    // 乘上全局组合数 C(m, m/2)
    ways = ways * C(m, m/2) % MOD;

    cout << ways << "\n";
    return 0;
}
