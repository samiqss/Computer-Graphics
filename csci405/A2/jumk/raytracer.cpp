#include <fstream>
#include <math.h>
#include <iostream>
#include "mathvector.h"
using namespace std;

//Some Notes for self:
//equation of sphere is (x-Cx)^2+(y-Cy)^2+(z-Cz)^2-r^2=0 <-c is center
//sphere vector form (P-C).(P-C)-r^2=0
//point along ray pt=O+tD <- O is origin D is direction t is kind of like distance from point
//Now if we sub pt in for P in the sphere vector form (O+tD-C).(O+tD-C)-r^2=0
//=> (D.D)t^2 +2D.(O-C)t+(O-C).(O-C)-r^2=0
//then we solve for t using quadratic equation 
//if discriminant is negative, the ray misses the spehere 
//and if both t's are negative the sphere is behind the ray
//vector3

struct Ray {
	vector3 o; //origin 
	vector3 d; //direction
	Ray(const vector3 i, const vector3 j){
		o=i;
		d=j;
	}
};

struct Sphere {
	vector3 c; //center 
	double  r;  //radius 
	Sphere(const vector3 i, double j){
		c=i;
		r=j;
	}
	vector3 getNormal(const vector3 pi) const { return (pi - c) / r; }
	bool intersect(const Ray& ray, double& t) const {
		const vector3 o = ray.o;
		const vector3 d = ray.d;
		const vector3 oc = o-c;
		double b = 2*vector3::DotProduct(oc,d);
		double c = vector3::DotProduct(oc,oc)-r*r;
		double disc = b*b-4*c;
		if(disc < 1e-4) return false;
		disc = sqrt(disc);

			const double t0 = (-b-disc)/2;
			const double t1 = (-b+disc)/2;

			t = (t0 < t1) ? t0 : t1;
			return true;
			
	}
};

void clamp255(vector3 col){
	double xi= (col.X() > 255) ? 255 : (col.X() < 0) ? 0 : col.X();
	double yi= (col.Y() > 255) ? 255 : (col.Y() < 0) ? 0 : col.Y();
	double zi= (col.Z() > 255) ? 255 : (col.Z() < 0) ? 0 : col.Z();	
	col.X (xi);
	col.Y (yi);
	col.Z (zi);

}

int main(){

	const int W=500; //image width
	const int H= 500; //image height
	
	
	const vector3 white(255, 255, 255);
	const vector3 black(0, 0, 0);
	const vector3 red(255, 0, 0);
	//Sphere spheres[] = {
	std::ofstream out("out.ppm");
	out << "P3\n" << W << ' ' << H <<' '<<"255\n";
	const Sphere sphere(vector3(W*0.5, H*0.5, 50), 50);
	const Sphere light(vector3(0, 0, 50), 1);//light
	/*      
	Sphere(vector3(27,16.5,47), 16.5),//obj 1
	// Sphere(vector3(0, 0, 50), 1),//light
	 //Sphere(vector3(50,681.6-.27,81.6), 600),//light

	//const Sphere sphereB(vector3(W*0.5+110, H*0.5, 50), 50);
	Sphere(vector3(1e5+1,40.8,81.6),1e5),//left
	Sphere(vector3(-1e5+99,40.8,81.6),1e5),//right
	Sphere(vector3(50,40.8, 1e5),1e5),//back
	Sphere(vector3(50,40.8,-1e5+170),1e5),//front
	Sphere(vector3(50, 1e5, 81.6),1e5),//bottom
	Sphere(vector3(50,-1e5+81.6,81.6),1e5),//top
	Sphere(vector3(50,681.6-.27,81.6), 600)//light

	};
	*/

	
	double t;
	vector3 pix_col(black);

	//for each pixel
	for (int y=0; y<H; y++){
		for (int x=0; x<W; x++){
			//send a ray through each pixel
			pix_col = black;
			const Ray ray(vector3(x,y,0),vector3(0,0,1));
			//check for intersections 
			if(sphere.intersect(ray,t)){
				//point of intersection

				vector3 pi = ray.o + ray.d*t; //point camera looks at
				//color the pixel
				
				//difference between point and camera position
				vector3 L = light.c - pi;
				vector3 N = sphere.getNormal(pi);//camera direction
				double dt = vector3::DotProduct(L.Normalize(), N.Normalize());
				pix_col = (red+white*dt) * 0.5;
				clamp255(pix_col);
			

			 cout<<pix_col.X()<<" "<<pix_col.Y()<<" "<<pix_col.Z()<<endl;
			}
			 /*else if(sphereB.intersect(ray,t)){
				vector3 pi = ray.o + ray.d*t;
				vector3 L = light.c - pi;
				vector3 N = sphereB.getNormal(pi);
				double dt = vector3::DotProduct(L.Normalize(), N.Normalize());
				pix_col = (red + white*dt) * 0.5;
				clamp255(pix_col);
			}*/
			out << (int)pix_col.X() <<' ';
			out << (int)pix_col.Y() <<' ';
           		out << (int)pix_col.Z() <<'\n';
			
		}
	}

	return 0;
}
