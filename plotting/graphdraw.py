import os
from matplotlib.ticker import ScalarFormatter
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from matplotlib import rcParams
import json
import numpy as np
import warnings
from scipy.optimize import OptimizeWarning

warnings.simplefilter("ignore", OptimizeWarning)

COLORS=["red","blue","purple","orange","black","green"]
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
    return np.clip(np.exp(-(A+B*s+C*s**2)),1e-3, 10) 
def Weakcoupl(beta):
    C=(-6*np.pi**2)/11
    return np.exp(C*(beta-2)) 

def Strongcoupl(beta):
    return -np.log(beta/4) 

def Graph4stuff(ax,ax2,ax3,N,Imax,Beta_array,Ldata):
    Xspace1=np.linspace(0,2,10**2)
    Xspace2=np.linspace(2,4,10**2)
    ax.plot(Xspace1,Xspace1/4,linestyle='--',label='1*1 Strong coupling lim',color='red')
    ax.plot(Xspace1,(Xspace1/4)**4,linestyle='--',label='2*2 Strong coupling lim',color='green')
    Weakc=1-0.75/Xspace2
    ax.plot(Xspace2,Weakc,linestyle='--',label='weak coupling lim',color='grey')
    graph5(ax2,N,Beta_array,Ldata)
    Graph6(ax3,N,Imax,Beta_array,Ldata)
    
def graph5(ax,N,Beta_array,Ldata):
    Xlabel='Loop size'
    Ylabel='Wilson loop'
    title=' Fit of Wilson loop as a function of \n loop sizes for different $\\beta$ \n & fixed N='+str(N)

    Betalist=[1.9+0.2*b for b in range(4)]
    size_array=np.linspace(0,5,10**2)
    np.seterr(over='ignore')
    for b,beta in enumerate(Betalist):
        beta_ind=(np.abs(Beta_array - beta)).argmin()#np.where(Beta_array==beta)
        
        if beta<=2.1:
            Xfit=[0,1,2]
            Yfit=[1.0]+[Ldata[l-1][beta_ind][1] for l in [1,2]]
            print(beta,Xfit,Yfit)
        else:
            Xfit=[si+1 for si in range(len(Ldata))]
            Yfit=[Ldata[l][beta_ind][1] for l in range(len(Ldata))]
        acoef,bcoef,ccoef = curve_fit(fitfunc,Xfit,Yfit )[0]
        Y=fitfunc(size_array,acoef,bcoef,ccoef)
        mask=np.where((Y<=1.0)&(Y>0))
        ax.scatter(Xfit,Yfit,marker='x',color=COLORS[b])
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
    Betalist=np.linspace(10**-1,2.5,30)
    for b,beta in enumerate(Betalist):
        beta_ind=(np.abs(Beta_array - beta)).argmin()
        if beta<=2.1:
            Xfit=[0,1,2]
            Yfit=[1.0]+[Ldata[l-1][beta_ind][1] for l in [1,2]]
        else:
            Xfit=[si+1 for si in range(len(Ldata))]
            Yfit=[Ldata[l][beta_ind][1] for l in range(len(Ldata))]
        fitpts.append(curve_fit(fitfunc, Xfit, Yfit)[0][2])
    
    ax.scatter(Betalist,fitpts,s=10,label='fit data',color='black')


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
    fig = plt.figure()
    if Graphnumb==4:
        ax = plt.subplot(221)
        ax2 = plt.subplot(222)
        ax3 = plt.subplot(212)
    else:
        ax = plt.subplot(111)
    
    rcParams["figure.autolayout"]
    for p in range(numbplots):
        if Fullplotdata[p]==[] or not VALS[p][:,1].any():#skip data that is empty or all zeros
            continue
        xdata,ydata=VALS[p][:,0],VALS[p][:,1]
        graphinfo=data_loaded["graphinfo"][p]
        
        marker,linestyle=Markers[Graphnumb-1],Linestyles[Graphnumb-1]
        
        if Graphnumb==1:
            n=int(0.5*(int(graphinfo[2])))-1
            inimode=int(graphinfo[-1])
            graphinfo=graphinfo[:-9]
            graphinfo+=["Coldstart","Hotstart"][inimode]
            color=COLORS[n]
            linestyle=linestyle[inimode]
            marker=marker[inimode]
        elif Graphnumb==3:
            color=COLORS[p]
            Nlist+=[xval for xval in xdata if xval not in Nlist]
        else:
            color=COLORS[p]
        ax.plot(xdata,ydata,color=color,marker=marker,ms=4,linestyle=linestyle,label=graphinfo)
    
    if Graphnumb==3:
        if bottom<=0:
            bottom=0.01
        labels = [str(int(n))+'$^{4}$' for n in Nlist]
        set_logplot(ax)
        ax.set_xticks(Nlist, labels=labels)

    Grsetup(ax,Xlabel,Ylabel,title,bottom,top)
    if Graphnumb==4:
        N,Imax=data_loaded["fixed"]
        Graph4stuff(ax,ax2,ax3,N,Imax,xdata,VALS)
    fig.tight_layout()
    plt.savefig(path.removesuffix('/plotting')+"/graphdata/figure"+str(Graphnumb)+".pdf")
    plt.show()
