#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // TODO: Request a service and pass the velocities to it to drive the robot

    ROS_INFO_STREAM("Passing velocities" + std::to_string(lin_x) + " " + std::to_string(ang_z));

    // call our service
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;
    if (!client.call(srv))
        ROS_ERROR("Failed to call");

   // wait for the robot to move a bit
   ros::Duration(3).sleep();
    

}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

    int white_pixel = 255;

    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera


    // start top first
    float dir = float(img.step)/3;

    bool found_ball = false;

    int count = 0;
    for (int i = 0; i < img.height; i ++){ 
    // look left and right 
        for (int j = 0; j < img.step; j ++){  
        
	    if  ((img.data[count]) == 255)  {
		found_ball = true;
                 //left
	        if ((j > 0) && (j <= dir )  ) {
	 	    	//ROS_INFO_STREAM("left");
                    	drive_robot(0.0,0.25);
                   }
 	    	 //forward
           	 if ((j > dir) && (j <= 2 * dir )  ) {
 			//ROS_INFO_STREAM("forward");
             		drive_robot(0.5,0.0);
             		}
	     	  //right
	   	 if ((j > 2 * dir) && (j <= 3 * dir )  ) {
			//ROS_INFO_STREAM("right");
             		drive_robot(0,-0.25);
            	 }

		break;
             }
		

      	count = count + 1;
	
	}
	   if (found_ball = false) {
   //ROS_INFO_STREAM("No White Ball- Stopping");
   drive_robot(0,0);
 		}			

  }

    

}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}

