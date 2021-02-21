#include "model.h"

void Point::Accept(shared_ptr<ShapeVisitor> visitor){ visitor->VisitPoint(*this); }
void Line::Accept(shared_ptr<ShapeVisitor> visitor){ visitor->VisitLine(*this); }
void Circle::Accept(shared_ptr<ShapeVisitor> visitor){ visitor->VisitCircle(*this); }
void Polygon::Accept(shared_ptr<ShapeVisitor> visitor){ visitor->VisitPolygon(*this); }