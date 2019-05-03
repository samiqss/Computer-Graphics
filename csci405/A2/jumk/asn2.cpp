#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "mathvector.h"
using namespace std;

#define MAX_RAY_DEPTH 4

//Some Notes for self:
//equation of sphere is (x-Cx)^2+(y-Cy)^2+(z-Cz)^2-r^2=0 <-c is center
//sphere vector form (P-C).(P-C)-r^2=0
//point along ray pt=O+tD <- O is origin D is direction t is kind of like distance from point
//Now if we sub pt in for P in the sphere vector form (O+tD-C).(O+tD-C)-r^2=0
//=> (D.D)t^2 +2D.(O-C)t+(O-C).(O-C)-r^2=0
//then we solve for t using quadratic equation 
//a = (D.D)
//b = 2D.(O-C)
//c = (O-C).(O-C)-r^2
//t= -b+-sqrt(b^2-4ac) /2a
//if discriminant is negative, the ray misses the spehere 
//and if both t's are negative the sphere is behind the ray

const vector3 white(255, 255, 255);
const vector3 black(0, 0, 0);
const vector3 red(255, 0, 0);

struct Ray {
	vector3 o; //origin 
	vector3 d; //direction
	Ray(const vector3 origin, const vector3 direction){
		o=origin;
		d=direction;
	}
};


//material types
enum Refl {AMB, DIFF, SPEC};

struct Sphere {
	vector3 c; //position at center 
	double  r;  //radius 
	vector3 color; //color
        Refl refl; //reflection type

	Sphere(const vector3 center, double radius, vector3 clr, Refl rfl){
		c=center;
		r=radius;
		color=clr;
		refl=rfl;
	}
        
	//return normal vector at cam intersec
	vector3 getNormal(const vector3 pi) const { return (pi - c).Normalize(); }
	
	bool intersect(const Ray& ray, double& t) const {
		double eps = 1e-4; //eps is small fudge factor
		const vector3 o = ray.o;
		const vector3 d = ray.d;
		const vector3 oc = o-c; 
		double b = 2*vector3::DotProduct(oc,d); //b from quadratic equation
		double c = vector3::DotProduct(oc,oc)-r*r; //c from quadratic equation
		double det = b*b-4*c; //(b^2-4ac), a is 1 due to ray normalized
		if(det < 1e-4) return false; //ray misses sphere
		det = sqrt(det);
		const double t0 = (-b-det)/2;
		const double t1 = (-b+det)/2;
		//return smaller positive t
		t = (t0 < t1) ? t0 : t1;
		return true;	
	}
};

//SCENE 
//some spheres in array to represent scene
// position radius emission color material 
Sphere sphere[] = {

	Sphere (vector3(1e5,250,250),1e5-400, vector3(130,140,150),DIFF),//right
	Sphere (vector3(-1e5,250,250),1e5+100, vector3(100,100,210),DIFF),//left
	Sphere (vector3(250,250,1e7),1e7, vector3(170,170,170),DIFF),//back
	Sphere (vector3(250,1e5,0),1e5-375, vector3(180,210,203),DIFF),//bottom
	Sphere (vector3(250,-1e5,0),1e5+100, vector3(210,210,210),DIFF),//top
	Sphere (vector3(150,350,-5500),60, vector3(160,80,100),DIFF),//sphere1
	Sphere (vector3(125*2+30,375,-5500),70, vector3(100,160,80),SPEC),//sphere2
	Sphere (vector3(125*3+30,375,-5500),30, vector3(200, 160,80),AMB),//sphere3
	Sphere (vector3(500,500,-7000),1, vector3(250,250,0), DIFF),//lighot1 with yellow color
	Sphere (vector3(0,0,-7000),1, vector3(250,250,250), DIFF)//light2 with white color
};

//size of array spheres, amount of lights and objects
int spheresNum= 10;
int lights = 2;
int objectNum=spheresNum - lights;

// just makes sure color is within bound
void clamp255(vector3 col){
	double xi= (col.X() > 255) ? 255 : (col.X() < 0) ? 0 : col.X();
	double yi= (col.Y() > 255) ? 255 : (col.Y() < 0) ? 0 : col.Y();
	double zi= (col.Z() > 255) ? 255 : (col.Z() < 0) ? 0 : col.Z();	
	col.X (xi);
	col.Y (yi);
	col.Z (zi);

}

//check one sphere at a time find closest 
bool trace(const Ray &r, double &t, int &id){
	
	double n= objectNum; //number of objects in scene
	double t_temp;
	double inf=t=1e20;
	bool truthVal=false;
	
	for(int i=0;i<n; i++){
		if(sphere[i].intersect(r,t_temp) && t_temp<t){
			t=t_temp;
			id=i;
			truthVal= true;
		}
	}
	return truthVal;
}

//lights up the object from all lights and bright objects near by
vector3 reflect(const Ray r, int id, double t, int depth ){
	
	vector3 x=r.o+r.d*t; //ray of intersection point
	Sphere temp = sphere[id];
	vector3 objColor = temp.color;
	
	
	for(int i=0;i<spheresNum; i++){//bright objects near by
		if(id != i && i < objectNum){
			vector3 L = sphere[i].c - x;
			vector3 N = sphere[id].getNormal(x);//normal @ camera direction
			double dt = vector3::DotProduct(L.Normalize(), N.Normalize());//if spec
			
			//here reflect depending on type
			if(temp.refl == SPEC) temp.color = (temp.color + sphere[i].color*dt);//spec
			else if (temp.refl == DIFF) temp.color = (temp.color + white*dt);//diffuse
			else temp.color = (temp.color + white*dt*0.2);//ambient
		
		}else if (i >= objectNum){//lights arounds
			vector3 L = sphere[i].c - x;
			vector3 N = sphere[id].getNormal(x);//camera direction
			double dt = vector3::DotProduct(L.Normalize(), N.Normalize());
			if(temp.refl == SPEC) temp.color = (temp.color + sphere[i].color*dt)*0.7;//spec
			else temp.color = (temp.color + sphere[i].color*dt)*0.5;

			
			//shadowing 
			for(int j=0;j<objectNum; j++){//search for objects blocked from light
				double tt;
				Ray shadow= Ray(sphere[id].c,x-sphere[i].c);
				if(i != j && id != j && sphere[j].intersect(shadow,tt)){
					if(tt<t && tt>=0) temp.color = black;
				}
			}
			//
		
		}
	}
	return temp.color;
	
}

//compute radiance estimate along ray r
vector3 radiance(const Ray &r){
	
	double t; //distance to intersection
	int id=0; //id of intersected obj
	if(!trace(r,t,id)) return vector3(0,0,0);//if not object hit return black
	
	// sphere[id] is the hit obj
	return reflect(r, id, t,0);
}

//loop over pixel, create image store it in PPM file
int main(){

	const int W=500; //image width
	const int H=500; //image height
	
	//background color
	const vector3 black(0, 0, 0);
	
	//start a ppm file
	std::ofstream out("out.ppm");
	out << "P3\n" << W << ' ' << H <<' '<<"255\n";

	vector3 pix_col= black; //color buffer
	//for each pixel
	for (int y=0; y<H; y++){
		for (int x=0; x<W; x++){
			
			//reset the color buffer
			pix_col = black; 
			//camera ray through each pixel
			const Ray cam(vector3(x,y,0),vector3(0,0,1));
			
			//do raytrace for current pixel if no object hit return black 
			pix_col = pix_col + radiance(cam);

			//make sure color is bounded between 0 and 255		
			clamp255(pix_col);
			//insert colors to ppm file
			out << (int)pix_col.X() <<' ';
			out << (int)pix_col.Y() <<' ';
           		out << (int)pix_col.Z() <<'\n';
		}
	}
	return 0;
}
