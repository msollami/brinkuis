// Find all admissible triples from a list of admissible words of length n
#include <stdio.h>
#define N 100     // max length of word
#define MM 50
#define M (64*MM) // max number of admissible words
int n,m,pal[M],w[M][N],trial[3],i0,i1,i2;
unsigned compat[MM*(M-1)];
main()
{
        int i,j,k,t;
        char adm_str[20];
        char admA_str[20];
        FILE *adm;
        FILE *admA;

        scanf("%d",&n);
        if (n < 12)
        {fprintf(stderr,"Error: n=%d is smaller than 12\n",n);
         exit(1); }
        if (n >= N)
        {fprintf(stderr,"Error: n=%d exceeds N-1=%d\n",n,N-1);
         exit(1); }
        j=getchar()-'0';
        while (j<0 || j>9) j=getchar()-'0';
        m=0;
        while (j>=0 && j<3)
        {if (m >= M)
         {fprintf(stderr,"Error: exceeding %d admissible words\n",M);
     exit(1); }
         w[m][0]=j;
         for (k=1; k<n; k++)
         {j=getchar()-'0';
     while (j<0 || j>9) j=getchar()-'0';
     w[m][k]=j; }
         j=getchar()-'0';
         while (j<0 || j>9) j=getchar()-'0';
         m++; }
        printf("%d admissible words of length %d read in\n",m,n);
        for (i=0; i<m; i++)
        {j=(n+1)/2;
         while (j>5 && w[i][j]==w[i][n-1-j]) j--;
         pal[i]=(j<6); }

        for (i=0; 64*i<m*(m-1); i++) compat[i]=0;
        for (i0=0; i0<m-1; i0++) for (i1=i0+1; i1<m; i1++) if (chk2(i0,i1)) cert(i0,i1);
        // printf("admissible pairs:\n");
        //
        // for (i0=0; i0<m-1; i0++)
        // {j=0;
        //  for (i1=i0+1; i1<m; i1++) if (adj(i0,i1))
        //          {if (j == 0) printf("%3d:",i0);
        //      j=1;
        //      printf(" %d",i1); }
        //  if (j) printf("\n"); }

        t=0;
        sprintf(admA_str,"t2%d.txt",n);
        admA=fopen(admA_str,"w");
        printf("admissible triples:\n");
        for (i0=0; i0<m-2; i0++) for (i1=i0+1; i1<m-1; i1++) if (adj(i0,i1))
                                for (i2=i1+1; i2<m; i2++) if (adj(i0,i2) && adj(i1,i2)) if (chk3(i0,i1,i2))
                                                {t++; fprintf(admA,"%d %d %d\n",1+i0,1+i1,1+i2);
                                                 /* printf("%d %d %d\n",i0,i1,i2); */}
        printf("%d admissible triples found\n",t);
}

// Return the i-th entry of ((w[j0]+t0)%3)(r0)|((w[j1]+t1)%3)(r1)|((w[j2]+t2)%3)(r2)
// where 0<=i<2*n; 0<=j0,j1,j2<m; t=0,1,2; r0,r1=0,1 (1 means reverse).
// t=16*t2+4*t1+t0; r=4*r2+2*r1+r0
int tr(int i,int j0,int j1,int j2,int t,int r)
{
        if (i < n)
        {if (r&1) return (w[j0][n-1-i]+(t&3))%3;
         return (w[j0][i]+(t&3))%3; }
        if (i < 2*n)
        {if (r&2) return (w[j1][2*n-1-i]+((t&12)>>2))%3;
         return (w[j1][i-n]+((t&12)>>2))%3; }
        if (r&4) return (w[j2][3*n-1-i]+((t&48)>>4))%3;
        return (w[j2][i-2*n]+((t&48)>>4))%3;
}

// Assuming w[i][0..n-1] and w[j][0..n-1] are squarefree (0 <= i < j < m),
// check that w[i] and w[j] are compatible.
int chk2(int i,int j)
{
        int frst,lst,len,s,t0,t1,t2;
        for (frst=0; frst<n; frst++) for (lst=n+((n+1+frst)&1); lst<2*n; lst+=2)
                {len=(lst+1-frst)/2;
                 for (t1=1; t1<3; t1++)
                 {s=0;
     while (s<len && tr(frst+s,i,j,0,4*t1,0)==tr(frst+len+s,i,j,0,4*t1,0)) s++;   // i j
     if (s == len) return 0; }}
        if (!pal[i]) for (frst=0; frst<n; frst++) for (lst=n+((n+1+frst)&1); lst<2*n; lst+=2)
                        {len=(lst+1-frst)/2;
                         for (t1=1; t1<3; t1++)
                         {s=0;
     while (s<len && tr(frst+s,i,j,0,4*t1,1)==tr(frst+len+s,i,j,0,4*t1,1)) s++;   // ir j
     if (s == len) return 0; }}
        if (!pal[j]) for (frst=0; frst<n; frst++) for (lst=n+((n+1+frst)&1); lst<2*n; lst+=2)
                        {len=(lst+1-frst)/2;
                         for (t1=1; t1<3; t1++)
                         {s=0;
     while (s<len && tr(frst+s,i,j,0,4*t1,2)==tr(frst+len+s,i,j,0,4*t1,2)) s++;   // i jr
     if (s == len) return 0; }}
        if (!(pal[i]||pal[j])) for (frst=0; frst<n; frst++) for (lst=n+((n+1+frst)&1); lst<2*n; lst+=2)
                        {len=(lst+1-frst)/2;
                         for (t1=1; t1<3; t1++)
                         {s=0;
     while (s<len && tr(frst+s,i,j,0,4*t1,3)==tr(frst+len+s,i,j,0,4*t1,3)) s++;   // ir jr
     if (s == len) return 0; }}
        for (frst=0; frst<n; frst++) for (lst=2*n+((1+frst)&1); lst<3*n; lst+=2)
                {len=(lst+1-frst)/2;
                 for (t1=1; t1<3; t1++) for (t2=0; t2<3; t2++) if (t2 != t1)
                                 {s=0;
                     while (s<len && tr(frst+s,i,j,i,16*t2+4*t1,0)==tr(frst+len+s,i,j,i,16*t2+4*t1,0)) s++; // i j i
                     if (s == len) return 0;
                     s=0;
                     while (s<len && tr(frst+s,j,i,j,16*t2+4*t1,0)==tr(frst+len+s,j,i,j,16*t2+4*t1,0)) s++; // j i j
                     if (s == len) return 0;
                     s=0;
                     while (s<len && tr(frst+s,i,j,j,16*t2+4*t1,0)==tr(frst+len+s,i,j,j,16*t2+4*t1,0)) s++; // i j j
                     if (s == len) return 0;
                     s=0;
                     while (s<len && tr(frst+s,i,i,j,16*t2+4*t1,0)==tr(frst+len+s,i,i,j,16*t2+4*t1,0)) s++; // i i j
                     if (s == len) return 0; }
                 if (!pal[i]) for (t1=1; t1<3; t1++) for (t2=0; t2<3; t2++) if (t2 != t1)
                                         {s=0;
                             while (s<len && tr(frst+s,i,j,i,16*t2+4*t1,1)==tr(frst+len+s,i,j,i,16*t2+4*t1,1)) s++; // ir j i
                             if (s == len) return 0;
                             s=0;
                             while (s<len && tr(frst+s,i,j,i,16*t2+4*t1,4)==tr(frst+len+s,i,j,i,16*t2+4*t1,4)) s++; // i j ir
                             if (s == len) return 0;
                             s=0;
                             while (s<len && tr(frst+s,i,i,j,16*t2+4*t1,1)==tr(frst+len+s,i,i,j,16*t2+4*t1,1)) s++; // ir i j
                             if (s == len) return 0;
                             s=0;
                             while (s<len && tr(frst+s,j,i,i,16*t2+4*t1,4)==tr(frst+len+s,j,i,i,16*t2+4*t1,4)) s++; // j i ir
                             if (s == len) return 0;
                             s=0;
                             while (s<len && tr(frst+s,j,j,i,16*t2+4*t1,4)==tr(frst+len+s,j,j,i,16*t2+4*t1,4)) s++; // j j ir
                             if (s == len) return 0;
                             s=0;
                             while (s<len && tr(frst+s,i,j,j,16*t2+4*t1,1)==tr(frst+len+s,i,j,j,16*t2+4*t1,1)) s++; // ir j j
                             if (s == len) return 0; }
                 if (!pal[j]) for (t1=1; t1<3; t1++) for (t2=0; t2<3; t2++) if (t2 != t1)
                                         {s=0;
                             while (s<len && tr(frst+s,j,i,j,16*t2+4*t1,1)==tr(frst+len+s,j,i,j,16*t2+4*t1,1)) s++; // jr i j
                             if (s == len) return 0;
                             s=0;
                             while (s<len && tr(frst+s,j,i,j,16*t2+4*t1,4)==tr(frst+len+s,j,i,j,16*t2+4*t1,4)) s++; // j i jr
                             if (s == len) return 0;
                             s=0;
                             while (s<len && tr(frst+s,i,i,j,16*t2+4*t1,4)==tr(frst+len+s,i,i,j,16*t2+4*t1,4)) s++; // i i jr
                             if (s == len) return 0;
                             s=0;
                             while (s<len && tr(frst+s,j,i,i,16*t2+4*t1,1)==tr(frst+len+s,j,i,i,16*t2+4*t1,1)) s++; // jr i i
                             if (s == len) return 0;
                             s=0;
                             while (s<len && tr(frst+s,j,j,i,16*t2+4*t1,1)==tr(frst+len+s,j,j,i,16*t2+4*t1,1)) s++; // jr j i
                             if (s == len) return 0;
                             s=0;
                             while (s<len && tr(frst+s,i,j,j,16*t2+4*t1,4)==tr(frst+len+s,i,j,j,16*t2+4*t1,4)) s++; // i j jr
                             if (s == len) return 0; }
                 if (!(pal[i]&&pal[j])) for (t1=1; t1<3; t1++) for (t2=0; t2<3; t2++) if (t2 != t1)
                                         {s=0;
                             while (s<len && tr(frst+s,j,i,i,16*t2+4*t1,0)==tr(frst+len+s,j,i,i,16*t2+4*t1,0)) s++; // j i i
                             if (s == len) return 0;
                             s=0;
                             while (s<len && tr(frst+s,j,j,i,16*t2+4*t1,0)==tr(frst+len+s,j,j,i,16*t2+4*t1,0)) s++; // j j i
                             if (s == len) return 0; }
                 if (!(pal[i]||pal[j])) for (t1=1; t1<3; t1++) for (t2=0; t2<3; t2++) if (t2 != t1)
                                         {s=0;
                             while (s<len && tr(frst+s,i,j,i,16*t2+4*t1,2)==tr(frst+len+s,i,j,i,16*t2+4*t1,2)) s++; // i jr i
                             if (s == len) return 0;
                             s=0;
                             while (s<len && tr(frst+s,j,i,j,16*t2+4*t1,2)==tr(frst+len+s,j,i,j,16*t2+4*t1,2)) s++; // j ir j
                             if (s == len) return 0;
                             s=0;
                             while (s<len && tr(frst+s,j,i,i,16*t2+4*t1,2)==tr(frst+len+s,j,i,i,16*t2+4*t1,2)) s++; // j ir i
                             if (s == len) return 0;
                             s=0;
                             while (s<len && tr(frst+s,i,i,j,16*t2+4*t1,2)==tr(frst+len+s,i,i,j,16*t2+4*t1,2)) s++; // i ir j
                             if (s == len) return 0;
                             s=0;
                             while (s<len && tr(frst+s,j,j,i,16*t2+4*t1,2)==tr(frst+len+s,j,j,i,16*t2+4*t1,2)) s++; // j jr i
                             if (s == len) return 0;
                             s=0;
                             while (s<len && tr(frst+s,i,j,j,16*t2+4*t1,2)==tr(frst+len+s,i,j,j,16*t2+4*t1,2)) s++; // i jr j
                             if (s == len) return 0; }}
        return 1;
}

// Assuming w[i][0..n-1], w[j][0..n-1] and w[k][0..n-1] are squarefree (0 <= i < j< k < m),
// and that they are pairwise compatible, check that the triple is compatible.
int chk3(int i,int j,int k)
{
        int frst,lst,len,s,t0,t1,t2;
        for (frst=0; frst<n; frst++) for (lst=2*n+((1+frst)&1); lst<3*n; lst+=2)
                {len=(lst+1-frst)/2;
                 for (t1=1; t1<3; t1++) for (t2=0; t2<3; t2++) if (t2 != t1)
                                 {s=0;
                     while (s<len && tr(frst+s,i,j,k,16*t2+4*t1,0)==tr(frst+len+s,i,j,k,16*t2+4*t1,0)) s++; // i j k
                     if (s == len) return 0;
                     s=0;
                     while (s<len && tr(frst+s,i,k,j,16*t2+4*t1,0)==tr(frst+len+s,i,k,j,16*t2+4*t1,0)) s++; // i k j
                     if (s == len) return 0;
                     s=0;
                     while (s<len && tr(frst+s,j,i,k,16*t2+4*t1,0)==tr(frst+len+s,j,i,k,16*t2+4*t1,0)) s++; // j i k
                     if (s == len) return 0;
                     s=0;
                     while (s<len && tr(frst+s,j,k,i,16*t2+4*t1,0)==tr(frst+len+s,j,k,i,16*t2+4*t1,0)) s++; // j k i
                     if (s == len) return 0;
                     s=0;
                     while (s<len && tr(frst+s,k,i,j,16*t2+4*t1,0)==tr(frst+len+s,k,i,j,16*t2+4*t1,0)) s++; // k i j
                     if (s == len) return 0;
                     s=0;
                     while (s<len && tr(frst+s,k,j,i,16*t2+4*t1,0)==tr(frst+len+s,k,j,i,16*t2+4*t1,0)) s++; // k j i
                     if (s == len) return 0; }
                 if (!pal[i]) for (t1=1; t1<3; t1++) for (t2=0; t2<3; t2++) if (t2 != t1)
                                         {s=0;
                             while (s<len && tr(frst+s,i,j,k,16*t2+4*t1,1)==tr(frst+len+s,i,j,k,16*t2+4*t1,1)) s++; // ir j k
                             if (s == len) return 0;
                             s=0;
                             while (s<len && tr(frst+s,i,k,j,16*t2+4*t1,1)==tr(frst+len+s,i,k,j,16*t2+4*t1,1)) s++; // ir k j
                             if (s == len) return 0;
                             s=0;
                             while (s<len && tr(frst+s,j,k,i,16*t2+4*t1,4)==tr(frst+len+s,j,k,i,16*t2+4*t1,4)) s++; // j k ir
                             if (s == len) return 0;
                             s=0;
                             while (s<len && tr(frst+s,k,j,i,16*t2+4*t1,4)==tr(frst+len+s,k,j,i,16*t2+4*t1,4)) s++; // k j ir
                             if (s == len) return 0; }
                 if (!pal[j]) for (t1=1; t1<3; t1++) for (t2=0; t2<3; t2++) if (t2 != t1)
                                         {s=0;
                             while (s<len && tr(frst+s,i,k,j,16*t2+4*t1,4)==tr(frst+len+s,i,k,j,16*t2+4*t1,4)) s++; // i k jr
                             if (s == len) return 0;
                             s=0;
                             while (s<len && tr(frst+s,j,i,k,16*t2+4*t1,1)==tr(frst+len+s,j,i,k,16*t2+4*t1,1)) s++; // jr i k
                             if (s == len) return 0;
                             s=0;
                             while (s<len && tr(frst+s,j,k,i,16*t2+4*t1,1)==tr(frst+len+s,j,k,i,16*t2+4*t1,1)) s++; // jr k i
                             if (s == len) return 0;
                             s=0;
                             while (s<len && tr(frst+s,k,i,j,16*t2+4*t1,4)==tr(frst+len+s,k,i,j,16*t2+4*t1,4)) s++; // k i jr
                             if (s == len) return 0; }
                 if (!pal[k]) for (t1=1; t1<3; t1++) for (t2=0; t2<3; t2++) if (t2 != t1)
                                         {s=0;
                             while (s<len && tr(frst+s,i,j,k,16*t2+4*t1,4)==tr(frst+len+s,i,j,k,16*t2+4*t1,4)) s++; // i j kr
                             if (s == len) return 0;
                             s=0;
                             while (s<len && tr(frst+s,j,i,k,16*t2+4*t1,4)==tr(frst+len+s,j,i,k,16*t2+4*t1,4)) s++; // j i kr
                             if (s == len) return 0;
                             s=0;
                             while (s<len && tr(frst+s,k,i,j,16*t2+4*t1,1)==tr(frst+len+s,k,i,j,16*t2+4*t1,1)) s++; // kr i j
                             if (s == len) return 0;
                             s=0;
                             while (s<len && tr(frst+s,k,j,i,16*t2+4*t1,1)==tr(frst+len+s,k,j,i,16*t2+4*t1,1)) s++; // kr j i
                             if (s == len) return 0; }
                 if (pal[i]+pal[j]+pal[k] < 2) for (t1=1; t1<3; t1++) for (t2=0; t2<3; t2++) if (t2 != t1)
                                         {s=0;
                             while (s<len && tr(frst+s,i,j,k,16*t2+4*t1,2)==tr(frst+len+s,i,j,k,16*t2+4*t1,2)) s++; // i jr k
                             if (s == len) return 0;
                             s=0;
                             while (s<len && tr(frst+s,j,i,k,16*t2+4*t1,2)==tr(frst+len+s,j,i,k,16*t2+4*t1,2)) s++; // j ir k
                             if (s == len) return 0;
                             s=0;
                             while (s<len && tr(frst+s,i,k,j,16*t2+4*t1,2)==tr(frst+len+s,i,k,j,16*t2+4*t1,2)) s++; // i kr j
                             if (s == len) return 0;
                             s=0;
                             while (s<len && tr(frst+s,j,k,i,16*t2+4*t1,2)==tr(frst+len+s,j,k,i,16*t2+4*t1,2)) s++; // j kr i
                             if (s == len) return 0;
                             s=0;
                             while (s<len && tr(frst+s,k,i,j,16*t2+4*t1,2)==tr(frst+len+s,k,i,j,16*t2+4*t1,2)) s++; // k ir j
                             if (s == len) return 0;
                             s=0;
                             while (s<len && tr(frst+s,k,j,i,16*t2+4*t1,2)==tr(frst+len+s,k,j,i,16*t2+4*t1,2)) s++; // k jr i
                             if (s == len) return 0; }}
        return 1;
}

// Look up whether admissible words i and j are compatible
int adj(int i,int j)
{
        unsigned e,p;
        e=(j*(j-1))/2+i;
        p=e&31;
        return (compat[(e-p)>>5]>>p)&1;
}

// Designate admissible words i and j as compatible
int cert(int i,int j)
{
        unsigned e,p;
        e=(j*(j-1))/2+i;
        p=e&31;
        compat[(e-p)>>5]|=(1<<p);
}
