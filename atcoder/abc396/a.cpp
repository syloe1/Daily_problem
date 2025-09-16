#include <bits/stdc++.h>
using namespace std; 
int main() {
    int n;cin>>n;
    vector<int> a(n);
    for(int i = 0; i < n; i++) {
        cin>>a[i];
    }
    for(int i = 0; i + 2 < n; i++) {
        if(a[i] == a[i+1] && a[i+1] == a[i+2]) {
            cout<<"Yes\n";return 0;
        }
    }
    cout<<"No\n";
    return 0;
}
/*
题目核心

判断数组中是否存在 连续三个相同的数。

注意⚠️：题目要求的是 连续，不是某个数出现了 3 次或以上。

解题思路步骤

读入数据

输入 
𝑛
n（数组长度），再输入数组 
𝐴
A。

遍历数组

从第 1 个元素（下标 0）一直遍历到第 
𝑛
−
3
n−3 个元素（下标 
𝑛
−
3
n−3）。

对每个位置 
𝑖
i，检查：

𝐴
[
𝑖
]
=
=
𝐴
[
𝑖
+
1
]
 
&
&
 
𝐴
[
𝑖
+
1
]
=
=
𝐴
[
𝑖
+
2
]
A[i]==A[i+1] && A[i+1]==A[i+2]

判断是否成立

如果找到满足条件的位置，直接输出 Yes 并结束程序。

如果遍历结束仍未找到，说明不存在 → 输出 No。
*/