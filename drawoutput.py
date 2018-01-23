from Tkinter import *

master = Tk()

w = Canvas(master, width=1000, height=1000)
w.pack()


f=open("polygon.txt","r");

for line in f:
	int_list = [int(j) for j in line.split()]
	w.create_line(int_list[0], int_list[1], int_list[2], int_list[3])
#w.create_line(0, 100, 200, 0)



mainloop()
