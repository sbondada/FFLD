#include "Intersector.h"

using namespace FFLD;

Intersector::Intersector(Rectangle reference, double threshold, bool felzenszwalb) :
	reference_(reference), threshold_(threshold), felzenszwalb_(felzenszwalb)
	{
	}
	
bool Intersector::operator()(Rectangle rect, double * score) const
	{
		if (score)
			*score = 0.0;
		
		const int left = std::max(reference_.left(), rect.left());
		const int right = std::min(reference_.right(), rect.right());
		
		if (right < left)
			return false;
		
		const int top = std::max(reference_.top(), rect.top());
		const int bottom = std::min(reference_.bottom(), rect.bottom());
		
		if (bottom < top)
			return false;
		
		const int intersectionArea = (right - left + 1) * (bottom - top + 1);
		const int rectArea = rect.area();
		
		if (felzenszwalb_) {
			if (intersectionArea >= rectArea * threshold_) {
				if (score)
					*score = static_cast<double>(intersectionArea) / rectArea;
				
				return true;
			}
		}
		else {
			const int referenceArea = reference_.area();
			const int unionArea = referenceArea + rectArea - intersectionArea;
			
			if (intersectionArea >= unionArea * threshold_) {
				if (score)
					*score = static_cast<double>(intersectionArea) / unionArea;
				
				return true;
			}
		}
		
		return false;
	}
