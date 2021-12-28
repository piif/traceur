from Tkinter import *

class Grille:
    size = 0
    pixelSize = 6
    canvas = None

    X = 0
    Y = 0

    def __init__(self, root, size, pixelSize=6):
        self.size = size
        self.pixelSize = pixelSize
        self.winSize = (size + 1) * self.pixelSize
        self.centerX = self.pixelSize / 2
        self.centerY = self.winSize + self.pixelSize / 2

        frame = Frame(root, width = self.winSize, height = self.winSize)
        frame.pack(expand = True, fill=BOTH)
         
        self.canvas = Canvas(frame, bg = 'white', width = self.winSize, height = self.winSize)
         
        self.canvas.pack(expand = True, fill = BOTH)
        self.draw('gray')

    def moveTo(self, x, y):
        self.X = x
        self.Y = y

    def moveOf(self, dx, dy):
        self.X += dx
        self.Y += dy

    def draw(self, color):
        self.drawAt(self.X, self.Y, color)

    def convert(self, P):
        (x, y) = P
        return (self.centerX + x * self.pixelSize, self.centerY - y * self.pixelSize)

    def drawAt(self, x, y, color):
        (x0, y0) = self.convert((x, y))
        x1 = x0 + (self.pixelSize - 1)
        y1 = y0 - (self.pixelSize - 1)
        self.canvas.create_rectangle(x0, y0, x1, y1, fill = color, outline = color, width = 1)

    def line(self, points, color):
        (x1, y1) = self.convert(points[0])
        for i in range(1, len(points)) :
            (x0, y0) = (x1, y1)
            (x1, y1) = self.convert(points[i])
            self.canvas.create_line(x0, y0, x1, y1, fill=color, width=1)

    def clear(self):
        self.canvas.delete('all')
        self.moveTo(0, 0)
        self.draw('gray')