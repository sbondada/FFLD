//--------------------------------------------------------------------------------------------------
// Implementation of the paper "Exact Acceleration of Linear Object Detectors", 12th European
// Conference on Computer Vision, 2012.
// 
// Copyright (c) 2012 Idiap Research Institute, <http://www.idiap.ch/>
// Written by Charles Dubout <charles.dubout@idiap.ch>
// 
// This file is part of FFLD (the Fast Fourier Linear Detector)
// 
// FFLD is free software: you can redistribute it and/or modify it under the terms of the GNU
// General Public License version 3 as published by the Free Software Foundation.
// 
// FFLD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
// Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with FFLD. If not, see
// <http://www.gnu.org/licenses/>.
//--------------------------------------------------------------------------------------------------

#ifndef FFLD_INTERSECTOR_H
#define FFLD_INTERSECTOR_H

#include "Rectangle.h"
#include <algorithm>

namespace FFLD
{
/// Functor used to test for the intersection of two rectangles according to the Pascal criterion
/// (area of intersection over area of union).
class Intersector
{
public:
	/// Constructor.
	/// @param[in] reference The reference rectangle.
	/// @param[in] threshold The threshold of the criterion.
	/// @param[in] felzenszwalb Use Felzenszwalb's criterion instead (area of intersection over area
	/// of second rectangle). Useful to remove small detections inside bigger ones.
	Intersector(Rectangle reference, double threshold, bool felzenszwalb = false);
	
	/// Tests for the intersection between a given rectangle and the reference.
	/// @param[in] rect The rectangle to intersect with the reference.
	/// @param[out] score The score of the intersection.
	bool operator()(Rectangle rect, double * score = 0) const;
	
private:
	Rectangle reference_;
	double threshold_;
	bool felzenszwalb_;
};
}

#endif
