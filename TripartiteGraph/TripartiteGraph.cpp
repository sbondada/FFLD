#include <boost/config.hpp> // kept it to suppress some VC++ warnings

#include <iostream>
#include <iterator>
#include <algorithm>
#include <time.h>

#include <boost/utility.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

namespace tptg
{

enum type{subject,verb,object};

struct TripartiteGraphVertexParam
{
	std::vector<std::string> subjectvlist;
	std::vector<std::string> verbvlist;
	std::vector<std::string> objectvlist;
};

struct TripartiteGraphEdgeParam
{

	std::vector<std::pair<std::string,int> > subjectverbelist;
	std::vector<std::pair<std::string,int> > verbobjectelist;
};

struct edgeprop
{
	std::string edgename;
	float weight;
};

typedef property < vertex_name_t,std::string >name;
typedef adjacency_list < vecS, vecS, undirectedS,name,edgeprop,vecS> UndirectedGraph;

struct Collection
{
	std::vector<graph_traits<UndirectedGraph>::vertex_descriptor> nodelist; 
};

class TripartiteGraph
{
	
private:

UndirectedGraph udgraph;
TripartiteGraphVertexParam vparam;
TripartiteGraphEdgeParam eparam;
Collection subjlist;
Collection verlist;
Collection objlist;

public:

TripartiteGraph(UndirectedGraph& tptgraph,TripartiteGraphVertexParam& vparameters,TripartiteGraphEdgeParam& eparameters):udgraph(tptgraph),vparam(vparameters),eparam(eparameters){}

void constructvertex()
{
	 typedef typename UndirectedGraph::vertex_property_type Name;
	 for(int i=0;i<sizeof(vparam.subjectvlist);i++)
	 {
		 subjlist.nodelist.push_back(add_vertex(Name(vparam.subjectvlist[i]),udgraph));
	 }
	 for(int i=0;i<sizeof(vparam.verbvlist);i++)
	 {
		 verlist.nodelist.push_back(add_vertex(Name(vparam.verbvlist[i]),udgraph));
	 }
	 for(int i=0;i<sizeof(vparam.objectvlist);i++)
	 {
		 objlist.nodelist.push_back(add_vertex(Name(vparam.objectvlist[i]),udgraph));
	 } 
	 return;
}

void constructedge()
{
	std::vector<std::string> splitstr;
	typename graph_traits<UndirectedGraph>::vertex_descriptor src,tgt;
	int matchsrc,matchtgt;
	for(int i=0;i<eparam.subjectverbelist.size();i++)
	{
	boost::split(splitstr,eparam.subjectverbelist[i].first,boost::is_any_of("-"));
	if(splitstr.size()==2)
	{
		matchsrc=getIndex(vparam.subjectvlist,splitstr[0]);
		matchsrc=getIndex(vparam.verbvlist,splitstr[1]);
		if(matchsrc!=-1 && matchtgt!=-1)
		{
		src=subjlist.nodelist[matchsrc];
		tgt=verlist.nodelist[matchtgt];
		typename graph_traits<UndirectedGraph>::edge_descriptor e1;
		bool found;
		boost::tie(e1,found)=add_edge(src,tgt,udgraph); 
		udgraph[e1].edgename=eparam.verbobjectelist[i].first;
		udgraph[e1].weight=eparam.verbobjectelist[i].second;
		}	
	}
	}
	for(int i=0;i<eparam.verbobjectelist.size();i++)
	{
	boost::split(splitstr,eparam.verbobjectelist[i].first,boost::is_any_of("-"));
	if(splitstr.size()==2)
	{
		matchsrc=getIndex(vparam.verbvlist,splitstr[0]);
		matchsrc=getIndex(vparam.objectvlist,splitstr[1]);
		if(matchsrc!=-1 && matchtgt!=-1)
		{
		src=verlist.nodelist[matchsrc];
		tgt=objlist.nodelist[matchtgt];
		typename graph_traits<UndirectedGraph>::edge_descriptor e1;
		bool found;
		boost::tie(e1,found)=add_edge(src,tgt,udgraph); 
		udgraph[e1].edgename=eparam.verbobjectelist[i].first;
		udgraph[e1].weight=eparam.verbobjectelist[i].second;
		}	
	}
	}
}

int getIndex(std::vector<std::string>& collist,std::string matchstr)
{
	int index=0;
	for(std::vector<std::string>::iterator it = collist.begin(); it != collist.end(); ++it) 
	{
		if(matchstr.compare(*it)==0)
		{
			return index;
		}
		else
		{
			index++;
	    }
    }
    return -1;
}

edgeprop getEdgeDescription(std::string source,std::string target,type sourcetype)
{
	typename property_map < UndirectedGraph,std::string edgeprop::*>::type
    edgname = get(&edgeprop::edgename, udgraph);
    typename property_map < UndirectedGraph,float edgeprop::*>::type
    edgweight = get(&edgeprop::weight, udgraph);
	typename graph_traits<UndirectedGraph>::vertex_descriptor src,tgt;
	int matchsrc,matchtgt;
	if(sourcetype==subject)
	{
	matchsrc=getIndex(vparam.subjectvlist,source);
	matchtgt=getIndex(vparam.verbvlist,target);
		if(matchsrc!=-1 && matchtgt!=-1)
		{
		src=subjlist.nodelist[matchsrc];
		tgt=verlist.nodelist[matchtgt];
		}
	}	
	else if(sourcetype==verb)
	{
	matchsrc=getIndex(vparam.verbvlist,source);
	matchtgt=getIndex(vparam.objectvlist,target);
		if(matchsrc!=-1 && matchtgt!=-1)
		{
		src=verlist.nodelist[matchsrc];
		tgt=objlist.nodelist[matchtgt];
		}
	}	
	//if(src// add the test case
	typename graph_traits<UndirectedGraph>::edge_descriptor e1,e2;
	bool found;
	boost::tie(e1,found)=edge(src,tgt,udgraph);
	edgeprop edgedescription;
	edgedescription.edgename=get(edgname,e1);
	edgedescription.weight=get(edgweight,e1);
	return edgedescription;
}

std::string getVertexDescription(std::string nodename,type wordtype)
{
	typename property_map < UndirectedGraph, vertex_name_t >::type
    vname = get(vertex_name, udgraph);
	typename graph_traits<UndirectedGraph>::vertex_descriptor vertexnode;
	int matchnode;
	if(wordtype==subject)
	{
		matchnode=getIndex(vparam.subjectvlist,nodename);
		vertexnode=subjlist.nodelist[matchnode];
	}
	else if(wordtype==verb)
	{
		matchnode=getIndex(vparam.verbvlist,nodename);
		vertexnode=verlist.nodelist[matchnode];
	}
	else 
	{
		matchnode=getIndex(vparam.objectvlist,nodename);
		vertexnode=objlist.nodelist[matchnode];
	}
	return get(vname,vertexnode);
}

};

}
