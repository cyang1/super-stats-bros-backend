#ifndef GAMEOCR_H
#define GAMEOCR_H

#include <vector>
#include <tesseract/baseapi.h>
#include <opencv2/core/core.hpp>

/**
 * @brief Structure holding definition and results of an OCR case
 */
struct OCRCase{
    cv::Rect roi; /** ROI in the input frame (absolute coordinates) */
    std::string text; /** The detected text */
    int confidence; /** Confidence of the text (int, 0-100, 100 tha best) */
    const int min_confidence; /** Minimal confidence to update the text */

    OCRCase(cv::Rect roi, std::string text, int min_confidence) :
        roi(roi), text(text), confidence(100), min_confidence(min_confidence)
        {}

    /**
     * @brief Update the text results only when confidence higher than min_confidence
     */
    void update(std::string text, int confidence) {
        if (confidence > this->min_confidence) {
            this->confidence = confidence;
            this->text = text;
        }
    }
};

/**
 * @brief Thinniest wrapper around tesseract OCR
 */
class TessOCR
{
public:
    /**
     * @brief Construct a game screen text recogniser. Assumes the text is brighter in background
     * in the binarization operation (simple threshold over some value).
     * @param text_thr Minimal text brightnes in GrayScale for the foreground segmentation
     * @param whitelist_char String of allowed characters
     * @param psm Tesseract mode, default PSM_SINGLE_WORD
     */
    TessOCR(double text_thr=200, const char* whitelist_char="0123456789",
            tesseract::PageSegMode psm=tesseract::PSM_SINGLE_WORD);
    ~TessOCR();

    /**
     * @brief Perform the OCR, calls update on each OCR case.
     * @param frame Input image frame
     * @param ocr_cases List of OCR cases
     * @param draw_binarization Do draw binarization results to the input frame, false by default
     */
    void analyze(const cv::Mat& frame, std::vector<OCRCase>& ocr_cases, bool draw_binarization=false);
protected:
    bool is_initialized;
    tesseract::TessBaseAPI *api;
    double text_thr;
};

#endif // GAMEOCR_H
