//https://ac.nowcoder.com/acm/contest/87
//A
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    // 元音（包含半元音 y），按字母表顺序
    vector<char> vowels = {'a','e','i','o','u','y'};
    // 预计算映射表：对于 'a'..'z'，映射到不大于它的最大元音
    char mapc[26];
    for (char c = 'a'; c <= 'z'; ++c) {
        char best = 'a';
        for (char v : vowels) {
            if (v <= c) best = v;
            else break;
        }
        mapc[c - 'a'] = best;
    }

    string s;
    while (getline(cin, s)) {
        // 如果可能存在空行并且需要输出空行：题目输入为只包含小写字母的字符串，通常不会有空行。
        // 这里我们仍然对空行直接输出空行以保持行为直观。
        if (s.empty()) {
            cout << "\n";
            continue;
        }
        for (char &ch : s) {
            if ('a' <= ch && ch <= 'z') cout << mapc[ch - 'a'];
            else cout << ch; // 若出现非小写字母则原样输出（鲁棒性）
        }
        cout << "\n";
    }
    return 0;
}
//B
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using ull = unsigned long long;
const ull mod = 2000120420010122ULL;
const string pat = "cwbc";
const int m = pat.size();
int main() {
    string s;
    while(getline(cin, s)) {
        vector<ull> dp(m+1, 0);
        dp[0] = 1ULL;
        for(char ch : s) {
            char c = tolower((unsigned char)(ch));
            for(int j = m - 1; j >= 0; j--) {
                if(c == pat[j]) {
                    ull sum = dp[j+1] + dp[j];
                    if(sum >= mod) sum -= mod;
                    dp[j+1] = sum;
                }
            }
        }
        cout<<(dp[m] % mod)<<'\n';
    }
    return 0;
}
//C
