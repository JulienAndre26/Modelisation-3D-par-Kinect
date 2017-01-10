#pragma once

#include <PCLCore.h>

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


class ThreadReduce : public QThread
{
	Q_OBJECT

public:
	ThreadReduce(std::string filePath, QMutex* lock) { this->path = filePath; this->lock = lock; }

	public slots :
		void onEnd()
	{
		cout << "Thread reduce " << path << " finished" << endl;
	}

private:
	std::string path;
	QMutex* lock;

	void run()
	{
		cout << "Thread reduce " << path << " launched..." << endl;
		PCLCore* core(new PCLCore);
		core->compress(&path);
		//lock->lock();
		// MARK AS COMPRESSED
		//lock->unlock();
		
	}
};