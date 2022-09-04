#pragma once

#include "model.h"
#include "view.h"
#include <iostream>

class DrawingVisitor : public ShapeVisitor{
    public:
        DrawingVisitor(std::shared_ptr<Canvas> canvas)
            :m_canvas(canvas)
        {
        }

        void VisitPoint(const Point& point) override {std::cout<<"Point drawn!"<<std::endl;};
        void VisitLine(const Line& line) override {std::cout<<"Line drawn!"<<std::endl;};
        void VisitCircle(const Circle& line) override {std::cout<<"Circle drawn!"<<std::endl;};
        void VisitPolygon(const Polygon& polygon) override {std::cout<<"Polygon drawn!"<<std::endl;};

    private:
        std::shared_ptr<Canvas> m_canvas;
};

enum class OverlapDirection{
    Overlapped,
    Overlapping
};

// Gathers shapes that are either overlapped by the considered shape or overlaps it themself
class OverlappingVisitor : public ShapeVisitor{
    public:
        OverlappingVisitor(std::shared_ptr<Shape> shape, OverlapDirection overlapDirection)
        :m_considered_shape(shape), m_direction(overlapDirection)
        {}

        void VisitPoint(const Point& point) override {};
        void VisitLine(const Line& line) override {};
        void VisitCircle(const Circle& line) override {};
        void VisitPolygon(const Polygon& polygon) override {};

        // Should be called after visiting all tested shapes 
        std::vector<std::shared_ptr<Shape>> GetShapesOverlaps() {return std::vector<std::shared_ptr<Shape>>();};

    private:
        std::shared_ptr<Shape> m_considered_shape;
        OverlapDirection m_direction;
};

// Gathers shapes that contains specified point
class PointContaingVisitor : public ShapeVisitor{
    public:
        PointContaingVisitor(int x, int y)
        :m_x(x), m_y(y)
        {}

        void VisitPoint(const Point& point) override {};
        void VisitLine(const Line& line) override {};
        void VisitCircle(const Circle& line) override {};
        void VisitPolygon(const Polygon& polygon) override {};

        // Should be called after visiting all tested shapes 
        std::vector<std::shared_ptr<Shape>> GetShapesContainingPoint() {return std::vector<std::shared_ptr<Shape>>();};

    private:
        int m_x;
        int m_y;
};

class Editor{
    public:
        Editor()
        {
            m_drawing_visitor = std::make_shared<DrawingVisitor>(Form.ShapesCanvas);
        }

        EditorForm Form;
        std::unique_ptr<Document> ActiveDocument;
        // Is selected by clicking on the shape area on the canvas
        std::shared_ptr<Shape> SelectedShape;

        void CreateNewDocument()
        {
            if (ActiveDocument)
            {
                CloseCurrentDocumentConfirmed();
            }
            ActiveDocument.reset(new Document());
            // ...
        }

        void ExportDocument()
        {
            // ...
            GetPathDialog saveFilePathDialog;
            // ...
        }

        void ImportDocument()
        {
            if (ActiveDocument)
            {
                CloseCurrentDocumentConfirmed();
            }
            // ...
            GetPathDialog loadFilePathDialog;
            // ...

            ActiveDocument->VisitAllShapes(m_drawing_visitor);
            // ...
        }

        void AddShape(const std::shared_ptr<Shape>& shape)
        {
            // ...
            ActiveDocument->AddShape(shape);
            // ...

            shape->Accept(m_drawing_visitor);
            // ...
        }

        // Activates shape selected with mouse click on the corresponding canvas region
        void SelectShapeClicked(int x, int y)
        {
            auto pointVisitor = std::make_shared<PointContaingVisitor>(x, y);
            ActiveDocument->VisitAllShapes(pointVisitor);
            auto candidates = pointVisitor->GetShapesContainingPoint();
            // Select the shape to be selected and highlighted based on Z-index or other criteria
            //...
        }

        // Erases shape selected with mouse click 
        void EraseActiveShape()
        {
            //...
            if (ActiveDocument && SelectedShape)
            {
                ActiveDocument->RemoveShape(SelectedShape);
                // ...
                auto overlapsSearcher = std::make_shared<OverlappingVisitor>(SelectedShape, OverlapDirection::Overlapped);
 
                ActiveDocument->VisitAllShapes(overlapsSearcher);
                for(const auto& overlappedShape: overlapsSearcher->GetShapesOverlaps())
                {
                    overlappedShape->Accept(m_drawing_visitor);
                }
                // ...
                SelectedShape.reset();
            }
        }

    private:
        bool CloseCurrentDocumentConfirmed()
        {
            if (!ActiveDocument)
            {
                return true;
            }

            bool closed = false;
            //...
            ConfirmationDialog confirmCloseDialog;
            //...
            if (ActiveDocument->HasUnsavedChanges)
            {
                GetPathDialog saveFilePathDialog;
                // ...
            }
            // ...
            ActiveDocument.reset(); 
            return closed;
        }

        std::shared_ptr<DrawingVisitor> m_drawing_visitor;  
};