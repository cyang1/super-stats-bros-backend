#include "tessocr.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <assert.h>

#include <sstream>

using namespace std;
using namespace cv;

TessOCR::TessOCR(double text_thr, const char* whitelist_char,
                 tesseract::PageSegMode psm)
{
    this->api = new tesseract::TessBaseAPI();
    if (api->Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
    }
    api->SetPageSegMode(psm);
    api->SetVariable("tessedit_char_whitelist", whitelist_char);
    this->text_thr = text_thr;
}

void TessOCR::analyze(const Mat& frame, std::vector<OCRCase> &ocr_cases, bool draw_binarization) {
    vector<OCRCase>::iterator it = ocr_cases.begin();
    for (; it != ocr_cases.end(); ++it) {
        Mat sel, gray;
        sel = frame(it->roi);
        if (sel.channels() == 3)
            cvtColor(sel, gray, CV_BGR2GRAY);
        else
            sel.copyTo(gray);

        // Make sure that row step is equal number of rows
        assert(gray.isContinuous());
        // Threshold - keep only the whites...
        threshold(gray, gray, this->text_thr, 255, THRESH_BINARY_INV);
        if (draw_binarization) {
            sel.setTo(Scalar(0,0,0), gray < 100);
            sel.setTo(Scalar(0,255,0), gray > 100);
        }

        this->api->SetImage((uchar*)gray.data, gray.cols, gray.rows, 1, gray.step);
        char* outText = this->api->GetUTF8Text();
        std::string str(outText);
        // trim the text
        str.erase(str.find_last_not_of(" \n\r\t")+1);
        it->update(str, api->MeanTextConf());

        delete outText;
    }
}


TessOCR::~TessOCR()
{
    delete this->api;
}
