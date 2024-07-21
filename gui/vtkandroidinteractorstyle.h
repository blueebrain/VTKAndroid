#ifndef VTKANDROIDINTERACTORSTYLEH_H
#define VTKANDROIDINTERACTORSTYLEH_H
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleMultiTouchCamera.h>
#include <vtkInteractorStyle.h>
#include <vtkCellPicker.h>

#define AndroidInteractorStyle vtkInteractorStyle

VTK_ABI_NAMESPACE_BEGIN

class vtkAndroidInteractorStyle : public AndroidInteractorStyle
{
public:
    static vtkAndroidInteractorStyle* New();
    vtkTypeMacro(vtkAndroidInteractorStyle, AndroidInteractorStyle);
    void PrintSelf(ostream& os, vtkIndent indent) override;

    void OnMouseMove() override;//handel all actions
    void OnLeftButtonDown() override;
    void OnMiddleButtonDown() override;
    void OnRightButtonDown() override;
    void OnLeftButtonUp() override;//release all actions

    void Rotate() override;
    void Spin() override;
    void Pan() override;
    void Dolly() override;
    void UniformScale() override;

protected:
    vtkAndroidInteractorStyle();
    ~vtkAndroidInteractorStyle() override;

    void FindPickedActor(int x, int y);

    void Prop3DTransform(
        vtkProp3D* prop3D, double* boxCenter, int NumRotation, double** rotate, double* scale);

    double MotionFactor;

    vtkProp3D* InteractionProp;
    vtkCellPicker* InteractionPicker;

private:
    vtkAndroidInteractorStyle(const vtkAndroidInteractorStyle&) = delete;
    void operator=(const vtkAndroidInteractorStyle&) = delete;
};
VTK_ABI_NAMESPACE_END
#endif // VTKANDROIDINTERACTORSTYLEH_H
