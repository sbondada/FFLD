#include <boost/config.hpp> // kept it to suppress some VC++ warnings
#include <boost/filesystem.hpp> 
#include <boost/algorithm/string.hpp>
#include <boost/utility.hpp>
#include "jiObjectDetector.h"
#include "ffld.h"
#include "cv.h"

#include <algorithm>  
#include <iostream> 

using namespace boost;
using namespace std;
using namespace cv;
using namespace FFLD;
using namespace jake;

struct perTypeDetections
{
	int detectionindex;
	std::string detectionname;
	int detectioncount;
	float maxdetectionscore;
	std::vector<jake::jiObjectDetection> detectionList;
	bool operator<(const perTypeDetections &a) const
	{
		return (a.maxdetectionscore < this->maxdetectionscore);
	}
};

struct perFrameDetections
{
	int frameindex;
	std::vector<perTypeDetections>	typeList;
};

struct conceptEntry
{
	std::string detectionname;
	int occurunces;
	bool operator<(const conceptEntry &a) const
	{
		return (a.detectionname < this->detectionname);
	}
};

struct conceptDetections
{
	std::set<conceptEntry>  detectorset;
};

std::set<conceptEntry>::iterator findmatch(std::set<conceptEntry>& detectorset,std::string matchword)
{
	for (std::set<conceptEntry>::iterator i = detectorset.begin(); i != detectorset.end(); i++) 
	{
		if(i->detectionname==matchword)
		{
			return i;
		}
	}
	return detectorset.end();
}

void getDetectionSet(int framestart,std::set<conceptEntry>& detectorset,std::vector<perFrameDetections>& framedetectionList,int windowsize)
{
	cout<<"getDetection--Outside "<<framestart<<endl;
	for(int i=framestart;(i<framestart+windowsize && i<framedetectionList.size());i++)
	{
		cout<<"getDetection--inside "<<framedetectionList[i].typeList.size()<<endl;
		for(int j=0;j<framedetectionList[i].typeList.size();j++)
		{
			cout<<"getDetection--"<<endl;
			std::set<conceptEntry>::iterator matchworditr=findmatch(detectorset,framedetectionList[i].typeList[j].detectionname);
			if(matchworditr!=detectorset.end())
			{
				conceptEntry tempConceptEntry;
				tempConceptEntry.detectionname=framedetectionList[i].typeList[j].detectionname;
				tempConceptEntry.occurunces=matchworditr->occurunces+framedetectionList[i].typeList[j].detectioncount;
				detectorset.erase(matchworditr);
				detectorset.insert(tempConceptEntry);
			}
			else
			{
				conceptEntry tempConceptEntry;
				tempConceptEntry.detectionname=framedetectionList[i].typeList[j].detectionname;
				tempConceptEntry.occurunces=framedetectionList[i].typeList[j].detectioncount;
				detectorset.insert(tempConceptEntry);
			}
		}
	}	
}

float calcJaccardDist(std::set<conceptEntry>& currentdetectorset,std::set<conceptEntry>& nextdetectorset)
{
	int intersectioncount=0;
	int unioncount =0;
	float jaccarddist=0;
	  
	cout<<"calculate jaccard--Outside"<<endl;
	  
	if(currentdetectorset.size()==0 || nextdetectorset.size()==0)
	{
	    cout<<"calculate jaccard--inside size cond"<<endl;
		return 1.0f;	
	}

	std::set<std::string> intersectresult;
	std::set_intersection (currentdetectorset.begin(),currentdetectorset.end(),nextdetectorset.begin(),nextdetectorset.end(),inserter(intersectresult, intersectresult.begin()));
	intersectioncount=intersectresult.size();
	cout<<"intercount "<<intersectioncount<<endl;
	
	std::set<std::string> unionresult;
	std::set_union (currentdetectorset.begin(),currentdetectorset.end(),nextdetectorset.begin(),nextdetectorset.end(),inserter(unionresult, unionresult.begin()));
	unioncount=unionresult.size();
	cout<<"unioncount "<<unioncount<<endl;
	
	jaccarddist=(1-((float)intersectioncount/unioncount));
	
	return jaccarddist;	
}

void performUnion(std::set<conceptEntry>& currentdetectorset,std::set<conceptEntry>& nextdetectorset)
{
	cout<<"performunion--out"<<endl;
	if(nextdetectorset.size()!=0)
	{	
		cout<<"performunion--inside if cond"<<endl;
	for (set<conceptEntry>::iterator i = nextdetectorset.begin(); i != nextdetectorset.end(); i++) 
	{
		set<conceptEntry>::iterator matchworditr=findmatch(currentdetectorset,i->detectionname);
		if(matchworditr!=currentdetectorset.end())
		{
			conceptEntry tempConceptEntry;
			tempConceptEntry.detectionname=i->detectionname;
			tempConceptEntry.occurunces=matchworditr->occurunces+i->occurunces;
			currentdetectorset.erase(matchworditr);
			currentdetectorset.insert(tempConceptEntry);
		}
		else
		{
			conceptEntry tempConceptEntry;
			tempConceptEntry.detectionname=i->detectionname;
			tempConceptEntry.occurunces=i->occurunces;
			currentdetectorset.insert(tempConceptEntry);
		}
	}
	}
	
}

void generateConcepts(std::vector<perFrameDetections>& framedetectionList,std::vector<conceptDetections>& conceptList,int windowsize)
{
	for(int i=0;i<framedetectionList.size();i=i+3)
	{
		cout<<"entry-statement"+i<<endl;
		conceptDetections concept;
		getDetectionSet(i,concept.detectorset,framedetectionList,windowsize);
		float jaccarddist=0;
		while (jaccarddist<0.5)
		{
		i=i+3;
		std::set<conceptEntry>  nextdetectorset;
		getDetectionSet(i,nextdetectorset,framedetectionList,windowsize);
		cout<<"hi inside and after second getdetection"<<endl ;
		jaccarddist=calcJaccardDist(concept.detectorset,nextdetectorset);
		cout<<"jaccard dist "<<jaccarddist<<endl;
		if(jaccarddist<0.5)
		{
			performUnion(concept.detectorset,nextdetectorset);
		}
		}
		conceptList.push_back(concept);
		i=i-3; //considering the next frame as the current frame
	}
}

namespace fs=boost::filesystem;

int main() 
{ 
	//argument declaration incase of functions
	std::vector<perFrameDetections> framedetectionList ;//reference type when used as the argument type
	std::string predictVideoPath="/home/kaushal/Documents/Vedio2Text/TDM/ffld/data/test_video_model";
	std::string modelPath="/home/kaushal/Documents/Vedio2Text/TDM/ffld/data/test_video_model/txt_models";
	int frameInterval=1;
	
	//====extra argument required for the sake of not having jvvideo or jake 
	std::string framespath="/home/kaushal/Documents/Vedio2Text/TDM/ffld/data/test_video_model/frames";
	
	int numOfFramesAccessed=0;
	//=====jake::jvVideoFrames predictVid;
	FFLD::FFLDObjDetector o;
    std::string fpath=predictVideoPath+"/small_video.mp4";
    fs::path fp(fpath);
    if(fs::exists(fp))
    {
		cout<<"hi"<<endl;
		//=====predictVid.load(fpath.c_str());
		//=====int length=predictVid.length();
		/*=====for(int i=0;i<length;i=i+frameInterval)
		{	
			numOfFramesAccessed++;
			cv::Mat frame;
			frame=video->frame(i);
			int numOfDetector=0;
		====*/	
			
		fs::path framep(framespath);
		fs::directory_iterator frameend_iter;
		for( fs::directory_iterator frame_iter(framep) ; frame_iter != frameend_iter ; ++frame_iter)
		{
			cout<<"hi1"<<endl;
			if (fs::is_regular_file(frame_iter->status()) )
			{
				numOfFramesAccessed++;
				cout<<frame_iter->path().c_str()<<endl;
				const string fileim(frame_iter->path().c_str());
				cv::Mat frame;
				frame=cv::imread(fileim,1);
		
				perFrameDetections framedetection;
				framedetection.frameindex=numOfFramesAccessed;
				int numOfDetector=0;
				
				fs::path mp(modelPath);
				if(fs::is_directory(mp))
				{
					fs::directory_iterator modelend_iter;
					for( fs::directory_iterator model_iter(mp) ; model_iter != modelend_iter ; ++model_iter)
					{
						if (fs::is_regular_file(model_iter->status()) )
						{
						numOfDetector++;
						perTypeDetections typedetection;
						jake::jiObjectDetectionParams modelparam;
						modelparam.model=(char*)model_iter->path().c_str();
						o.detect(frame,modelparam,typedetection.detectionList);
						
						typedetection.detectionindex=numOfDetector;
						
						vector<std::string> splitstr;
						boost::split(splitstr,model_iter->path().leaf().string(),is_any_of("."));
						typedetection.detectionname=splitstr[0];
						typedetection.detectioncount=typedetection.detectionList.size();
						if(typedetection.detectionList.size()==0)
						{
							typedetection.maxdetectionscore=0.0f;
						}
						else
						{
							typedetection.maxdetectionscore=typedetection.detectionList[0].score;
						}
		
						cout<<"hi3"<<endl;
						framedetection.typeList.push_back(typedetection);
				
						std::sort(framedetection.typeList.begin(),framedetection.typeList.end());
						
						}
					}
				}
				//include the code to remove the part of the detections which is not satisfying the threshold.
						unsigned int index=0;
						for(int j=0;j<framedetection.typeList.size();j++)
						{
							if(framedetection.typeList[j].maxdetectionscore==0)
							{
								index=j;
								cout<<"breaking"<<endl;
								break;
							}
						}
						
						framedetection.typeList.resize(index);
				cout<<framedetection.typeList.size()<<endl;
				framedetectionList.push_back(framedetection);
			}
		}
	}
	cout<<framedetectionList[0].typeList.size()<<endl;	
	cout<<framedetectionList[1].typeList.size()<<endl;
	cout<<framedetectionList[2].typeList.size()<<endl;
	cout<<framedetectionList[3].typeList.size()<<endl;
	cout<<framedetectionList[4].typeList.size()<<endl;
	
	for(int i=0;i<framedetectionList[0].typeList.size();i++)
	{
	cout<<framedetectionList[0].typeList[i].maxdetectionscore<<endl;
	}
	for(int i=0;i<framedetectionList[1].typeList.size();i++)
	{
	cout<<framedetectionList[1].typeList[i].maxdetectionscore<<endl;
	}
	for(int i=0;i<framedetectionList[2].typeList.size();i++)
	{
	cout<<framedetectionList[2].typeList[i].maxdetectionscore<<endl;
	}
	for(int i=0;i<framedetectionList[3].typeList.size();i++)
	{
	cout<<framedetectionList[3].typeList[i].maxdetectionscore<<endl;
	}
	for(int i=0;i<framedetectionList[4].typeList.size();i++)
	{
	cout<<framedetectionList[4].typeList[i].maxdetectionscore<<endl;
	}

	std::vector<conceptDetections> conceptList;
	generateConcepts(framedetectionList,conceptList,3);
	cout<<conceptList.size()<<endl;
	
	for(int k=0;k<conceptList.size();k++)
	{
		for (set<conceptEntry>::iterator it = conceptList[k].detectorset.begin(); it != conceptList[k].detectorset.end(); it++) 
		{
		cout<<it->detectionname<<endl;
		}
		cout<<"======================================================="<<endl;
	}
	
} 
