#include "Yolo3Detector.h"

Yolo3Detector::Yolo3Detector(
	bool collectPoints,
    cv::UMat& colorFrame
	)
    :
      BaseDetector(collectPoints, colorFrame),
      m_WHRatio(m_inWidth / (float)m_inHeight)

{
    
}


Yolo3Detector::~Yolo3Detector(void)
{
}

bool Yolo3Detector::Init(const config_t& config)
{
    auto modelConfiguration = config.find("modelConfiguration");
    auto modelBinary = config.find("modelBinary");
    if (modelConfiguration != config.end() && modelBinary != config.end())
    {
        m_detector = std::make_unique<Detector>(modelConfiguration->second, modelBinary->second);
    }
    
    auto classNames = config.find("classNames");
    if (classNames != config.end())
    {
        std::ifstream classNamesFile(classNames->second);
        if (classNamesFile.is_open())
        {
            m_classNames.clear();
            std::string className;
            for (; std::getline(classNamesFile, className); )
            {
                m_classNames.push_back(className);
            }
        }
    }

    auto confidenceThreshold = config.find("confidenceThreshold");
    if (confidenceThreshold != config.end())
    {
        m_confidenceThreshold = std::stof(confidenceThreshold->second);
    }

    //TODO  need crop ?

    return m_detector.get() != nullptr;
}

void Yolo3Detector::Detect(cv::UMat& colorFrame)
{
    m_regions.clear();

    cv::Mat colorMat = colorFrame.getMat(cv::ACCESS_READ);

    m_detector

}