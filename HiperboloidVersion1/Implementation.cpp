#include "stdafx.h"
#include "HiperboloidVersion1.h"




void Block::paint(){
	glColor3dv(color);
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x + w, y);
	glVertex2f(x + w, y - h);
	glVertex2f(x, y - h);
	glColor3d(1, 1, 1);
	glEnd();
}

void Board::paint(){
	glColor3dv(color);
	glBegin(GL_QUADS);
	glVertex2f(xCoord - radius, -1);
	glVertex2f(xCoord + radius, -1);
	glVertex2f(xCoord + radius, -0.95f);
	glVertex2f(xCoord - radius, -0.95f);
	glEnd();
}
bool Touchable::touch(Ball& ball){
	return false;

}

const int BALL_SIDES_COUNT = 6;
bool Block::touch(Ball& ball){
	
	OpenGLCoord ballEdges[BALL_SIDES_COUNT];
	for (int i = 0; i < BALL_SIDES_COUNT; i++)
	{
		float angle = 2.0 * 3.1415926 * float(i) / float(BALL_SIDES_COUNT);

		ballEdges[i].coordX = ball.x + ball.r * cosf(angle);
		ballEdges[i].coordY = ball.y + ball.rver * sinf(angle);
	}
	
	for (int side = 0; side < BALL_SIDES_COUNT; side++){
		float xBall = ballEdges[side].coordX;
		float yBall = ballEdges[side].coordY;
		if (xBall > x && xBall < x + w && yBall < y && yBall > y - h){
		/*if (ballCorners[side].x > x && ballCorners[side].x < x + w &&
			ballCorners[side].y < y && ballCorners[side].y > y - h){*/

			float left = xBall - x;
			float right = abs(xBall - (x + w));
			float top = abs(yBall - y);
			float bot = abs(yBall - (y - h));
			float min = left;

			if (right < min)
				min = right;
			if (top < min)
				min = top;
			if (bot < min)
				min = bot;

			if (min == left || min == right){
				ball.vector.x = -ball.vector.x;
				return true;
			}
			else{
				ball.vector.y = -ball.vector.y;
				return true;
			}


		}
	}
	//точки контроля
	//glColor3f(1.0, 1.0, 1.0);
	return false;
}

void Ball::paint(){

	glBegin(GL_POLYGON);

	for (int i = 0; i < 10; i++)
	{
		float angle = 2.0 * 3.1415926 * float(i) / float(10);

		float dx = r * cosf(angle);
		float dy = rver * sinf(angle);
		glVertex2f(x + dx, y + dy);
	}
	glEnd();
	

}
void Ball::move(){
	
	incSpeed++;
	if (incSpeed > 1000){
		incSpeed = 0;
		if (speed > 80.0f)
		speed *= 0.95f;
	}
	x += vector.x / speed * (r / rver);
	y += vector.y / speed;
	if (y < -1)
		exit(0);
}


void Block::destroy(){
	destroyed = true;
}


bool Board::touch(Ball& ball){
	
	if (Block::touch(ball)){
		float d = (ball.x - xCoord)/radius;
		ball.vector.x +=  d;
		ball.y += 0.02f;
		Normalize(ball.vector);
		return true;
	}
	return false;
}
/*float sqr(float x){
	return x*x;
}
float range_p_p(float x1, float y1, float x2, float y2){
	return sqrtf(sqr(x1 - x2) + sqr(y1 - y2));
}
void rotateVec(Vector& v, float fi){
	v.x = v.x*cos(fi) - v.y*sinf(fi);
	v.y = v.y*cos(fi) + v.x*sin(fi);
}
float range_p_s(float xp, float yp, float x1, float y1, float x2, float y2){
	float a = range_p_p(x1, y1, x2, y2);
	float b = range_p_p(x1, y1, xp, yp);
	float c = range_p_p(x2, y2, xp, yp);
	float p = (a + b + c) / 2;
	if (b*b > c*c + a*a) return c;
	if (c*c > b*b + a*a) return b;
	return 2 * sqrtf(p*(p - a)*(p - b)*(p - c)) / a;
}
bool Line::touch(Ball& ball){
	float xr = ball.vector.x * ball.r*1.2f;
	float yr = ball.vector.y * ball.r*1.2f;
	
	/*float xb = ball.x;
	float yb = ball.y;
	float rlen = sqrtf((xr - xb)*(xr - xb) + (yr - yb)*(yr - yb));
	float xb = ball.x;
	float yb = ball.y;
	float rlen = ball.r;
	//float range = (a*xb + b*yb + c) / sqrtf(a*a + b*b);
	float range = range_p_s(xb, yb, x1, y1, x2, y2);
	if (abs(range) > rlen)
		return false;
	float a1 = ball.vector.x;
	float b1 = ball.vector.y;
	
	Normalize(ball.vector);
	return true;

}

void Line::paint(){
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}

void BlockFromLines::paint(){
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x + w, y);
	glVertex2f(x + w, y - h);
	glVertex2f(x, y - h);
	glEnd();
	/*for each (Line l in lines)
	{
		l.paint();
	}
}
bool BlockFromLines::touch(Ball& ball){
	for each (Line l in lines)
	{
		if (l.touch(ball))
			return true;;
	}
	return false;
}
void BlockFromLines::destroy(){
	destroyed = true;
	for each (Line l in lines)
	{
		l.destroy();
	}
}
*/
void MovableBlock::move(){
	if (destroyed) return;
	if (abs(dx) > maxdx)
		ddx *= -1;
	if (abs(dy) > maxdy)
		ddy *= -1;
	dx += ddx;
	dy += ddy;
	x += ddx;
	y += ddy;
}
void MovableBlock::destroy(){
	if (++touchesCount >= maxTouches)
		Block::destroy();
	if (touchesCount == 1){
		for (int i = 0; i < 3; i++)
			color[i] /= 1.5;
	}
	if (touchesCount == 2){
		for (int i = 0; i < 3; i++)
			color[i] /= 3;
	}
}
void MovableBlock::paint(){
	Block::paint();
}