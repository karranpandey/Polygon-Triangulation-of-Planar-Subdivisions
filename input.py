import Tkinter as tk
root = tk.Tk()
def mmove(event):
    print(event.x, event.y)
root.bind('<Motion>', mmove)
root.mainloop()
