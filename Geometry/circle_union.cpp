static const ld EPS = 1e-12;
static const ld PI = acos(-1.0);

struct Pt {
    ld x, y;
    Pt() {}
    Pt(ld _x,ld _y):x(_x),y(_y){}
    Pt operator+(const Pt& o) const { return Pt(x+o.x, y+o.y); }
    Pt operator-(const Pt& o) const { return Pt(x-o.x, y-o.y); }
    Pt operator*(ld k)   const { return Pt(x*k,   y*k);   }
};
static inline ld dot(Pt a, Pt b){ return a.x*b.x + a.y*b.y; }
static inline ld cross(Pt a, Pt b){ return a.x*b.y - a.y*b.x; }
static inline ld norm2(Pt a){ return dot(a,a); }
static inline ld dist(Pt a, Pt b){ return hypot(a.x-b.x, a.y-b.y); }

struct Circle {
    Pt c;
    ld r;
};

static inline ld normAng(ld a){
    // Normalize to (-PI, PI]
    while (a <= -PI) a += 2*PI;
    while (a >   PI) a -= 2*PI;
    return a;
}

static inline Pt polar(ld r, ld ang){
    return Pt(r*cos(ang), r*sin(ang));
}

static inline ld arcArea_contrib(const Circle& ci, ld a, ld b){
    Pt A = ci.c + polar(ci.r, a);
    Pt B = ci.c + polar(ci.r, b);
    ld dth = b - a;
    // 正确：弧段的有向面积 = 0.5 * ( cross(A,B) + r^2 * (dth - sin(dth)) )
    return 0.5 * ( cross(A, B) + ci.r * ci.r * (dth - sin(dth)) );
}

static inline void addInterval(ld a, ld b, vector<pair<ld,int>>& ev){
    a = normAng(a); b = normAng(b);
    if (a <= b){
        ev.emplace_back(a, +1);
        ev.emplace_back(b, -1);
    }else{
        ev.emplace_back(a, +1); ev.emplace_back( PI, -1);
        ev.emplace_back(-PI, +1); ev.emplace_back(b, -1);
    }
}

ld circle_union_area(const vector<tuple<ld,ld,ld>>& input){
    int n = (int)input.size();
    vector<Circle> C(n);
    for (int i=0;i<n;i++){
        auto [x,y,r] = input[i];
        C[i] = {Pt(x,y), r};
    }

    {
        vector<Circle> tmp;
        tmp.reserve(n);
        for (auto &ci: C) if (ci.r > EPS) tmp.push_back(ci);
        C.swap(tmp); n = (int)C.size();
    }

    ld area = 0.0;

    for (int i=0;i<n;i++){
        const Circle &ci = C[i];
        vector<pair<ld,int>> ev;
        int cover_all = 0;

        for (int j=0;j<n;j++) if (i!=j){
            const Circle &cj = C[j];
            ld d = dist(ci.c, cj.c);

            bool same = (d <= EPS) && (fabs(ci.r - cj.r) <= EPS);
            if (same){
                if (j < i) { cover_all++; }
                continue;
            }

            if (d + ci.r <= cj.r + EPS){
                cover_all++;
                continue;
            }

            if (d >= ci.r + cj.r - EPS) continue;

            ld base = atan2(cj.c.y - ci.c.y, cj.c.x - ci.c.x);
            ld cosang = (ci.r*ci.r + d*d - cj.r*cj.r) / (2.0*ci.r*d);
            cosang = max(-1.0L, min(1.0L, cosang));
            ld ang = acos(cosang);

            ld a1 = base - ang;
            ld a2 = base + ang;
            addInterval(a1, a2, ev);
        }

        if (cover_all > 0 && ev.empty()){
            continue;
        }

        sort(ev.begin(), ev.end(), [](const auto& A, const auto& B){
            if (fabs(A.first - B.first) > 1e-15) return A.first < B.first;
            return A.second > B.second;
        });

        ld prev = -PI;
        int cnt = cover_all;
        for (auto [ang, typ] : ev){
            ld cur = ang;
            if (cnt == 0){
                area += arcArea_contrib(ci, prev, cur);
            }
            cnt += typ;
            prev = cur;
        }
        if (cnt == 0){
            area += arcArea_contrib(ci, prev, PI);
        }
    }
    return area;
}
