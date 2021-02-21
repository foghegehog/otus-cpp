#pragma once

#include <list>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class ShapeVisitor;

class Shape{
    public:
        // Position along the axis perpendicular to the screen plane.
        // Denotes which shape is "closer" to user and overlaps others. 
        int Z;

        virtual void Accept(shared_ptr<ShapeVisitor> visitor) = 0;
};

class Point: public Shape{
    public:
        int X;
        int Y;

        virtual void Accept(shared_ptr<ShapeVisitor> visitor) override;
};

class Line: public Shape{
    public:
        Point From;
        Point To;

        virtual void Accept(shared_ptr<ShapeVisitor> visitor) override;
};

class Circle: public Shape{
    public:
        Point Center;
        double Radius;

        virtual void Accept(shared_ptr<ShapeVisitor> visitor) override;
};

class Polygon: public Shape{
    public:
        vector<Line> Lines;

        virtual void Accept(shared_ptr<ShapeVisitor> visitor) override;
};

// The GoF Visitor pattern
class ShapeVisitor{
    public:
        virtual void VisitPoint(const Point& point) = 0;
        virtual void VisitLine(const Line& line) = 0;
        virtual void VisitCircle(const Circle& line) = 0;
        virtual void VisitPolygon(const Polygon& polygon) = 0;
};

class Document{
    public:
        string Path;
        bool HasUnsavedChanges;
        shared_ptr<Shape> ActiveShape;
        void AddShape(const shared_ptr<Shape>& shape){};
        void RemoveShape(const shared_ptr<Shape>& shape) {};
        void VisitAllShapes(const shared_ptr<ShapeVisitor>& visitor)
        {
            for(const auto& shape: m_shapes)
            {
                shape->Accept(visitor);
            }
        }
    private:
        list<shared_ptr<Shape>> m_shapes;
};