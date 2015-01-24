#include <jni.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <vector>

using namespace std;
using namespace cv;

static double angle (Point pt1, Point pt2, Point pt0) {
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;

	return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

extern "C" {
JNIEXPORT void JNICALL Java_com_application_detector_RoadSignDetector_findFeatures (JNIEnv*, jobject, jlong addrGray, jlong addrRgba);
JNIEXPORT void JNICALL Java_com_application_detector_RoadSignDetector_detect (JNIEnv*, jobject, jlong addrRgba, jlong croppedImage);

JNIEXPORT void JNICALL Java_com_application_detector_RoadSignDetector_findFeatures (JNIEnv*, jobject, jlong addrGray, jlong addrRgba) {
    Mat& mGr  = *(Mat*)addrGray;
    Mat& mRgb = *(Mat*)addrRgba;

    vector<KeyPoint> v;

    FastFeatureDetector detector(50);
    detector.detect(mGr, v);
    for (unsigned int i = 0; i < v.size(); i++) {
        const KeyPoint& kp = v[i];
        circle(mRgb, Point(kp.pt.x, kp.pt.y), 10, Scalar(255,0,0,255));
    }
}

JNIEXPORT void JNICALL Java_com_application_detector_RoadSignDetector_detect (JNIEnv *env, jobject obj, jlong addrRgba, jlong croppedImage) {
	Mat& croppedMat  = *(Mat*)croppedImage;
	Mat& src = *(Mat*)addrRgba;

	Mat dst1;
	Mat hsv;
	Mat bw;

	Size sizeI = Size(3,3);

	GaussianBlur(src, dst1, sizeI, 3, 0.0, 4);

	cvtColor(dst1, hsv, CV_BGR2HSV);

	// segmentare imagine => imagine binara
	//limita experimentala
	inRange(hsv, Scalar(1,108,55), Scalar(180,225,255), bw);

	vector<vector<Point> > contours;
	vector<Point> cont;
	vector<vector<Point> >shapes;
	findContours(bw, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	Mat dst = Mat::zeros(src.size(), src.type());

	double area;
	std::vector<cv::Point> approx;

		for (std::size_t i = 0; i < contours.size(); i++) {
			//limita experimentala (filtrezi contrurile mici)
			if (contours.front().size() > 10) {

				cv::InputArray imgs = contours[i];
				area = contourArea(imgs);

				// limita experimentala (filtrezi dupa dimensiunea ariei)
				if (area > 50) {
					Rect roi = boundingRect(imgs);

	                // aproximeaza countururile la cea mai apropiata forma folosind cat mai putine drepte
					cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.025, true);

					// Skip small or non-convex objects
					int vtc = approx.size();

					if (vtc == 3) {
						// Triangles
						//printf("triangle ");

						// Get the cosines of all corners
						std::vector<double> cos;
						int z = 0;
						for (int j = 2; j < vtc+1; j++) {
							Point_<int> p1 = approx[j%vtc];
							Point_<int> p2 = approx[j-2];
							Point_<int> p3 = approx[j-1];

							double an = angle(p1, p2, p3);
							//printf(" angle %d: %f", j, an);
							if(an > 0.38 && an < 0.62) {
								z++;
							}
						}

						//printf("\n");
						if (z >= 2) {
							vector<Point> temp = contours[i];
							shapes.push_back(temp);
						}
					}
					else if (vtc >= 4 && vtc <= 6) {
						// Number of vertices of polygonal curve

						// Get the cosines of all corners
						std::vector<double> cos;
						for (int j = 2; j < vtc+1; j++) {
							Point_<int> p1 = approx[j%vtc];
							Point_<int> p2 = approx[j-2];
							Point_<int> p3 = approx[j-1];

							cos.push_back(angle(p1, p2, p3));
						}

						// Sort ascending the cosine values
						std::sort(cos.begin(), cos.end());

						// Get the lowest and the highest cosine
						double mincos = cos.front();
						double maxcos = cos.back();

						// Use the degrees obtained above and the number of vertices
						// to determine the shape of the contour
						if (vtc == 4 && mincos >= -0.1 && maxcos <= 0.3) {
						//	printf("rectangle ");
							vector<Point> temp = contours[i];
							shapes.push_back(temp);
						}
						else if (vtc == 6 && mincos >= -0.55 && maxcos <= -0.45) {
						//	printf("hexagon");
							vector<Point> temp = contours[i];
							shapes.push_back(temp);
						}
						//printf(" ceva: %d", vtc);
					}
					else {
						// Detect and label circles
						double area = cv::contourArea(imgs);
						cv::Rect r = cv::boundingRect(imgs);
						int radius = r.width / 2;


						// Get the cosines of all corners
						std::vector<double> cos;
						for (int j = 2; j < vtc+1; j++) {
							Point_<int> p1 = approx[j%vtc];
							Point_<int> p2 = approx[j-2];
							Point_<int> p3 = approx[j-1];

							cos.push_back(angle(p1, p2, p3));
						}

						// Sort ascending the cosine values
						std::sort(cos.begin(), cos.end());

						// Get the lowest and the highest cosine
						double mincos = cos.front();
						double maxcos = cos.back();

						if (std::abs(1 - ((double)r.width / r.height)) <= 0.2 &&
							std::abs(1 - (area / (CV_PI * pow(radius,(float)2)))) <= 0.2) {
							//printf("circle ");
							vector<Point> temp = contours[i];
							shapes.push_back(temp);
						}
						else if (vtc == 6 && mincos >= -0.55 && maxcos <= -0.45) {
							vector<Point> temp = contours[i];
							shapes.push_back(temp);
						}
					}
				}
			}
		}

		for (int i = 0; i < shapes.size(); i++) {
			cv::InputArray imgs = shapes[i];
			Rect roi = boundingRect(imgs);

			rectangle(src, roi.tl(), roi.br(), cv::Scalar(10, 255, 25), 2, CV_AA);
		}
	}
}
