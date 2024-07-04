#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

struct Point {
    double x, y, z;
};

bool doBoxesIntersect(vector<Point> p1, vector<Point> p2) {
    // Check for intersection on X-axis
    double min1X = p1[0].x;
    double max1X = p1[0].x;
    double min2X = p2[0].x;
    double max2X = p2[0].x;

    // Find min and max X for each parallelepiped
    for (int i = 1; i < 8; ++i) {
        min1X = std::min(min1X, p1[i].x);
        max1X = std::max(max1X, p1[i].x);
        min2X = std::min(min2X, p2[i].x);
        max2X = std::max(max2X, p2[i].x);
    }

    // Check for intersection on Y-axis
    double min1Y = p1[0].y;
    double max1Y = p1[0].y;
    double min2Y = p2[0].y;
    double max2Y = p2[0].y;

    for (int i = 1; i < 8; ++i) {
        min1Y = std::min(min1Y, p1[i].y);
        max1Y = std::max(max1Y, p1[i].y);
        min2Y = std::min(min2Y, p2[i].y);
        max2Y = std::max(max2Y, p2[i].y);
    }

    // Check for intersection on Z-axis
    double min1Z = p1[0].z;
    double max1Z = p1[0].z;
    double min2Z = p2[0].z;
    double max2Z = p2[0].z;

    for (int i = 1; i < 8; ++i) {
        min1Z = std::min(min1Z, p1[i].z);
        max1Z = std::max(max1Z, p1[i].z);
        min2Z = std::min(min2Z, p2[i].z);
        max2Z = std::max(max2Z, p2[i].z);
    }

    // Check for intersection on X-axis, Y-axis, and Z-axis
    if (max1X < min2X || max2X < min1X ||
        max1Y < min2Y || max2Y < min1Y ||
        max1Z < min2Z || max2Z < min1Z) {
        return false; // No intersection
    }

    return true; // Intersection found
}

vector<Point> createParallelepiped(Point A, Point B, Point C, Point D)
{
    Point E = {C.x - A.x + B.x, C.y - A.y + B.y, C.z - A.z + B.z}; //e=(xc-xa+xb,....)
    Point F = {D.x - A.x + B.x, D.y - A.y + B.y, D.z - A.z + B.z}; //f(xd-xa+xb,....)
    Point K = {D.x - A.x + C.x, D.y - A.y + C.y, D.z - A.z + C.z}; //k(xd-xa+xc,....)
    Point I = {D.x - A.x + E.x, D.y - A.y + E.y, D.z - A.z + E.z}; //I(xd-xa+xe,....)
    vector<Point> points;
    points.push_back(A);
    points.push_back(B);
    points.push_back(C);
    points.push_back(E);
    points.push_back(D);
    points.push_back(F);
    points.push_back(K);
    points.push_back(I);
    return points;
}

Point inputPoint(string point)
{
    float x, y, z;
    cout << point << ".x = ";
    cin >> x;
    cout << point << ".y = ";
    cin >> y;
    cout << point << ".z = ";
    cin >> z;
    return {x, y, z};
}

int main() {
    cout << "*** Parallelepiped 1 ***" << endl;
    Point A1 = inputPoint("A");
    Point B1 = inputPoint("B");
    Point C1 = inputPoint("C");
    Point D1 = inputPoint("D");

    cout << "*** Parallelepiped 2 ***" << endl;
    Point A2 = inputPoint("A");
    Point B2 = inputPoint("B");
    Point C2 = inputPoint("C");
    Point D2 = inputPoint("D");

    vector<Point> parallelepiped1 = createParallelepiped(A1, B1, C1, D1);
    vector<Point> parallelepiped2 = createParallelepiped(A2, B2, C2, D2);

    if (doBoxesIntersect(parallelepiped1, parallelepiped2)) {
        std::cout << "The parallelepipeds intersect." << std::endl;
    } else {
        std::cout << "The parallelepipeds do not intersect." << std::endl;
    }
    return 0;
}