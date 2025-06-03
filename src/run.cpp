#include <iostream>
#include <QApplication>
#include "Window.h"
#include "application/Application.h"
#include "structure/top_layout/TopLayout.h"
#include "structure/mid_layout/MidLayout.h"
#include "structure/bottom_layout/BottomLayout.h"

using namespace std;

int run(int argc, char* argv[]) {
    QApplication app(argc, argv);
    TopLayout *topLayout = new TopLayout();
    MidLayout *midLayout = new MidLayout();
    BottomLayout *bottomLayout = new BottomLayout();
    
    Window win(nullptr, 800, 600);
    Application coreApp(&win, argc, argv);
    
    QObject::connect(topLayout, &TopLayout::newDocumentSignal, midLayout, &MidLayout::newDocumentCreated);
    QObject::connect(midLayout, &MidLayout::addDocumentAndTextEdit, &coreApp, &Application::addDocumentAndTextEditHandler);
    QObject::connect(midLayout, &MidLayout::removeDocumentAndTextEdit, &coreApp, &Application::removeDocumentAndTextEditHandler);
    QObject::connect(midLayout, &MidLayout::widgedClickedMapSignalReThrow, &coreApp, &Application::widgetClickedHanlder);
    QObject::connect(&win, &Window::emitResizeSignal, &coreApp, &Application::resizeHandler);

    win.addChildLayout(topLayout, 3);
    win.addChildLayout(midLayout, 5);
    win.addChildLayout(bottomLayout, 50);
    win.showWindow();
    coreApp.setLayouts(topLayout, midLayout, bottomLayout, argc, argv);
    
    app.exec();
    return 0;
};