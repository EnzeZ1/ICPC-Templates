struct Point {
    int x, y;
    Point operator-(const Point& b) const {
        return {x - b.x, y - b.y};
    }
    int operator*(const Point& b) const {
        return x * b.y - y * b.x;
    }
};

int n;
vector<Point> q;
vector<int> stk;
vector<bool> used;
int top = 0;

int area(const Point& a, const Point& b, const Point& c) {
    return (b - a) * (c - a);
}

int get_dist(const Point& a, const Point& b) {
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    return dx * dx + dy * dy;
}

void get_convex() {
    sort(q.begin(), q.end(), [](const Point& a, const Point& b) {
        if (a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    });

    stk.clear();
    used.assign(n, false);
    top = 0;

    for (int i = 0; i < n; i++) {
        while (top >= 2 && area(q[stk[top - 2]], q[stk[top - 1]], q[i]) <= 0) {
            if (area(q[stk[top - 2]], q[stk[top - 1]], q[i]) < 0)
                used[stk[--top]] = false, stk.pop_back();
            else
                top--, stk.pop_back();
        }
        stk.push_back(i);
        top++;
        used[i] = true;
    }

    used[0] = false;
    int k = top;
    for (int i = n - 1; i >= 0; i--) {
        if (used[i]) continue;
        while (top >= k + 1 && area(q[stk[top - 2]], q[stk[top - 1]], q[i]) <= 0)
            top--, stk.pop_back();
        stk.push_back(i);
        top++;
    }
    top--;
}

tuple<int, int, int> rotating_calipers() {
    if (top <= 2) return {get_dist(q[0], q[n - 1]), 0, 1};

    int res = 0;
    vector<pair<int, int>> cand; 

    auto add = [&](int a, int b) {
        if (a > b) swap(a, b);
        cand.push_back({a, b});
        res = max(res, get_dist(q[stk[a]], q[stk[b]]));
    };

    for (int i = 0, j = 2; i < top; i++) {
        int ni = (i + 1) % top;
        auto d = q[stk[i]], e = q[stk[ni]];
        while (area(d, e, q[stk[j]]) < area(d, e, q[stk[(j + 1) % top]]))
            j = (j + 1) % top;

        add(i, j);
        add(ni, j);

        int nj = (j + 1) % top;
        if (area(d, e, q[stk[j]]) == area(d, e, q[stk[nj]])) {
            add(i, nj);
            add(ni, nj);
        }
    }

    sort(cand.begin(), cand.end());
    cand.erase(unique(cand.begin(), cand.end()), cand.end());

    int cnt_in = 0, cnt_edge = 0;
    for (auto [a, b] : cand) {
        if (get_dist(q[stk[a]], q[stk[b]]) != res) continue;
        bool is_edge = (b - a == 1) || (a == 0 && b == top - 1);
        if (is_edge) cnt_edge++;
        else cnt_in++;
    }

    return {res, cnt_in, cnt_edge};
}

const long double eps = 1e-12;

bool is_int(long double x) {
    return fabs(x - round(x)) < eps;
}
