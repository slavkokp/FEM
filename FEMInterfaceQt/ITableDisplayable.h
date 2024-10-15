#ifndef ITABLEDISPLAYABLE_H
#define ITABLEDISPLAYABLE_H
#include <QVariant>

class ITableDisplayable
{
public:
    virtual const QVariant data(int idx) = 0;
    virtual bool setData(int idx, const QVariant &value) = 0;
};

#endif // ITABLEDISPLAYABLE_H
