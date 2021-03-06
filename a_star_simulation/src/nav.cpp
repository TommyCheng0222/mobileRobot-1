//  Copyright 2019 Tommy Huang, Tsung-Han Brian Lee
/***************************************************************
 ** ---------------- Mobile Vehicle Control -------------------
 **   AUTHOR      : Tommy Huang, Tsung-Han Brian Lee
 **   DESCRIPTION : Revised from Dynamic Systems n Control Lab
 ** -----------------------------------------------------------
 ***************************************************************/
#include <math.h>
#include "ros/ros.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"

#define PI 3.14159265358979323      // circumference ratio.

// [TODO] ... adjusting the control gain !! ...
const double k_rho      = 2.0;
const double k_alpha    = -5.0;

/************************************************
 **  Global variables to allow communication
 **  between functions
 ************************************************/
geometry_msgs::Twist command;

float x_now;
float y_now;
float th_now;
/************************************************
 **  Callback Functions
 ************************************************/
void subgoalCallback(const geometry_msgs::Point &msg);
void poseCallback(const nav_msgs::Odometry &location);

int main(int argc, char* argv[]) {
    /**
     ** Initializing the ROS nh : "nav"
     **/
    ros::init(argc, argv, "nav");
    ros::NodeHandle nh;

    ros::Publisher pub;
    ros::Subscriber sub;
    ros::Subscriber sub2;

    /**
     ** Publish robot movement
     **     => advertise topic
     **/
    pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    /**
     ** Subscribe robot pose and the reference signal
     **     => install callback function
     **/
    sub = nh.subscribe("/subgoal_position", 10, subgoalCallback);
    sub2 = nh.subscribe("/robot_pose", 10, poseCallback);

    /* Looping in Fs : 100 Hz */
    ros::Rate rate(100);

    while (ros::ok()) {
        /* Attempting to fire callback */
        ros::spinOnce();

        /* Publish control command message */
        pub.publish(command);

        /* constant looping in 100 Hz */
        rate.sleep();
    }

    return 0;
}

void subgoalCallback(const geometry_msgs::Point &msg) {
    /* initializing command ? */
    command.linear.x  = 0.0;
    command.linear.y  = 0.0;
    command.angular.z = 0.0;

    /*********************************************
	 ** Apply control law following
     *********************************************/
    double xDiff = static_cast<double>(msg.x - x_now);
    double yDiff = static_cast<double>(msg.y - y_now);
    /**
     **  Euclidean distance between robot and target
     **/
    double distance = sqrt(pow(xDiff, 2) + pow(yDiff, 2));
    /**
     **  Angle difference between heading direction and target
     **/
    double headingAngleDiff = atan2(yDiff, xDiff) - th_now;
    /* wrapping the angle between -PI ~ PI */
    if (headingAngleDiff > PI) {
        headingAngleDiff -= 2*PI;
    } else if (headingAngleDiff < PI) {
        headingAngleDiff += 2*PI;
    }

    double linear_vel  = k_rho    * distance;
    double angular_vel = k_alpha * headingAngleDiff;

    /**
     ** 1. Saturating linear velocity
     ** 2. Setting control command
     **/
    if (linear_vel > 0.25) {
        command.linear.x = 0.25;
    } else if (linear_vel < -0.25) {
        command.linear.x = -0.25;
    } else {
        command.linear.x = linear_vel;
    }
    /**
     ** 1. Saturating angular velocity
     ** 2. Setting control command
     **/
    if (angular_vel > 1.0) {
        command.angular.z = 1.0;
    } else if (angular_vel < -1.0) {
        command.angular.z = -1.0;
    } else {
        command.angular.z = angular_vel;
    }
}

void poseCallback(const nav_msgs::Odometry &location) {
    //  Update robot position
    x_now = location.pose.pose.position.x;
    y_now = location.pose.pose.position.y;
    th_now = location.pose.pose.orientation.z;
}
