#ifndef FFLD_H
#define FFLD_H

#include "Intersector.h"
#include "Mixture.h"
#include "Object.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#ifndef _WIN32
#include <sys/time.h>

namespace FFLD
{	
class ObjectDetect
{
private:
	timeval Start;
	timeval Stop;
	
public:

struct Detection : public FFLD::Rectangle
	{
		public:
		Detection();
		Detection(HOGPyramid::Scalar score, int l, int x, int y, FFLD::Rectangle bndbox);
		HOGPyramid::Scalar score;
		int l;
		int x;
		int y;
		bool operator<(const Detection & detection) const;
	};

	inline void start();
	inline int stop();


   
#endif

#include "cv.h"
#include "highgui.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/features2d/features2d.hpp"

	
	void showUsage();

	void draw(JPEGImage & image, const FFLD::Rectangle & rect, uint8_t r, uint8_t g, uint8_t b,int linewidth);
			  
	void detect(const Mixture & mixture, int width, int height, const HOGPyramid & pyramid,double threshold, double overlap, cv::Mat image, std::ostream & out,const std::string & images, std::vector<Detection> & detections);
			
	int detectObjects(cv::Mat image,char * model,std::vector<Detection> & detections);
	
};
}

#endif
