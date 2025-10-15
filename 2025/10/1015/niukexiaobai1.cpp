//https://ac.nowcoder.com/acm/contest/85?channelPut=w251acm
//A
int main() {
    const long double e = expl(1.0L);  // 高精度自然常数 e
    int T;
    cin >> T;
    while (T--) {
        long double alpha, beta;
        int gamma;
        cin >> alpha >> beta >> gamma;

        long double result = beta * powl(e, alpha);

        cout << fixed << setprecision(gamma) << result << "\n";
    }
    return 0;
}
//B
int main() {
    const long double e = expl(1.0L);
    int t;cin>>t;while(t--) {
        long double delta, zeta;
        int sigma;
        cin>>delta>>zeta>>sigma;
        long double xi = powl(delta, e) / zeta;
        cout<<fixed<<setprecision(sigma)<<xi<<"\n";
    }
    return 0;
}
//C
using i64 = long long;
using i128 = __int128_t;
i64 qpow(i64 a, i64 b, i64 mod) {
    i64 res = 1 % mod;
    a %= mod;
    while(b > 0) {
        if(b&1) res = (i128)res*a%mod;
        a = (i128)a*a%mod;
        b >>= 1;
    }
    return res; 
}
int main() {
    i64 s, t, w, r, mod;
    cin>>s>>t>>w>>r>>mod;
    i64 base = ((i128)s * t) % mod;
    i64 exp = w * r;
    i64 ans = qpow(base, exp, mod);
    cout<<ans%mod<<'\n';
    return 0;
}
//D
int main() {
    int n,m;cin>>n>>m;
    vector<i64> a(n+1), b(m+1), c(n+m+1,0);
    for(auto& e : a) cin>>e;
    for(auto& e : b) cin>>e;
    for(int i = 0; i <= n; i++) for(int j = 0; j <= m; j++) {
        c[i+j] += a[i] * b[j];
    }
    for(int k = 0; k <= n + m; k++) {
        if(k) cout<<" ";
        cout<<c[k];
    }cout<<'\n';
    return 0;
}
//E
int main() {
    double r;cin>>r;
    double ans = 1.0+r;
    cout<<fixed<<setprecision(2)<<ans<<'\n';
    return 0;
}
