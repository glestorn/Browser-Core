#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QScrollArea>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QKeyEvent>
#include <QProcess>
#include <QFont>
#include <QStyle>
#include <vector>
#include "httprequest.h"
#include <windows.h>

typedef struct object {
    char* content;
    int stringSize;

    int objectType;
    int objectConfig;
} Object;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

    void doParse(QString fileName);
//    void drawingPage();

    void createHeader(Object object);
    void createParagraph(Object object);
    void createPhrase(Object object);
    void createListElement(Object object);
    void createLink(Object object);
    void createLine();


protected:
    virtual void keyPressEvent(QKeyEvent* event);

private slots:
    void doDownload();
    void doParse();
    void requestFinished();
    void drawingPage();
    void redraw();

signals:

private:
    Ui::MainWindow *ui;
    QLineEdit* adressRow;
    QPushButton* loadButton;
    QLineEdit* fileName;
    QPushButton* loadFileButton;
    QScrollArea* contentWindow;
    QFrame* inner;
    HttpRequest server;

    std::vector<QWidget*> objectList;

    QProcess* parser;
};

#endif // MAINWINDOW_H
