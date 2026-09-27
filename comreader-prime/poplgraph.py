import matplotlib.pyplot as grp
from PIL import Image
import math
import serial
from playsound3 import playsound

def show_image(filename,ax):
    img = Image.open(filename)
    ax.imshow(img)
    ax.axis('off')
    grp.draw()
    grp.pause(0.01)

def draw_graph(ax):
    minn = 0
    maxx = -1
    subGrahphs_count = 6

    y = []
    maxx = -1
    print("открыли")
    with open("dannye.txt", "r") as file:
        string = file.readline()[:-1:]
        y = list(map(int,string.split("/")))[-15::]
        print(y)
        file.close()

    for i in range(len(y)):
        y[i] *= 3.14 * 7.6
        if y[i] > maxx:
            maxx = y[i]
    grp.ion()
    grp.plot(range(0,len(y),1),y, color = "red")
    grp.ylabel('Глубина, см')
    grp.xlabel('Номер пакета')

    grp.axis([0,len(y),0,maxx+maxx//10])
    grp.xticks(range(0, len(y), 1))
    grp.savefig('graph.png')
    show_image('graph.png',ax)
    grp.show(block=False)
    grp.clf()
