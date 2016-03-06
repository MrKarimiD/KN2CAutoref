#ifndef REFCOMHANDLER_H
#define REFCOMHANDLER_H

#include <QObject>

class refComHandler : public QObject
{
    Q_OBJECT
public:
    explicit refComHandler(QObject *parent = 0);

signals:

public slots:
};

#endif // REFCOMHANDLER_H
