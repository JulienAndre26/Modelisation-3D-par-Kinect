#pragma once

#include <QThread.h>
#include <iostream>

#include "mainwindow.h"

class ThreadOpen : public QThread
{
	Q_OBJECT

public:
	ThreadOpen(MainWindow * mw, int n) { this->mw = mw;  this->nView = n; }

	public slots :
		void onEnd()
	{
		mw->setViewDisplay(nView, true, NULL);
		cout << "Thread " << nView << " finished" << endl;
	}

private:
	MainWindow * mw;
	int nView;

	void run()
	{
		cout << "Thread " << nView << " launched..." << endl;
		mw->processView(nView);
	}
};

class ThreadMerge : public QThread
{
	Q_OBJECT

public:
	ThreadMerge(MainWindow * mw) { this->mw = mw; }

	public slots :
		void onEnd()
	{
		cout << "Merge Thread finished" << endl;
		this->mw->onMergeEnd();
	}

private:
	MainWindow * mw;

	void run()
	{
		cout << "Merge Thread launched..." << endl;
		mw->processMerge();
	}
};
