#include "SubQuest.h"

#include "Quest.h"

#include <utility>

SubQuest::SubQuest(QWidget *parent)
    : QWidget(parent)
    , mCompleted(false)
    , mDesc(this)
    , mBar(this)
{
    show();
}

QGridLayout* SubQuest::getLayout() const
{
    return dynamic_cast<Quest*>(parent())->getLayout();
}

bool SubQuest::isCompleted() const
{
    return mCompleted;
}