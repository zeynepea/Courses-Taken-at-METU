#include "Camera.h"
#include <string.h>

Camera::Camera(int id,                      // Id of the camera
               const char* imageName,       // Name of the output PPM file 
               const Vector3f& pos,         // Camera position
               const Vector3f& gaze,        // Camera gaze direction
               const Vector3f& up,          // Camera up direction
               const ImagePlane& imgPlane)  // Image plane parameters
{
	this->id = id;
     this->pos = pos;
     this->gaze = gaze;
     this->up = up;
     this->imgPlane = imgPlane;
     strcpy(this->imageName, imageName);
}

/* Takes coordinate of an image pixel as row and col, and
 * returns the ray going through that pixel. 
 */
Ray Camera::getPrimaryRay(int col, int row) const
{
	float s_u = (col + 0.5) * (imgPlane.right - imgPlane.left) / imgPlane.nx;
     float s_v = (row + 0.5) * (imgPlane.top - imgPlane.bottom) / imgPlane.ny;

     Vector3f u_gaze = gaze / sqrt(gaze*gaze);
     Vector3f u_up = up / sqrt(up*up);
     Vector3f m = pos + u_gaze * imgPlane.distance;

     // u = v x w
     Vector3f u = u_up.crossProduct(-u_gaze); 
     u = u / sqrt(u*u);
     Vector3f q = m + u * imgPlane.left + u_up * imgPlane.top;

     Vector3f s = q + u * s_u - u_up * s_v;

     Vector3f direction = s - pos;
     //direction = direction / sqrt(direction*direction);

     return Ray(pos, direction);

}

