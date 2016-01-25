#include <list>
#include <iostream>
using std::list;
#include <vector>
class Ball;
class Block;
struct Vector{
	float x;
	float y;
};
void Normalize(Vector&);
class Destroyable{
public:
	bool destroyed = false;
	virtual ~Destroyable(){};
	virtual void destroy() = 0;
};
class Paintable : public Destroyable{
	
public:
	virtual void paint() = 0;
	virtual ~Paintable(){}
protected:
	double color[3];
};


class Touchable : public Paintable{
public:
	virtual bool touch(Ball& ball);
	virtual ~Touchable(){}
};

class Movable{
public:
	virtual void move() = 0;
	virtual ~Movable(){}
};


class Ball : public Paintable,
	 public Movable{
public:
	virtual ~Ball(){}
	//virtual ~Ball();
	Ball(float x) : x(x){
		r = 0.01f;
		//x = 0;
		y = -0.9f;
		vector.x = 0.5f;
		vector.y = 1.0f;
		Normalize(vector);
		RECT wcoord;
		extern HWND hWnd;
		GetClientRect(hWnd, &wcoord);
		rver = r * wcoord.right / wcoord.bottom; 
			//rhor = r;
	}
	Ball() : Ball(0) {
		
	}
	virtual void move();
	virtual void paint();
	virtual void destroy(){}
	float r;
	float rver;
	float x;
	float y;
	Vector vector;
	float speed = 250.0f;
	float incSpeed = 0;
};



class Block : public Touchable{
	
public:
	virtual ~Block(){}
	Block(){}
	Block(float x, float y) : Block(x, y, 0.19f, 0.09f){}
	Block(float x, float y, float w, float h) : x(x), y(y), w(w), h(h){
		color[0] = color[1] = color[2] = 0.7f;
	}
	virtual void paint();
	virtual bool touch(Ball& ball);
	virtual void destroy();
protected:
	
	float x;
	float y; 
	float w; 
	float h;
};

class Board : public Block{
public:
	virtual ~Board(){
	}
	float radius;
	float xCoord;
	void setXCoord(float newValue){
		xCoord = newValue;
		x = xCoord - radius;
	}
	Board(){
		color[0] = color[1] = color[2] = 1;
		radius = 0.15f;
		xCoord = 0.0f;
		
		y = -0.95f;
		w = 2 * radius;
		h = 0.05f;
	}
	void paint();
	virtual void destroy(){}
	bool touch(Ball& ball);
};

class Border : public Block{
public:
	virtual ~Border(){
	}
	Border(float x, float y, float w, float h) : Block(x,y,w,h){
		color[0] = color[1] = color[2] = 1;
	}
	virtual void destroy(){}
};

class MovableBlock : public Block, public Movable{
public:
	virtual ~MovableBlock(){}
	MovableBlock(float x, float y) : Block(x, y) {
		dx = dy = 0;
		ddx = 0.0005;
		ddy = 0;
		maxdx = 0.05;
		maxdy = 0;
		touchesCount = 0;
		maxTouches = 3;
		color[0] = color[1] = color[2] = 1;
	}
	virtual void move();
	virtual void destroy();
	virtual void paint();
protected:
	
	int touchesCount;
	int maxTouches;
	float maxdx;
	float maxdy;
	float dx;
	float dy;
	float ddx;
	float ddy;
};

/*
class Line : public Touchable{
public:
	virtual ~Line(){}
	Line(float x1, float y1, float x2, float y2)
		:x1(x1), x2(x2), y1(y1), y2(y2) 
	{
		a = y2 - y1;
		b = x1 - x2;
		c = y1*x2 - x1*y2;
	}
	virtual void paint();
	virtual bool touch(Ball& ball);
	virtual void destroy(){
		destroyed = true;
	}
protected:
	//ax+by+c=0
	float a;
	float b;
	float c;
	float x1;
	float x2;
	float y1;
	float y2;
};

class BlockFromLines : public Touchable{
public:
	BlockFromLines(float x, float y) : x(x), y(y){
		w = 0.19f;
		h = 0.09f;

		lines.push_back(Line(x, y, x + w, y));
		lines.push_back(Line(x + w, y, x + w, y - h));
		lines.push_back(Line(x + w, y - h, x, y - h));
		lines.push_back(Line(x, y - h, x, y));
	}
	virtual void paint();
	virtual bool touch(Ball& ball);
	virtual void destroy();
protected:
	float x;
	float y;
	float w;
	float h;

	std::vector<Line> lines;
};*/


















class Screen : public Paintable, public Movable{
private:
	//vector<Paintable*> objects;
	/*std::vector<unique_ptr<Paintable>> paintables;
	std::vector<unique_ptr<Ball>> balls;
	std::vector<unique_ptr<Touchable>> touchables;*/
	Paintable** c;
	Ball** b;
	Touchable** t;
	Movable** m;
	int endm;
	int endb = 0;
	int end = 0;
	int endt = 0;
public:
	Board board;
	//Board board2;
	Screen(){
		c = new Paintable*[300];
		b = new Ball*[100];
		t = new Touchable*[200];
		m = new Movable*[200];
		add(new Border(-1, 1, 2, 0.05f));
		add(new Border(-1, 1, 0.05f, 2));
		add(new Border(0.95f, 1, 0.05f, 2));
		
	}
	void destroy(){}
	~Screen(){
		for (int i = 0; i < end; i++){
			delete c[i];
		}
		delete[] c;
		
		delete[] b;
		
		delete[] t;

		delete[] m;
	}
	void paint(){
		
		/*for (auto i = paintables.begin(); i != paintables.end(); i++){
			if (!i->get()->destroyed)
				i->get()->paint();
		}*/
		for (int i = 0; i < end; i++){
			if (!c[i]->destroyed)
				c[i]->paint();
		}
		board.paint();
		//board2.paint();
		
	}
	
	void move(){
		
		/*for (auto i = balls.begin(); i != balls.end(); i++){
			i->get()->move();
			board.touch(*(i->get()));
			for (auto j = touchables.begin(); j != touchables.end(); j++){
				if (!j->get()->destroyed)
				if (j->get()->touch(*(i->get())))
					j->get()->destroy();
			}
		}*/
		/*for (int i = 0; i < endb; i++){
			b[i]->move();
			board.touch(*b[i]);
			//board2.touch(*b[i]);
			
			for (int j = 0; j < endt; j++){
				if (!t[j]->destroyed)
				if (t[j]->touch(*b[i])){
					t[j]->destroy();
				}
			}
		}*/
		for (int i = 0; i < endm; i++){
			m[i]->move();
		}
		for (int i = 0; i < endb; i++){
			board.touch(*b[i]);
			for (int j = 0; j < endt; j++){
				if (!t[j]->destroyed)
				if (t[j]->touch(*b[i])){
					t[j]->destroy();
				}
			}
		}
		static float f = board.radius-0.05f;
		static float df = 0.001f;
		if (f < -board.radius+0.05f)
			df = 0.001f;
		if (f > board.radius-0.05f)
			df = -0.001f;
		f += df;
		Ball* min = b[0];
		for (int i = 1; i < endb; i++)
		if (b[i]->y < min->y)
			min = b[i];
		board.setXCoord(min->x + f);
		//board2.setXCoord(b[1]->x + f);
		
		////board.setXCoord(balls.at(0)->x + f);
	}
	void add(Paintable* p){
		c[end++] = p;
		//paintables.push_back(unique_ptr<Paintable>(p));
		using namespace std;
		if (Movable* mp = dynamic_cast<Movable*>(p)){
			m[endm++] = mp;
		}
		if (Ball* bp = dynamic_cast<Ball*>(p)){
			b[endb++] = bp;
			//balls.push_back(unique_ptr<Ball>(mp));
		}
		if (Touchable* tp = dynamic_cast<Touchable*>(p)){
			t[endt++] = tp;
			//touchables.push_back(unique_ptr<Touchable>(tp));
		}
		
		//objects.push_back(p);
	}
	void remove(Paintable* p){
		//objects.remove(p);
	}

};