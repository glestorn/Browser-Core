#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <windows.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QString>
#include <QDebug>
#include <QStringList>

int convert(char* buffer);
void printObject(Object object);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(1366, 700);

    connect(&server, SIGNAL(requestFinished()), this, SLOT(requestFinished()));

    adressRow = new QLineEdit(this);
    adressRow->setGeometry(351, 0, 866, 30);
    adressRow->setAlignment(Qt::AlignHCenter);
    QFont buff;
    buff.setPointSize(15);
    adressRow->setFont(buff);
    adressRow->installEventFilter(this);

    loadButton = new QPushButton(this);
    loadButton->setGeometry(1216, 0, 150, 30);
    loadButton->setText("Load Site");
    connect(loadButton, SIGNAL(clicked()), this, SLOT(doDownload()));
    loadButton->show();

    fileName = new QLineEdit(this);
    fileName->setGeometry(150, 0, 200, 30);
    fileName->setAlignment(Qt::AlignHCenter);
    fileName->setFont(buff);

    loadFileButton = new QPushButton(this);
    loadFileButton->setGeometry(0, 0, 150, 30);
    loadFileButton->setText("Load file content");
    connect(loadFileButton, SIGNAL(clicked()), this, SLOT(doParse()));

    contentWindow = new QScrollArea(this);
    contentWindow->setGeometry(0, 30, 1366, 670);
    contentWindow->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    contentWindow->setWidgetResizable(true);

    inner = new QFrame(contentWindow);
    inner->setLayout(new QVBoxLayout());
    contentWindow->setWidget(inner);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int key= event->key();
    if (key == Qt::Key_Enter) {
        doDownload();
    }
}

void MainWindow::doDownload()
{
    FILE* file = fopen("C:\\Users\\Gleb-PC\\Documents\\Browser_frontend\\html_code.txt", "w");
    printf("we clean file");
    fclose(file);
    server.doDownload(adressRow->text());
}

void MainWindow::redraw()
{
    QApplication::quit();
}

void MainWindow::requestFinished()
{
    doParse("html_code.txt");
}

void MainWindow::doParse()
{
    doParse(fileName->text());
}

void MainWindow::doParse(QString fileName)
{
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        ZeroMemory(&pi, sizeof(pi));

        QString cmdLine = "C:\\Users\\Gleb-PC\\Documents\\Browser_frontend\\finalApp C:\\Users\\Gleb-PC\\Documents\\Browser_frontend\\" + fileName;

        if (!CreateProcess(0, (TCHAR*)(cmdLine.utf16()), NULL, NULL, FALSE,
                           CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
            printf("Error with creating of process!!!");
            return;
        }

        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        printf("process finished");
        drawingPage();

}

void MainWindow::drawingPage()
{
    objectList.clear();
    delete inner;
    inner = new QFrame(contentWindow);
    inner->setLayout(new QVBoxLayout());
    contentWindow->setWidget(inner);

    QFile file("C:\\Users\\Gleb-PC\\Documents\\Browser_frontend\\config_data.txt");
    if (!file.open(QIODevice::ReadOnly)) {
        printf("Error with opening config file");
        return;
    }

    QFile file2("C:\\Users\\Gleb-PC\\Documents\\Browser_frontend\\content_data.txt");
    if (!file2.open(QIODevice::ReadOnly)) {
        printf("Error with opening data file");
        return;
    }

    char buffer[3];
    Object object;
    bool errorFlag = 0;
    while (!file.atEnd()) {
        file.read((char*)&buffer, 3);
        object.stringSize = convert(buffer);
        file.read((char*)&buffer, 3);
        object.objectType = convert(buffer);
        file.read((char*)&buffer, 3);
        object.objectConfig = convert(buffer);
        object.content = new char[object.stringSize];
        file2.read((char*)object.content, object.stringSize-1);
        object.content[object.stringSize-1] = '\0';
        printObject(object);

        if (object.objectConfig < 0) {
           break;
        }

        switch(object.objectType) {
            case 1: createHeader(object); break;
            case 2: createParagraph(object); break;
            case 3: createPhrase(object); break;
            case 4: createListElement(object); break;
            case 5: createLink(object); break;
            case 6: createLine(); break;
            default: errorFlag = true;
        }

        if (errorFlag) {
            break;
        }

        inner->layout()->addWidget(objectList.back());
    }

    file.close();
    file2.close();
}

void MainWindow::createHeader(Object object)
{
    QLabel* header = new QLabel;
    header->setText(object.content);

    QFont font;
    font.setBold(true);

    switch (object.objectConfig) {
        case 1: font.setPointSize(23); break;
        case 2: font.setPointSize(17); break;
        case 3: font.setPointSize(14); break;
        case 4: font.setPointSize(11); break;
        case 5: font.setPointSize(8); break;
        case 6: font.setPointSize(7);
    }

    header->setFont(font);
    objectList.push_back(header);
}

void MainWindow::createParagraph(Object object)
{
    QLabel* paragraph = new QLabel;
    paragraph->setText(object.content);

    QFont font;
    font.setPointSize(10);

    paragraph->setFont(font);
    objectList.push_back(paragraph);
}

void MainWindow::createPhrase(Object object)
{
    QLabel* phrase = new QLabel;
    phrase->setText(object.content);

    QFont font;
    font.setItalic(true);
    font.setPointSize(10);

    phrase->setFont(font);
    objectList.push_back(phrase);
}

void MainWindow::createListElement(Object object)
{
    QLabel* listElement = new QLabel;
    QString content;
    if (object.objectConfig == 0) {
        content = "●";
        content.append(object.content);
    }

    else {
        char buffer[3];
        itoa(object.objectConfig, buffer, 10);
        content.append(buffer);
        content.append(". ");
        content.append(object.content);
    }

    listElement->setText(content);

    QFont font;
    font.setPointSize(10);

    listElement->setFont(font);
    objectList.push_back(listElement);
}

void MainWindow::createLink(Object object)
{
    QPushButton* link = new QPushButton;
    link->setText(object.content);

    QFont font;
    font.setUnderline(true);
    font.setPointSize(10);

    link->setFont(font);
    link->setStyleSheet("outline: none; border: none; color: blue; text-align: left");
    link->showMinimized();

    connect(link, SIGNAL(clicked()), this, SLOT(redraw()));
    objectList.push_back(link);
}

void MainWindow::createLine()
{
    QLabel* line = new QLabel;
    line->setFixedSize(1330, 5);
    line->setStyleSheet("background-color: gray");
    objectList.push_back(line);
}


int convert(char* buffer)
{
    int returnValue = 0;
    for (int i = 0; i < 3 ; i++) {
        if (buffer[i] == ' ') {
            continue;
        }
        returnValue = returnValue*10 + (buffer[i] - '0');
    }
    return returnValue;
}

void printObject(Object object)
{
    printf("Content of struct is : %s\n", object.content);
    printf("string size of struct is : %d\n", object.stringSize);
    printf("object type of struct is : %d\n", object.objectType);
    printf("object config of struct is : %d\n\n", object.objectConfig);
}
