#include <bits/stdc++.h>
using namespace std;

struct Point {
    long long x, y;
    int id;
};

bool comparePoints(const Point& a, const Point& b) {
    return a.x != b.x ? a.x < b.x : a.y < b.y;
}

bool isValid(const Point& p, const Point& q, const Point& r) {
    long long val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val > 0) return true;
    else return false;
}

double getYIntersection(const Point& pA, const Point& pB, double x_mid) {
    double slope = (double)(pB.y - pA.y) / (pB.x - pA.x);
    return pA.y + slope * (x_mid - pA.x);
}

void findUpperTangent(int &uA, int &uB, const vector<Point>& A, const vector<Point>& B, double x_mid) {
    int nB = B.size();
    int nA = A.size();
    bool changed = true;
    while (changed) {
        changed = false;
        double current_y = getYIntersection(A[uA], B[uB], x_mid);

        // Try rotating B Clockwise
        int nextB = (uB + 1) % nB;
        if (getYIntersection(A[uA], B[nextB], x_mid) > current_y) {
            uB = nextB;
            changed = true;
            continue;
        }

        // Try rotating A Counter-Clockwise
        int prevA = (uA - 1 + nA) % nA;
        if (getYIntersection(A[prevA], B[uB], x_mid) > current_y) {
            uA = prevA;
            changed = true;
            continue;
        }
    }
}

void findLowerTangent(int &lowA, int &lowB, const vector<Point>& A, const vector<Point>& B, double x_mid) {
    int nB = B.size();
    int nA = A.size();
    bool changed = true;
    while (changed) {
        changed = false;
        double current_y = getYIntersection(A[lowA], B[lowB], x_mid);

        // Try rotating B Counter-Clockwise
        int prevB = (lowB - 1 + nB) % nB;
        if (getYIntersection(A[lowA], B[prevB], x_mid) < current_y) {
            lowB = prevB;
            changed = true;
            continue;
        }

        // Try rotating A Clockwise
        int nextA = (lowA + 1) % nA;
        if (getYIntersection(A[nextA], B[lowB], x_mid) < current_y) {
            lowA = nextA;
            changed = true;
            continue;
        }
    }
}

vector<Point> combine(int uA, int uB, int lowA, int lowB, const vector<Point>& A, const vector<Point>& B) {
    vector<Point> res;
    int nB = B.size();
    int nA = A.size();
    for (int i = uB; ; i = (i + 1) % nB) {
        res.push_back(B[i]);
        if (i == lowB) break;
    }
    for (int i = lowA; ; i = (i + 1) % nA) {
        res.push_back(A[i]);
        if (i == uA) break;
    }
    return res;
}

vector<Point> bruteForce(vector<Point>& pts) {
    int n = pts.size();

    vector<int> next_node(n, -1);
    int start_idx = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            bool ok = true;
            for (int k = 0; k < n; k++) {
                if (k == i || k == j) continue;
                if (!isValid(pts[i], pts[j], pts[k])) { ok = false; break; }
            }
            if (ok) {
                next_node[i] = j;
                start_idx = i;
            }
        }
    }

    vector<Point> h; // h contains the sequence of points which make the polygon
    int curr = start_idx;
    do {
        h.push_back(pts[curr]);
        curr = next_node[curr];
    } while (curr != start_idx && curr != -1 && (int)h.size() < n);

    return h;
}

// Two-Finger Merge
vector<Point> mergeHulls(const vector<Point>& A, const vector<Point>& B) {
    int rA = max_element(A.begin(), A.end(), comparePoints) - A.begin();
    int lB = min_element(B.begin(), B.end(), comparePoints) - B.begin();

    // Define the vertical dividing line between the two hulls
    double x_mid = (A[rA].x + B[lB].x) / 2.0;

    // Find Upper Tangent (Maximize Y-intersection)
    int uA = rA, uB = lB;
    findUpperTangent(uA, uB, A, B, x_mid);

    // Find Lower Tangent (Minimize Y-intersection)
    int lowA = rA, lowB = lB;
    findLowerTangent(lowA, lowB, A, B, x_mid);

    // Cut & Paste Output Construction (Clockwise traversal)
    vector<Point> res;
    res = combine(uA, uB, lowA, lowB, A, B);
    return res;
}

vector<Point> divideAndConquer(vector<Point>& pts) {
    int n = pts.size();
    if (n <= 5) return bruteForce(pts);

    int mid = n / 2;
    vector<Point> left(pts.begin(), pts.begin() + mid);
    vector<Point> right(pts.begin() + mid, pts.end());

    vector<Point> hullL = divideAndConquer(left);
    vector<Point> hullR = divideAndConquer(right);

    return mergeHulls(hullL, hullR);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    vector<Point> pts(n);

    for (int i = 0; i < n; i++) {
        cin >> pts[i].x >> pts[i].y;
        pts[i].id = i + 1;
    }

    sort(pts.begin(), pts.end(), comparePoints);

    vector<Point> hull = divideAndConquer(pts);

    int m = hull.size();
    int best = 0;
    
    for (int i = 1; i < m; i++)
        if (hull[i].id < hull[best].id) best = i;

    for (int i = 0; i < m; i++) {
        cout << hull[(best + i) % m].id;
        if (i + 1 < m) cout << ' ';
    }
    cout << '\n';
    return 0;
}
