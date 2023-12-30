import os

import matplotlib.pyplot as plt
import json
import numpy as np
COLORS=["red","green","blue","purple","orange","black"]
path=os.path.dirname(os.path.abspath(__file__))
def Grsetup(ax,Xlabel,Ylabel,title,b,t):
    ax.set_xlabel(Xlabel)
    ax.set_ylabel(Ylabel)
    ax.set_title(title)
    ax.set_ylim(bottom=b,top=t)
    ax.legend()

def DRAW(location):
    with open(location, "r") as f:
        data_loaded = json.load(f)
        Xlabel,Ylabel,title,b,t,numbplots=data_loaded["Xlabel"],data_loaded["Ylabel"],data_loaded["title"],data_loaded["b"],data_loaded["t"],data_loaded["numbplots"]
        VALS=np.array(data_loaded["plots"][:numbplots])
    ax=plt.subplot(121)
    for p in range(numbplots):
        xdata,ydata=VALS[p,:,0],VALS[p,:,1]
        color=COLORS[p]
        ax.plot(xdata,ydata,color=color,label=data_loaded["graphinfo"][p])
    Grsetup(ax,Xlabel,Ylabel,title,b,1)
    plt.tight_layout()
    plt.show()

DRAW(path+'/graphdata/json_data.json')
