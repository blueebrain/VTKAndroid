#ifndef VTKANDROIDINTERACTORADAPTER_H
#define VTKANDROIDINTERACTORADAPTER_H

#include <QtCore/QObject>
#include <QEvent>

#include <vtkRenderWindowInteractor.h>

class vtkAndroidInteractorAdapter:public QObject
{
    Q_OBJECT;
public:
    vtkAndroidInteractorAdapter(QObject*parent=nullptr);
    bool ProcessEvent(QEvent* e, vtkRenderWindowInteractor* iren);
    void SetDevicePixelRatio(float ratio, vtkRenderWindowInteractor* iren);
    float GetDevicePixelRatio() { return this->DevicePixelRatio; }
protected:
    float DevicePixelRatio;
    static const double DevicePixelRatioTolerance;
    float lastDist = 0;

    //double-clicked ==> change modes(pan or rotate)
    ulong lastTimeTemp = 0;
    bool isPan = false;
    bool lastPressed = false;
};

#endif // VTKANDROIDINTERACTORADAPTER_H
