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

def draw_graph(ax,count):
    minn = 0
    maxx = -1
    subGrahphs_count = 6

    y = []
    maxx = -1
    print("открыли")

    with open("2.txt", "r") as file:
        string = file.readline()[:-1:]
        y2 = list(map(int,string.split("/")))[-10::]
        print(y2)
        file.close()

    grp.ion()
    grp.plot(range(0,len(y2),1),y2, color = "blue") 
    grp.ylabel('Данные')
    grp.xlabel(f'Номер пакета (от {count-10} до {count})')

    grp.xticks(range(0, len(y2), 1))
    grp.savefig('graph.png')
    show_image('graph.png',ax)
    grp.show(block=False)
    grp.clf()
