#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <QString>

struct BoundingBox {
    struct Area {
        int x, y;
        int width, height;
    };

    QString label;
    qreal possibility;
    Area area;
};

#endif  // STRUCTURES_H
