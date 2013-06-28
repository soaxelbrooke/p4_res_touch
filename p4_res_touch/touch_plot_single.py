# pressurePlot.py

from threading import Timer
from sys import version_info as ver
import Tkinter as tk
import serial

class pointDrawer:

    def __init__(self, master):

        # create serial com object
        self.s = serial.Serial(11, timeout = 0.01, writeTimeout = 0.01)
        self.s.baudrate = 9600

        # create variables for press location
        self.xPress = 0.5
        self.yPress = 0.5
        self.zPress = 1

        self.x_filter = [0 for a in range(2)]
        self.y_filter = [0 for a in range(2)]
        self.z_filter = [0 for a in range(2)]

        # define canvas width and height
        self.canvasWidth = 400
        self.canvasHeight = 640
        
        frame = tk.Frame(master)
        frame.pack()

        # setup screen rectangle
        self.myCanvas = tk.Canvas(frame, height = self.canvasHeight,
                                  width = self.canvasWidth,
                                  background = 'white')
        self.myCanvas.grid(row = 0, column = 0, rowspan = 1, columnspan = 1)

        # setup exit button
        self.exitButton = tk.Button(frame, text = "Quit",
                                    command = self.quitThis)
        self.exitButton.grid(row = 1, column = 0)

        self.moveCircle()

    def moveCircle(self):
        # retrieve measurements from com port
        self.s.write('t')
        # Read 22 bytes from com port (x, y, and z, two bytes each, and int16
        # sensor readings)
        measString = [self.s.read() for a in range(22)]
        if measString[0] != '':
            if len(measString) == 22:
                measurements = self.measStringConvert(measString)
                print("X = " + str(measurements[0]) +
                      "\tY = " + str(measurements[1]) +
                      "\tZ = " + str(measurements[2]) +
                      "\tRaw: " + str(measurements[3:]))
                # calculate locaiton and amount of measured press
                x, y, z = measurements[:3]
                z = z if (z > 40) else 0
                z = (z/6)**2
                self.setPress(x/256.0, y/256.0, z)

        self.t = Timer(0.016/4.0, self.moveCircle)
        self.t.start()

    def updatePlot(self):
        # Clear canvas contents to prepare for next circle draw
        self.myCanvas.delete("all")

        if (self.zPress != 0):
            # Define new circle
            drawX = self.xPress*self.canvasWidth
            drawY = self.yPress*self.canvasHeight
            circleRad = self.zPress/float(pow(2, 3))

            # Draw circle
            self.myCanvas.create_oval(int(drawX+circleRad), int(drawY-circleRad),
                                      int(drawX-circleRad), int(drawY+circleRad),
                                      fill = 'blue')
        
    def measStringConvert(self, measStringList):
        ret_list = [ord(measStringList[2*i]) + 256*ord(measStringList[2*i+1])
                    for i in range(11)]
        
#        retList = [0, 0, 0] # x, y, z
#        i = 0

#        retList[0] = ord(measStringList[0]) + 256*ord(measStringList[1])
#        retList[1] = ord(measStringList[2]) + 256*ord(measStringList[3])
#        retList[2] = ord(measStringList[4]) + 256*ord(measStringList[5])
        
        return ret_list

    def setPress(self, x, y, z):
        self.x_filter.pop(0)
        self.x_filter.append(x)
        self.y_filter.pop(0)
        self.y_filter.append(y)
        self.z_filter.pop(0)
        self.z_filter.append(z)
        
        self.xPress = sum(self.x_filter)/float(len(self.x_filter))
        self.yPress = sum(self.y_filter)/float(len(self.y_filter))
        self.zPress = sum(self.z_filter)/float(len(self.z_filter))
        self.updatePlot()

    def quitThis(self):
        self.s.close()
        exit()


root = tk.Tk()
testGUI = pointDrawer(root)
root.mainloop()
