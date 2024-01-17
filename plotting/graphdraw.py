import os
from matplotlib.ticker import ScalarFormatter
import matplotlib.pyplot as plt
import json
import numpy as np
COLORS=["red","green","blue","purple","orange","black"]
Lines=["-","--"]

path=os.path.dirname(os.path.abspath(__file__))
def Grsetup(ax,Xlabel,Ylabel,title,b,t):
    ax.set_xlabel(Xlabel)
    ax.set_ylabel(Ylabel)
    ax.set_title(title)
    ax.set_ylim(bottom=b,top=t)
    ax.legend()

def DRAW(location):
    Nlist=[]
    with open(location, "r") as f:
        data_loaded = json.load(f)
        Xlabel,Ylabel,title,b,t,numbplots=data_loaded["Xlabel"],data_loaded["Ylabel"],data_loaded["title"],data_loaded["b"],data_loaded["t"],data_loaded["numbplots"]
        Fullplotdata=data_loaded["plots"]
        VALS=[np.array(data) for data in Fullplotdata if data!=[]]
    ax=plt.subplot(121)
    for p in range(numbplots):
        if Fullplotdata[p]==[] or not VALS[p][:,1].any():#skip empty or all zeros
            continue
        xdata,ydata=VALS[p][:,0],VALS[p][:,1]
        graphinfo=data_loaded["graphinfo"][p]
        if location[-6]=="3":
            for xval in xdata:
                if xval not in Nlist:
                    Nlist.append(xval)
        if location[-6]=="1":
            N=int(np.log2(int(graphinfo[4])))
            inimode=int(graphinfo[-1])
            linestyle=Lines[inimode]
            color=COLORS[N]
        else:
            linestyle=Lines[0]
            color=COLORS[p]
        ax.plot(xdata,ydata,color=color,linestyle=linestyle,label=graphinfo)
    
    if location[-6]=="3":
        ax.set_yscale('log')
        if b<=0:
            b=0.01
            Grsetup(ax,Xlabel,Ylabel,title,b,1)
        labels = [str(int(n))+'$^{4}$' for n in Nlist]
        ax.tick_params(which='both',right=True, left=True,top=True , bottom=True)
        ax.set_xticks(Nlist, labels=labels)
        ax.yaxis.set_major_formatter(ScalarFormatter())
    else:
        Grsetup(ax,Xlabel,Ylabel,title,b,1)
    plt.tight_layout()
    plt.show()