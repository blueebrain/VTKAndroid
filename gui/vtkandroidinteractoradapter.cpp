#include "vtkandroidinteractoradapter.h"

#include <QEvent>
#include <QGestureEvent>
#include <QSignalMapper>
#include <QTimer>
#include <QWidget>
#include <vtkCommand.h>
const double vtkAndroidInteractorAdapter::DevicePixelRatioTolerance = 1e-5;

vtkAndroidInteractorAdapter::vtkAndroidInteractorAdapter(QObject*parent):
    QObject(parent),
    DevicePixelRatio(1.0)
{

}

void vtkAndroidInteractorAdapter::SetDevicePixelRatio(float ratio, vtkRenderWindowInteractor* iren)
{
    if (ratio != DevicePixelRatio)
    {
        if (iren)
        {
            int tmp[2];
            iren->GetSize(tmp);
            if (ratio == 1.0)
            {
                iren->SetSize(tmp[0] / 2, tmp[1] / 2);
            }
            else
            {
                iren->SetSize(static_cast<int>(tmp[0] * ratio + DevicePixelRatioTolerance),
                              static_cast<int>(tmp[1] * ratio + DevicePixelRatioTolerance));
            }
        }
        this->DevicePixelRatio = ratio;
    }
}

bool vtkAndroidInteractorAdapter::ProcessEvent(QEvent* e, vtkRenderWindowInteractor* iren){
    if (iren == nullptr || e == nullptr)
        return false;
    if (!iren->GetEnabled())
        return false;
    const QEvent::Type t = e->type();
    if (t == QEvent::TouchBegin || t == QEvent::TouchUpdate || t == QEvent::TouchEnd)
    {
        QTouchEvent* e2 = dynamic_cast<QTouchEvent*>(e);
        const int hitPoint = e2->pointCount();
        QPointF center(0,0);
        for(auto& point:e2->points()){
            if (point.id() >= VTKI_MAX_POINTERS)
            {
                break;
            }

            if(hitPoint==2){
                center+=point.position();
            }

            if(point.state()==QEventPoint::Pressed){
                lastPressed = true;
            }
            else if(point.state()==QEventPoint::Released){
                if(lastPressed){
                    if(lastTimeTemp==0){
                        lastTimeTemp = point.lastTimestamp();
                    }
                    else if(point.lastTimestamp()-lastTimeTemp<200){
                        isPan = !isPan;
                        lastTimeTemp = 0;
                    }
                }
                lastPressed = false;
            }
            else{
                lastPressed = false;
                lastTimeTemp =0;
            }

            QPointF pos = point.position();
            int x = static_cast<int>(pos.x() * this->DevicePixelRatio + DevicePixelRatioTolerance);
            int y = static_cast<int>(pos.y() * this->DevicePixelRatio + DevicePixelRatioTolerance);
            iren->SetEventInformationFlipY(x,y,
                                           (e2->modifiers() & Qt::ControlModifier) > 0 ? 1 : 0,
                                           (e2->modifiers() & Qt::ShiftModifier) > 0 ? 1 : 0, 0, 0, nullptr, point.id());
        }
        if(hitPoint == 1){
            for(auto& point:e2->points()){
                if (point.id() >= VTKI_MAX_POINTERS)
                {
                    break;
                }
                iren->SetPointerIndex(point.id());
                if (point.state() & Qt::TouchPointPressed)
                {
                    if(!isPan) iren->InvokeEvent(vtkCommand::LeftButtonPressEvent, nullptr);
                    else iren->InvokeEvent(vtkCommand::MiddleButtonPressEvent, nullptr);
                }
                if (point.state() & Qt::TouchPointMoved)
                {
                    iren->InvokeEvent(vtkCommand::MouseMoveEvent, nullptr);
                }
                if (point.state() & Qt::TouchPointReleased)
                {
                    iren->InvokeEvent(vtkCommand::LeftButtonReleaseEvent, nullptr);
                }
            }
        }
        else if(hitPoint == 2){
            QPointF position = center/2;
            iren->SetEventInformationFlipY(//SetEventPosition(center');
                static_cast<int>(position.x() * this->DevicePixelRatio + DevicePixelRatioTolerance),
                static_cast<int>(position.y() * this->DevicePixelRatio + DevicePixelRatioTolerance));
            int eventPosition[2];
            int lastEventPosition[2];
            iren->GetEventPosition(eventPosition);
            iren->GetLastEventPosition(lastEventPosition);
            QLineF line(eventPosition[0],eventPosition[1],
                        lastEventPosition[0],lastEventPosition[1]);
            float dist = line.length();
            float scale =lastDist?dist/lastDist:1.0;
            iren->SetScale(1.0);
            iren->SetScale(scale);
            lastDist = dist;
            for(auto& point:e2->points()){
                if (point.id() >= VTKI_MAX_POINTERS)
                {
                    break;
                }
                if (point.state() & Qt::TouchPointPressed)
                {
                    iren->InvokeEvent(vtkCommand::RightButtonPressEvent, nullptr);
                }
                if (point.state() & Qt::TouchPointMoved)
                {
                    iren->InvokeEvent(vtkCommand::MouseMoveEvent, nullptr);
                    if (e2->points().at(1).state() & Qt::TouchPointMoved)
                        break;
                }
                if (point.state() & Qt::TouchPointReleased)
                {
                    iren->InvokeEvent(vtkCommand::LeftButtonReleaseEvent, nullptr);
                }
            }

        }
        e2->accept();
        return true;
    }
    return false;
}

