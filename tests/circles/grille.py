from Tkinter import *

class Grille:
    size = 0
    pixelSize = 6
    canvas = None

    X = 0
    Y = 0

    def __init__(self, root, size):
        self.size = size
        self.winSize = (size * 2 + 2) * self.pixelSize
        self.center = (self.winSize - self.pixelSize) / 2

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

    def drawAt(self, x, y, color):
        x0 = self.center + x * self.pixelSize
        y0 = self.center - y * self.pixelSize
        x1 = x0 + (self.pixelSize - 1)
        y1 = y0 - (self.pixelSize - 1)
        self.canvas.create_rectangle(x0, y0, x1, y1, fill = color, width = 0)

    def clear(self):
        self.canvas.delete('all')
        self.moveTo(0, 0)
        self.draw('gray')