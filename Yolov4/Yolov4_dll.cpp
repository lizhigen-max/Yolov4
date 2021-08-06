
#include <iostream>

#define _DLL_EXPORTS
#include "Yolov4_dll.h"
#include "../Src/Detector.h"
#include <opencv2/opencv.hpp>

using namespace std;

class Yolov4 : public DynamicYolov4
{
public:
	Yolov4();
	~Yolov4();

	virtual int interfaceTest(int a, int b);
	virtual YOLO_RESULT initialize(const std::string& classes, const std::string& widgets, 
		int gpu_id = -1, int width = 416, int height = 416);

	virtual YOLO_RESULT predictImg(const std::string& imgPath, bool isShow = false, const std::string& savePath = "",
		float conf_thresh = 0.3, float nms_thresh = 0.3);
	virtual YOLO_RESULT predictImgWithCallback(const std::string& imgPath, PredictImgCallback callback, 
		float conf_thresh = 0.1, float nms_thresh = 0.3);

	virtual YOLO_RESULT predictVideo(uint8_t cnum = 0, bool isShow = false, const std::string& savePath = "",
		float conf_thresh = 0.1, float nms_thresh = 0.3);

	virtual void del_yolo();
	virtual std::string lastErrorStr();

private:
	Detector detector;
};

Yolov4::Yolov4()
{
	cout << "Yolov4 constructor" << endl;
}

Yolov4::~Yolov4()
{
	cout << "Yolov4 destructor" << endl;
}


int Yolov4::interfaceTest(int a, int b)
{
	return a * b;
}

YOLO_RESULT Yolov4::initialize(const std::string& classes, const std::string& widgets, int gpu_id, int width, int height)
{
	YOLO_RESULT res = detector.Initialize(gpu_id, width, height, classes);
	if (res != YO_SUCCESS)
		return res;

	return detector.LoadWeight(widgets);
}

YOLO_RESULT Yolov4::predictImg(const std::string& imgPath, bool isShow, const std::string& savePath, float conf_thresh, float nms_thresh)
{
	if (!does_exist(imgPath)) {
		detector.setErrStr(imgPath + " does not exist.");
		return YO_ERROR;
	}

	cv::Mat image = cv::imread(imgPath);
	if (image.empty()) {
		detector.setErrStr(imgPath + " readed empty Mat.");
		return YO_FAIL;
	}

	try {
		detector.Predict(P_IMG, image, isShow, savePath, conf_thresh, nms_thresh);
	}
	catch (std::exception&e) {
		detector.setErrStr(e.what());
		return YO_ERROR;
	}
	
	return YO_SUCCESS;
}

YOLO_RESULT Yolov4::predictImgWithCallback(const std::string& imgPath, PredictImgCallback callback, float conf_thresh, float nms_thresh)
{
	//detector.Train("C:/Users/Zhigen/Desktop/LibtorchDetection-main/dataset", ".jpg", 50, 4, 0.001, 
	//	R"(C:\Users\Zhigen\Desktop\LibtorchDetection-main\weights\detector_)", 
	//	R"(C:\Users\Zhigen\Desktop\LibtorchDetection-main\weights\yolo4_tiny.pt)");

	if (!does_exist(imgPath)) {
		detector.setErrStr(imgPath + " does not exist.");
		return YO_ERROR;
	}

	cv::Mat image = cv::imread(imgPath);
	if (image.empty()) {
		detector.setErrStr(imgPath + " readed empty Mat.");
		return YO_FAIL;
	}

	try {
		cv::Mat frame = detector.Predict(P_CALLBACK, image, false, "", conf_thresh, nms_thresh);
		if (detector.laseErrStr().empty())
			callback(frame);
		else
			return YO_FAIL;
	}
	catch (std::exception&e) {
		detector.setErrStr(e.what());
		return YO_ERROR;
	}

	return YO_SUCCESS;
}

YOLO_RESULT Yolov4::predictVideo(uint8_t cnum, bool isShow, const std::string& savePath,
	float conf_thresh, float nms_thresh)
{
	return YO_SUCCESS;
}

std::string Yolov4::lastErrorStr()
{
	return detector.laseErrStr();
}

void Yolov4::del_yolo()
{
	delete this;
}



extern "C" DLL_API DynamicYolov4* CreateYolov4()
{
	return new Yolov4();
}