#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

struct Task
{
    int start;
    int finish;
};

bool cmp(const Task &a, const Task &b)
{
    return a.start < b.start;
}

int getNumOfMinimumGPU(vector<Task> &tasks)
{
    if (tasks.empty()) return 0;

    sort(tasks.begin(), tasks.end(), cmp);

    // Min-heap to store the finish times of active tasks
    priority_queue<int, vector<int>, greater<int>> pq;

    int maxGPU = 0;

    for (const Task &task : tasks)
    {
        while (!pq.empty() && pq.top() <= task.start)
        {
            pq.pop();
        }

        pq.push(task.finish);

        maxGPU = max(maxGPU, (int)pq.size());
    }

    return maxGPU;
}

int main()
{
    

    int n;
    if (!(cin >> n) || n <= 0)
    {
        cout << 0 << '\n';
        return 0;
    }

    vector<Task> tasks(n);
    for (int i = 0; i < n; i++)
    {
        cin >> tasks[i].start >> tasks[i].finish;
    }

    cout << getNumOfMinimumGPU(tasks) << '\n';

    return 0;
}