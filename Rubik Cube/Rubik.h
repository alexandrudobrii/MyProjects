#include "WorldDrawer3d.h"
#include <ctime>

/* identificatori pentru fiecare strat in parte */
#define BOTTOM 0
#define CENTER_X 1
#define TOP 2
#define LEFT 3
#define CENTER_Y 4
#define RIGHT 5
#define FRONT  6
#define CENTER_Z 7
#define BACK 8
#define UP 9
#define DOWN 10

/* identificatori pentru determinarea 
 * vitezei  de roatie a cubului*/
#define MOVING_LAYERS 9
#define MAX_ITERATION 157
#define LAYER_SPEED 0.01f
#define RUBIK_SPEED 0.01f
#define NONE 1000

typedef std::vector<int> Layer;
typedef std::vector<Layer> Layers;
typedef std::vector<Object3d *> Cube;

class Rubik { 
  public:
  	Rubik();
  	~Rubik();
  	void rotate_all (int direction, float speed);
  	void rotate_layer (int layer, float speed);
	void scale (int i, int sign);
	void add();
	//void draw_face(int, std::vector<float>);
    std::vector<int> get_topology();
    std::vector<Point3d> get_points();
	std::vector<int> get_layer (int layer);
	Cube get_components (int i);
	
  private:
    void set_points();
  	void set_topology();
    void set_layers();
	//void set_colors();
    void draw_faces();
    void rotateX (int i, Point3d center, float speed);
    void rotateY (int i, Point3d center, float speed);
    void rotateZ (int i, Point3d center, float speed);

    CoordinateSystem3d *cs;
	Layers layers;
	std::vector< std::vector<float> >colors;
	std::vector <Cube> rubik;
	std::vector<Point3d> points;
	std::vector<int> topology;
	std::vector< std::vector<int> > faces;
};

