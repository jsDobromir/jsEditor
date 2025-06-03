#ifndef ACTIVE_TAB_H
#define ACTIVE_TAB_H
#include <QString>
#include <QMetaType>

class ActiveTab {
    public:
        ActiveTab(){}
        bool isFile;
        QString name;
        uint64_t index;
        QString textContent;
};
Q_DECLARE_METATYPE(ActiveTab)
Q_DECLARE_METATYPE(std::vector<ActiveTab>)
#endif