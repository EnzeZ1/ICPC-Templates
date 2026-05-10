struct P{
    ld x, y;
};
struct C{
    P c; ld r; bool valid;
};

const ld EPS = 1e-12L;

inline ld dist2(const P& a, const P& b){
    ld dx = a.x - b.x, dy = a.y - b.y;
    return dx * dx + dy * dy;
}

inline ld dist(const P& a, const P& b){
    return sqrtl(dist2(a, b));
}

inline bool in_circle(const C& cir, const P& p){
    if(!cir.valid){
        return false;
    }
    return dist2(cir.c, p) <= (cir.r + EPS) * (cir.r + EPS);
}

inline C circle1(const P& a){
    return C{a, 0.0L, true};
}
inline C circle2(const P& a, const P& b){
    P o{(a.x + b.x) / 2.0L, (a.y + b.y) / 2.0L};
    return C{o, dist(a, b) / 2.0L, true};
}

// 严格三点外接圆（共线返回 invalid）
inline C circle3_strict(const P& A, const P& B, const P& Cc){
    ld a2 = A.x * A.x + A.y * A.y;
    ld b2 = B.x * B.x + B.y * B.y;
    ld c2 = Cc.x * Cc.x + Cc.y * Cc.y;
    ld D  = 2.0L * (A.x * (B.y - Cc.y) + B.x * (Cc.y - A.y) + Cc.x * (A.y - B.y));

    if(fabsl(D) < EPS){
        return C{{0,0}, 0.0L, false};
    }

    ld ux = (a2 * (B.y - Cc.y) + b2 * (Cc.y - A.y) + c2 * (A.y - B.y)) / D;
    ld uy = (a2 * (Cc.x - B.x) + b2 * (A.x - Cc.x) + c2 * (B.x - A.x)) / D;
    P o{ux, uy};
    return C{o, dist(o, A), true};
}

// 三点最小覆盖圆（共线回退为两点圆）
inline C circle3(const P& a, const P& b, const P& c){
    C t = circle3_strict(a, b, c);
    if(t.valid) return t;

    C ab = circle2(a, b);
    C ac = circle2(a, c);
    C bc = circle2(b, c);

    C best; best.valid = false;
    auto upd = [&](const C& z){
        if(!z.valid){
            return;
        }
        if(in_circle(z, a) && in_circle(z, b) && in_circle(z, c)){
            if(!best.valid || z.r < best.r){
                best = z;
            }
        }
    };
    upd(ab); upd(ac); upd(bc);
    return best;  
}

inline C mec(vector<P> pts){
    if(pts.empty()) return C{{0,0}, 0.0L, true};
    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

    shuffle(pts.begin(), pts.end(), rng);

    C cir; cir.valid = false;
    for(size_t i = 0; i < pts.size(); i++){
        if(cir.valid && in_circle(cir, pts[i])){
            continue;
        }
        cir = circle1(pts[i]);

        for(size_t j = 0; j < i; j++){
            if(in_circle(cir, pts[j])){
                continue;
            }
            cir = circle2(pts[i], pts[j]);

            for(size_t k = 0; k < j; k++){
                if(in_circle(cir, pts[k])){
                    continue;
                }
                cir = circle3(pts[i], pts[j], pts[k]);
            }
        }
    }
    return cir;
}