#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

enum YOLO_RESULT
{
	YO_SUCCESS = 0,
	YO_FAIL,
	YO_ERROR
};

// »Øµ÷Ô¤²âÍ¼Æ¬
typedef void(*PredictImgCallback)(const cv::Mat& img);

#define CLEAR_STR(str) str.clear()

#define YoloAPI_FUNC(type) type
#define YoloAPI_INSTANCE(cls) cls