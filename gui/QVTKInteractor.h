// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-FileCopyrightText: Copyright 2004 Sandia Corporation
// SPDX-License-Identifier: LicenseRef-BSD-3-Clause-Sandia-USGov

#ifndef Q_VTK_INTERACTOR_H
#define Q_VTK_INTERACTOR_H

#include "QVTKWin32Header.h"
#include <QtCore/QObject>
#include <vtkCommand.h>
#include <vtkRenderWindowInteractor.h>

#include "vtkTDxConfigure.h" // defines VTK_USE_TDX
VTK_ABI_NAMESPACE_BEGIN
#if defined(VTK_USE_TDX) && defined(Q_OS_WIN)
class vtkTDxWinDevice;
#endif
#if defined(VTK_USE_TDX) && defined(Q_OS_MAC)
class vtkTDxMacDevice;
#endif
#if defined(VTK_USE_TDX) && (defined(Q_WS_X11) || defined(Q_OS_LINUX))
class vtkTDxDevice;
class vtkTDxUnixDevice;
#endif

class QVTKInteractorInternal;

/**
 * @class QVTKInteractor
 * @brief - an interactor for QVTKOpenGLNativeWidget (and QVTKWiget).
 *
 * QVTKInteractor handles relaying Qt events to VTK.
 * @sa QVTKOpenGLNativeWidget
 */

class QVTKInteractor : public vtkRenderWindowInteractor
{
public:
  static QVTKInteractor* New();
  vtkTypeMacro(QVTKInteractor, vtkRenderWindowInteractor);

  /**
   * Enum for additional event types supported.
   * These events can be picked up by command observers on the interactor.
   */
  enum vtkCustomEvents
  {
    ContextMenuEvent = vtkCommand::UserEvent + 100,
    DragEnterEvent,
    DragMoveEvent,
    DragLeaveEvent,
    DropEvent
  };

  /**
   * Overloaded terminate app, which does nothing in Qt.
   * Use qApp->exit() instead.
   */
  void TerminateApp() override;

  /**
   * Overloaded start method does nothing.
   * Use qApp->exec() instead.
   */
  void Start() override;
  void Initialize() override;

  /**
   * Start listening events on 3DConnexion device.
   */
  virtual void StartListening();

  /**
   * Stop listening events on 3DConnexion device.
   */
  virtual void StopListening();

  /**
   * timer event slot
   */
  virtual void TimerEvent(int timerId);

#if defined(VTK_USE_TDX) && (defined(Q_WS_X11) || defined(Q_OS_LINUX))
  virtual vtkTDxUnixDevice* GetDevice();
  virtual void SetDevice(vtkTDxDevice* device);
#endif

protected:
  // constructor
  QVTKInteractor();
  // destructor
  ~QVTKInteractor() override;

  // create a Qt Timer
  int InternalCreateTimer(int timerId, int timerType, unsigned long duration) override;
  // destroy a Qt Timer
  int InternalDestroyTimer(int platformTimerId) override;
#if defined(VTK_USE_TDX) && defined(Q_OS_WIN)
  vtkTDxWinDevice* Device;
#endif
#if defined(VTK_USE_TDX) && defined(Q_OS_MAC)
  vtkTDxMacDevice* Device;
#endif
#if defined(VTK_USE_TDX) && (defined(Q_WS_X11) || defined(Q_OS_LINUX))
  vtkTDxUnixDevice* Device;
#endif

private:
  QVTKInteractorInternal* Internal;

  QVTKInteractor(const QVTKInteractor&) = delete;
  void operator=(const QVTKInteractor&) = delete;
};

VTK_ABI_NAMESPACE_END
#endif
// VTK-HeaderTest-Exclude: QVTKInteractor.h
