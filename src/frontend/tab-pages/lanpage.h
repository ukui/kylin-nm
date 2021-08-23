#ifndef LANPAGE_H
#define LANPAGE_H

#include "tabpage.h"

class LanPage : public TabPage
{
    Q_OBJECT
public:
    explicit LanPage(QWidget *parent = nullptr);
    ~LanPage()=default;

signals:

private:
    void initLanUI();
};

#endif // LANPAGE_H
