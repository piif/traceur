from grille import Grille
from Tkinter import Tk
from arc import Arc

def init():
    root = Tk()
    g = Grille(root, 500, 2)
    return g

O = (  0,   0)
A = ( -5,  40)
B = ( 10, -30)
C = (-15,  30)
D = (-10,  35)
E = (-25,  10)
F = (  5,  40)
G = ( 49,  10)

g = init()
a = Arc()

def G02(X, Y, I, J, color):
  a.arcCW(g, (g.X, g.Y), (X, Y), (g.X + I, g.Y + J), color)
  
#g.moveTo(467, 176)

#G02( 454, 136,  -64,  -0, 'black' )
#G02( 405,  95, -121,  93, 'red' )
#G02( 340,  71, -119, 221, 'blue' )
g.moveTo(340,  71)
G02( 255,  61,  -86, 383, 'green' )

#G02( 169,  71,    0, 393, 'orange' )
#G02( 105,  95,   54, 245, 'purple' )
#G02(  56, 136,   72, 135, 'pink' )
#G02(  42, 176,   51,  39, 'yellow' )

#G02(  57, 217,   65,  -0, 'black' )
#G02( 105, 257,  121, -97, 'red' )
#G02( 166, 280,  118,-219, 'blue' )
#G02( 255, 290,   88,-379, 'green' )

#G02( 343, 280,   -0,-390, 'orange' )
#G02( 405, 257,  -56,-243, 'purple' )
#G02( 453, 217,  -73,-137, 'pink' )
#G02( 467, 176,  -51, -41, 'yellow' )

# a.arcCW(g, F, G, (10, 10), 'orange')
# a.arcCCW(g, F, G, (10, 10), 'pink')
# a.arcCW(g, G, B, O, 'blue')
# a.arcCCW(g, G, B, O, 'purple')
# a.arcCW(g, B, F, (-15, -20), 'red')
# a.arcCCW(g, B, F, (-15, -20), 'black')

# a.arcCW(g, (154 , -64), (105 , -105), (32 , 29), 'black')
