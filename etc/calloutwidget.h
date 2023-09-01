#pragma once

#include "contentwidget.h"

class CalloutView;

class CalloutWidget : public ContentWidget {
    Q_OBJECT
public:
    CalloutWidget(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *) override;

private:
	CalloutView* mView;
};
