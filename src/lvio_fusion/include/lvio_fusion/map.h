#ifndef MAP_H
#define MAP_H

#include "lvio_fusion/common.h"
#include "lvio_fusion/frame.h"
#include "lvio_fusion/navsat/navsat.h"
#include "lvio_fusion/visual/landmark.h"

namespace lvio_fusion
{

class Map
{
public:
    typedef std::shared_ptr<Map> Ptr;

    Map() {}

    visual::Landmarks &GetAllLandmarks()
    {
        std::unique_lock<std::mutex> lock(data_mutex_);
        return landmarks_;
    }

    Frames &GetAllKeyFrames()
    {
        std::unique_lock<std::mutex> lock(data_mutex_);
        return keyframes_;
    }
    //NEWADD
    int GetAllKeyFramesSize(){return keyframes_.size();}
    void ApplyScaledRotation(const cv::Mat &R, const float s, const bool bScaledVel, const cv::Mat t=cv::Mat::zeros(cv::Size(1,3),CV_32F));
//NEWADDEND 

    Frames GetKeyFrames(double start, double end = 0, int num = 0);
    
    void InsertKeyFrame(Frame::Ptr frame);

    void InsertLandmark(visual::Landmark::Ptr landmark);

    void RemoveLandmark(visual::Landmark::Ptr landmark);

    SE3d ComputePose(double time);

    void Reset()
    {
        landmarks_.clear();
        keyframes_.clear();
    }

    Frame::Ptr current_frame;
    NavsatMap::Ptr navsat_map;
    PointRGBCloud simple_map;

private:
    std::mutex data_mutex_;
    visual::Landmarks landmarks_;
    Frames keyframes_;
};
} // namespace lvio_fusion

#endif // MAP_H
