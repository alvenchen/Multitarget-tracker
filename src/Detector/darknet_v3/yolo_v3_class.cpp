#include "yolo_v3_class.hpp"
#include "network.h"

using namespace yolo_v3;
using namespace std;

YOLODLL_API Detector::Detector(std::string cfg_filename, std::string weight_filename, int gpu_id)
{
    gpu_index = gpu_id;

    m_net = load_network(const_cast<char *>(cfg_filename.c_str()), const_cast<char *>(weight_filename.c_str()), 0);
    set_batch_network(m_net, 1);

}

YOLODLL_API Detector::~Detector()
{
    free_network(m_net);
}

YOLODLL_API std::vector<bbox_t> Detector::detect(image img, float thresh, bool use_mean)
{
    image sized = letterbox_image(img, m_net->w, m_net->h);

    float *X = sized.data;

    float *prediction = network_predict(m_net, X);

    int nboxes = 0;
    detection *dets = get_network_boxes(m_net, img.w, img.h, thresh, 0.5, 0, 1, &nboxes);

    // the last fc
    layer l = m_net->layers[m_net->n-1];
    if (m_nms) do_nms_sort(dets, nboxes, l.classes, m_nms);
    
    
}