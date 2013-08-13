
#include "ffld.h"
#include "cv.h"
#include <iostream>

using namespace std;
using namespace cv;
using namespace FFLD;
using namespace jake;

int main(int argc, char * argv[])
{
	
    char* imname = argv[1];
    char* model = argv[2];

    printf("%s\n",imname);
    char buffer[1000];
    
	const string fileim(imname);
	cv::Mat image;
	image=cv::imread(fileim,1);
	FFLD::FFLDObjDetector o;
	
	if (image.empty()) 
	{
			o.showUsage();
			cerr << "\nInvalid image " << fileim << endl;
			return -1;
	}
	  
	vector<jake::jiObjectDetection>  detections;
	JPEGImage img(image);
	jake::jiObjectDetectionParams modelparam;
	modelparam.model=model;
	o.detect(image,modelparam,detections);
		
	for (int j = 0; j < detections.size(); ++j) 
	{
		FFLD::Rectangle rect;
		rect.setX(detections[j].box.x);
		rect.setY(detections[j].box.y);
		rect.setWidth(detections[j].box.width);
		rect.setHeight(detections[j].box.height);
		o.draw(img,rect, 255, 0, 0, 2);
	}     
    img.save("hello.jpg");         
}
