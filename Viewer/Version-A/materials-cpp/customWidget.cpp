#include "customWidget.h"

#include <QGuiApplication>
#include <QApplication>

#include "window.h"
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QAspectEngine>
#include <Qt3DCore/QCamera>

#include <Qt3DInput/QInputAspect>

#include <Qt3DRender/QRenderAspect>
#include <Qt3DRender/QPhongMaterial>
#include <Qt3DRender/QDiffuseMapMaterial>
#include <Qt3DRender/QForwardRenderer>
#include <Qt3DRender/QFrameGraph>
#include <Qt3DRender/QTextureImage>

#include <QtWidgets/QMessageBox>

#include "planeentity.h"
#include "renderableentity.h"
#include <iostream>
//#include <QVTKWidget.h>

CustomWidget::CustomWidget(QWidget * parent)
	: QOpenGLWidget(parent)
{
	Qt3DCore::QAspectEngine engine;

	// register render Aspect
	   engine.registerAspect(new Qt3DRender::QRenderAspect());
	
	// define & register input Aspect
	   Qt3DInput::QInputAspect *input = new Qt3DInput::QInputAspect;
	   engine.registerAspect(input);

	// define Variant Map
	   QVariantMap data;
	   data.insert(QStringLiteral("surface"), QVariant::fromValue(this));
	   data.insert(QStringLiteral("eventSource"), QVariant::fromValue(this));
	   engine.setData(data);
	   

	   // Scene Root
	   Qt3DCore::QEntity *sceneRoot = new Qt3DCore::QEntity();

	   // Scene Camera
	   Qt3DCore::QCamera *basicCamera = new Qt3DCore::QCamera(sceneRoot);
	   basicCamera->setProjectionType(Qt3DCore::QCameraLens::PerspectiveProjection);
	   basicCamera->setAspectRatio(this->width() / this->height());
	   basicCamera->setUpVector(QVector3D(0.0f, 1.0f, 0.0f));
	   basicCamera->setViewCenter(QVector3D(0.0f, 3.5f, 0.0f));
	   basicCamera->setPosition(QVector3D(0.0f, 3.5f, 25.0f));
	   
	   // For camera controls
	   input->setCamera(basicCamera);
	
	   // Forward Renderer FrameGraph
	   Qt3DRender::QFrameGraph *frameGraphComponent = new Qt3DRender::QFrameGraph(sceneRoot);
	   Qt3DRender::QForwardRenderer *forwardRenderer = new Qt3DRender::QForwardRenderer();
	   forwardRenderer->setCamera(basicCamera);
	   frameGraphComponent->setActiveFrameGraph(forwardRenderer);
	   sceneRoot->addComponent(frameGraphComponent);


	
	   // define entity to render
	   RenderableEntity *chest = new RenderableEntity(sceneRoot);

	// reduce size by half
	   chest->transform()->setScale(0.5f);

	// load it
	   chest->mesh()->setSource(QUrl(QStringLiteral("file:C:/Users/user/VisualStudioProjects/man.obj")));


	   //Qt3DRender::QDiffuseMapMaterial *diffuseMapMaterial = new Qt3DRender::QDiffuseMapMaterial();
	   //diffuseMapMaterial->setSpecular(QColor::fromRgbF(0.5f, 0.5f, 0.5f, 1.0f));
	   //diffuseMapMaterial->setShininess(2.0f);

	   //chest->addComponent(diffuseMapMaterial);


	   engine.setRootEntity(sceneRoot);

	   std::cout << "OK " << std::endl;
}
