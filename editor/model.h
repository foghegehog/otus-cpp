#pragma once

#include <list>
#include <memory>
#include <string>
#include <vector>

class ShapeVisitor;

class Shape{
    public:
        // Position along the axis perpendicular to the screen plane.
        // Denotes which shape is "closer" to user and overlaps others. 
        int Z;

        virtual void Accept(std::shared_ptr<ShapeVisitor> visitor) = 0;
};

class Point: public Shape{
    public:
        int X;
        int Y;

        void Accept(std::shared_ptr<ShapeVisitor> visitor) override;
};

class Line: public Shape{
    public:
        Point From;
        Point To;

        void Accept(std::shared_ptr<ShapeVisitor> visitor) override;
};

class Circle: public Shape{
    public:
        Point Center;
        double Radius;

        void Accept(std::shared_ptr<ShapeVisitor> visitor) override;
};

class Polygon: public Shape{
    public:
        std::vector<Line> Lines;

        void Accept(std::shared_ptr<ShapeVisitor> visitor) override;
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
        std::string Path;
        bool HasUnsavedChanges;
        std::shared_ptr<Shape> ActiveShape;
        void AddShape(std::shared_ptr<Shape> shape){};
        void RemoveShape(std::shared_ptr<Shape> shape) {};
        void VisitAllShapes(std::shared_ptr<ShapeVisitor> visitor)
        {
            for(const auto& shape: m_shapes)
            {
                shape->Accept(visitor);
            }
        }
    private:
        std::list<std::shared_ptr<Shape>> m_shapes;
};