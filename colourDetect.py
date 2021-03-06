"""

<Description>
This code detects red objects and calculates the area of said color and connects to the Arduino board.
If the area is bigger than 25000, it gives the input 3 to the Arduino board.

</Description>

<Author>
Anders Karlsson
George Sarkisian
</Author>

<Credits to>
Xiaotian Dai
YunFei Robotics Labrotary
htttp://www.yfworld.com
</Credits to>

"""
# import the necessary packages
from picamera.array import PiRGBArray
from picamera import PiCamera
import time
import cv2
import numpy as np
import io
import serial
import time

ser = serial.Serial('/dev/ttyACM0',9600)
 
# initialize the camera and grab a reference to the raw camera capture
camera = PiCamera()
camera.resolution = (320, 240)
camera.framerate = 24
rawCapture = PiRGBArray(camera, size=(320, 240))
 
# allow the camera to warmup
time.sleep(0.1)
 
# capture frames from the camera
for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
	# grab the raw NumPy array representing the image, then initialize the timestamp
	# and occupied/unoccupied text
	image = frame.array
     
        hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

    # This is for RED tag
        thresh = cv2.inRange(hsv,np.array((120, 80, 80)), np.array((180, 255, 255)))

    # This is for GREEN tag
        #thresh = cv2.inRange(hsv,np.array((50, 80, 80)), np.array((120, 255, 255)))

##        thresh2 = thresh.copy()

    # find contours in the threshold image
        contours,hierarchy = cv2.findContours(thresh, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)

        # finding contour with maximum area and store it as best_cnt
        max_area = 0
        for cnt in contours:
            area = cv2.contourArea(cnt)
            
            if area > max_area:
                max_area = area
                best_cnt = cnt
        if max_area > 12500:
            #time.sleep(5)
            ser.write('r')
            print 'r'
#        print 'red'+str(max_area)
        
        #for blue tag
        #thresh3 = cv2.inRange(hsv,np.array((110, 50, 50)), np.array((130, 255, 255)))

    # This is for GREEN tag
        thresh3 = cv2.inRange(hsv,np.array((40,40,40)), np.array((70,255,255)))

#        thresh4 = thresh3.copy()

    # find contours in the threshold image
        contours2,hierarchy = cv2.findContours(thresh3, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)

        # finding contour with maximum area and store it as best_cnt
        max_area2 = 0
        for cnt2 in contours2:
            area2 = cv2.contourArea(cnt2)
            
            if area2 > max_area2:
                max_area2 = area2
                best_cnt2 = cnt2
        if max_area2 > 12500:
         #   time.sleep(5)
            ser.write('g')
            print 'g'
#        print 'green'+str(max_area2)
        
        
        # finding centroids of best_cnt and draw a circle there     
        #if isset('best_cnt'):
        #    M = cv2.moments(best_cnt)
        #    cx,cy = int(M['m10']/M['m00']), int(M['m01']/M['m00'])
        #    cv2.circle(image,(cx,cy),5,255,-1)
            #print 'Central pos: (%d, %d)' % (cx,cy)
        #else:
            #print "[Warning]Tag lost..."
            
        # Show the original and processed image
#        cv2.imshow('thresh', thresh2)
#        cv2.imshow('thresh3', thresh4)

    
	# show the frame
	##cv2.imshow("Frame", image)
	#key = cv2.waitKey(1) & 0xFF
        
	# clear the stream in preparation for the next frame
	rawCapture.truncate(0)
 
	# if the `q` key was pressed, break from the loop
	#if key == ord("q"):
        #    camera.close();
	#    break
