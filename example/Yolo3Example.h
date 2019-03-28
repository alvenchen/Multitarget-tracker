#include "VideoExample.h"

/*
live example for human tracking
*/

class Yolo3Example : public VideoExample
{
public:
    Yolo3Example(const cv::CommandLineParser& parser)
        :
          VideoExample(parser)
    {
    }

protected:
    ///
    /// \brief InitTracker
    /// \param grayFrame
    ///
    bool InitTracker(cv::UMat frame)
    {
        m_useLocalTracking = false;
        config_t config;        

#ifdef _WIN32
		std::string pathToModel = "../../data/";
#else
		std::string pathToModel = "../data/";
#endif

        config["modelConfiguration"] = pathToModel + "yolov3.cfg";
        config["modelBinary"] = pathToModel + "yolov3.weights";
        config["classNames"] = pathToModel + "coco.names";
       
       config["dnnTarget"] = "DNN_TARGET_CPU";
       
        config["confidenceThreshold"] = "0.7";
        //config["maxCropRatio"] = "2.0";        
        //config["dnnBackend"] = "DNN_BACKEND_INFERENCE_ENGINE";


        // TODO human detect
        m_detector = std::unique_ptr<BaseDetector>(CreateDetector(tracking::Detectors::Yolo3, config, m_useLocalTracking, frame));
        if (!m_detector.get())
        {
            return false;
        }
        //m_detector->SetMinObjectSize(cv::Size(frame.cols / 40, frame.rows / 40));
        m_detector->SetMinObjectSize(cv::Size(frame.cols / 80, frame.rows / 80));

        TrackerSettings settings;
        settings.m_useLocalTracking = m_useLocalTracking;
        settings.m_distType = tracking::DistRects;
        //settings.m_kalmanType = tracking::KalmanLinear;
        //settings.m_kalmanType = tracking::KalmanAugmentedUnscented;
        settings.m_kalmanType = tracking::KalmanUnscented;
        settings.m_filterGoal = tracking::FilterRect;
        settings.m_lostTrackType = tracking::TrackKCF;       // Use KCF tracker for collisions resolving
        settings.m_matchType = tracking::MatchHungrian;
        settings.m_dt = 0.3f;                                // Delta time for Kalman filter
        settings.m_accelNoiseMag = 0.2f;                     // Accel noise magnitude for Kalman filter
        //settings.m_distThres = frame.rows / 10.f;            // Distance threshold between region and object on two frames
        settings.m_distThres = frame.rows / 5.f;            // Distance threshold between region and object on two frames
        //settings.m_maximumAllowedSkippedFrames = cvRound(2 * m_fps); // Maximum allowed skipped frames
        settings.m_maximumAllowedSkippedFrames = cvRound(m_fps); // Maximum allowed skipped frames
        //settings.m_maxTraceLength = cvRound(5 * m_fps);      // Maximum trace length
        settings.m_maxTraceLength = cvRound(10 * m_fps);      // Maximum trace length

        m_tracker = std::make_unique<CTracker>(settings);

        return true;
    }

    ///
    /// \brief DrawData
    /// \param frame
    ///
    void DrawData(cv::Mat frame, int framesCounter, int currTime)
    {
        if (m_showLogs)
        {
            std::cout << "Frame " << framesCounter << ": tracks = " << m_tracker->tracks.size() << ", time = " << currTime << std::endl;
        }

        for (const auto& track : m_tracker->tracks)
        {
            if (track->IsRobust(1,                           // Minimal trajectory size
                                0.1f,                        // Minimal ratio raw_trajectory_points / trajectory_lenght
                                cv::Size2f(0.1f, 8.0f))      // Min and max ratio: width / height
                    )
            {
                DrawTrack(frame, 1, *track);

                std::string label = track->m_lastRegion.m_type + ": " + std::to_string(track->m_lastRegion.m_confidence);
                int baseLine = 0;
                cv::Size labelSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
                auto rect(track->GetLastRect());
#if (CV_VERSION_MAJOR >= 4)
                cv::rectangle(frame, cv::Rect(cv::Point(rect.x, rect.y - labelSize.height), cv::Size(labelSize.width, labelSize.height + baseLine)), cv::Scalar(255, 255, 255), cv::FILLED);
#else
				cv::rectangle(frame, cv::Rect(cv::Point(rect.x, rect.y - labelSize.height), cv::Size(labelSize.width, labelSize.height + baseLine)), cv::Scalar(255, 255, 255), CV_FILLED);
#endif
                cv::putText(frame, label, cv::Point(rect.x, rect.y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
            }
        }

        m_detector->CalcMotionMap(frame);
    }

    ///
    /// \brief GrayProcessing
    /// \return
    ///
    bool GrayProcessing() const
    {
        return false;
    }
};