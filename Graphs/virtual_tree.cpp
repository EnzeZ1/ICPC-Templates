nt h[N], e[M], ne[M], w[M], idx;    
int dep[N], sz[N], dfn[N], cnt;
int fa[N][21], dp[N][21];
int s[N], a[N], k, top;

vector<int> adj[N];

void add(int u, int v, int c){
    e[idx] = v, w[idx] = c, ne[idx] = h[u], h[u] = idx++;
}

void dfs(int u, int pa){
    dep[u] = dep[pa] + 1;
    dfn[u] = ++cnt;
    fa[u][0] = pa;
    sz[u] = 1;

    for(int i = 0; i < 20; i++){
        fa[u][i + 1] = fa[fa[u][i]][i];
        dp[u][i + 1] = chmin(dp[u][i], dp[fa[u][i]][i]);
    }

    for(int i = h[u]; i != -1; i = ne[i]){
        int v = e[i];
        if(v == pa){
            continue;
        }

        dp[v][0] = w[i];
        dfs(v, u);
    }
}

int lca(int u, int v){
    if(dep[u] < dep[v]){
        swap(u, v);
    }
 
    for(int i = 20; i >= 0; i--){
        if(dep[fa[u][i]] >= dep[v]){
            u = fa[u][i];
        }
    }
    if(u == v) return u;
 
    for(int i = 20; i >= 0; i--){
        if (fa[u][i] != fa[v][i]){
            u = fa[u][i];
            v = fa[v][i];
        }
    }
    return fa[u][0];
}

int cmp(int& a, int& b){
    return dfn[a] < dfn[b];
}

vector<int> vt;

void build(){
    sort(a, a + k, cmp);
    top = 0;

    s[++top] = 1;
    vt.push_back(1);

    if(a[0] != 1){
        s[++top] = a[0];
        vt.push_back(a[0]);
    }

    for(int i = 1; i < k; i++){
        int anc = lca(a[i], s[top]);
        while(top > 1 && dep[s[top - 1]] >= dep[anc]){
            if(s[top - 1] != s[top]){
                adj[s[top - 1]].push_back(s[top]);
            }
            top--;
        }

        if(anc != s[top]){
            adj[anc].push_back(s[top]);
            s[top] = anc;   
            vt.push_back(anc);
        }

        s[++top] = a[i];
        vt.push_back(a[i]);
    }

    while(top > 1){
        if(s[top - 1] != s[top]){
            adj[s[top - 1]].push_back(s[top]);
        }
        top--;
    }
} //vt use to clean adj