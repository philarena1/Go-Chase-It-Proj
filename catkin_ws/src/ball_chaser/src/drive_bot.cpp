#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
//DONE: Include the ball_chaser "DriveToTarget" header file
#include "ball_chaser/DriveToTarget.h" 


// ROS::Publisher motor commands;
ros::Publisher motor_command_publisher;


// TODO: Create a handle_drive_request callback function that executes whenever a drive_bot service is requested
// This function should publish the requested linear x and angular velocities to the robot wheel joints
// After publishing the requested velocities, a message feedback should be returned with the requested wheel velocities
bool handle_drive_request(ball_chaser::DriveToTarget::Request& req,
ball_chaser::DriveToTarget::Response& res)
{

	float updated_x = (float)req.linear_x;
	float updated_z = (float)req.angular_z;
	
	ROS_INFO("DriveToTargetRequest Received - j1:%1.2f, j2:%1.2f", updated_x, updated_z);

	//motor_command_publisher.publish(linear_x);
        // TODO: Delete the loop, move the code to the inside of the callback function and make the necessary changes to publish the requested velocities instead of constant values
	   
        // Create a motor_command object of type geometry_msgs::Twist
        geometry_msgs::Twist motor_command;
	 	
        // Set wheel velocities, forward [0.5, 0.0]
        motor_command.linear.x = updated_x; //0.5;
        motor_command.angular.z = updated_z; //0.0;
        
	// DONE - After publishing the requested velocities
        motor_command_publisher.publish(motor_command);
	
	//ros::Duration(3).sleep();

	// DONE - a message feedback should be returned with the requested wheel velocities
	res.msg_feedback = "Linear x set - j1: " + std::to_string(updated_x) + " , j2: " + std::to_string(updated_z);
	ROS_INFO_STREAM(res.msg_feedback);

	return true;

}


int main(int argc, char** argv)
{
    // Initialize a ROS node
    ros::init(argc, argv, "drive_bot");

    // Create a ROS NodeHandle object
    ros::NodeHandle n;

    // Inform ROS master that we will be publishing a message of type geometry_msgs::Twist on the robot actuation topic with a publishing queue size of 10
    motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    // TODO: Define a drive /ball_chaser/command_robot service with a handle_drive_request callback function
	ros::ServiceServer service = n.advertiseService("/ball_chaser/command_robot", handle_drive_request); 
	ROS_INFO("Ready to send drive commands");


    // TODO: Handle ROS communication events
    ros::spin();

    return 0;
}

