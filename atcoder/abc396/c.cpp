#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    if (!(cin >> N >> M)) return 0;
    vector<ll> a(N), b(M);
    for (int i = 0; i < N; ++i) cin >> a[i];
    for (int j = 0; j < M; ++j) cin >> b[j];

    // 降序排序
    sort(a.begin(), a.end(), greater<ll>());
    sort(b.begin(), b.end(), greater<ll>());

    // 前缀和（prefix[0] = 0）
    vector<ll> prefixA(N+1, 0), prefixB(M+1, 0);
    for (int i = 1; i <= N; ++i) prefixA[i] = prefixA[i-1] + a[i-1];
    for (int j = 1; j <= M; ++j) prefixB[j] = prefixB[j-1] + b[j-1];

    // bestB[j] = max(prefixB[0..j])
    vector<ll> bestB(M+1, LLONG_MIN);
    bestB[0] = 0;
    for (int j = 1; j <= M; ++j) bestB[j] = max(bestB[j-1], prefixB[j]);

    ll ans = 0; // 可以选择不选任何球
    for (int x = 0; x <= N; ++x) {
        int limit = min(x, M);
        ll candidate = prefixA[x] + bestB[limit];
        ans = max(ans, candidate);
    }

    cout << ans << "\n";
    return 0;
}
