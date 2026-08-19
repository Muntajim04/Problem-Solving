#include <bits/stdc++.h>
using namespace std;
int main(){
    int T,X,B,A;
    cin >> T;
    for (int i = 0; i < T; i++)
    {
        cin >> A >> B >> X;
        int res = (B - A) / X;
        cout << res << endl;
    }

    return 0;
}