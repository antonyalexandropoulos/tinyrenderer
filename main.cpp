#include "tgaimage.h"
#include "model.h"
#include <vector>
#include <iostream>
#include <numeric>
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor blue   = TGAColor(0, 0,   255,   255);

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    bool steep = false; 
    if (std::abs(x0-x1)<std::abs(y0-y1)) { 
        std::swap(x0, y0); 
        std::swap(x1, y1); 
        steep = true; 
    } 
    if (x0>x1) { 
        std::swap(x0, x1); 
        std::swap(y0, y1); 
    } 
    int dx = x1-x0; 
    int dy = y1-y0; 
    int derror2 = std::abs(dy)*2; 
    int error2 = 0; 
    int y = y0; 
    for (int x=x0; x<=x1; x++) { 
        if (steep) { 
            image.set(y, x, color); 
        } else { 
            image.set(x, y, color); 
        } 
        error2 += derror2; 
        if (error2 > dx) { 
            y += (y1>y0?1:-1); 
            error2 -= dx*2; 
        } 
    } 
 

}

void DrawRect(int x0,int y0,int x1,int y1, TGAImage &image, TGAColor color)
{
	line(x0, y0, x0, y1, image, color);
	line(x0, y0, x1, y0, image, color);
	line(x1, y0, x1, y1, image, color);
	line(x1, y1, x0, y1, image, color);
}

void DrawCube(int x0,int y0,int x1,int y1, TGAImage &image, TGAColor color)
{
	DrawRect(x0,y0,x1,y1,image,color);
	DrawRect(x0-15,y0-15,x1-15,y1-15,image,color);
	line(x1-15, y1-15, x1, y1, image, color);
	line(x0-15, y1-15, x0, y1, image, color);
	line(x0-15, y0-15, x0, y0, image, color);
	line(x1-15, y0-15, x1, y0, image, color);
}
int BariCentric(int a0,int a1,int b0,int b1,int c0,int c1)
{
    return ((b0-a0) * (c1-a1) )- ((b1-a1) * (c0-a0));
}
struct Point
{
    int x,y;
};
void Barycentric(Point p, Point a, Point b, Point c, float &u, float &v, float &w)
{
    std::vector<int> v0(2);
    std::vector<int> v1(2);
    std::vector<int> v2(2);
    std::cout<<"helo"<<std::endl;
    v0[0] = b.x- a.x;
    v0[1] = b.y - a.y;
    v1[0] = c.x - a.x;
    v1[1] = c.y - a.y;
    v2[0] = p.x - a.x;
    v2[1] = p.y - a.y;
    std::cout<<"b"<<std::endl;

    float d00 = std::inner_product(v0.begin(), v0.end(), v0.begin(), 0);
    float d01 = std::inner_product(v0.begin(), v0.end(), v1.begin(), 0);
    float d11 = std::inner_product(v1.begin(), v1.end(), v1.begin(), 0);
    float d20 = std::inner_product(v2.begin(), v2.end(), v0.begin(), 0);
    float d21 = std::inner_product(v2.begin(), v2.end(), v1.begin(), 0);
   
    float denom = d00 * d11 - d01 * d01;
   
    v = (d11 * d20 - d01 * d21) / denom;
    w = (d00 * d21 - d01 * d20) / denom;
    u = 1.0f - v - w;
    

}

void DrawTriangle(int a0,int a1,int b0,int b1,int c0,int c1, TGAImage &image, TGAColor color)
{
    int minx = std::min(std::min(a0,b0),c0);
    int miny = std::min(std::min(a1,b1),c1);
    int maxx = std::max(std::max(a0,b0),c0);
    int maxy = std::max(std::max(a1,b1),c1);
    minx = std::max(minx, 0);
    miny = std::max(miny, 0);
    maxx = std::min(maxx, image.get_width()- 1);
    maxy = std::min(maxy, image.get_height() - 1);
	//line(a0, a1, b0, b1, image, color);
	//line(c0, c1, b0, b1, image, white);
	//line(c0, c1, a0, a1, image, blue);
   
    for(int y= miny;y<=maxy;++y)
    {
        
        for(int x=minx;x<=maxx;++x)
        {
            
            float * a,*b,*c;
            float t,tt,ttt;
            a = &t;
            b = &tt;
            c  = &ttt;
            Point z,v,l,p;
            z.x = a0;
            z.y = a1;
            v.x = b0;
            v.y = b1;
            l.x = c0;
            l.y = c1;
            p.x = x;
            p.y = y;
            Barycentric(p,z,v,l,*a,*b,*c);
            
            std::cout<<"here"<<std::endl;
            std::cout<<*a<<" "<<*b<<" " << *c<< std::endl;
            if(*a>=0 && *b>=0 && *c>=0)
            {
                std::cout<<"??"<<std::endl;
                 image.set(x,y,color);    
            }
           //image.set(x,y,color);  
           
        }

    }
}
void DrawCircle(int x0,int y0,int radius ,TGAImage &image, TGAColor color)
{
	int x = radius - 1 ;
	int y = 0;
	int dx = 1;
	int dy = 1;
	int err = dx - (radius<<1);
	while(x>=y)
	{
		image.set(x0 + x, y0 + y,color);
        image.set(x0 + y, y0 + x,color);
        image.set(x0 - y, y0 + x,color);
        image.set(x0 - x, y0 + y,color);
        image.set(x0 - x, y0 - y,color);
        image.set(x0 - y, y0 - x,color);
        image.set(x0 + y, y0 - x,color);
        image.set(x0 + x, y0 - y,color);
        if(err<=0)
        {
        	y++;
        	err+=dy;
        	dy+=2;
        }
        if(err>0)
        {
        	x--;
        	dx+=2;
        	err+= dx - (radius<<1);
        }
	}

}
int main(int argc, char** argv) {
    Model *model = NULL;
    const int width  = 800;
    const int height = 800;
    model = new Model("obj/african_head.obj");
	TGAImage image(height, width, TGAImage::RGB);
    Vec3f light_dir(0,0,-1);
    for (int i=0; i<model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        Vec2i screen_coords[3];
        Vec3f world_coords[3];
        for (int j=0; j<3; j++) {
            Vec3f v = model->vert(face[j]);
            screen_coords[j] = Vec2i((v.x+1.)*width/2., (v.y+1.)*height/2.);
            world_coords[j]  = v;
        }
        Vec3f n = (world_coords[2]-world_coords[0])^(world_coords[1]-world_coords[0]);
        n.normalize();
        float intensity = n*light_dir;
        if (intensity>0) {
          DrawTriangle(screen_coords[0].u,screen_coords[0].v, screen_coords[1].u,screen_coords[1].v, screen_coords[2].u,screen_coords[2].v, image,  TGAColor(intensity*255, intensity*255, intensity*255, 255)); 
        }
    }
    //std::cout<<screen_coords[0].v<<std::endl;
    //DrawTriangle(screen_coords[0].u,screen_coords[0].v, screen_coords[1].u,screen_coords[1].v, screen_coords[2].u,screen_coords[2].v, image, TGAColor(rand()%255, rand()%255, rand()%255, 255)); 

	DrawCircle(30,30,30,image,blue);
	DrawCube(35,20,95,80,image,blue);
	//DrawTriangle(10,10,100,30,190,160,image,white);
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}

