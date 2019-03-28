#pragma once

#include "BaseDetector.h"
#include "darknet_v3/yolo_v3_class.hpp"


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
	static const int m_inWidth = 608;
    static const int m_inHeight = 608;

	float m_WHRatio;

	std::unique_ptr<Detector> m_detector;

	float m_confidenceThreshold;
	float m_maxCropRatio;
	
	std::vector<std::string> m_classNames;
};
