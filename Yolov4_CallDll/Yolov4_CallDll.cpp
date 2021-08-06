// Yolov4_CallDll.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "../Yolov4/Yolov4_dll.h"

#pragma comment(lib, "../x64/Debug/Yolov4.lib")

using namespace std;

void imgCallback(const cv::Mat& img) {
	cout << "imgCallback" << endl;
	cv::imshow("img", img);
}

void main()
{
	// 实例化
	DynamicYolov4* yolo = CreateYolov4();
	int result = yolo->interfaceTest(10, 5);
	cout << "interfaceTest res: " << result << endl;

	// 初始化
	YOLO_RESULT res = yolo->initialize("../Dataset/berrl_classes.txt", "../Weights/detector_berrl2.pt");
	cout << "initialize res: " << res << " errStr: " << yolo->lastErrorStr().c_str() << endl;

	// 预测图片
	//res = yolo->predictImg("../Dataset/5.jpg", true, "../Dataset/predict.jpg");
	//cout << "predictImg res: " << res << " errStr: " << yolo->lastErrorStr().c_str() << endl;

	// 回调预测图片
	res = yolo->predictImgWithCallback("../Dataset/5.jpg", imgCallback);
	cout << "predictImgWithCallback res: " << res << " errStr: " << yolo->lastErrorStr().c_str() << endl;

	cv::waitKey(0);
	cv::destroyAllWindows();

	// 销毁实例
	Yo_DECREF(yolo);
	system("pause");
}

