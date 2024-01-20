import os
from matplotlib.ticker import ScalarFormatter
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
import json
import numpy as np
COLORS=["red","green","blue","purple","orange","black"]
#Lines=["-","--"]
Markers=[["x","o"],"x","o","o"]
Linestyles=[["-","--"],"-","-","None"]
path=os.path.dirname(os.path.abspath(__file__))

def set_logplot(ax):
    ax.set_yscale('log')
    ax.tick_params(which='both',right=True, left=True,top=True , bottom=True)
    ax.yaxis.set_major_formatter(ScalarFormatter())
def Grsetup(ax,Xlabel,Ylabel,title,b,t):
    ax.set_xlabel(Xlabel)
    ax.set_ylabel(Ylabel)
    ax.set_title(title)
    ax.set_ylim(bottom=b,top=t)
    ax.legend()

def fitfunc(s, A, B, C):
    return np.clip(np.exp(-(A+B*s+C*s**2)), 0.01, 1) 
def Weakcoupl(beta):
    C=(-6*np.pi**2)/11
    return np.clip(np.exp(C*(beta-2)), 0.01, 10) 

def Strongcoupl(beta):
    return np.clip(-np.log(beta/4), 0.01, 10) 

def Graph4stuff(ax0,N,Imax,Beta_array,Ldata):
    Xspace1=np.linspace(0,2,10**2)
    Xspace2=np.linspace(2,4,10**2)
    ax0.plot(Xspace1,Xspace1/4,linestyle='--',label='1*1 Strong coupling lim',color='red')
    ax0.plot(Xspace1,(Xspace1/4)**4,linestyle='--',label='2*2 Strong coupling lim',color='green')
    Weakc=1-0.75/Xspace2
    ax0.plot(Xspace2,Weakc,linestyle='--',label='weak coupling lim',color='grey')
    graph5(plt.subplot(222),N,Beta_array,Ldata)
    Graph6(plt.subplot(223),N,Imax,Beta_array,Ldata)
    
def graph5(ax,N,Beta_array,Ldata):
    Xlabel='Loop size'
    Ylabel='Wilson loop'
    title='Fig.4 Fit of Wilson loop as a function of \n loop sizes for different $\\beta$ \n & fixed N='+str(N)

    Betalist=[1.9+0.2*b for b in range(4)]
    size_array=np.linspace(0,5,10**2)
    np.seterr(over='ignore')
    for b,beta in enumerate(Betalist):
        beta_ind=(np.abs(Beta_array - beta)).argmin()#np.where(Beta_array==beta)
        Wloopb_si=[Ldata[l][beta_ind][1] for l in range(len(Ldata))]
        Xfit=[si+1 for si in range(len(Ldata))]
        Yfit=Wloopb_si
        if beta<=2.1:
            Xfit=[0]+Xfit
            Yfit=[1.0]+Yfit
        acoef,bcoef,ccoef = curve_fit(fitfunc,Xfit,Yfit )[0]
        Y=fitfunc(size_array,acoef,bcoef,ccoef)
        mask=np.where((Y<=1.0)&(Y>0))
        ax.scatter([si+1 for si in range(len(Ldata))],Wloopb_si,marker='x',color=COLORS[b])
        ax.plot(size_array[mask],Y[mask],label="B="+str(beta),color=COLORS[b])
    np.seterr(over='warn')
   
    Grsetup(ax,Xlabel,Ylabel,title,0.01,1)
    ax.set_xlim(0,5)
    set_logplot(ax)


def Graph6(ax,N,Imax,Beta_array,Ldata):
    Xlabel=r"$\beta=\frac{4}{e_{0}^2} $"
    Ylabel=r"$a^2 \kappa $"
    title='Cutof squared times string tension\n as a function of $\\beta$ in \n Strong & weak coupling limits'

    fitpts=[]
    Betalist=np.linspace(10**-1,2.5,15)
    for b,beta in enumerate(Betalist):
        beta_ind=(np.abs(Beta_array - beta)).argmin()
        Wloopb_si=[Ldata[l][beta_ind][1] for l in range(len(Ldata))]
        Xfit=[si+1 for si in range(len(Ldata))]
        Yfit=Wloopb_si
        if beta<=2.1:
            Xfit=[0]+Xfit
            Yfit=[1.0]+Yfit
        fitpts.append(curve_fit(fitfunc, [si+1 for si in range(len(Ldata))], Wloopb_si)[0][2])
    ax.scatter(Betalist,fitpts,label='fit data',color='black')


    Beta_spaceS=np.linspace(10**-2,3.5,10**3)
    Beta_spaceW=np.linspace(1.5,2.9,10**3)
    
    Strlim=Strongcoupl(Beta_spaceS)
    Smask=(Strlim>0.01)&(Strlim<10)
    ax.plot(Beta_spaceS[Smask],Strlim[Smask],label='Strong coupling lim: $-ln\\;\\frac{\\beta}{4}$',color='red')
    
    Weaklim=Weakcoupl(Beta_spaceW)
    Wmask=(Weaklim>0.01)&(Weaklim<10)
    ax.plot(Beta_spaceW[Wmask],Weaklim[Wmask],label='Weak coupling lim: $\\exp{\\frac{-6\\pi^{2}}{11}(\\beta-2)}$',color='blue')

    Grsetup(ax,Xlabel,Ylabel,title,0.01,10)
    ax.set_xlim(0,3.5)
    set_logplot(ax)



def DRAW(Graphnumb):
    Nlist=[]
    location=path.removesuffix('/plotting')+"/graphdata/json_data" + str(Graphnumb) + ".json"
    with open(location, "r") as f:
        data_loaded = json.load(f)
        Xlabel,Ylabel,title=data_loaded["Xlabel"],data_loaded["Ylabel"],data_loaded["title"]
        bottom,top,numbplots=data_loaded["b"],data_loaded["t"],data_loaded["numbplots"]
        Fullplotdata=data_loaded["plots"]
        VALS=[np.array(data) for data in Fullplotdata if data!=[]]
    
    if Graphnumb==4:
        ax=plt.subplot(221)
    else:
        ax=plt.subplot(111)
    for p in range(numbplots):
        if Fullplotdata[p]==[] or not VALS[p][:,1].any():#skip data that is empty or all zeros
            continue
        xdata,ydata=VALS[p][:,0],VALS[p][:,1]
        graphinfo=data_loaded["graphinfo"][p]
        
        marker,linestyle=Markers[Graphnumb-1],Linestyles[Graphnumb-1]
        color=COLORS[p]
        if Graphnumb==1:
            N=int(np.log2(int(graphinfo[4])))
            inimode=int(graphinfo[-1])
            color=COLORS[N]
            linestyle=linestyle[inimode]
            marker=marker[inimode]
        elif Graphnumb==3:
            Nlist+=[xval for xval in xdata if xval not in Nlist]
        
        ax.plot(xdata,ydata,color=color,marker=marker,linestyle=linestyle,label=graphinfo)
    
    if Graphnumb==3:
        if bottom<=0:
            bottom=0.01
        labels = [str(int(n))+'$^{4}$' for n in Nlist]
        set_logplot(ax)
        ax.set_xticks(Nlist, labels=labels)

    Grsetup(ax,Xlabel,Ylabel,title,bottom,top)
    if Graphnumb==4:
        N,Imax=data_loaded["fixed"]
        Graph4stuff(ax,N,Imax,xdata,VALS)
    plt.tight_layout()
    plt.savefig(path.removesuffix('/plotting')+"/graphdata/figure"+str(Graphnumb)+".pdf")
    plt.show()
