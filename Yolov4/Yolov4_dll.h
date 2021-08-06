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
	//	@brief   : 接口测试
	//	@author  : Zhigen
	//	@input   : int int
	//	@output  : int(a * b)
	//**************************************************
	virtual YoloAPI_FUNC(int) interfaceTest(int a, int b) = 0;


	//**************************************************
	//	@brief   : 初始化Yolov4
	//	@author  : Zhigen
	//	@input   : classes: 类别 .txt文件路径 \
	//             widgets: 模型文件 .pt文件路径 \
	//			   gpu_id : -1代表CPU，0,1,2 ... 代表GPU0, GPU1, GPU2 ... \
	//             width  : 模型图像宽 \
	//			   height : 模型图像高 \
	//	@output  : YOLO_RESULT
	//**************************************************
	virtual YoloAPI_FUNC(YOLO_RESULT) initialize(const std::string& classes, const std::string& widgets, 
		int gpu_id = -1, int width = 416, int height = 416) = 0;


	//**************************************************
	//	@brief   : 图片预测
	//	@author  : Zhigen
	//	@input   : imgPath: 本地图片位置 \
	//             isShow: 预测后是否显示预测图片 \
	//			   savePath : 预测后图片保存位置，"" 表示不保存 \
	//			   conf_thresh、nms_thresh 训练好模型执行检测，会得到多组检测结果，每组包括一个bounding box四个坐标
	//			   相关值和每个bounding box所对应类别的置信度，可通过这两个参数筛选预选框
	//	@output  : YOLO_RESULT
	//**************************************************
	virtual YoloAPI_FUNC(YOLO_RESULT) predictImg(const std::string& imgPath, bool isShow = false, const std::string& savePath = "",
		float conf_thresh = 0.1, float nms_thresh = 0.3) = 0;


	//**************************************************
	//	@brief   : 回调图片预测
	//	@author  : Zhigen
	//	@input   : imgPath: 本地图片位置 \
	//             PredictImgCallback: 回调函数，格式：void Callback(const cv::Mat& img){ ... } \
	//			   conf_thresh、nms_thresh 训练好模型执行检测，会得到多组检测结果，每组包括一个bounding box四个坐标
	//			   相关值和每个bounding box所对应类别的置信度，可通过这两个参数筛选预选框
	//	@output  : YOLO_RESULT
	//**************************************************
	virtual YoloAPI_FUNC(YOLO_RESULT) predictImgWithCallback(const std::string& imgPath, PredictImgCallback,
		float conf_thresh = 0.1, float nms_thresh = 0.3) = 0;


	//**************************************************
	//	@brief   : 视频预测
	//	@author  : Zhigen
	//	@input   : imgPath: 本地图片位置 \
	//             isShow: 预测后是否显示预测图片 \
	//			   savePath : 预测后图片保存位置，"" 表示不保存 \
	//			   conf_thresh、nms_thresh 训练好模型执行检测，会得到多组检测结果，每组包括一个bounding box四个坐标
	//			   相关值和每个bounding box所对应类别的置信度，可通过这两个参数筛选预选框
	//	@output  : YOLO_RESULT
	//**************************************************
	virtual YoloAPI_FUNC(YOLO_RESULT) predictVideo(uint8_t cnum = 0, bool isShow = false, const std::string& savePath = "",
		float conf_thresh = 0.1, float nms_thresh = 0.3) = 0;





	//**************************************************
	//	@brief   : 销毁Yolov4对象
	//	@author  : Zhigen
	//	@input   : None
	//	@output  : None 
	//**************************************************
	virtual YoloAPI_FUNC(void) del_yolo() = 0;


	//**************************************************
	//	@brief   : 上一次错误信息
	//	@author  : Zhigen
	//	@input   : None
	//	@output  : None
	//**************************************************
	virtual YoloAPI_FUNC(std::string) lastErrorStr() = 0;
};

/// 生成实例对象
extern "C" DLL_API YoloAPI_INSTANCE(DynamicYolov4* CreateYolov4());


#define Yo_CREATE() CreateYolov4();

#define Yo_CHECK(obj) \
		 DynamicYolov4* o = dynamic_cast<DynamicYolov4*>(obj); \
		 if (nullptr == o) \
			return;

#define Yo_DECREF(obj) \
		Yo_CHECK(obj) \
		obj->del_yolo();

							