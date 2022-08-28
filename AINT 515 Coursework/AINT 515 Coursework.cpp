// code from Dr Chunxu Li
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
int main()
{

	//for (int k = 1; k <= 3; k++){
		int video_number = 2;

		string VideoFolder = "C:\\Users\\ivanc\\Documents\\Repos\\AINT 515\\Coursework\\\OpenCV\\Videos\\";
		string suffix = ".avi";
		VideoCapture vid_capture(VideoFolder + "Video" + to_string(video_number) + suffix);
		if (!vid_capture.isOpened())
		{
			cout << "Unable to load video" << endl;
		}
		else
		{
			int fps = vid_capture.get(CAP_PROP_FPS);
			cout << "Frames per second: " << fps << endl;
			int frame_count = vid_capture.get(CAP_PROP_FRAME_COUNT);
			cout << "Frame count: " << frame_count << endl;
		}

		int i = 0;
		while (vid_capture.isOpened())
		{
			i = i + 1;
			Mat inputFrame;
			bool captured = vid_capture.read(inputFrame);
			if (captured)
			{
				Mat FrameHSV, FrameFiltered, FrameFiltered2, FrameEdges;
				cvtColor(inputFrame, FrameHSV, COLOR_BGR2HSV);
				cvtColor(inputFrame, FrameEdges, COLOR_BGR2HSV);

				int hue = 18;

				dilate(FrameHSV, FrameHSV, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));
				erode(FrameHSV, FrameHSV, getStructuringElement(MORPH_ELLIPSE, Size(4, 4)));

				inRange(FrameHSV, Vec3b(hue - 18, 0, 0), Vec3b(hue + 18, 255, 255), FrameFiltered);
				inRange(FrameEdges, Vec3b(hue - 18, 0, 0), Vec3b(hue + 18, 255, 255), FrameEdges);

				SimpleBlobDetector::Params params;
				params.filterByArea = true;
				params.minArea = 40;
				params.filterByConvexity = true;
				params.minConvexity = 0.65;
				params.blobColor = 255;
				// Set up detector with params
				Ptr<SimpleBlobDetector> BlobDetector =
					SimpleBlobDetector::create(params);
				// perform detection
				vector<KeyPoint> Blobs;
				BlobDetector->detect(FrameFiltered, Blobs);

				Mat Drawnframe = inputFrame;
				if (video_number != 2)
				{
					drawKeypoints(inputFrame, Blobs, Drawnframe, Scalar(0, 0, 255),
						DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
					// draw the blob centre been detected
					// the centre point is already stored in keypoint vector-Blobs, here	we can directly load it for usage

					for (int n = 0; n < Blobs.size(); ++n)
					{																									 // initialized from 0 and been set as the stop point being the size of the blobs
						circle(Drawnframe, Blobs[n].pt, 5, Scalar(0, 255, 255), 1);										 // common command to draw a circle whose origin is the centre of mass
						line(Drawnframe, Blobs[n].pt + Point2f(0, 7), Blobs[n].pt + Point2f(0, 7), Scalar(0, 255, 255)); // draw a straight line passing through the centre of mass
						line(Drawnframe, Blobs[n].pt + Point2f(7, 0), Blobs[n].pt + Point2f(7, 0), Scalar(0, 255, 255)); // draw a straight line passing through the centre of mass
					}

				}

				Mat FrameEdges2;
				erode(FrameEdges, FrameEdges, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));
				dilate(FrameEdges, FrameEdges, getStructuringElement(MORPH_ELLIPSE, Size(6, 6)));

				Canny(FrameEdges, FrameEdges2, 50, 100);
				cvtColor(FrameEdges2, FrameEdges2, CV_GRAY2BGR);
				Drawnframe += FrameEdges2;

				Mat ImageGray, ImageGray2;
				cvtColor(inputFrame, ImageGray, CV_BGR2GRAY);
				cvtColor(inputFrame, ImageGray2, CV_BGR2GRAY);

				Mat element = getStructuringElement(MORPH_ELLIPSE, Size(2, 2));
				erode(ImageGray, ImageGray, element);

				// dilate(ImageGray, ImageGray, element)));
				vector<Vec3f> circles;




				if (video_number == 2)
				{
					int droplets = 0;
					HoughCircles(ImageGray2, circles, CV_HOUGH_GRADIENT, 1.2, 5, 30, 13, 5, 8);
					for (size_t i = 0; i < circles.size(); i++)
					{
						Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
						int radius = cvRound(circles[i][2]);
						// circle outline
						if (center.y > 80 && center.y < 100 && center.x > 150 && center.x < 820)
						{
							droplets += 1;
							circle(Drawnframe, center, radius, Scalar(255, 100, 255), 3, 8, 0);
						}
					}
					cout << "Frame: " << i << "; Number of droplets: " << droplets << endl;
				}
				else
				{
					HoughCircles(ImageGray, circles, CV_HOUGH_GRADIENT, 1.2, 100, 60, 30, 25, 32);

					for (size_t i = 0; i < circles.size(); i++)
					{
						Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
						int radius = cvRound(circles[i][2]);
						// circle center
						circle(Drawnframe, center, 3, Scalar(0, 255, 0), -1, 8, 0);
						// circle outline
						circle(Drawnframe, center, radius, Scalar(0, 100, 255), 3, 8, 0);
					}
					cout << "Frame: " << i << "; Outer Wraps: " << circles.size() << "; Inner Droplets: " << Blobs.size() << endl;
				}
				imshow("Actual", Drawnframe);
				imshow("Actual", FrameEdges);
				waitKey(10);
			}
			else
			{
				break;
			}
			char c = (char)waitKey(25);
			if (c == 27)
			{
				cout << "Interrupted" << endl;
				break;
			}
		}
		vid_capture.release();
		destroyAllWindows();

		cout << "Video Ended" << endl;
	//}
}
