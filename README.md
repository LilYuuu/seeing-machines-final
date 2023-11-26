# Seeing Machines Final
Fall 2023

Collaborator: [@yclanlan](https://github.com/yclanlan)


## Progress Update
### Nov. 25
#### People Detection
Used Kinect to track people from above.
* Used depth thresholds to set a range of heights
* Calculated and displayed centroids of contour blobs
  * Reference: https://forum.openframeworks.cc/t/finding-center-of-opencv-blobs/11836/8

#### Clock Mechanism
Display real-time clock.
* Smoothed the movement of the second hand by using the frame rate

#### Shadow Effects
* Turn contour into filled shape (then found just use blob will be easier)
* Draw the shape in Fbo (pixel)
* Manipulate pixel by Dilate, medianBlur, then blur.
* Draw to texture and draw the Fbo out

#### Documentation
Single-user detection

![ezgif com-video-to-gif (2)](https://github.com/LilYuuu/seeing-machines-final/assets/44248733/f72f7f27-2048-48e6-8d83-13822ca60cc4)
---

Multi-user detection

![ezgif com-video-to-gif (1)](https://github.com/LilYuuu/seeing-machines-final/assets/44248733/354626d8-4e72-4a6b-8c06-9fa1bd9cc5ce)
---

Real-time clock

![ezgif com-video-to-gif (3)](https://github.com/LilYuuu/seeing-machines-final/assets/44248733/46e1cf65-dc75-4950-866a-de5f364082bf)
---

Shadow effect


![螢幕錄影 2023-11-25 下午10 23 29](https://github.com/LilYuuu/seeing-machines-final/assets/97862198/b3e58aed-9511-44d0-8aed-09a4b0fc13d4)
---