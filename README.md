# OpenCV-Blob-Detection
OpenCV v3.4.3. was used to recognise different components of 3D printed droplets that were filmed using a high-speed camera. We were tasked to implement computer vision algorithms to recognise the inner droplet and the outer wrap of the droplet. We were also tasked to implement a method to count the number of formed inner droplets per frame and Three videos of the droplets were provided, each with a different set of droplets. Video 1 and Video 3 both had fully formed outer wraps, which isolated the inner droplets. Video 1 and Video 3 differ in the appearance of the inner droplets, with video 3 having more translucent droplets. Video 2 was significantly different from the other videos as the inner droplets have no outer wrap. This meant that the inner droplets were not isolated from each other and were physically connected.
### Video 1:
![Video 1](./Images/Video%201%20Actual.png)
### Video 2:
![Video 2](./Images/Video%202%20Actual.png)
### Video 3:
![Video 3](./Images/Video%203%20Actual.png)

# Design
## Centre of Mass/Blob Detection
The first step in the design was to implement a method to detect the inner droplets. The method was to use blob detection to detect the inner droplets. First the OpenCV function cvtColor was used to convert the image from BGR to HSV colour space. We then dilated and eroded the image to remove noise and make the inner droplets more distinguishable. This is particularly important for the processing of Video 2 as this process allowed the inner droplets to be isolated from each other.

### Video 2 without dilating and eroding:
![Video 2 without dilating and eroding](./Images/Video2%20no%20erode.png)

### Video 2 with dilating and eroding:
![Video 2 with dilating and eroding](./Images/Video2%20with%20erode.png)

Then, the inRange function was used to isolate the inner 
droplets. The inRange works by setting all pixels in the image to zero except for the pixels that 
are within the range that is specified by the lower and upper parameters. In this case, 
the lower and upper bounds were set to the HSV colour space values that were determined to be the inner 
droplet colour. Finally, the blobDetector function was used to detect the inner droplets. 


## Detecting the inner wraps with Canny
Using the Canny function, we were able to detect the edges of the inner droplets. In a similar process
to Blob Detection, This was done by
converting the frame into the HSV colour space before using the inRange function to isolate the
inner droplets. The image was then eroded and dilated to remove noise and reduce the size of each droplet. 
This was especially needed for the detection of individual droplets on Video2 as this process separated each droplet
allowing canny to find the complete edge around each droplet. 

## Detecting the inner droplets with HoughCircles
While using canny worked well for Videos 1 and 3 (as seen in the results section), it was still not ideal for the detection of the inner droplets 
in video 2, as many of the edges between droplets got merged. 

To solve this, a specialised inner droplet detection method was implemented, which used the 
HoughCircles function instead of the Canny function. This function was used to isolate complete
circles instead of just edges allowing for the separation of each droplet.
First, the frame was converted to grayscale using the cvtColor function. Then, the HoughCircles
the function was applied. The parameters had to be tuned only to detect small circles and were found through trial and
error. While the function could locate the droplets, many false circles were found.

### Many false circles are detected when using HoughCircles:
![Many false circles are detected when using HoughCircles](./Images/Video%202%20False%20Circles.png)

To resolve this issue, a bounding box around the droplet area was used to only display circles within the area of
the droplets. 

### False circles removed using the bounding box:
![False circles removed using the bounding box](./Images/Video%202%20Removed%20False%20Circles.png)

## Detecting the outer wrap
The outer wrap was detected using the HoughCircles function. The image had to first be converted to grayscale 
using the cvtColor function. The HoughCircles function was then applied to the image. 
The parameters had to be tuned to only detect large circles.

## Counting the formed droplets
The droplets were detected by totalling the number of detected blobs from blob detection for videos 1 and 3. 

# Results
## Centre of Mass/Blob Detection
The blob detection was able to successfully detect the inner blobs for videos 1 and 3 to a high level of accuracy.

### Blob Detection on video 1 to find centre of mass (red and yellow ring is centre of mass):
![Blob Detection on video 1 to find centre of mass](./Images/Video%203%20blob%20detection.png)

## Blob Detection on video 3 to find centre of mass:
![Blob Detection on video 3 to find centre of mass](./Images/Video%201%20blob%20detection.png)

The use of blob detection on video 2 was not successful as the inner droplets were not isolated from each other.
This meant that it struggled to identify individual droplets resulting in a large number of unregistered droplets and 
incorrectly detected droplets.
Furthermore, the size of each droplet was small meaning that it was more difficult to separate them through preprocessing.

### Blob Detection on video 2 to find centre of mass:
![Blob Detection on video 2 to find centre of mass](./Images/Video%202%20blob%20detection.png)

## Detecting the inner droplets with Canny:
Canny found greater success in the detection of the inner droplets in video 2 than blob detection, however it does not
identify the centre of mass. Canny works well in detection of the inner droplets for all of the videos, however 
on video 2, many droplets are connected. 

### Video 1 edge detection with canny works well:
![Video 1 edge detection with canny works well](./Images/Video%201%20Canny.png)

### Video 2 edge detection with canny merges some droplets:
![ Video 2 edge detection with canny merges some droplets](./Images/Video%202%20Canny.png)

### Video 3 edge detection with canny:
![Video 3 edge detection with canny](./Images/Video%203%20canny.png)

## Detecting the droplets of Video 2 with HoughCircles:
The detection and counting of the droplets in Video 2 was solved by the use of HoughCircles.
Given the correct settings and the use of a bounding box, the HoughCircles function was able to successfully
identify each individual droplet to a high level of accuracy that was unachievable using canny or blob detection.

### Droplet detection of video 2 using HoughCircles:
![Droplet detection of video 2 using HoughCircles](./Images/Video%202%20Removed%20False%20Circles.png)

## Counting the droplets
The droplets were counted by totalling the number of detected blobs from blob detection for videos 1 and 3.
The droplets in video 2 were counted by totalling the number of circles per frame.

### Screenshot showing droplets being counted on video 1:
![Screenshot showing droplets being counted on video 1](./Images/Video%201%20number%20of%20droplets.png)

### Screenshot of droplets being counted on video 2:
![Screenshot showing droplets being counted on video 2](./Images/Video%202%20number%20of%20droplets.png)

### Screenshot of droplets being counted on video 3:
![Screenshot showing droplets being counted on video 3](./Images/Video%203%20number%20of%20droplets.png)

## Conclusion
The results are shown in the results section of the project document. The results show that the algorithm 
was able to successfully identify the centre of mass of the droplets in Video 1 and 3. However, the centre of mass
was not accurate enough to identify the centre of mass of the droplets in Video 2. The use of HoughCircles on
Video 2 was able to identify the centre of mass of the droplets successfully. Only Video 1 and 3 contained outer wraps.
These were correctly identified and circled. The number of droplets in each video was also successfully counted.
