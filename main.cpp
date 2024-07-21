#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QtGui/QGuiApplication>
#include <QtGui/QSurfaceFormat>

#include "./gui/QQuickVTKItem.h"
#include "./gui/QVTKInteractor.h"
#include "./gui/vtkandroidinteractorstyle.h"

#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCommand.h>
#include <vtkConeSource.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkDataSet.h>
#include <vtkPointData.h>
#include <vtkInteractorStyleTrackballActor.h>
#if ANDROID
#include <vtkAndroidRenderWindowInteractor.h>
#endif

namespace
{
struct MyVtkItem : QQuickVTKItem
{
    MyVtkItem(){
    }
    vtkUserData initializeVTK(vtkRenderWindow* renw) override
    {
        vtkNew<vtkPoints> pointCloud;
        int x = 100;
        int y = 100;
        int z = 100;
        vtkNew<vtkCellArray> vertices;
        vertices->InsertNextCell(x*y*z);
        for(int i=0;i<x;++i){
            for(int j=0;j<y;++j){
                for(int k=0;k<z;++k){
                    vtkIdType id = pointCloud->InsertNextPoint(i*1.0,j*1.0,k*1.0);
                    vertices->InsertCellPoint(id);
                }
            }
        }
        vtkNew<vtkPolyData> poly;
        poly->SetPoints(pointCloud);
        poly->SetVerts(vertices);
        vtkNew<vtkUnsignedCharArray> colors;
        colors->SetNumberOfComponents(3);
        colors->SetNumberOfTuples(pointCloud->GetNumberOfPoints());
        for(size_t i = 0;i<colors->GetNumberOfTuples();++i){
            double rgb[3]={0,0,0};
            rgb[0] = (100-pointCloud->GetPoint(i)[2])/100.0*255;
            rgb[2] = pointCloud->GetPoint(i)[2]/100.0*255;
            colors->InsertTuple(i,rgb);
        }
        poly->GetPointData()->SetScalars(colors);
        vtkNew<vtkPolyDataMapper> pointsMapper;
        pointsMapper->SetInputData(poly);
        vtkNew<vtkActor> pointActor;
        pointActor->SetMapper(pointsMapper);
        pointActor->GetProperty()->SetPointSize(1);
        vtkNew<vtkRenderer> ren;
        ren->SetBackground(0,0,0);
        ren->AddActor(pointActor);
        renw->AddRenderer(ren);
        vtkNew<QVTKInteractor> iren;
        vtkNew<vtkAndroidInteractorStyle> style;
        iren->SetInteractorStyle(style);
        renw->SetInteractor(iren);
        return nullptr;
    }
};

}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickVTKItem::setGraphicsApi();

    qmlRegisterType<MyVtkItem>("com.vtk.example", 1, 0, "MyVtkItem");

    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/VTKAndroid/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
