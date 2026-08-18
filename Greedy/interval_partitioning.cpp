#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    cin >> n;
    vector<pair<int, int>> lectures(n);
    for (int i = 0; i < n; i++)
    {
        cin >> lectures[i].first >> lectures[i].second;
    }
    sort(lectures.begin(), lectures.end());

    priority_queue<int, vector<int>, greater<int>> rooms;

    for (auto lecture:lectures)
    {
        int start = lecture.first;
        int end = lecture.second;
        if (!rooms.empty() && rooms.top() <=start)
        {
            rooms.pop();
        }
        rooms.push(end);
    }

    cout << "Number of rooms needed" << rooms.size() << endl;
    return 0;
}