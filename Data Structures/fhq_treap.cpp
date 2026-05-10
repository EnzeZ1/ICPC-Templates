
struct FHQTreap {
    struct Node { 
        int ls, rs, key, si; 
        ll v; 
    };
    vector<Node> t;

    int root = 0, idx = 0;
    mt19937 rng{
        (uint32_t)chrono::steady_clock::now().time_since_epoch().count()
    };

    Splay(int cap = 0){ 
        if(cap){
            init(cap); 
        }
    }

    inline void init(int cap){
        t.assign(cap + 5, {}); 
        root = 0; 
        idx = 0; 
    }

    inline int new_node(ll v){ 
        int x = ++idx; 
        t[x].ls = t[x].rs = 0; 
        t[x].key = (int)rng(); 
        t[x].si = 1; 
        t[x].v = v; 
        return x; 
    }

    inline void upd(int x){ 
        t[x].si = 1 + (t[x].ls ? t[t[x].ls].si : 0) + (t[x].rs ? t[t[x].rs].si : 0); 
    }
    
    void split(int rt, int &x, int &y, ll val){
        if(!rt){ 
            x = y = 0; 
            return; 
        }
        if(t[rt].v <= val){ 
            x = rt; 
            split(t[rt].rs, t[rt].rs, y, val); 
            upd(rt); 
        }
        else{ 
            y = rt; 
            split(t[rt].ls, x, t[rt].ls, val); 
            upd(rt); 
        }
    }
    
    int merge(int x, int y){
        if(!x || !y) return x ? x : y;
        if(t[x].key > t[y].key){ 
            t[x].rs = merge(t[x].rs, y); 
            upd(x); 
            return x; 
        }
        else{ 
            t[y].ls = merge(x, t[y].ls); 
            upd(y); 
            return y; 
        }
    }
    
    inline void insert(ll v){
        int x, y; 
        split(root, x, y, v);
        int z = new_node(v);
        root = merge(merge(x, z), y);
    }
    
    inline ll count_le(ll T){
        int x, y; 
        split(root, x, y, T);
        ll res = x ? t[x].si : 0;
        root = merge(x, y);
        return res;
    }
    
    inline ll count_ge(ll T){
        int x, y; 
        split(root, x, y, T - 1);
        ll res = y ? t[y].si : 0;
        root = merge(x, y);
        return res;
    }

    inline int _min_node(int r){
        while(t[r].ls){
            r = t[r].ls;
        }
        return r;
    }

    inline int _max_node(int r){
        while(t[r].rs){
            r = t[r].rs;
        }
        return r;
    }

    inline bool next(ll u, ll &res){
        int x, y;
        split(root, x, y, u - 1);

        bool ok = false;
        if(y){
            int z = _min_node(y);
            res = t[z].v;
            ok = true;
        }
        root = merge(x, y);
        return ok;
    }

    inline bool prev(ll u, ll &res){
        int x, y;
        split(root, x, y, u);
        bool ok = false;

        if(x){
            int z = _max_node(x);
            res = t[z].v;
            ok = true;
        }
        root = merge(x, y);
        return ok;
    }

    inline bool remove(ll v){
        int x, y, a, b;

        split(root, x, y, v);
        split(x, a, b, v - 1);
        bool ok = false;

        if(b){
            int r = b;
            b = merge(t[r].ls, t[r].rs);
            ok = true;
        }
        root = merge(merge(a, b), y);
        return ok;
    }
        
    inline void clear(){ 
        root = 0; 
        idx = 0; 
    }
};