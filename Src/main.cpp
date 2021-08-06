
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
	char string[10];  // ���ڴ��֡�ʵ��ַ���

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

		sprintf_s(string, "%.2f", fps);      // ֡�ʱ�����λС��
		std::string fpsString("FPS:");
		fpsString += string;                    // ��"FPS:"�����֡����ֵ�ַ���
		
		// ��֡����Ϣд�����֡��
		cv::putText(frame, // ͼ�����
			fpsString,                  // string����������
			cv::Point(5, 20),           // �������꣬�����½�Ϊԭ��
			cv::FONT_HERSHEY_SIMPLEX,   // ��������
			0.5, // �����С
			cv::Scalar(0, 0, 0));       // ������ɫ


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