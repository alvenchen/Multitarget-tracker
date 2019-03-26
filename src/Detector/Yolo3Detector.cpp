#include "Yolo3Detector.h"

Yolo3Detector::Yolo3Detector(
	bool collectPoints,
    cv::UMat& colorFrame
	)
    :
      BaseDetector(collectPoints, colorFrame),
      m_WHRatio(InWidth / (float)InHeight)

{
    
}


Yolo3Detector::~Yolo3Detector(void)
{
}

