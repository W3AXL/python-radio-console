/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt for Python.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/


#ifndef SBK_QTAXCONTAINER_PYTHON_H
#define SBK_QTAXCONTAINER_PYTHON_H

#include <sbkpython.h>
#include <sbkconverter.h>
// Module Includes
#include <pyside6_qtwidgets_python.h>
#include <pyside6_qtgui_python.h>
#include <pyside6_qtcore_python.h>

// Bound library includes
#include <QtAxContainer/qaxwidget.h>
#include <QtAxContainer/qaxselect.h>
#include <QtAxContainer/qaxobjectinterface.h>
#include <QtAxContainer/qaxbase.h>
#include <QtAxContainer/qaxobject.h>
#include <QtAxContainer/qaxscript.h>
// Conversion Includes - Primitive Types
#include <wtypes.h>
#include <qabstractitemmodel.h>
#include <QString>
#include <QStringList>
#include <QStringView>
#include <signalmanager.h>

// Conversion Includes - Container Types
#include <pysideqflags.h>
#include <QList>
#include <QMap>
#include <QMultiMap>
#include <QPair>
#include <QQueue>
#include <QSet>
#include <QStack>

// Type indices
enum : int {
    SBK_QAXBASE_IDX                                          = 0,
    SBK_QAXBASEOBJECT_IDX                                    = 1,
    SBK_QAXBASEWIDGET_IDX                                    = 2,
    SBK_QAXOBJECT_IDX                                        = 3,
    SBK_QAXOBJECTINTERFACE_IDX                               = 4,
    SBK_QAXSCRIPT_FUNCTIONFLAGS_IDX                          = 6,
    SBK_QAXSCRIPT_IDX                                        = 5,
    SBK_QAXSCRIPTENGINE_STATE_IDX                            = 8,
    SBK_QAXSCRIPTENGINE_IDX                                  = 7,
    SBK_QAXSCRIPTMANAGER_IDX                                 = 9,
    SBK_QAXSELECT_SANDBOXINGLEVEL_IDX                        = 11,
    SBK_QAXSELECT_IDX                                        = 10,
    SBK_QAXWIDGET_IDX                                        = 12,
    SBK_QtAxContainer_IDX_COUNT                              = 13
};
// This variable stores all Python types exported by this module.
extern PyTypeObject **SbkPySide6_QtAxContainerTypes;

// This variable stores the Python module object exported by this module.
extern PyObject *SbkPySide6_QtAxContainerModuleObject;

// This variable stores all type converters exported by this module.
extern SbkConverter **SbkPySide6_QtAxContainerTypeConverters;

// Converter indices
enum : int {
    SBK_QTAXCONTAINER_QLIST_QVARIANT_IDX                     = 0, // QList<QVariant >
    SBK_QTAXCONTAINER_QLIST_QOBJECTPTR_IDX                   = 1, // QList<QObject* >
    SBK_QTAXCONTAINER_QLIST_QBYTEARRAY_IDX                   = 2, // QList<QByteArray >
    SBK_QTAXCONTAINER_QLIST_QACTIONPTR_IDX                   = 3, // QList<QAction* >
    SBK_QTAXCONTAINER_QMAP_QSTRING_QVARIANT_IDX              = 4, // QMap<QString,QVariant >
    SBK_QTAXCONTAINER_QLIST_QSTRING_IDX                      = 5, // QList<QString >
    SBK_QtAxContainer_CONVERTERS_IDX_COUNT                   = 6
};
// Macros for type check

namespace Shiboken
{

// PyType functions, to get the PyObjectType for a type T
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
template<> inline PyTypeObject *SbkType< ::QAxBase >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtAxContainerTypes[SBK_QAXBASE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAxBaseObject >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtAxContainerTypes[SBK_QAXBASEOBJECT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAxBaseWidget >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtAxContainerTypes[SBK_QAXBASEWIDGET_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAxObject >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtAxContainerTypes[SBK_QAXOBJECT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAxObjectInterface >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtAxContainerTypes[SBK_QAXOBJECTINTERFACE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAxScript::FunctionFlags >() { return SbkPySide6_QtAxContainerTypes[SBK_QAXSCRIPT_FUNCTIONFLAGS_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAxScript >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtAxContainerTypes[SBK_QAXSCRIPT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAxScriptEngine::State >() { return SbkPySide6_QtAxContainerTypes[SBK_QAXSCRIPTENGINE_STATE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAxScriptEngine >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtAxContainerTypes[SBK_QAXSCRIPTENGINE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAxScriptManager >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtAxContainerTypes[SBK_QAXSCRIPTMANAGER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAxSelect::SandboxingLevel >() { return SbkPySide6_QtAxContainerTypes[SBK_QAXSELECT_SANDBOXINGLEVEL_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAxSelect >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtAxContainerTypes[SBK_QAXSELECT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAxWidget >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtAxContainerTypes[SBK_QAXWIDGET_IDX]); }
QT_WARNING_POP

} // namespace Shiboken

#endif // SBK_QTAXCONTAINER_PYTHON_H

