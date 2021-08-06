
#include"Detector.h"
#include <opencv2/opencv.hpp>

using namespace cv;

int main()
{

	cv::Mat image = cv::imread(R"(C:\Users\Zhigen\Desktop\LibtorchDetection-main\dataset\val\images\0250.jpg)");
	Detector detector;
	detector.Initialize(-1, 416, 416, R"(C:\Users\Zhigen\Desktop\LibtorchDetection-main\dataset\voc_classes.txt)");
	//detector.Train("dataset", ".jpg", 30, 4, 0.001, "weights/detector_", "weights/yolo4_tiny.pt");

	detector.LoadWeight(R"(C:\Users\Zhigen\Desktop\LibtorchDetection-main\weights\detector_0.558029_20210804155809.pt)");

	VideoCapture capture(0);
	const char *name = "video";
	cv::namedWindow(name, 0);
	double fps;
	double t = 0;
	char string[10];  // 用于存放帧率的字符串

	while (true)
	{
		t = (double)cv::getTickCount();
		Mat frame;
		capture >> frame;

		if (frame.empty()) {
			break;
		}

		frame = detector.Predict(frame, true, 0.1);
		if (frame.empty()) {
			break;
		}

		t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
		fps = 1.0 / t;

		sprintf_s(string, "%.2f", fps);      // 帧率保留两位小数
		std::string fpsString("FPS:");
		fpsString += string;                    // 在"FPS:"后加入帧率数值字符串
		
		// 将帧率信息写在输出帧上
		cv::putText(frame, // 图像矩阵
			fpsString,                  // string型文字内容
			cv::Point(5, 20),           // 文字坐标，以左下角为原点
			cv::FONT_HERSHEY_SIMPLEX,   // 字体类型
			0.5, // 字体大小
			cv::Scalar(0, 0, 0));       // 字体颜色


		int h = frame.rows;
		int w = frame.cols;
		
		cv::resizeWindow(name, w / 1, h / 1);
		imshow(name, frame);
		waitKey(1);
	}

	capture.release();

	//speed test
	//int64 start = cv::getTickCount();
	//int loops = 10;
	//for (int i = 0; i < loops; i++) {
	//	detector.Predict(image, false);
	//}
	//double duration = (cv::getTickCount() - start) / cv::getTickFrequency();
	//std::cout << duration/ loops <<" s per prediction" << std::endl;

	return 0;
}