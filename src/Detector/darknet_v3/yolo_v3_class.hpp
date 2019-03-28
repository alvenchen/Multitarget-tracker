#pragma once
#ifdef YOLODLL_EXPORTS
#if defined(_MSC_VER)
#define YOLODLL_API __declspec(dllexport)
#else
#define YOLODLL_API __attribute__((visibility("default")))
#endif
#else
#if defined(_MSC_VER)
#define YOLODLL_API __declspec(dllimport)
#else
#define YOLODLL_API
#endif
#endif

#include<string>
#include<deque>
#include <vector>

namespace yolo_v3{

struct bbox_t {
    unsigned int x, y, w, h;    // (x,y) - top-left corner, (w, h) - width & height of bounded box
    float prob;                    // confidence - probability that the object was found correctly
    unsigned int obj_id;        // class of object - from range [0, classes-1]
    unsigned int track_id;        // tracking id for video (0 - untracked, 1 - inf - tracked object)
    unsigned int frames_counter;// counter of frames on which the object was detected
};


#define C_SHARP_MAX_OBJECTS 2000
struct bbox_t_container {
    bbox_t candidates[C_SHARP_MAX_OBJECTS];
};


class Detector {
    network *m_net;
    
public:
    float m_nms = .45;

    YOLODLL_API Detector(std::string cfg_filename, std::string weight_filename, int gpu_id = 0);
    YOLODLL_API ~Detector();

    YOLODLL_API std::vector<bbox_t> detect(image img, float thresh = 0.2, bool use_mean = false);
    

};

}