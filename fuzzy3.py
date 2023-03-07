import matplotlib.pyplot as plt
import numpy as np


def triangular(a,b,c,x):
   return np.maximum(np.minimum(((x-a)/(b-a)),((c-x)/(c-b))),0)
  

def plotTri(a,b,c):
    x = np.arange(a-1,c+1,0.1)
    y = triangular(a, b, c, x)
    plt.plot(x,y)

def trapezoidal(a,b,c,d,x):
    return np.maximum(np.minimum(np.minimum(((x-a)/(b-a)),((d-x)/(d-c))),1),0)


def plotTra(a,b,c,d):
    x = np.arange(a-1,d+1,0.1)
    y = trapezoidal(a, b, c,d, x)
    plt.plot(x,y)
    
def gaussian(a,b,c,x):
    return (1/(1+(((x-c)/a)**(2*b))))

def plotGau(a,b,c):
    x = np.arange(a-1,c+1,0.1)
    y = gaussian(a, b, c, x)
    plt.plot(x,y)
    plt.show()
    
def plotTriangle(speedDifference,name):
    for key in speedDifference:
        temp = speedDifference[key]
        a = temp[0]
        b = temp[1]
        c = temp[2]
        if key=='NL' or key=='PL':
            d = temp[3]
            plotTra(a, b, c, d)
        else:
            plotTri(a, b, c)
    plt.title(name,fontsize = 14, fontweight ='bold')
    plt.show()
            

def fuzzy_cal(x):
     res = {
    'NL' : trapezoidal(-1, 0, 31, 61, x),
    'NM':triangular(31,61, 95, x),
    'NS': triangular(61, 95, 127, x),
    'ZE':triangular(95,127, 159, x),
    'PS' : triangular(127, 159,191, x),
    'PM': triangular(159, 191, 223, x),
    'PL': trapezoidal(191, 223, 225, 226, x)}
     return res

def chooseValues(val):
    final = {}
    for key in val:
        if val[key] != 0:
            final[key] = val[key]
    print(final)
    return final
  

def calculateArea(throttle,speedDifference):
    area = []
    for key in throttle:
        ran = speedDifference[key]
        #1/2*h*(a+b)
        b = ran[2]-ran[0]
        h = throttle[key]
        #(127,0)*(159,1)
        y11 = 0
        y12 =1
        m = (y12-y11)/(ran[1]-ran[0])
        #y=mx+c
        #c = y-mx
        c = y11-m*ran[0]
        #x = (y-c)/m
        val1 = (throttle[key] - c)/m
        y21 = 1
        y22 = 0
        m = (y22-y21)/(ran[2]-ran[1])
        c = y21-m*ran[1]
        val2 = (throttle[key] - c)/m
        a = val2-val1
        single_area =round( 1/2*h*(a+b),3)
        area.append(single_area)
    return area

    
def calculateAggregate(area,throttle,speedDifference):
    cg = 0
    ran = []
    for key in throttle:
        ran.append(speedDifference[key])
    for i in range(len(area)):
        cg+=area[i]*ran[i][1]
    cg = cg/sum(area)
    return cg


speedDifference ={
    'NL':[-1,0,31,63],
    'NM':[31,61,95],
    'NS':[61,95,127],
    'ZE':[95,127,159],
    'PS':[127,159,191],
    'PM':[159,191,223],
    'PL':[191,223,255,260]
    }

speed_diff = fuzzy_cal(100)
acceleration = fuzzy_cal(70)

def check_rules(speed_diff,acceleration):
    speed = {}
    acc = {}
    throttle = {}
    for x,y in speed_diff.items():
        if y!=0:
            speed[x] = y
            
    for x,y in acceleration.items():
         if y!=0:
             acc[x] = y
    
    if 'NL' in speed.keys() and 'ZE' in acc.keys():
        throttle['PL'] = min(speed['NL'],acc['ZE'])
    else:
        throttle['PL'] = 0
    
    if 'ZE' in speed.keys() and 'NL' in acc.keys():
        throttle['PL'] = min(speed['ZE'],acc['NL'])
    else:
        throttle['PL'] = 0
        
    if 'NM' in speed.keys() and 'ZE' in acc.keys():
        throttle['PM'] = min(speed['NM'],acc['ZE'])
    else:
        throttle['PM'] = 0
        
    if 'NS' in speed.keys() and 'PS' in acc.keys():
        throttle['PS'] = min(speed['NS'],acc['PS'])
    else:
        throttle['PS'] = 0
        
    if 'PS' in speed.keys() and 'NS' in acc.keys():
        throttle['NS'] = min(speed['PS'],acc['NS'])
    else:
        throttle['NS'] = 0
        
    if 'PL' in speed.keys() and 'ZE' in acc.keys():
        throttle['NL'] = min(speed['PL'],acc['ZE'])
    else:
        throttle['NL'] = 0
        
    if 'ZE' in speed.keys() and 'NS' in acc.keys():
        throttle['PS'] = min(speed['ZE'],acc['NS'])
    else:
        throttle['PS'] = 0
        
    if 'ZE' in speed.keys() and 'NM' in acc.keys():
        throttle['PM'] = min(speed['ZE'],acc['NM'])
    else:
        throttle['PM'] = 0
        
    return throttle

print(speed_diff)
print(acceleration)
values =check_rules(speed_diff, acceleration)
plotTriangle(speedDifference, 'Speed Differeence')
throttle  = chooseValues(values)
area = calculateArea(throttle,speedDifference)
cg = calculateAggregate(area,throttle,speedDifference)
print('Defuzzified value is,',cg)

    


