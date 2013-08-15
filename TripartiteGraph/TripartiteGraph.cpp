#include <boost/config.hpp> // put this first to suppress some VC++ warnings

#include <iostream>
#include <iterator>
#include <algorithm>
#include <time.h>

#include <boost/utility.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace std;
using namespace boost;

namespace tptg
{

enum type{subject,verb,object};

struct TripartiteGrapgparam
{
	std::vector<std::string> subjectlist;
	std::vector<std::string> verblist;
	std::vector<std::string> objectlist;
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
TripartiteGrapgparam param;
Collection subjlist;
Collection verlist;
Collection objlist;

public:

TripartiteGraph(UndirectedGraph& tptgraph,TripartiteGrapgparam& parameters):udgraph(tptgraph),param(parameters){}

void constructvertex()
{
	 typedef typename UndirectedGraph::vertex_property_type Name;
	 for(int i=0;i<sizeof(param.subjectlist);i++)
	 {
		 subjlist.nodelist.push_back(add_vertex(Name(param.subjectlist[i]),udgraph));
	 }
	 for(int i=0;i<sizeof(param.verblist);i++)
	 {
		 verlist.nodelist.push_back(add_vertex(Name(param.verblist[i]),udgraph));
	 }
	 for(int i=0;i<sizeof(param.objectlist);i++)
	 {
		 objlist.nodelist.push_back(add_vertex(Name(param.objectlist[i]),udgraph));
	 } 
	 return;
}

void constructedge()
{
	
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
	matchsrc=getIndex(param.subjectlist,source);
	matchtgt=getIndex(param.verblist,target);
		if(matchsrc!=-1 && matchtgt!=-1)
		{
		src=subjlist.nodelist[matchsrc];
		tgt=verlist.nodelist[matchtgt];
		}
	}	
	else if(sourcetype==verb)
	{
	matchsrc=getIndex(param.verblist,source);
	matchtgt=getIndex(param.objectlist,target);
		if(matchsrc!=-1 && matchtgt!=-1)
		{
		src=verlist.nodelist[matchsrc];
		tgt=objlist.nodelist[matchtgt];
		}
	}	
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
		matchnode=getIndex(param.subjectlist,nodename);
		vertexnode=subjlist.nodelist[matchnode];
	}
	else if(wordtype==verb)
	{
		matchnode=getIndex(param.verblist,nodename);
		vertexnode=verlist.nodelist[matchnode];
	}
	else 
	{
		matchnode=getIndex(param.objectlist,nodename);
		vertexnode=objlist.nodelist[matchnode];
	}
	return get(vname,vertexnode);
}

};

}
