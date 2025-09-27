//https://atcoder.jp/contests/abc390
//A
#include <bits/stdc++.h>
using namespace std;

bool is_sorted_ascending(const vector<int>& A) {
    for (int i = 0; i < 4; i++) {
        if (A[i] > A[i + 1]) return false;
    }
    return true;
}

int main() {
    vector<int> A(5);
    for (int i = 0; i < 5; i++) cin >> A[i];

    for (int i = 0; i < 4; i++) {
        swap(A[i], A[i + 1]);
        if (is_sorted_ascending(A)) {
            cout << "Yes" << endl;
            return 0;
        }
        swap(A[i], A[i + 1]); // 恢复原数组
    }

    cout << "No" << endl;
    return 0;
}
//B
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstring> // 我的码风谔谔
using namespace std;

int a[105]; // 开大

int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a % b); // 辗转相除（小学奥数谔谔）
}

int main() {
    int n, p, q, g;
    cin >> n;
    for (int i = 1; i <= n; i ++) {
        cin >> a[i];
    }
    if (n == 2) { // 在 N = 2 时恒成立
        cout << "Yes";
        return 0;
    }
    g = gcd(a[1], a[2]);
    p = a[2] / g;
    q = a[1] / g; // 算 d
    for (int i = 2; i < n; i ++) { // 枚举，无需重复
        if (a[i + 1] / p != a[i] / q || a[i + 1] / p * p != a[i + 1] || a[i] / q * q != a[i]) { // 后面两个表达式防止整型除法四舍五入的误差
            cout << "No";
            return 0;
        }
    }
    cout << "Yes"; // 过关斩将的等比数列 lol
    return 0;
}
//C
#include <bits/stdc++.h>
using namespace std;
int main() {
    int h,w;cin>>h>>w;
    vector<string> s(h);
    for(int i = 0; i < h; i++) cin>>s[i];
    int mirow = h, marow = -1, micol = w, macol = -1;
    for(int i = 0; i < h; i++) {
        for(int j = 0; j < w; j++) {
            if(s[i][j] == '#') {
                mirow = min(mirow, i);
                marow = max(marow, i);
                micol = min(micol, j);
                macol = max(macol, j);
            }
        }
    }
    for(int i = mirow; i <= marow; i++) {
        for(int j = micol; j <= macol; j++) {
            if(s[i][j] == '.') {
                cout<<"No\n"; return 0;
            }
        }
    }
    for(int i = 0; i < h; i++) {
        for(int j = 0; j < w; j++) {
            if(s[i][j] == '#') {
                if(i < mirow || i > marow || j < micol || j > macol) {
                    cout<<"No\n"; return 0;
                }
            }
        }
    }
    cout<<"Yes\n"; return 0;
}
//D
#include<bits/stdc++.h>

using namespace std;

typedef long long i64;

int n;
vector<i64> a, val;
unordered_set<i64> s;

void dfs(int cur, int k, i64 sum) {
    for(int i = 0; i <= k; i++) {
        i64 nxt = sum ^ val[i] ^ (val[i] + a[cur]);
        val[i] += a[cur];
        if(cur == n - 1) {
            s.insert(nxt);
        } else if(i == k) {
            dfs(cur + 1, k + 1, nxt);
        } else {
            dfs(cur + 1, k, nxt);
        }
        val[i] -= a[cur];
    }
}

int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    
    cin >> n;
    a.resize(n), val = a;
    for(i64 &x: a) {
        cin >> x;
    }

    dfs(0, 0, 0);

    cout << s.size() << '\n';

    return 0;
}
//E
