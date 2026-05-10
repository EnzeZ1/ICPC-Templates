struct HopcroftKarp{
    int l, r;
    vector<vector<int>> g;
    vector<int> pl, pr, d; //pl: l -> r matching

    HopcroftKarp(int l, int r)
        : l(l), r(r), g(l),
          pl(l, -1), pr(r, -1), d(l, -1) {}

    void add(int u, int v){
        g[u].push_back(v);
    }

    bool bfs(){
        queue<int> q;
        d.assign(l, -1);

        for(int u = 0; u < l; ++u){
            if(pl[u] == -1){ 
                d[u] = 0;
                q.push(u);
            }
        }

        bool found = false;
        while(!q.empty()){
            int u = q.front(); q.pop();
            for(int v : g[u]){
                int u2 = pr[v];    

                if(u2 == -1){
                    found = true;
                }
                else if (d[u2] == -1) {
                    d[u2] = d[u] + 1;
                    q.push(u2);
                }
            }
        }
        return found;
    }

    bool dfs(int u){
        for(int v : g[u]){
            int u2 = pr[v];

            if(u2 == -1 || (d[u2] == d[u] + 1 && dfs(u2))){
                pl[u] = v;
                pr[v] = u;
                return true;
            }
        }

        d[u] = -1;
        return false;
    }

    int HK(){
        int ans = 0;
        while(bfs()){
            for(int u = 0; u < l; ++u){
                if(pl[u] == -1 && dfs(u)){
                    ++ans;
                }
            }
        }
        return ans;
    }
};