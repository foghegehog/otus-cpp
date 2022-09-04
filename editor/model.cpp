#include "model.h"

void Point::Accept(std::shared_ptr<ShapeVisitor> visitor){ visitor->VisitPoint(*this); }
void Line::Accept(std::shared_ptr<ShapeVisitor> visitor){ visitor->VisitLine(*this); }
void Circle::Accept(std::shared_ptr<ShapeVisitor> visitor){ visitor->VisitCircle(*this); }
void Polygon::Accept(std::shared_ptr<ShapeVisitor> visitor){ visitor->VisitPolygon(*this); }