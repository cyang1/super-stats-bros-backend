#include <opencv2/opencv.hpp>
#include <iostream>

#include "imgproc/tessocr.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    if( argc != 2)
    {
     cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
     return -1;
    }
    VideoCapture cap(argv[1]); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    TessOCR ocr(150);
    vector<OCRCase> ocr_cases;
    int min_confidence = 90;
    ocr_cases.push_back(OCRCase(Rect(267, 37, 40, 25), "00", min_confidence)); // Time min
    ocr_cases.push_back(OCRCase(Rect(315, 37, 40, 25), "00", min_confidence)); // Time sec
    //ocr_cases.push_back(OCRCase(Rect(356, 43, 30, 20), "00", min_confidence)); // Time msec*10

    ocr_cases.push_back(OCRCase(Rect(140, 300, 55, 40), "0", min_confidence)); // Hits player 1
    ocr_cases.push_back(OCRCase(Rect(440, 300, 55, 40), "0", min_confidence)); // Hits player 2

    namedWindow("edges",1);
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        ocr.analyze(frame, ocr_cases);

        for (int ri = 0; ri < ocr_cases.size(); ++ri) {
            OCRCase& res = ocr_cases[ri];
            rectangle(frame, res.roi, Scalar(255, 0, 0));
            putText(frame, res.text, Point(res.roi.x, res.roi.y), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0));
        }

        imshow("frame", frame);
        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}

