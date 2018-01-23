#include <bits/stdc++.h>
//#include "primitives.h"
#define ull unsigned long long
#define ll long long
#define ff first
#define ss second
#define pb push_back
#define ins insert
#define mp make_pair
using namespace std;


int n;

typedef struct{
    double x,y;
}point;

class Vertex;
class HalfEdge;
class Face;

class Vertex {
public:
	point c;
	HalfEdge* IncEdge;
    int chain;      // 0 for left chain, 1 for right chain
public:
	Vertex(point x)
	{
		c=x;
	}
	Vertex getCoord(Vertex v)
	{
		return v.c;
	}

};

class Face {
public:
	HalfEdge* IncEdge;
	Face(HalfEdge* e)
	{
	    IncEdge=e;
	}
};

class HalfEdge {
public:
	Vertex* Origin;
	HalfEdge* twin;
	HalfEdge* prev;
	HalfEdge* next;
	Face* IncFace;
	
public:
	HalfEdge(Vertex* v)
	{
		Origin=v;
	}
};

class Mesh {
public:
	vector<Vertex*> vertices;
	vector<Face*>	faces;
	vector<HalfEdge*>	edges;
	Face* OuterFace;
public:
	Mesh(vector<Vertex*> verts)
	{
	    vertices=verts;
		for(int j=0;j<n;j++)
        {
            vertices[j]->IncEdge=new HalfEdge(vertices[j]);
            edges.pb(vertices[j]->IncEdge);
        }
        for(int j=0;j<n;j++)
        {
            vertices[j]->IncEdge=new HalfEdge(vertices[j]);
            edges.pb(vertices[j]->IncEdge);
        }
		for(int j=0;j<n;j++)
        {
            if(j==n-1)
            {
                edges[j]->next=edges[0];
                edges[0]->prev=edges[j];
                edges[j]->twin=edges[n];
                edges[n]->twin=edges[j];
            }
            else {
                edges[j]->next=edges[j+1];
                edges[j+1]->prev=edges[j];
                edges[j]->twin=edges[j+n+1];
                edges[j+n+1]->twin=edges[j];
            }
        }
        for(int j=n;j<2*n;j++)
        {
            if(j==2*n-1)
            {
                edges[j]->prev=edges[n];
                edges[n]->next=edges[j];
            }
            else
            {
                edges[j]->prev=edges[j+1];
                edges[j+1]->next=edges[j];

            }
        }
        edges[0]->IncFace=new Face(edges[0]);
        faces.pb(edges[0]->IncFace);
        for(int j=1;j<n;j++)
            edges[j]->IncFace=edges[0]->IncFace;
        edges[n]->IncFace=new Face(edges[n]);
        OuterFace=edges[n]->IncFace;
        faces.pb(OuterFace);
        for(int j=n;j<2*n;j++)
            edges[j]->IncFace=edges[n]->IncFace;
	}
	vector<HalfEdge*> EdgefromOrigin(Vertex* u)
	{
        vector<HalfEdge*> temp;
        HalfEdge* e=u->IncEdge;
        HalfEdge* startedge=e;
        temp.pb(e);
        while(e->twin->next!=startedge)
        {
            e=e->twin->next;
            temp.pb(e);
        }
        return temp;
	}
	void adddiagonal(Vertex* u,Vertex* v)
	{
		//cout << u->c.x << " " << u->c.y << " " << v->c.x << " " << v->c.y << endl;
	    HalfEdge* e1=new HalfEdge(u);
	    HalfEdge* e2=new HalfEdge(v);
        e1->twin=e2; e2->twin=e1;
        vector<HalfEdge*> edgeList1=EdgefromOrigin(u);
        
        vector<HalfEdge*> edgeList2=EdgefromOrigin(v);
        
        int i=0,j=0;
        for(i=0;i<edgeList1.size();i++)
        {
        	int flag=0;
            for(j=0;j<edgeList2.size();j++)
            {
                if(edgeList1[i]->IncFace==edgeList2[j]->IncFace && edgeList1[i]->IncFace!=OuterFace)
                {
                	flag=1;
                    break;
            		
            	}
            }
            if(flag==1)	break;
            j=0;
        }
       //if(i==edgeList1.size())	cout << "Error here\n" << endl;
       
        e1->prev=edgeList1[i]->prev;
        (edgeList1[i]->prev)->next=e1;
        edgeList1[i]->prev = e2;
        e1->next=edgeList2[j];
        e2->next=edgeList1[i];
        e2->prev=edgeList2[j]->prev;
        (edgeList2[j]->prev)->next=e2;
        edgeList2[j]->prev = e1;
        e1->IncFace=new Face(e1);
        e2->IncFace=new Face(e2);
        
        for(int k=0;k<faces.size();k++)
        {
            if(faces[k]==edgeList1[i]->IncFace)
            {
                faces.erase(faces.begin()+k);
                break;
            }
        }
        
        faces.pb(e1->IncFace);
        faces.pb(e2->IncFace);
        HalfEdge* iter=e1->next;
        while(iter!=e1)
        {
        	
            iter->IncFace=e1->IncFace;
        	iter=iter->next;
        }
        
        iter=e2->next;
       
        while(iter!=e2)
        {
        	//cout<<"YES"<<endl;
            iter->IncFace=e2->IncFace;
        	iter=iter->next;
        }
        edges.pb(e1);
        edges.pb(e2);
		
	}
    static bool compareVertex(Vertex* u,Vertex* v)
    {
        if((u->c.y)>(v->c.y))   return true;
        else if(u->c.y==v->c.y && (u->c.x)<(v->c.x))    return true;
        else return false;

    }
	 static int orientation(point p,point q,point r)
	{
		double val=(q.x-p.x)*(r.y-p.y)-(q.y-p.y)*(r.x-p.x);		
		if(val==0) return 0;            // collinear points
		else if(val>0) return 1;        // left turn
		else return 2;                  // right turn
	}
	void triangulate_face(Face* f)
	{
		
        vector<HalfEdge*> fedges;
        vector<Vertex*> fvertices;
        HalfEdge* start=f->IncEdge;
        HalfEdge* curr;
        //fedges.pb(start);
        curr=start;
        while(curr->next!=start)
        {
            fvertices.pb(curr->Origin);
            //fedges.pb(curr);
            curr=curr->next;
        }
        fvertices.pb(curr->Origin);
        sort(fvertices.begin(),fvertices.end(),compareVertex);
        
        vector<HalfEdge*> temp = EdgefromOrigin(fvertices[0]);

        int i=0;
        for(i=0;i<temp.size();i++)
        {
        	
        	if(temp[i]->IncFace==f)
        	{
        		break;
        		
        	}
        }
        start=temp[i];
        curr=start;
        
        while(curr->next->Origin->c.y <= curr->Origin->c.y )
        {
        	curr->Origin->chain=0;
        	curr = curr->next;
        }
        while(curr->next->Origin->c.y >= curr->Origin->c.y )
        {
        	curr->Origin->chain=1;
        	curr = curr->next;
        }        
        if(curr==start)
        	cout << "Successful classification" << endl;
        fvertices[0]->chain=2;
        fvertices[fvertices.size()-1]->chain=2;
        
        stack<Vertex*> st;
        st.push(fvertices[0]);
        st.push(fvertices[1]);
        
        for(int j=2;j<fvertices.size()-1;j++)
        {
        	Vertex* vtemp = st.top();
        	//cout << fvertices[j]->chain << " " << vtemp->chain << endl; 
        	if(fvertices[j]->chain!=vtemp->chain)
        	{
        		while(!st.empty())
        		{
        			vtemp=st.top();
        			st.pop();
        			if(vtemp->chain!=2)
        			{
        				adddiagonal(fvertices[j],vtemp);
        			}
        		}
        		st.push(fvertices[j-1]);
        		st.push(fvertices[j]);
        	}
        	else
        	{
        		if(fvertices[j]->chain==0)			//left chain
        		{
        			vtemp=st.top();
        			st.pop();
        			while(orientation(st.top()->c, vtemp->c, fvertices[j]->c)==2)	//right turn
        			{
        				adddiagonal(fvertices[j],st.top());
        				vtemp=st.top();
        				//if(vtemp->chain==2)	break;
        				st.pop();
        				if(st.empty())	break;
        			}	
        			st.push(vtemp);
        		}
        		else if(fvertices[j]->chain==1)		//right chain
        		{
        			vtemp=st.top();
        			st.pop();
        			while(orientation(st.top()->c, vtemp->c, fvertices[j]->c)==1)	//left turn
        			{
        				adddiagonal(fvertices[j],st.top());
        				vtemp=st.top();
        				//if(vtemp->chain==2)	break;
        				st.pop();
        				if(st.empty())	break;
        			}	
        			st.push(vtemp);        			
        		}	
        		st.push(fvertices[j]);
        	}
        	
        	
        }
        Vertex* vtemp=st.top();
        st.pop();
        while(!st.empty())
        {
        	vtemp=st.top();
        	st.pop();
        	if(st.empty())	break;
			adddiagonal(fvertices[fvertices.size()-1], vtemp);
			
		}
	}
	void triangulate_monotone()
	{
	    vector<Face*> temp=faces;

        for(int i=0;i<temp.size();i++)
        {
        	if(temp[i]!=OuterFace)
            	triangulate_face(temp[i]);
        }
	}
	int isabove(point a,point b)
	{
    	if(a.y!=b.y) return a.y>b.y;
   		return a.x<b.x;
	}
	
	vector<int> classifyPoints(vector<point> v)
	{
		vector<int> ret(v.size(),0);
		int x=v.size();
		//cout<<ret[1]<<endl;
		//cout<<"helo"<<endl;
		for(int i=0;i<v.size();i++)
		{
		    point cur=v[i];
		    point prev=v[(i-1+x)%x];
		    point next=v[(i+1)%x];
		    if(isabove(cur,prev) && isabove(cur,next))
		    {
		        if(orientation(prev,cur,next)==1)
		            ret[i]=1;          // start vertex
		        else ret[i]=2;         // split vertex
		    }
		    else if(isabove(prev,cur) && isabove(next,cur))
		    {
		        if(orientation(prev,cur,next)==1)
		            ret[i]=3;   // end vertex
		        else
		            ret[i]=4;   // merge vertex
		    }
		    else
		    {
		        if(isabove(prev,cur) && isabove(cur,next))
		            ret[i]=5;   // left reg
		        else ret[i]=6; // right reg
		    }
		}
		return ret;
	}
	
	int getleft(point p,vector<pair<pair<point,point> ,int> > edl,set<int> s)
	{
		int ret=-1,md=1e7;
		for(int i=0;i<edl.size();i++)
		{
			cout<<i<<" "<<edl[i].ff.ff.x<<" "<<edl[i].ff.ff.y<<endl;
		    if(orientation(edl[i].ff.ss,edl[i].ff.ff,p)!=1 && edl[i].ff.ss.y<=p.y && p.y<=edl[i].ff.ff.y &&(p.x!=edl[i].ff.ff.x&&p.y!=edl[i].ff.ff.y))
		    {
		        int dx=abs(edl[i].ff.ff.x-p.x);
		        cout<<dx<<endl;
		        if(dx<md)
		        {
		            md=dx;
		            ret=i;
		        }
		    }
		   /// cout<<"RET "<<ret<<endl;
		}
		//cout<<"yoLO "<<ret<<endl;
		return ret;
	}

	int getright(point p,vector<pair<pair<point,point> ,int> > edl,set<int> s)
	{
		int ret=-1,md=1e7;
		for(int i=0;i<edl.size();i++)
		{
			//cout<<i<<" "<<edl[i].ff.ff.x<<" "<<edl[i].ff.ff.y<<endl;
		    if(orientation(edl[i].ff.ss,edl[i].ff.ff,p)!=2 && edl[i].ff.ss.y<=p.y && p.y<=edl[i].ff.ff.y &&(p.x!=edl[i].ff.ff.x&&p.y!=edl[i].ff.ff.y))
		    {
		        int dx=abs(edl[i].ff.ff.x-p.x);
		       // cout<<dx<<endl;
		        if(dx<md)
		        {
		            md=dx;
		            ret=i;
		        }
		    }
		   /// cout<<"RET "<<ret<<endl;
		}
		//cout<<"yoLO "<<ret<<endl;
		return ret;
	}

	int ismerge[1000];
	void handleStart(point p,int pid,vector<int>& help,set<int> s,map<pair<int,int> ,int> mm)
	{
		pair<int,int> pp;
		pp.ff=p.x;pp.ss=p.y;
		int eid=mm[pp];
		help[eid]=pid;
		s.ins(eid);
	}
	
	void handleSplit(point p,int pid,vector<int>& help,vector<pair<pair<point,point> ,int> > edl,set<int> s,map<pair<int,int> ,int> mm)
	{
		pair<int,int> pp;
		pp.ff=p.x;pp.ss=p.y;
		int lid=getleft(p,edl,s);
		// add edge between point pid and helper[lid]
		cout<<"tt "<<lid<<endl;
		cout<<help[lid]<<endl;
		adddiagonal(vertices[pid],vertices[help[lid]]);
		cout<<"ewew"<<endl;
		help[lid]=pid;
		s.ins(mm[pp]);
	}
	void handleEnd(point p,int pid,vector<int>& help,vector<pair<pair<point,point> ,int> > edl,set<int> s,map<pair<int,int> ,int> mm)
	{
	pair<int,int> pp;
		pp.ff=p.x;pp.ss=p.y;
		int lid=getleft(p,edl,s);
		if(ismerge[help[lid]])
			adddiagonal(vertices[pid],vertices[help[lid]]);
		    //add diagonal between pid and help[lid]
		//s.erase(lid);
	}
	void handleMerge(point p,int pid,vector<int>& help,vector<pair<pair<point,point> ,int> > edl,set<int> s,map<pair<int,int> ,int> mm)
	{
	pair<int,int> pp;
		pp.ff=p.x;pp.ss=p.y;
		int lid=getleft(p,edl,s);
		int rid=getright(p,edl,s);
		if(ismerge[help[lid]])
		    adddiagonal(vertices[pid],vertices[help[lid]]);
		if(ismerge[help[rid]])
		    adddiagonal(vertices[pid],vertices[help[lid]]);
		//s.erase(rid);
		//cout<<"tyy "<<lid<<" "<<help[lid]<<" "<<pid<<endl;
		help[lid]=pid;
		cout<<"RER "<<lid<<" "<<help[lid]<<endl;
	}

	void handleLeftReg(point p,int pid,vector<int>& help,vector<pair<pair<point,point> ,int> > edl,set<int> s,map<pair<int,int> ,int> mm)
	{
		pair<int,int> pp;
		pp.ff=p.x;pp.ss=p.y;
		int lid=getleft(p,edl,s);
		if(ismerge[help[lid]])
		    adddiagonal(vertices[pid],vertices[help[lid]]);
		//s.erase(lid);
		s.ins(mm[pp]);
		//cout<<"EWEWWWW "<<mm[pp]<<" "<<pid<<endl;
		help[mm[pp]]=pid;
		cout<<"now "<<mm[pp]<<" "<<help[mm[pp]]<<endl;
	}
	void handleRightReg(point p,int pid,vector<int>& help,vector<pair<pair<point,point> ,int> > edl,set<int> s,map<pair<int,int> ,int> mm)
	{
		pair<int,int> pp;
		pp.ff=p.x;pp.ss=p.y;
		int lid=getleft(p,edl,s);
		if(ismerge[help[pid]])
		    adddiagonal(vertices[pid],vertices[help[lid]]);
		help[lid]=pid;
	}
	
	void splitMonotone(vector<point> v)
	{
		vector<int> vclass=classifyPoints(v);
		cout << "higi" << endl;
		int id=0;
		vector<pair<pair<point,point> ,int> > edl;
		map<pair<int,int> ,int> rep;
		for(int i=0;i<v.size();i++)
		{

		    point cur=v[i];
		    point next=v[(i+1)%v.size()];
		    rep[mp(cur.x,cur.y)]=id;
		    cout<<cur.x<<" "<<cur.y<<" "<<id<<endl;
		    //id++;
		    if(next.y>cur.y) continue;
		   // cout<<cur.x<<" "<<cur.y<<" "<<id<<endl;
		    edl.pb(mp(mp(cur,next),i));
		 	id++;
		}
		for(int i=0;i<edl.size();i++)
			cout<<i<<" "<<edl[i].ff.ff.x<<" "<<edl[i].ff.ff.y<<endl;
		vector<int> helper(edl.size(),-1);
		set<pair<pair<int,int> ,int> > q;
		set<int> stat;
		for(int i=0;i<v.size();i++)
		    q.ins(mp(mp(-v[i].y,v[i].x),i));
		while(!q.empty())
		{
		    pair<int,int> top=q.begin()->ff;
		    point x;
		    x.x=top.ss;
		    x.y=-top.ff;
		    int id=q.begin()->ss;
		    q.erase(q.begin());
		    cout<<vclass[id]<<endl;
		    if(vclass[id]==1)
		        handleStart(x,id,helper,stat,rep);
		    else if(vclass[id]==2)
		        handleSplit(x,id,helper,edl,stat,rep);
		    else if(vclass[id]==3)
		        handleEnd(x,id,helper,edl,stat,rep);
		    else if(vclass[id]==4)
		        handleMerge(x,id,helper,edl,stat,rep);
		    else if(vclass[id]==5)
		        handleLeftReg(x,id,helper,edl,stat,rep);
		    else
		        handleRightReg(x,id,helper,edl,stat,rep);
		}
		cout<<"DOME"<<endl;
	}

	void makeYmonotone()
	{
		vector<point> v;
		for(int i=0;i<vertices.size();i++)
		{
			v.pb(vertices[i]->c);
			
		}
		//cout<<"helo"<<endl;
		splitMonotone(v);
	}
	void test()
	{
		adddiagonal(vertices[1],vertices[5]);
		adddiagonal(vertices[2],vertices[5]);
		adddiagonal(vertices[2],vertices[4]);
		//cout << faces.size() << endl;
	}
};

int main(void)
{
	cout << "Enter the number of vertices in the polygon" << endl;

	cin >> n;
	cout << "Enter the coordinates of the vertices in anti-clockwise order" << endl;
	vector<Vertex*> vertices;
	for(int i=0;i<n;i++)
	{

		point c;
		cin >> c.x >> c.y;
		Vertex* v=new Vertex(c);
		vertices.pb(v);
	}

	Mesh* polygon=new Mesh(vertices);
	
	//polygon->makeYmonotone();
	polygon->triangulate_monotone();
	//polygon->test();
	ofstream output;
	output.open("polygon.txt");
	output << polygon->edges.size() << endl;
	int vert = polygon->vertices.size();
	int edg = polygon->edges.size();
	//output << vert << " " << edg << endl;
	//for(int i=0;i<vert;i++)
	//	cout << polygon.vertices[i]->coord.x << " " << polygon.vertices[i]->coord.y << endl;
	for(int i=0;i<edg;i++)
	{
		output << polygon->edges[i]->Origin->c.x << " " << polygon->edges[i]->Origin->c.y << " " << polygon->edges[i]->next->Origin->c.x << " " << polygon->edges[i]->next->Origin->c.y << endl;
	}
	 
	
}


