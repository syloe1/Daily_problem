#include <bits/stdc++.h>
using namespace std;
using int64 = long long;

// 计算 1 ~ n 的所有数字和
long long digit_sum(long long n) {
    if (n <= 0) return 0;
    long long res = 0;
    long long base = 1;
    while (base <= n) {
        long long high = n / (base * 10);
        long long cur  = (n / base) % 10;
        long long low  = n % base;
        // 高位贡献
        res += high * 45 * base;
        // 当前位贡献
        res += cur * (low + 1);
        // 当前位以下完整轮次的和
        res += cur * (cur - 1) / 2 * base;
        base *= 10;
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        long long k;
        cin >> k;

        long long len = 1;       // 当前数字的位数
        long long count = 9;     // 该位数的数字个数
        long long totalDigits = len * count;

        long long start = 1;     // 当前段起始数字
        while (k > totalDigits) {
            k -= totalDigits;
            start += count;
            len++;
            count *= 10;
            totalDigits = len * count;
        }

        // k 落在这一段
        long long offset = (k - 1) / len;  // 段内第几个数字
        long long num = start + offset;    // 具体的数字
        long long pos = (k - 1) % len;     // 在这个数字的第几位

        // 前面完整数字的和
        long long ans = digit_sum(num - 1);

        // 再加上 num 的前 pos+1 个数字
        string s = to_string(num);
        for (int i = 0; i <= pos; i++) ans += s[i] - '0';

        cout << ans << "\n";
    }
    return 0;
}
