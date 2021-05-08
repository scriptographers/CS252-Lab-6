import numpy as np
import scipy.stats as st
import re

#for reno, open data.txt;
#for cubic open data2.txt
myf=open('data.txt') 

#reading the data
t=myf.readlines()

for k in range(len(t)):
    #stripping whitespace
    t[k]=t[k].strip()

print("The following show respective confidence intervals, unit is kBps")

for i in range(9):
    print(t[21*i])
    #to store data for particular
    meta=[]
    for j in range(20):
        mytime=float((re.findall("\d+\.\d+", t[21*i+j+1]))[0])
        meta.append(5*1024/mytime)
    iv=st.t.interval(alpha=0.9, df=len(meta)-1, loc=np.mean(meta), scale=st.sem(meta)) 
    print(iv)
    print("")