from math import sqrt

class Arc:
    def quadrant(self, P):
        (x,y) = P
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
    
    def splitArc(self, P0, P1):
        (x0, y0) = P0
        (x1, y1) = P1
        qi = self.quadrant(P0)
        qf = self.quadrant(P1)
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
    
    # initialize computation for an arc into top right quadrant, centered in (0,0)
    def initArcQ0(self, P0, P1):
        (self.x, self.y) = P0
        (self.x1, self.y1) = P1

        r2 = self.x*self.x + self.y*self.y
        self.r = int(round( sqrt(r2) ))

        # compute equation Ax + By = 0 for (0,0)->(x1,y1)
        # (y1-yc)(x1-x) - (x1-xc)(y1-y) = 0 , with xc=yc=0
        # E = (y1-0)(x1-x) - (x1-0)(y1-y) = y1.x1 - y1.x - x1.y1 + x1.y = x1.y - y1.x
        # initialize E with (x0,y0), and loop until it crosses line, thus change its sign
        self.E = self.x1*self.y - self.y1*self.x
        self.sign = (self.E > 0)

        # d=r^2+r-x^2-y^2-1
        self.d = self.r - 1
        print 'r, x, y, d, E', self.r, self.x, self.y, self.d, self.E

    # return next (dx,dy) movement to draw arc initialize by initArcQ0
    # returns (0,0) when draw ends
    def stepArcQ0(self):
        if self.E == 0 or self.sign != (self.E > 0):
            return (0, 0)
        elif self.d >= 2*self.x : 
            self.d = self.d - 2*self.x - 1
            self.x = self.x+1
            self.E -= self.y1
            return (1, 0)
        elif self.d < 2*(self.r-self.y) :
            self.d = self.d+2*self.y-1
            self.y = self.y-1  
            self.E -= self.x1
            return (0, -1)  
        else: 
            self.d = self.d+2*(self.y-self.x-1)
            self.y = self.y-1
            self.x = self.x+1
            self.E -= self.x1 + self.y1
            return (1, -1)  

    def arcQ0(self, g, P0, P1, Pc = (0,0), color = 'black'):
        (x0, y0) = P0
        (x1, y1) = P1
        (xc, yc) = Pc
        self.initArcQ0((x0 - xc, y0 - yc), (x1 - xc, y1 - yc))
        g.moveTo(x0, y0)
        while True :
            g.draw(color)
            (dx, dy) = self.stepArcQ0()
            if dx == 0 and dy == 0:
                break
            g.moveOf(dx, dy)
        g.draw('yellow')

        g.drawAt(xc, yc, 'green')
        g.drawAt(x0, y0, 'yellow')
        g.drawAt(x1, y1, 'green')

    def arcCW(self, g, P0, P1, Pc = (0,0), color = 'black'):
        self.arc(g, P0, P1, Pc, True, color)

    def arcCCW(self, g, P0, P1, Pc = (0,0), color = 'black'):
        self.arc(g, P0, P1, Pc, False, color)

    def arc(self, g, P0, P1, Pc = (0,0), clockwise = True, color = 'black'):
        (x0, y0) = P0
        (x1, y1) = P1
        (xc, yc) = Pc

        # Counter clockwise -> invert x
        if clockwise:
            PP0 = (x0 - xc, y0 - yc)
            PP1 = (x1 - xc, y1 - yc)
        else:
            PP0 = (xc - x0, y0 - yc)
            PP1 = (xc - x1, y1 - yc)

        todo = self.splitArc(PP0, PP1)
        g.moveTo(x0, y0)
        
        print todo
        l = len(todo)-1
        i = 0
        for part in todo :
            if i == 0:
                if part == 0:
                    Pfrom = PP0
                elif part == 1:
                    Pfrom = (-PP0[1], PP0[0])
                elif part == 2:
                    Pfrom = (-PP0[0], -PP0[1])
                elif part == 3:
                    Pfrom = (PP0[1], -PP0[0])
            else:
                Pfrom = (0, self.x) # last iteration ended at (r, 0), must restart at (0, r)
            if i == l:
                if part == 0:
                    Pto = PP1
                elif part == 1:
                    Pto = (-PP1[1], PP1[0])
                elif part == 2:
                    Pto = (-PP1[0], -PP1[1])
                elif part == 3:
                    Pto = (PP1[1], -PP1[0])
            else:
                Pto = (1,0)
            i+=1

            print part, Pfrom, Pto
            self.initArcQ0(Pfrom, Pto)
            while True :
                g.draw(color)
                (dx, dy) = self.stepArcQ0()
                if dx == 0 and dy == 0:
                    break
                if part == 0:
                    if clockwise:
                        g.moveOf( dx,  dy)
                    else:
                        g.moveOf(-dx,  dy)
                elif part == 1:
                    if clockwise:
                        g.moveOf( dy, -dx)
                    else:
                        g.moveOf(-dy, -dx)
                elif part == 2:
                    if clockwise:
                        g.moveOf(-dx, -dy)
                    else:
                        g.moveOf( dx, -dy)
                elif part == 3:
                    if clockwise:
                        g.moveOf(-dy,  dx)
                    else:
                        g.moveOf( dy,  dx)
            g.draw('yellow')

        g.drawAt(xc, yc, 'green')
        g.drawAt(x0, y0, 'yellow')
        g.drawAt(x1, y1, 'green')
    