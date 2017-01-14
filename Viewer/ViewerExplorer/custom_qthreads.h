#pragma once

#include <PCLCore.h>

#include <QThread.h>
#include <iostream>

#include "mainwindow.h"

class ThreadLoad : public QThread
{
	Q_OBJECT

public:
	ThreadLoad(MainWindow * mw) { this->mw = mw; }

	public slots :
		void onEnd()
	{
		mw->setAllViewDisplay(true, NULL);
		cout << "Thread Load finished" << endl;
	}

private:
	MainWindow * mw;

	void run()
	{
		cout << "Thread Load launched..." << endl;
		mw->processLoadThread(mw);
	}
};

class ThreadOpen : public QThread
{
	Q_OBJECT

public:
	ThreadOpen(MainWindow * mw, pcl::PolygonMesh::Ptr mesh, int n) { this->mw = mw;  this->mesh = mesh;  this->nView = n; }

	public slots :
		void onEnd()
	{
		cout << "Thread " << nView << " finished" << endl;
	}

private:
	MainWindow * mw;
	pcl::PolygonMesh::Ptr mesh;
	int nView;

	void run()
	{
		cout << "Thread " << nView << " launched..." << endl;
		mw->processView(mesh, nView);
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