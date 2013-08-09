/*
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/features2d/features2d.hpp"
*/
#include "cv.h"
#include "highgui.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/features2d/features2d.hpp"

//#include "imgproc.h"
//#include "features2d.h"

#include <cstdio> 
#include <stdio.h>
#include <string.h>
using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    VideoCapture cap;

    char* vidname = argv[1];
    printf("%s\n",vidname);
    int id = 0;
    char buffer[1000];
  //  cap.open(-1); // 
    cap.open(vidname); 
    if( !cap.isOpened() )
    {

        puts("***Could not initialize capturing...***\n");
        return 0;
    }
    namedWindow( "Capture ", CV_WINDOW_AUTOSIZE);
    namedWindow( "Foreground ", CV_WINDOW_AUTOSIZE );
    //namedWindow( "BG ", CV_WINDOW_AUTOSIZE );
    Mat frame,foreground,image,bg;
    BackgroundSubtractorMOG mog;
    int fps=cap.get(CV_CAP_PROP_FPS);
    if(fps<=0)
        fps=10;
    else
        fps=1000/fps;
    for(;;)
    {
        cap>>frame;  
        if( frame.empty() )
                break;
        image=frame.clone();
        // Background Substraction
        mog(frame,foreground,-1);
	
	//Postprocessing for BS
        threshold(foreground,foreground,128,255,THRESH_BINARY);
        medianBlur(foreground,foreground,9);
        erode(foreground,foreground,Mat());
        dilate(foreground,foreground,Mat());

	mog.getBackgroundImage(bg);


        // Capture initial video
        imshow( "Capture ",image );

	sprintf(buffer,"image/%s/%u.jpg",vidname,id);
	//printf("%s\n",buffer);

	imwrite(buffer,image);
        // image.copyTo(foreground,foreground); // birde böyle deneyin bakalým ne olacak
        imshow("Foreground ",foreground);
	sprintf(buffer,"fg/%s/%u.jpg",vidname,id);
	imwrite(buffer,foreground);

        //imshow("BG ",bg);
	//sprintf(buffer,"bg/%s/%u.jpg",vidname,id);
	//imwrite(buffer,bg);
	id++;
        char c = (char)waitKey(fps);
        if( c == 27 )   
            break;

    }


}
