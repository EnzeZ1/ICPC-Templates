int n_;
int h[N], e[M], ne[M], idx;
ll w_[M];

int first_[N], euler_[M], dep[M], dep_[N], par[N];
ll dist[N];

int st_[21][M], lg2_[M], len;

inline void init(int n){
    n_ = n;
    idx = 0;
    for(int i = 1; i <= n_; i++){
        h[i] = -1;
    }
}

inline void add(int u, int v, ll w){
    e[idx] = v, w_[idx] = w, ne[idx] = h[u], h[u] = idx++;
    e[idx] = u, w_[idx] = w, ne[idx] = h[v], h[v] = idx++;
}

inline void build_euler(int rt){
    len = 0;
    for(int i = 1; i <= n_; i++){
        first_[i] = -1;
        dep_[i] = dist[i] = par[i] = 0;
    }

    static int node[N], edge[N];

    int top = 0;
    node[top] = rt;
    edge[top] = h[rt];

    top++;
    first_[rt] = len;
    euler_[len] = rt;
    dep[len] = 0;
    len++;

    while(top){
        int u = node[top - 1];
        int &ei = edge[top - 1];

        if(ei == -1){
            top--;
            if(top){
                int p = node[top - 1];
                euler_[len] = p;
                dep[len] = dep_[p];
                len++;
            }
            continue;
        }

        int ed = ei;
        ei = ne[ei];
        int v = e[ed];

        if(v == par[u]){
            continue;
        }

        par[v] = u;
        dep_[v] = dep_[u] + 1;
        dist[v] = dist[u] + w_[ed];
        node[top] = v;
        edge[top] = h[v];
        top++;
        first_[v] = len;
        euler_[len] = v;
        dep[len] = dep_[v];
        len++;
    }
}

inline void build_rmq(){
    lg2_[1] = 0;
    for(int i = 2; i <= len; i++){
        lg2_[i] = lg2_[i >> 1] + 1;
    }

    for(int i = 0; i < len; i++){
        st_[0][i] = i;
    }

    for(int k = 1; (1 << k) <= len; k++){
        int seg = 1 << k;
        int half = seg >> 1;
        for(int i = 0; i + seg <= len; i++){
            int x = st_[k - 1][i];
            int y = st_[k - 1][i + half];
            if(dep[x] < dep[y]){
                st_[k][i] = x;
            }
            else{
                st_[k][i] = y;
            }
        }
    }
}

inline int lca(int u, int v){
    int L = first_[u];
    int R = first_[v];
    if(L > R){
        swap(L, R);
    }

    int k = lg2_[R - L + 1];
    int x = st_[k][L];
    int y = st_[k][R - (1 << k) + 1];
    int idx;

    if(dep[x] < dep[y]){
        idx = x;
    }
    else{
        idx = y;
    }
    return euler_[idx];
}

inline ll get_dist(int u, int v){
    int a = lca(u, v);
    return dist[u] + dist[v] - 2ll * dist[a];
}
