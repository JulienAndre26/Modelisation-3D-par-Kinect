#pragma once
#include <QObject>

class CallInterface : public QObject {
	Q_OBJECT

public:
	CallInterface(QObject * parent = Q_NULLPTR);
	~CallInterface();

	void btnLoadModelClicked();
	

private:
	
};
