/*
 * opencvmat.cpp
 *
 *  Created on: Jan 19, 2015
 *      Author: so
 */

#include "opencvmat.h"
#include "opencv2/core.hpp"

#ifdef __arch_cortex__
void *__dso_handle = 0;
#endif

void create_opencv_mat() {
	static cv::Mat mat(2,2, CV_8UC3, cv::Scalar(126,0,255));
	mat.at<double>(0,0) = 3;
	cv::Scalar val(126,0,255);

}

