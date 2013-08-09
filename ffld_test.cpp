#include "ffld.h"
#include "cv.h"
#include <iostream>

using namespace std;
using namespace cv;
using namespace FFLD;

int main(int argc, char * argv[])
{
	
    char* imname = argv[1];
    char* model = argv[2];

    printf("%s\n",imname);
    char buffer[1000];
    
	const string fileim(imname);
	cv::Mat image;
	image=cv::imread(fileim,1);
	ObjectDetect o;
	
	if (image.empty()) 
	{
			o.showUsage();
			cerr << "\nInvalid image " << fileim << endl;
			return -1;
	}
	  
	  std::vector<FFLD::ObjectDetect::Detection> detections;
	  JPEGImage img(image);
	  o.detectObjects(image,model,detections);
		
		for (int j = 0; j < detections.size(); ++j) 
		{
			o.draw(img, detections[j], 255, 0, 0, 2);
		}     
        img.save("hello.jpg");       
      
}
