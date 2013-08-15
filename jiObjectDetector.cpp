/*******************************************************************
 * jakeImage Library
 * Copyright (c) 2013, Jason Corso
 * All rights reserved.
 *
 *
 *******************************************************************
 * FILENAME:    jiObjectDetector.cpp
 * AUTHOR(S):   Jason Corso (jcorso@buffalo.edu)
 *
 ********************************************************************
 */
/** \file */

#include "jiObjectDetector.h"

namespace jake
{

jiObjectDetector::jiObjectDetector()
{

}

/*jiObjectDetector::jiObjectDetector(std::shared_ptr<drwnClassifier> classifier_)
                : _classifier(classifier_)
{

}*/

jiObjectDetector::~jiObjectDetector()
{
}

bool jiObjectDetector::detect(cv::Mat& image_, const jake::jiObjectDetectionParams& params,std::vector<jake::jiObjectDetection>& detections)
{
}

} /* namespace jake */
