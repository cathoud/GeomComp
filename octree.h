#include <CGAL/Simple_cartesian.h>
#include <CGAL/Point_2.h>
#include <CGAL/Vector_2.h>
#include <CGAL/Point_3.h>


#include <CGAL/IO/Geomview_stream.h>
#include <CGAL/IO/Triangulation_geomview_ostream_2.h>
#include <CGAL/IO/Triangulation_geomview_ostream_3.h>
#include <CGAL/IO/Polyhedron_geomview_ostream.h>

//My Includes
#include <CGAL/number_utils.h>
#include <CGAL/bounding_box.h>
#include <CGAL/Polyhedron_3.h>

#include <iostream>
#include <vector>

using namespace std;

typedef CGAL::Simple_cartesian<double> Kernel; //Kernel for building on cartesian double coordinates
typedef Kernel::Point_2 Point_2; //Point type for our kernel
typedef Kernel::Segment_2 Segment_2; //Segment type for our kernel
typedef Kernel::Vector_2 Vector_2;

//3D
typedef Kernel::Point_3 Point_3;
typedef Kernel::Vector_3 Vector_3;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;


class GeomObject {
  public:
  	GeomObject(){};
  	GeomObject(vector<Point_3> pts): points(pts) {
  		CGAL::Bbox_3 b = bbox_3(pts.begin(),pts.end());
  		center = Point_3((b.xmin()+b.xmax())/2,(b.ymin()+b.ymax())/2,(b.zmin()+b.zmax())/2);
  		for(int i = 0; i<3; i++) {
  			half_edges[i] = abs(center.cartesian(i) - b.min(i));
  		}

  		/*
  		half_x = center.x()-b.xmin();
  		half_y = center.y()-b.ymin();
  		half_z = center.z()-b.zmin();
  		*/
  	}

	vector<Point_3> points;
	Point_3 center;
	double half_edges[3];

	/*
	double half_x;
	double half_y;
	double half_z;
	*/
};

class NodeOctree {
  public:
  	NodeOctree(){};
  	NodeOctree(Point_3 center, double halfwidth, NodeOctree* parent): center(center),halfwidth(halfwidth), parent(parent) {
  	};

  	void insert(GeomObject* obj);
  	vector<GeomObject>::iterator getObjects();
  	void remove(GeomObject* obj);

  private:
  	NodeOctree* child[8];
  	NodeOctree* parent;
  	Point_3 center;
  	double halfwidth;
  	vector<GeomObject*> objects;
};

void NodeOctree::insert(GeomObject* obj) {
	int octant = 0;
	bool straddleNode = false;
	bool straddleChild = false;

	for(int i = 0; i<3; i++) {
		float delta = obj->center.cartesian(i) - center.cartesian(i) ; //Distancia do centro do objeto ao centro do nodo

		//Verifica se extensão do objeto e menor que o do nodo e se delta
		if(halfwidth < obj->half_edges[i] || abs(delta) > halfwidth - obj->half_edges[i]) {
			straddleNode = true;
			break;
		}
		
		if(abs(delta) < obj->half_edges[i]) {
			straddleChild = true;
			break;
		}

		if(delta > 0)
			octant |= (1 << i);

	}

	//TODO: alterar a ordem das condições
	if(!straddleChild)
		child[octant]->insert(obj);
	else if(!straddleNode)
		objects.push_back(obj);
	else {
		int octant = 0;
		for(int i = 0; i<3; i++) {
			if(true);
		}
	}
}

class Octree {
  public:
  	Octree();
  	void insert(GeomObject *);

  private:
	NodeOctree* root;
};

void Octree::insert(GeomObject *obj) {
	int octant = 0;
	bool straddle = false;

	for(int i = 0; i<3; i++) {
		float delta = obj->center.cartesian(i) - center.cartesian(i) ; //Distancia do centro do objeto ao centro do nodo

		//Verifica se extensão do objeto e menor que o do nodo e se delta
		if(halfwidth < obj->half_edges[i] || abs(delta) > halfwidth - obj->half_edges[i]) {
			straddle = true;
			break;
		}
	}

	/*
	for(int i = 0; i<3; i++) {
		float delta = obj->center.cartesian(i) - center.cartesian(i) ; //Distancia do centro do objeto ao centro do nodo

		//Verifica se extensão do objeto e menor que o do nodo e se delta
		if(halfwidth < obj->half_edges[i] || abs(delta) > halfwidth - obj->half_edges[i]) {
			straddleNode = true;
			break;
		}
		
		if(abs(delta) < obj->half_edges[i]) {
			straddleChild = true;
			break;
		}

		if(delta > 0)
			octant |= (1 << i);

	}

	//TODO: alterar a ordem das condições
	if(straddleNode) {

	}
	if(!straddleChild)
		child[octant]->insert(obj);
	else if(!straddleNode)
		objects.push_back(obj);
	else {
		int octant = 0;
		for(int i = 0; i<3; i++) {
			if(true);
		}
	}
	*/
}