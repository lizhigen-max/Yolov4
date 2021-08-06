#pragma once

#ifdef _DLL_EXPORTS
#define DLL_API _declspec(dllexport)
#else
#define DLL_API _declspec(dllimport)
#endif

#include "Yolov4_header.h"

class DynamicYolov4
{
public:

	//**************************************************
	//	@brief   : �ӿڲ���
	//	@author  : Zhigen
	//	@input   : int int
	//	@output  : int(a * b)
	//**************************************************
	virtual YoloAPI_FUNC(int) interfaceTest(int a, int b) = 0;


	//**************************************************
	//	@brief   : ��ʼ��Yolov4
	//	@author  : Zhigen
	//	@input   : classes: ��� .txt�ļ�·�� \
	//             widgets: ģ���ļ� .pt�ļ�·�� \
	//			   gpu_id : -1����CPU��0,1,2 ... ����GPU0, GPU1, GPU2 ... \
	//             width  : ģ��ͼ��� \
	//			   height : ģ��ͼ��� \
	//	@output  : YOLO_RESULT
	//**************************************************
	virtual YoloAPI_FUNC(YOLO_RESULT) initialize(const std::string& classes, const std::string& widgets, 
		int gpu_id = -1, int width = 416, int height = 416) = 0;


	//**************************************************
	//	@brief   : ͼƬԤ��
	//	@author  : Zhigen
	//	@input   : imgPath: ����ͼƬλ�� \
	//             isShow: Ԥ����Ƿ���ʾԤ��ͼƬ \
	//			   savePath : Ԥ���ͼƬ����λ�ã�"" ��ʾ������ \
	//			   conf_thresh��nms_thresh ѵ����ģ��ִ�м�⣬��õ�����������ÿ�����һ��bounding box�ĸ�����
	//			   ���ֵ��ÿ��bounding box����Ӧ�������Ŷȣ���ͨ������������ɸѡԤѡ��
	//	@output  : YOLO_RESULT
	//**************************************************
	virtual YoloAPI_FUNC(YOLO_RESULT) predictImg(const std::string& imgPath, bool isShow = false, const std::string& savePath = "",
		float conf_thresh = 0.1, float nms_thresh = 0.3) = 0;


	//**************************************************
	//	@brief   : �ص�ͼƬԤ��
	//	@author  : Zhigen
	//	@input   : imgPath: ����ͼƬλ�� \
	//             PredictImgCallback: �ص���������ʽ��void Callback(const cv::Mat& img){ ... } \
	//			   conf_thresh��nms_thresh ѵ����ģ��ִ�м�⣬��õ�����������ÿ�����һ��bounding box�ĸ�����
	//			   ���ֵ��ÿ��bounding box����Ӧ�������Ŷȣ���ͨ������������ɸѡԤѡ��
	//	@output  : YOLO_RESULT
	//**************************************************
	virtual YoloAPI_FUNC(YOLO_RESULT) predictImgWithCallback(const std::string& imgPath, PredictImgCallback,
		float conf_thresh = 0.1, float nms_thresh = 0.3) = 0;


	//**************************************************
	//	@brief   : ��ƵԤ��
	//	@author  : Zhigen
	//	@input   : imgPath: ����ͼƬλ�� \
	//             isShow: Ԥ����Ƿ���ʾԤ��ͼƬ \
	//			   savePath : Ԥ���ͼƬ����λ�ã�"" ��ʾ������ \
	//			   conf_thresh��nms_thresh ѵ����ģ��ִ�м�⣬��õ�����������ÿ�����һ��bounding box�ĸ�����
	//			   ���ֵ��ÿ��bounding box����Ӧ�������Ŷȣ���ͨ������������ɸѡԤѡ��
	//	@output  : YOLO_RESULT
	//**************************************************
	virtual YoloAPI_FUNC(YOLO_RESULT) predictVideo(uint8_t cnum = 0, bool isShow = false, const std::string& savePath = "",
		float conf_thresh = 0.1, float nms_thresh = 0.3) = 0;





	//**************************************************
	//	@brief   : ����Yolov4����
	//	@author  : Zhigen
	//	@input   : None
	//	@output  : None 
	//**************************************************
	virtual YoloAPI_FUNC(void) del_yolo() = 0;


	//**************************************************
	//	@brief   : ��һ�δ�����Ϣ
	//	@author  : Zhigen
	//	@input   : None
	//	@output  : None
	//**************************************************
	virtual YoloAPI_FUNC(std::string) lastErrorStr() = 0;
};

/// ����ʵ������
extern "C" DLL_API YoloAPI_INSTANCE(DynamicYolov4* CreateYolov4());


#define Yo_CREATE() CreateYolov4();

#define Yo_CHECK(obj) \
		 DynamicYolov4* o = dynamic_cast<DynamicYolov4*>(obj); \
		 if (nullptr == o) \
			return;

#define Yo_DECREF(obj) \
		Yo_CHECK(obj) \
		obj->del_yolo();

							