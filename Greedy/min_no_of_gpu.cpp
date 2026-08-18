#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Task
{
    int start;
    int finish;
};

bool cmp(Task a, Task b)
{
    return a.start < b.start;
}

int getNumOfMinimumGPU(vector<Task> &tasks)
{

    sort(tasks.begin(), tasks.end(), cmp);

    priority_queue<int, vector<int>, greater<int>> pq;

    int maxGPU = 0;

    for (Task task : tasks)
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
    cin >> n;

    vector<Task> tasks(n);

    for (int i = 0; i < n; i++)
    {
        cin >> tasks[i].start >> tasks[i].finish;
    }

    cout << getNumOfMinimumGPU(tasks) << '\n';

    return 0;
}