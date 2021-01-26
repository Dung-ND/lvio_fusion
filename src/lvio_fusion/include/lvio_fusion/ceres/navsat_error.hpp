#ifndef lvio_fusion_NAVSAT_ERROR_H
#define lvio_fusion_NAVSAT_ERROR_H

#include "lvio_fusion/ceres/base.hpp"

namespace lvio_fusion
{

class NavsatInitError
{
public:
    NavsatInitError(Vector3d p0, Vector3d p1)
        : x0_(p0.x()), y0_(p0.y()), z0_(p0.z()),
          x1_(p1.x()), y1_(p1.y()), z1_(p1.z())
    {
    }

    template <typename T>
    bool operator()(const T *rx, T *residuals) const
    {
        T p1[3] = {T(x1_), T(y1_), T(z1_)};
        T tf_p1[3];
        T tf[7] = {rx[0], rx[1], rx[2], rx[3], rx[4], T(0), T(0)};
        ceres::SE3TransformPoint(tf, p1, tf_p1);
        residuals[0] = T(x0_) - tf_p1[0];
        residuals[1] = T(y0_) - tf_p1[1];
        residuals[2] = T(z0_) - tf_p1[2];
        return true;
    }

    static ceres::CostFunction *Create(Vector3d p0, Vector3d p1)
    {
        return (new ceres::AutoDiffCostFunction<NavsatInitError, 3, 5>(new NavsatInitError(p0, p1)));
    }

private:
    double x0_, y0_, z0_;
    double x1_, y1_, z1_;
};

class NavsatXError
{
public:
    NavsatXError(Vector3d p0, Vector3d p1, SE3d pose)
        : x0_(p0.x()), y0_(p0.y()), z0_(p0.z()),
          x1_(p1.x()), y1_(p1.y()), z1_(p1.z()),
          pose_(pose)
    {
    }

    template <typename T>
    bool operator()(const T *x, T *residuals) const
    {
        T pose[7], tf[7], relative_pose[7] = {T(0), T(0), T(0), T(1), x[0], T(0), T(0)};
        ceres::Cast(pose_.data(), SE3d::num_parameters, pose);
        ceres::SE3Product(pose, relative_pose, tf);
        T p1[3] = {T(x1_), T(y1_), T(z1_)};
        T tf_p1[3];
        ceres::SE3TransformPoint(tf, p1, tf_p1);
        residuals[0] = T(x0_) - tf_p1[0];
        residuals[1] = T(y0_) - tf_p1[1];
        residuals[2] = T(z0_) - tf_p1[2];
        return true;
    }

    static ceres::CostFunction *Create(Vector3d p0, Vector3d p1, SE3d pose)
    {
        return (new ceres::AutoDiffCostFunction<NavsatXError, 3, 1>(new NavsatXError(p0, p1, pose)));
    }

private:
    double x0_, y0_, z0_;
    double x1_, y1_, z1_;
    SE3d pose_;
};

class NavsatRPError
{
public:
    NavsatRPError(Vector3d p0, Vector3d p1, SE3d pose)
        : x0_(p0.x()), y0_(p0.y()), z0_(p0.z()),
          x1_(p1.x()), y1_(p1.y()), z1_(p1.z()),
          pose_(pose)
    {
    }

    template <typename T>
    bool operator()(const T *roll, const T *pitch, T *residuals) const
    {
        T pose[7], tf[7], relative_pose[7];
        T rpyxyz[6] = {roll[0], pitch[0], T(0), T(0), T(0), T(0)};
        ceres::RpyxyzToSE3(rpyxyz, relative_pose);
        ceres::Cast(pose_.data(), SE3d::num_parameters, pose);
        ceres::SE3Product(pose, relative_pose, tf);
        T p1[3] = {T(x1_), T(y1_), T(z1_)};
        T tf_p1[3];
        ceres::SE3TransformPoint(tf, p1, tf_p1);
        residuals[0] = T(x0_) - tf_p1[0];
        residuals[1] = T(y0_) - tf_p1[1];
        residuals[2] = T(z0_) - tf_p1[2];
        return true;
    }

    static ceres::CostFunction *Create(Vector3d p0, Vector3d p1, SE3d pose)
    {
        return (new ceres::AutoDiffCostFunction<NavsatRPError, 3, 1, 1>(new NavsatRPError(p0, p1, pose)));
    }

private:
    double x0_, y0_, z0_;
    double x1_, y1_, z1_;
    SE3d pose_;
};

class NavsatYawError
{
public:
    NavsatYawError(Vector3d p0, Vector3d p1, SE3d pose)
        : x0_(p0.x()), y0_(p0.y()), z0_(p0.z()),
          x1_(p1.x()), y1_(p1.y()), z1_(p1.z()),
          pose_(pose)
    {
    }

    template <typename T>
    bool operator()(const T *yaw, T *residuals) const
    {
        T pose[7], tf[7], relative_pose[7];
        T rpyxyz[6] = {T(0), T(0), yaw[0], T(0), T(0), T(0)};
        ceres::RpyxyzToSE3(rpyxyz, relative_pose);
        ceres::Cast(pose_.data(), SE3d::num_parameters, pose);
        ceres::SE3Product(pose, relative_pose, tf);
        T p1[3] = {T(x1_), T(y1_), T(z1_)};
        T tf_p1[3];
        ceres::SE3TransformPoint(tf, p1, tf_p1);
        residuals[0] = T(x0_) - tf_p1[0];
        residuals[1] = T(y0_) - tf_p1[1];
        residuals[2] = T(z0_) - tf_p1[2];
        return true;
    }

    static ceres::CostFunction *Create(Vector3d p0, Vector3d p1, SE3d pose)
    {
        return (new ceres::AutoDiffCostFunction<NavsatYawError, 3, 1>(new NavsatYawError(p0, p1, pose)));
    }

private:
    double x0_, y0_, z0_;
    double x1_, y1_, z1_;
    SE3d pose_;
};

} // namespace lvio_fusion

#endif // lvio_fusion_NAVSAT_ERROR_H
