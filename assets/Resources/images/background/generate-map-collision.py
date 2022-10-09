import imageio.v2 as iio
import numpy as np
import os
import cv2
import imutils
import sys

if len(sys.argv) <= 1:
	print("Few arguments, please enter mode (1 only transform image, 2 only generate collision map, 3 both)")
	exit(1)
elif len(sys.argv) <= 2:
	print("Few arguments, please enter input file")
	exit(1)
elif len(sys.argv) <= 3:
	print("Few arguments, please enter output file")
	exit(1)

mapimage = sys.argv[2]
pic = iio.imread(mapimage)

rows, cols = (pic.shape[0], pic.shape[1])
arr = np.zeros((rows, cols))

if sys.argv[1] == "1" or sys.argv[1] == "3":
	i = 0
	while i < rows:
		j = 0
		while j < cols:
			if pic[i,j,0] == 255 and pic[i,j,1] == 0 and pic[i,j,2] == 255:
				arr[i][j] = 255
			else:
				arr[i][j] = 0
			j += 1
		i += 1
		print((i / rows ) * 100, "%")

	image = iio.imwrite(sys.argv[3], arr)

if sys.argv[1] == "2" or sys.argv[1] == "3":
	image = cv2.imread(sys.argv[3])
	gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

	#create a binary thresholded image from gray image
	ret,thresh = cv2.threshold(gray,127,255,0)

	#find contours in the thresholded image
	contours,hierarchy = cv2.findContours(thresh,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)

	#for all countours get an approximated inscribed rectangle with at least 5 pixels of area and draw them and store them with only the coordinate up left and down right
	rectangles = []
	for cnt in contours:
		x,y,w,h = cv2.boundingRect(cnt)
		if w*h > 5:
			rectangles.append([x,y,x+w,y+h])
			cv2.rectangle(image,(x,y),(x+w,y+h),(0,255,0),2)

	for rect in rectangles:
		rect[0] += int(rect[1]/224) * 2560
		rect[1] = int(rect[1]%224)
		rect[2] += int(rect[3]/224) * 2560
		rect[3] = int(rect[3]%224)

	# sort rectangles by x
	rectangles.sort(key=lambda x: x[0])

	#store the rectangles in a file as json format with the coordinate up left and down right
	with open(sys.argv[3].split(".png")[0] + ".txt", "w") as f:
		f.write(str(len(rectangles)) + "\n")
		for rect in rectangles:
			f.write(str(rect[0]) + " " + str(rect[1]) + " " + str(rect[2]) + " " + str(rect[3]) + str("\n"))



	#draw the rectangles on the new image with dimension 23000x224
	image2 = np.zeros((224,23000,3), np.uint8)
	for rect in rectangles:
		cv2.rectangle(image2,(rect[0],rect[1]),(rect[2],rect[3]),(0,255,0),1)

	#display image2
	cv2.imshow('image',image)
	cv2.waitKey(0)
	# save image2
	iio.imwrite(sys.argv[3] + "_collitionBoxes.png", image2)