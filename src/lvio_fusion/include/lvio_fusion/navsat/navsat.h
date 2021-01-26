#ifndef lvio_fusion_NAVSAT_H
#define lvio_fusion_NAVSAT_H

#include "lvio_fusion/common.h"
#include "lvio_fusion/frame.h"
#include "lvio_fusion/loop/pose_graph.h"
#include "lvio_fusion/sensor.h"

namespace lvio_fusion
{

class Navsat : public Sensor
{
public:
    typedef std::shared_ptr<Navsat> Ptr;

    static int Create()
    {
        devices_.push_back(Navsat::Ptr(new Navsat));
        return devices_.size() - 1;
    }

    static int Num()
    {
        return devices_.size();
    }

    static Navsat::Ptr Get(int id = 0)
    {
        return devices_[id];
    }

    void AddPoint(double time, double x, double y, double z);

    Vector3d GetFixPoint(Frame::Ptr frame);
    Vector3d GetRawPoint(double time);
    Vector3d GetPoint(double time);

    Vector3d GetAroundPoint(double time);

    double Optimize(double time);

    bool initialized = false;
    std::map<double, Vector3d> raw;
    double finished = 0;
    Vector3d fix;

private:
    Navsat() : Sensor(SE3d()) {}
    Navsat(const Navsat &);
    Navsat &operator=(const Navsat &);

    void Initialize();

    void OptimizeRP(Frame::Ptr frame, double time);
    void OptimizeYaw(Frame::Ptr frame, double time);
    void OptimizeX(Frame::Ptr frame, double time);

    static std::vector<Navsat::Ptr> devices_;
};

} // namespace lvio_fusion

#endif // lvio_fusion_NAVSAT_H