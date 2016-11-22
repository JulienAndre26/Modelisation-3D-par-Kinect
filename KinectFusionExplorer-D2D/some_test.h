#ifndef SOME_TEST_H
#define SOME_TEST_H

#include <QtWidgets/QMainWindow>
#include "GeneratedFiles/ui_some_test.h"

class some_test : public QMainWindow
{
	Q_OBJECT

public:
	some_test(QWidget *parent = 0);
	~some_test();

private:
	Ui::some_testClass ui;
};

#endif // SOME_TEST_H
