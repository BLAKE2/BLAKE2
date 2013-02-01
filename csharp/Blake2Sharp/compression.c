#define ROT(x, y)\
    ((x >> y)|(x << (64-y)))

#define G(r,i,a,b,c,d) \
    YY G(r,i,a,b,c,d) XXX\
    a = a + b + m[Sigma[16*r+2*i+0]]; XXX\
    d ^= a; XXX\
    d = ROT(d, 32); XXX\
    c = c + d; XXX\
    b ^= c; XXX\
    b = ROT(b, 24); XXX\
    a = a + b + m[Sigma[16*r+2*i+1]]; XXX\
    d ^= a; XXX\
    d = ROT(d, 16); XXX\
    c = c + d; XXX\
    b ^= c; XXX\
    b = ROT(b, 63); XXX

G( r, 0, v0, v4, v8, v12 )
G( r, 1, v1, v5, v9, v13 )
G( r, 2, v2, v6, v10, v14 )
G( r, 3, v3, v7, v11, v15 )
G( r, 4, v0, v5, v10, v15 )
G( r, 5, v1, v6, v11, v12 )
G( r, 6, v2, v7, v8, v13 )
G( r, 7, v3, v4, v9, v14 )