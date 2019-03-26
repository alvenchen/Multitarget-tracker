#pragma once

#include "BaseDetector.h"

#define OPENCV
#include "darknet_v3/yolo_v3_class.hpp"

#undef OPENCV

///
/// \brief The Yolo3Detector class
///
class Yolo3Detector : public BaseDetector
{
public:
	Yolo3Detector(bool collectPoints, cv::UMat& colorFrame);
	~Yolo3Detector(void);

	bool Init(const config_t& config);

	void Detect(cv::UMat& colorFrame);

private:
	static const int InWidth = 608;
    static const int InHeight = 608;
	float m_WHRatio;

	std::unique_ptr<Detector> m_detector;

	float m_confidenceThreshold;
	float m_maxCropRatio;
	std::vector<std::string> m_classNames;
};
