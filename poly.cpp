#include <windows.h>
#include <GL/glut.h>
#include <bits/stdc++.h>
using namespace std;

#define MAXWIDTH 640
#define MAXHEIGHT 480

class Point
{
    public:
        int xp;
        int yp;

        Point()
        {
            xp=0;
            yp=0;
        }

        Point(int x, int y)
        {
            xp=x;
            yp=y;
        }

        int x()
        {
            return xp;
        }

        int y()
        {
            return yp;
        }
};

vector<vector<Point>> points;
vector<Point> p;
int x_min,y_min,x_max,y_max;
int c1=0, c2=0;
bool flag=false;

void plotRect(vector<Point> point)
{
    glColor3d(0,1,1);
    glLineWidth(3);
    int x1=point[0].x(), y1=point[0].y(), x2=point[1].x(), y2=point[1].y();
    glBegin(GL_LINE_LOOP);
        glVertex2d(x1,y1);
        glVertex2d(x1,y2);
        glVertex2d(x2,y2);
        glVertex2d(x2,y1);
    glEnd();
    glFlush();
}

void plotPolygon(vector<Point> point, int r, int g, int b)
{
    glColor3d(r,g,b);
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    for(int i=0;i<point.size();i++)
    {
        glVertex2d(point[i].x(), point[i].y());
    }
    glEnd();
    glFlush();
}

void plot(int x, int y,int r, int g, int b)
{
    glColor3d(r,g,b);
    glBegin(GL_POINTS);
        glVertex2d(x,y);
    glEnd();
    glFlush();
}

void print(vector<Point> point)
{
    for(int i=0;i<point.size();i++)
    {
        cout<< point[i].x()<< " "<<point[i].y()<<endl;
    }
    cout<<endl;
}

vector<Point> clipLeft(vector<Point> point)
{
    vector<Point> clipped;
    int n=point.size();
    for(int i=0,j=1;i<n;i++,j=(++j)%n)
    {
        float x1=point[i].x(), x2=point[j].x(), y1=point[i].y(), y2=point[j].y();

        // first outside, second inside
        if(x1<x_min && x2>=x_min)
        {
            int x=x_min;
            float m=(y2-y1)/(x2-x1);
            int y=y1+(x-x1)*m;
            Point intersection=Point(x, y);
            clipped.push_back(intersection);
            clipped.push_back(point[j]);
        }
        // both inside
        else if(x1>=x_min && x2>=x_min)
        {
            clipped.push_back(point[j]);
        }
        // first inside, second outside
        else if(x1>=x_min && x2<x_min)
        {
            int x=x_min;
            float m=(y2-y1)/(x2-x1);
            int y=y1+(x-x1)*m;
            Point intersection=Point(x, y);
            clipped.push_back(intersection);
        }
    }
    return clipped;
}

vector<Point> clipRight(vector<Point> point)
{
    vector<Point> clipped;
    int n=point.size();
    for(int i=0,j=1;i<n;i++,j=(++j)%n)
    {
        float x1=point[i].x(), x2=point[j].x(), y1=point[i].y(), y2=point[j].y();

         // first outside, second inside
        if(x1>x_max && x2<=x_max)
        {
            int x=x_max;
            float m=(y2-y1)/(x2-x1);
            int y=y1+(x-x1)*m;
            Point intersection=Point(x, y);
            clipped.push_back(intersection);
            clipped.push_back(point[j]);
        }
        // both inside
        else if(x1<=x_max && x2<=x_max)
        {
            clipped.push_back(point[j]);
        }
        // first inside, second outside
        else if(x1<=x_max && x2>x_max)
        {
            int x=x_max;
            float m=(y2-y1)/(x2-x1);
            int y=y1+(x-x1)*m;
            Point intersection=Point(x, y);
            clipped.push_back(intersection);
        }
    }
    return clipped;
}

vector<Point> clipBottom(vector<Point> point)
{
    vector<Point> clipped;
    int n=point.size();
    for(int i=0,j=1;i<n;i++,j=(++j)%n)
    {
        float x1=point[i].x(), x2=point[j].x(), y1=point[i].y(), y2=point[j].y();

         // first outside, second inside
        if(y1<y_min && y2>=y_min)
        {
            int y=y_min;
            float m=(y-y1)/(y2-y1);
            int x=x1+(x2-x1)*m;
            Point intersection=Point(x, y);
            clipped.push_back(intersection);
            clipped.push_back(point[j]);
        }
        // both inside
        else if(y1>=y_min && y2>=y_min)
        {
            clipped.push_back(point[j]);
        }
        // first inside, second outside
        else if(y1>=y_min && y2<y_min)
        {
            int y=y_min;
            float m=(y-y1)/(y2-y1);
            int x=x1+(x2-x1)*m;
            Point intersection=Point(x, y);
            clipped.push_back(intersection);
        }
    }
    return clipped;
}

vector<Point> clipTop(vector<Point> point)
{
    vector<Point> clipped;
    int n=point.size();
    for(int i=0,j=1;i<n;i++,j=(++j)%n)
    {
        float x1=point[i].x(), x2=point[j].x(), y1=point[i].y(), y2=point[j].y();

        // first outside, second inside
        if(y1>y_max && y2<=y_max)
        {
            int y=y_max;
            float m=(y-y1)/(y2-y1);
            int x=x1+(x2-x1)*m;
            Point intersection=Point(x, y);
            clipped.push_back(intersection);
            clipped.push_back(point[j]);
        }
        // both inside
        else if(y1<=y_max && y2<=y_max)
        {
            clipped.push_back(point[j]);
        }
        // first inside, second outside
        else if(y1<=y_max && y2>y_max)
        {
            int y=y_max;
            float m=(y-y1)/(y2-y1);
            int x=x1+(x2-x1)*m;
            Point intersection=Point(x, y);
            clipped.push_back(intersection);
        }
    }
    return clipped;
}


void sutherlandHodgman(vector<Point> point)
{
    vector<Point> clipped;
    int side=1;
    print(point);
    clipped=clipLeft(point);
    clipped=clipRight(clipped);
    clipped=clipBottom(clipped);
    clipped=clipTop(clipped);
    plotPolygon(clipped,1,0,0);
}


static void display(void)
{

    glColor3d(0,0,0);
    glFlush();
}


static void mouse(int button, int state, int x, int y)
{
    y=MAXHEIGHT-y;

    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
    {
        plot(x,y,1,1,1);
        p.push_back(Point(x,y));
        if(!flag)
        {
            c2++;
            cout<<c2<<" ";
            if(c2==2)
            {
                x_min=p[0].x();
                x_max=p[1].x();
                y_min=p[0].y();
                y_max=p[1].y();
                plotRect(p);
                flag=true;
                plotRect(p);
                points.push_back(p);
                p.erase(p.begin(), p.end());
                c1=1;
                c2=1;
            }
        }
    }

    else if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
    {
        points.push_back(p);
        p.erase(p.begin(), p.end());
        plotPolygon(points[c1++],1,1,1);
    }
}


static void key(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'c':
            for(;c2<points.size();c2++)
                sutherlandHodgman(points[c2]);
    }
}


int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(MAXWIDTH,MAXHEIGHT);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

    glutCreateWindow("Polygon Clipping");
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,MAXWIDTH,0,MAXHEIGHT);
    glutDisplayFunc(display);

    glutMouseFunc(mouse);
    glutKeyboardFunc(key);

    glutMainLoop();

    return EXIT_SUCCESS;
}
