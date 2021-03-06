#ifndef ROBOT_STATE_HPP
#define ROBOT_STATE_HPP
#include <string>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "json.hpp"


/**
 * @struct robot_state
 * @brief used to describe the current state of the robot
 * @version 0.1.0
 * @author Alex Giokas <a.gkiokas@ortelio.co.uk>
 * @date 27.01.2017
 **/
struct robot_state
{
    float x = 0;
    float y = 0;
    float yaw = 0;

    bool operator==(const robot_state & arg) const
    {
        return (this->x == arg.x) && 
               (this->y == arg.y) &&
               (this->yaw == arg.yaw);
    }

    /// print on stdout
    void print() const
    {
        printf("x: %f y: %f, θ: %f \r\n", x, y, yaw);
    }
};


/**
 * @brief create states by observing topics publishing meta-data
 * @note  subscribes to `coordinates`
 * @note  subscribes to `imu`
 * @note  subscribes to `ping`
 */ 
class state_factory
{
public:
    state_factory(ros::NodeHandle & node)
    {
        coord_sub__ = node.subscribe("coordinates", 
                                      1000, 
                                      &state_factory::read_coordinates, 
                                      this
                                    );
    }

    /// parse JSON of {"x":.., "y":..., "yaw":...}
    void read_coordinates(const std_msgs::String::ConstPtr & msg)
    {
        auto obj = nlohmann::json::parse(msg->data);
        state__.x = obj["x"];
        state__.y = obj["y"];
        state__.yaw = obj["theta"];
    }

    /// @brief get a copy of the state as it its ATM
    robot_state get_state() const
    {
        return robot_state(state__);
    }

private:
    ros::Subscriber coord_sub__;
    ros::Subscriber ping_sub__;
    ros::Subscriber imu_sub__;
    robot_state state__;
};

namespace std 
{
template <> struct hash<robot_state>
{
    std::size_t operator()(robot_state const& arg) const 
    {
        std::size_t seed = 0;
        relearn::hash_combine(seed, arg.x);
        relearn::hash_combine(seed, arg.y);
        relearn::hash_combine(seed, arg.yaw);
        return seed;
    }
};
}
#endif
