#ifndef LOADINGDIV_H
#define LOADINGDIV_H

#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QTimer>
#include <QList>
#include <QDebug>

class LoadingDiv : public QWidget
{
    Q_OBJECT
public:
    explicit LoadingDiv(QWidget *parent = nullptr);

signals:

public slots:
    void switchAnimStep();
    void startLoading();
    void stopLoading();

private:
    QLabel *loadingGif;
    QTimer *switchTimer;

    int currentPage;
};

#endif // LOADINGDIV_H
