/*******************************************************************
 * jakeImage Library
 * Copyright (c) 2013, Jason Corso
 * All rights reserved.
 *
 *
 *******************************************************************
 * FILENAME:    jiObjectDetector.h
 * AUTHOR(S):   Jason Corso (jcorso@buffalo.edu)
 * DESCRIPTION:
 *    Implements a basic object detector class that stores a
 *    classifier and can run a sliding window over an image to return
 *    detections.
 *
 ********************************************************************
 */
/** \file */

#pragma once

#include <vector>
#include <memory>

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>


namespace jake
{

struct jiObjectDetection
{
    cv::Rect box;
    float score;
};

struct jiObjectDetectionParams
{
   char * model;
};

/** \class jiObjectDetector
 * \brief Implements a basic object detector class.
 */
class jiObjectDetector //: public drwnStdObjIface, drwnProperties
{
protected:
//    std::shared_ptr<drwnClassifier> _classifier;

public:
    jiObjectDetector();
    virtual ~jiObjectDetector();
/*    jiObjectDetector(std::shared_ptr<drwnClassifier> classifier_);

  virtual const char* type() { return "jiObjectDetector"; }
    virtual bool save(drwnXMLNode& node) const {return false;}
    virtual bool load(drwnXMLNode& node) {return false;}
*/
    /** Main detection function that takes an image, a set of parameters and
     *   runs the classifiers on it.
     */
   virtual bool detect(cv::Mat& image_, const jiObjectDetectionParams& params,std::vector<jiObjectDetection>& detections);
};

} /* namespace jake */


