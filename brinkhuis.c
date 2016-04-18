// Find all admissible words of length n
#include <stdio.h>
#define N 100
#define RANGE 3 /* values 0 to RANGE-1 permissible */
int n,trial[N];
main(int argc,char* argv[])
{int i,level,nsuccesses,a1p,a1n,a2p,a2n,b1p,b1n,b2p,b2n,next_value,pal;
char adm_str[20];
FILE *adm;

if (argc != 2)
   {fprintf(stderr,"Usage: brinkhuis n\n");
   exit(1);}
sscanf(argv[1],"%d",&n);
if (n < 12)
   {fprintf(stderr,"Error: n=%d is smaller than 12\n",n);
   exit(1);}
if (n >= N)
   {fprintf(stderr,"Error: n=%d exceeds N-1=%d\n",n,N-1);
   exit(1);}
b1p=b1n=b2p=b2n=a1p=a1n=a2p=a2n=nsuccesses=0;

trial[0]=0;
trial[1]=1;
trial[2]=2;
trial[3]=0;
trial[4]=2;
trial[5]=1;
trial[n-6]=1;
trial[n-5]=2;
trial[n-4]=0;
trial[n-3]=2;
trial[n-2]=1;
trial[n-1]=0;
trial[6]=0;
level=6;
sprintf(adm_str,"a1%d.txt",n);
adm=fopen(adm_str,"w+");
fprintf(adm,"%d\n",n);
while (level >= 6)
   {if (chk(level))
      {if (++level < n-6) next_value=0;
      else
	 {if (chk(n-6) && chk(n-5) && chk(n-4) && chk(n-3) && chk(n-2) && chk(n-1))
	    {if ((pal=chkn()) > 1)
	       {printf("Success [%3d]: ",++nsuccesses);
               for (i=0; i<n; i++)
	          {printf("%d",trial[i]);
	          fprintf(adm,"%d",trial[i]);}
	       if (pal&1)
	          {b1p++;
		  printf(" (palindromic)");}
	       else b1n+=2;
	       fprintf(adm,"\n");
               printf("\n");}
	    if (pal&1) a1p++;
	    else a1n+=2;}
	 next_value=trial[--level]+1;}
      }
   else next_value=trial[level]+1;
   while (level>=6 && next_value>=RANGE) next_value=trial[--level]+1;
   if (level >= 6) trial[level]=next_value;}
fprintf(adm,"9\n");
fclose(adm);

trial[0]=0;
trial[1]=1;
trial[2]=2;
trial[3]=1;
trial[4]=0;
trial[5]=2;
trial[n-6]=2;
trial[n-5]=0;
trial[n-4]=1;
trial[n-3]=2;
trial[n-2]=1;
trial[n-1]=0;
trial[6]=0;
level=6;
sprintf(adm_str,"a2%d.txt",n);
adm=fopen(adm_str,"w+");
fprintf(adm,"%d\n",n);
while (level >= 6)
   {if (chk(level))
      {if (++level < n-6) next_value=0;
      else
	 {if (chk(n-6) && chk(n-5) && chk(n-4) && chk(n-3) && chk(n-2) && chk(n-1))
	    {if ((pal=chkn()) > 1)
	       {printf("Success [%3d]: ",++nsuccesses);
               for (i=0; i<n; i++)
	          {printf("%d",trial[i]);
	          fprintf(adm,"%d",trial[i]);}
	       if (pal&1)
	          {b2p++;
		  printf(" (palindromic)");}
	       else b2n+=2;
	       fprintf(adm,"\n");
               printf("\n");}
	    if (pal&1) a2p++;
	    else a2n+=2;}
	 next_value=trial[--level]+1;}
      }
   else next_value=trial[level]+1;
   while (level>=6 && next_value>=RANGE) next_value=trial[--level]+1;
   if (level >= 6) trial[level]=next_value;}
fprintf(adm,"9\n");
fclose(adm);

printf("Done:  a1=%3d, a1p=%3d, a1n=%3d;   b1=%3d, b1p=%3d, b1n=%3d\n",a1p+a1n,a1p,a1n/2,b1p+b1n,b1p,b1n/2);
printf("       a2=%3d, a2p=%3d, a2n=%3d;   b2=%3d, b2p=%3d, b2n=%3d\n",a2p+a2n,a2p,a2n/2,b2p+b2n,b2p,b2n/2);}

// Return the i-th entry of trial[j0](r0)|trial[j1](r1)|trial[j2](r2)
// where j0,j1,j2=0,1,2; r0,r1,r2=0,1 (1 means reverse)
// j=16*j2+4*j1+j0; r=4*r2+2*r1+r0
int tr(int i,int j,int r)
{if (i < n)
   {if (r&1) return (trial[n-1-i]+(j&3))%3;
   return (trial[i]+(j&3))%3;}
if (i < 2*n)
   {if (r&2) return (trial[2*n-1-i]+((j&12)>>2))%3;
   return (trial[i-n]+((j&12)>>2))%3;}
if (r&4) return (trial[3*n-1-i]+((j&48)>>4))%3;
return (trial[i-2*n]+((j&48)>>4))%3;}

// Check that trial[0..lev] is squarefree, assuming trial[0..lev-1] is.
// Also if lev==n-7, check that trial is not preceded by its reversal
// (in which case it is redundant).
int chk(int lev)
{int i,j,k,l,ti,tj;
for (l=1; (j=(lev-2*l+1))>=0; l++)
   {i=0;
   while (i<l && trial[j+i]==trial[j+l+i]) i++;
   if (i == l) return 0;}
if (lev == n-7)
   {i=6;
   while (i < (j=(n-1-i)))
      {if ((ti=trial[i]) != (tj=trial[j])) return (ti<tj);
      i++;}
   }
return 1;}

// Assuming trial[0..n-1] is squarefree, check its various concatenations.
// Return 0 if trial is non-squarefree non-palindromic;
//        1 if trial is non-squarefree palindromic;
//        2 if trial is squarefree non-palindromic;
//        3 if trial is squarefree palindromic.
int chkn()
{int frst,lst,len,i,palindromic;
i=(n+1)/2;
while (i>5 && trial[n-1-i]==trial[i]) i--;
palindromic=(i==5);  // 1 for palindromic, 0 otherwise
for (frst=0; frst<n; frst++) for (lst=n+((n+1+frst)&1); lst<2*n; lst+=2)
   {len=(lst+1-frst)/2;
   i=0;
   while (i<len && tr(frst+i,4,0)==tr(frst+len+i,4,0)) i++;  // 0 1
   if (i == len) return palindromic;
   i=0;
   while (i<len && tr(frst+i,8,0)==tr(frst+len+i,8,0)) i++;  // 0 2
   if (i == len) return palindromic;
   if (!palindromic)
      {i=0;
      while (i<len && tr(frst+i,4,2)==tr(frst+len+i,4,2)) i++;  // 0 1r
      if (i == len) return 0;
      i=0;
      while (i<len && tr(frst+i,4,1)==tr(frst+len+i,4,1)) i++;  // 0r 1
      if (i == len) return 0;}
   }
for (frst=0; frst<n; frst++) for (lst=2*n+((1+frst)&1); lst<3*n; lst+=2)
   {len=(lst+1-frst)/2;
   i=0;
   while (i<len && tr(frst+i,4,0)==tr(frst+len+i,4,0)) i++;  // 0 1 0
   if (i == len) return palindromic;
   i=0;
   while (i<len && tr(frst+i,8,0)==tr(frst+len+i,8,0)) i++;  // 0 2 0
   if (i == len) return palindromic;
   i=0;
   while (i<len && tr(frst+i,36,0)==tr(frst+len+i,36,0)) i++;  // 0 1 2
   if (i == len) return palindromic;
   i=0;
   while (i<len && tr(frst+i,24,0)==tr(frst+len+i,24,0)) i++;  // 0 2 1
   if (i == len) return palindromic;
   if (!palindromic)
      {i=0;
      while (i<len && tr(frst+i,4,4)==tr(frst+len+i,4,4)) i++;  // 0 1 0r
      if (i == len) return 0;
      i=0;
      while (i<len && tr(frst+i,8,4)==tr(frst+len+i,8,4)) i++;  // 0 2 0r
      if (i == len) return 0;
      i=0;
      while (i<len && tr(frst+i,4,2)==tr(frst+len+i,4,2)) i++;  // 0 1r 0
      if (i == len) return 0;
      i=0;
      while (i<len && tr(frst+i,8,2)==tr(frst+len+i,8,2)) i++;  // 0 2r 0
      if (i == len) return 0;
      i=0;
      while (i<len && tr(frst+i,36,4)==tr(frst+len+i,36,4)) i++;  // 0 1 2r
      if (i == len) return 0;
      i=0;
      while (i<len && tr(frst+i,36,2)==tr(frst+len+i,36,2)) i++;  // 0 1r 2
      if (i == len) return 0;
      i=0;
      while (i<len && tr(frst+i,24,4)==tr(frst+len+i,24,4)) i++;  // 0 2 1r
      if (i == len) return 0;
      i=0;
      while (i<len && tr(frst+i,24,2)==tr(frst+len+i,24,2)) i++;  // 0 2r 1
      if (i == len) return 0;
      i=0;
      while (i<len && tr(frst+i,36,1)==tr(frst+len+i,36,1)) i++;  // 0r 1 2
      if (i == len) return 0;
      i=0;
      while (i<len && tr(frst+i,24,1)==tr(frst+len+i,24,1)) i++;  // 0r 2 1
      if (i == len) return 0;}
   }
return 2+palindromic;}
