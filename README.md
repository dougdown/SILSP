# SILSP

The repository contains the following three files:

1. The source code, bulk_ordering.c





2. A sample input file, i.text with the following format:

Number of periods: n

Discount threshold: Q

Demands: d[0], ... , d[n-1]

Expensive prices(non-increasing): p1[0], ... p1[n-1]

Cheap prices(non-increasing): p2[0], ... p2[n-1]

Holding costs: h[0], ..., h[n-1]

Inventory bounds: B[0],...,B[n-1]






3. A sample output file, o.txt, with the following format:

dd[n][n]:     dd[i-1][j-1]= d_{i,j} in the paper 

hh[n][n]:     hh[i-1][j-1]= h_{i,j} in the paper  

HH[n][n]:     HH[i-1][j-1]= H_{i,j} in the paper  

A[n][n]:      A[i-1][j-1] = A_{i,j} in the paper 

QQ[n][n]:     QQ[i-1][j-1]= 1 if there is a cheap order at time j for the sequence starting from time i.

	          QQ[i-1][j-1]= 0 if there is not an order at time j for the sequence starting from time i. It is also 0 when i>j.

	          QQ[i-1][j-1]= -1 if time j is out of the sequence starting from time i. 

II[n][n]:     II[i-1][j-1]= The inventory value at the beginning of time j for the sequence starting from time i. It is -1 if j>i is out of the sequence. It is 0 when i>j.

hold[n][n]:   hold[i-1][j-1]= The holding + production cost up to time j for the sequence starting from time i. It is -1 if j>i is out of the sequence. It is 0 when i>j.

Cost[n][n]:   Cost[i-1][j-1]= cost(i,j) in the paper.

last[n][n]:   last[i-1][j-1] = The value of x_j for the interval (i,j). It is -1 if cost(i,j)=inf.

cheap[n][n]:  cheap[i-1][j-1] = The value of the last cheap order before time j for the interval (i,j). It is -1 if there is not a cheap order before time j or cost(i,j)=inf.

t[n][n]:      t[i-1][j-1] + 1 = The index of the last cheap order before time j for the interval (i,j). It is -1 if there is not a cheap order before time j or cost(i,j)=inf.

OPT:          The value of optimal cost (opt(1) in the paper)

OPTx[n]:      The value of optimal orders

OPTi[n]:      The vector of opt(i)

S[n]:         S[i-1]= j-1 when (i,j) is the first regeneration interval in the solution of the problem starting from time i.
