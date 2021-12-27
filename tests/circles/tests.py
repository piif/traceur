from grille import Grille
from Tkinter import Tk
from math import sqrt
from arc import Arc

def init():
    root = Tk()
    g = Grille(root, 200, 3)
    return g


def circleAndresQ0(g, r, color = 'black'):
    # https://fr.wikipedia.org/wiki/Algorithme_de_trac%C3%A9_de_cercle_d%27Andres

    x = 0
    y = r
    # d=r^2+r-x^2-y^2-1
    d = r - 1
    g.moveTo(x, y)

    while y >= 0 :
        g.draw(color)
        if d >= 2*x : 
            d = d - 2*x - 1
            x = x+1
            g.moveOf(1, 0)
        elif d < 2*(r-y) :
            d = d+2*y-1
            y = y-1    
            g.moveOf(0, -1)
        else: 
            d = d+2*(y-x-1)
            y = y-1
            x = x+1
            g.moveOf(1, -1)
    g.draw(color)

def circleQ0(g, r, color = 'black'):
    # limit covering pixels

    x = 0
    y = r
    # d=r^2+r-x^2-y^2-1
    d = r - 1
    g.moveTo(x, y)

    while y >= 0 :
        g.draw(color)
        # print 2*x , d , 2*(r-y)
        if d <= 2*x and d >= 2*(r-y) :
            d = d+2*(y-x-1)
            y = y-1
            x = x+1
            g.moveOf(1, -1)
        elif d < 2*(r-y) :
            d = d+2*y-1
            y = y-1    
            g.moveOf(0, -1)
        else :
            d = d - 2*x - 1
            x = x+1
            g.moveOf(1, 0)
    g.draw(color)

def arcQ0(g, x0, y0, x1, y1, xc = 0, yc = 0, color = 'black'):
    x = x0 - xc
    y = y0 - yc
    r2 = x*x + y*y
    r = int(round( sqrt(r2) ))

    # compute (xf,yf) in segment (xc,yc)->(x1,y1) with len=r
    lf = sqrt( (y1 - yc)*(y1 - yc) + (x1 - xc)*(x1 - xc) )
    # f = float(y1 - yc)/(x1 - xc) * r / lf
    xf = int(round( (x1 - xc) * r / lf ))
    yf = int(round( (y1 - yc) * r / lf ))
    
    # compute equation y = ax+b for (xc,yc)->(x1,y1)
    # (y1-yc)(x1-x) - (x1-xc)(y1-y) = 0
    dxf = x1-xc
    dyf = y1-yc
    E = dyf*(x1-x) - dxf*(y1-y)
    sign = (E > 0)
    
    # d=r^2+r-x^2-y^2-1
    d = r - 1
    print 'r, x, y, d, xf, yf', r, x, y, d, xf, yf
    g.moveTo(x0, y0)
    # while sign of (E) doesn't change
    while sign == (E > 0) :
        print 'draw ', xc + x, yc + y
        print '(E)= ', E
        g.draw(color)
        if d >= 2*x : 
            d = d - 2*x - 1
            x = x+1
            E -= dyf
            g.moveOf(1, 0)
        elif d < 2*(r-y) :
            d = d+2*y-1
            y = y-1
            E -= dxf  
            g.moveOf(0, -1)  
        else: 
            d = d+2*(y-x-1)
            y = y-1
            E -= dxf  
            x = x+1
            E -= dyf
            g.moveOf(1, -1)  

    g.drawAt(x0, y0, 'yellow')
    g.drawAt(x1, y1, 'green')
    g.drawAt(xc + xf, yc + yf, 'yellow')

def quadrant(x, y):
    if x>0:
        if y>0:
            return 0
        else:
            return 1
    else:
        if y<0:
            return 2
        else:
            return 3

def splitArc(x0, y0, x1, y1):
    qi = quadrant(x0, y0)
    qf = quadrant(x1, y1)
    if qi == qf:
        if x0 < x1:
            if qi == 0 or qi == 3:
                return [ qi ]
            else:
                return [ qi, (qi+1)%4, (qi+2)%4, (qi+3)%4, qi ]
        else:
            if qi == 1 or qi == 2:
                return [ qi ]
            else:
                return [ qi, (qi+1)%4, (qi+2)%4, (qi+3)%4, qi ]
    else:
        l = [ qi ]
        while qi != qf:
            qi = (qi+1)%4
            l.append(qi)
        return l

g = init()

O = (  0,   0)
A = ( -5,  40)
B = ( 10, -30)
C = (-15,  30)
D = (-10,  35)
E = (-25,  10)
F = (  5,  40)
G = ( 49,  10)

a = Arc()
# a.arcCW(g, F, G, (10, 10), 'orange')
# a.arcCCW(g, F, G, (10, 10), 'pink')
# a.arcCW(g, G, B, O, 'blue')
# a.arcCCW(g, G, B, O, 'purple')
# a.arcCW(g, B, F, (-15, -20), 'red')
# a.arcCCW(g, B, F, (-15, -20), 'black')

a.arcCW(g, (154 , -64), (105 , -105), (32 , 29), 'black')
