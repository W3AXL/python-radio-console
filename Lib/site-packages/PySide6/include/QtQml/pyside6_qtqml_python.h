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


#ifndef SBK_QTQML_PYTHON_H
#define SBK_QTQML_PYTHON_H

#include <sbkpython.h>
#include <sbkconverter.h>
// Module Includes
#include <pyside6_qtcore_python.h>
#include <pyside6_qtnetwork_python.h>

// Bound library includes
#include <QtQml/qjsvalueiterator.h>
#include <QtQml/qqmlcomponent.h>
#include <QtQml/qqmlextensionplugin.h>
#include <QtQml/qqmlpropertymap.h>
#include <QtQml/qjsvalue.h>
#include <QtQml/qjsprimitivevalue.h>
#include <QtQml/qqmlnetworkaccessmanagerfactory.h>
#include <QtQml/qqmlapplicationengine.h>
#include <QtQml/qqmlcontext.h>
#include <QtQml/qqmlfile.h>
#include <QtQml/qqmlexpression.h>
#include <QtQml/qqmlfileselector.h>
#include <QtQml/qjsmanagedvalue.h>
#include <QtQml/qqmldebug.h>
#include <QtQml/qqmlextensioninterface.h>
#include <QtQml/qqml.h>
#include <QtQml/qqmlincubator.h>
#include <QtQml/qqmlpropertyvaluesource.h>
#include <QtQml/qqmlparserstatus.h>
#include <QtQml/qqmlabstracturlinterceptor.h>
#include <QtQml/qjsengine.h>
#include <QtQml/qqmlengine.h>
#include <QtQml/qqmllist.h>
#include <QtQml/qqmlerror.h>
#include <QtQml/qqmlscriptstring.h>
#include <QtQml/qqmlproperty.h>
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

// Begin code injection
// Volatile Bool Ptr type definition for QQmlIncubationController::incubateWhile(std::atomic<bool> *, int)
#include <atomic>

using AtomicBool = std::atomic<bool>;

typedef struct {
    PyObject_HEAD
    AtomicBool *flag;
} QtQml_VolatileBoolObject;
// End of code injection

// Type indices
enum : int {
    SBK_QJSENGINE_OBJECTOWNERSHIP_IDX                        = 4,
    SBK_QJSENGINE_EXTENSION_IDX                              = 3,
    SBK_QFLAGS_QJSENGINE_EXTENSION_IDX                       = 0,
    SBK_QJSENGINE_IDX                                        = 2,
    SBK_QJSMANAGEDVALUE_TYPE_IDX                             = 6,
    SBK_QJSMANAGEDVALUE_IDX                                  = 5,
    SBK_QJSPRIMITIVEVALUE_TYPE_IDX                           = 8,
    SBK_QJSPRIMITIVEVALUE_IDX                                = 7,
    SBK_QJSVALUE_SPECIALVALUE_IDX                            = 12,
    SBK_QJSVALUE_ERRORTYPE_IDX                               = 10,
    SBK_QJSVALUE_OBJECTCONVERSIONBEHAVIOR_IDX                = 11,
    SBK_QJSVALUE_IDX                                         = 9,
    SBK_QJSVALUEITERATOR_IDX                                 = 13,
    SBK_QQMLABSTRACTURLINTERCEPTOR_DATATYPE_IDX              = 16,
    SBK_QQMLABSTRACTURLINTERCEPTOR_IDX                       = 15,
    SBK_QQMLAPPLICATIONENGINE_IDX                            = 17,
    SBK_QQMLCOMPONENT_COMPILATIONMODE_IDX                    = 19,
    SBK_QQMLCOMPONENT_STATUS_IDX                             = 20,
    SBK_QQMLCOMPONENT_IDX                                    = 18,
    SBK_QQMLCONTEXT_IDX                                      = 21,
    SBK_QQMLDEBUGGINGENABLER_STARTMODE_IDX                   = 23,
    SBK_QQMLDEBUGGINGENABLER_IDX                             = 22,
    SBK_QQMLENGINE_IDX                                       = 24,
    SBK_QQMLERROR_IDX                                        = 25,
    SBK_QQMLEXPRESSION_IDX                                   = 26,
    SBK_QQMLEXTENSIONINTERFACE_IDX                           = 27,
    SBK_QQMLEXTENSIONPLUGIN_IDX                              = 28,
    SBK_QQMLFILE_STATUS_IDX                                  = 30,
    SBK_QQMLFILE_IDX                                         = 29,
    SBK_QQMLFILESELECTOR_IDX                                 = 31,
    SBK_QQMLIMAGEPROVIDERBASE_IMAGETYPE_IDX                  = 34,
    SBK_QQMLIMAGEPROVIDERBASE_FLAG_IDX                       = 33,
    SBK_QFLAGS_QQMLIMAGEPROVIDERBASE_FLAG_IDX                = 1,
    SBK_QQMLIMAGEPROVIDERBASE_IDX                            = 32,
    SBK_QQMLINCUBATIONCONTROLLER_IDX                         = 35,
    SBK_QQMLINCUBATOR_INCUBATIONMODE_IDX                     = 37,
    SBK_QQMLINCUBATOR_STATUS_IDX                             = 38,
    SBK_QQMLINCUBATOR_IDX                                    = 36,
    SBK_QQMLLISTREFERENCE_IDX                                = 39,
    SBK_QQMLNETWORKACCESSMANAGERFACTORY_IDX                  = 41,
    SBK_QQMLPARSERSTATUS_IDX                                 = 42,
    SBK_QQMLPROPERTY_PROPERTYTYPECATEGORY_IDX                = 44,
    SBK_QQMLPROPERTY_TYPE_IDX                                = 45,
    SBK_QQMLPROPERTY_IDX                                     = 43,
    SBK_QQMLPROPERTYMAP_IDX                                  = 46,
    SBK_QQMLPROPERTYVALUESOURCE_IDX                          = 47,
    SBK_QQMLSCRIPTSTRING_IDX                                 = 48,
    SBK_QQMLTYPESEXTENSIONINTERFACE_IDX                      = 49,
    SBK_QML_HAS_ATTACHED_PROPERTIES_IDX                      = 14,
    SBK_QQMLMODULEIMPORTSPECIALVERSIONS_IDX                  = 40,
    SBK_QtQml_IDX_COUNT                                      = 51
};
// This variable stores all Python types exported by this module.
extern PyTypeObject **SbkPySide6_QtQmlTypes;

// This variable stores the Python module object exported by this module.
extern PyObject *SbkPySide6_QtQmlModuleObject;

// This variable stores all type converters exported by this module.
extern SbkConverter **SbkPySide6_QtQmlTypeConverters;

// Converter indices
enum : int {
    SBK_QTQML_QLIST_QQMLERROR_IDX                            = 0, // QList<QQmlError >
    SBK_QTQML_QMAP_QSTRING_QVARIANT_IDX                      = 1, // QMap<QString,QVariant >
    SBK_QTQML_QHASH_QSTRING_QVARIANT_IDX                     = 2, // QHash<QString,QVariant >
    SBK_QTQML_QLIST_QJSVALUE_IDX                             = 3, // QList<QJSValue >
    SBK_QTQML_QLIST_QOBJECTPTR_IDX                           = 4, // QList<QObject* >
    SBK_QTQML_QLIST_QBYTEARRAY_IDX                           = 5, // QList<QByteArray >
    SBK_QTQML_QLIST_QVARIANT_IDX                             = 6, // QList<QVariant >
    SBK_QTQML_QLIST_QSTRING_IDX                              = 7, // QList<QString >
    SBK_QtQml_CONVERTERS_IDX_COUNT                           = 8
};
// Macros for type check

namespace Shiboken
{

// PyType functions, to get the PyObjectType for a type T
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
template<> inline PyTypeObject *SbkType< ::QQmlModuleImportSpecialVersions >() { return SbkPySide6_QtQmlTypes[SBK_QQMLMODULEIMPORTSPECIALVERSIONS_IDX]; }
template<> inline PyTypeObject *SbkType< ::QJSEngine::ObjectOwnership >() { return SbkPySide6_QtQmlTypes[SBK_QJSENGINE_OBJECTOWNERSHIP_IDX]; }
template<> inline PyTypeObject *SbkType< ::QJSEngine::Extension >() { return SbkPySide6_QtQmlTypes[SBK_QJSENGINE_EXTENSION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QJSEngine::Extension> >() { return SbkPySide6_QtQmlTypes[SBK_QFLAGS_QJSENGINE_EXTENSION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QJSEngine >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtQmlTypes[SBK_QJSENGINE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QJSManagedValue::Type >() { return SbkPySide6_QtQmlTypes[SBK_QJSMANAGEDVALUE_TYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QJSManagedValue >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtQmlTypes[SBK_QJSMANAGEDVALUE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QJSPrimitiveValue::Type >() { return SbkPySide6_QtQmlTypes[SBK_QJSPRIMITIVEVALUE_TYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QJSPrimitiveValue >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtQmlTypes[SBK_QJSPRIMITIVEVALUE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QJSValue::SpecialValue >() { return SbkPySide6_QtQmlTypes[SBK_QJSVALUE_SPECIALVALUE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QJSValue::ErrorType >() { return SbkPySide6_QtQmlTypes[SBK_QJSVALUE_ERRORTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QJSValue::ObjectConversionBehavior >() { return SbkPySide6_QtQmlTypes[SBK_QJSVALUE_OBJECTCONVERSIONBEHAVIOR_IDX]; }
template<> inline PyTypeObject *SbkType< ::QJSValue >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtQmlTypes[SBK_QJSVALUE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QJSValueIterator >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtQmlTypes[SBK_QJSVALUEITERATOR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QQmlAbstractUrlInterceptor::DataType >() { return SbkPySide6_QtQmlTypes[SBK_QQMLABSTRACTURLINTERCEPTOR_DATATYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QQmlAbstractUrlInterceptor >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtQmlTypes[SBK_QQMLABSTRACTURLINTERCEPTOR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QQmlApplicationEngine >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtQmlTypes[SBK_QQMLAPPLICATIONENGINE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QQmlComponent::CompilationMode >() { return SbkPySide6_QtQmlTypes[SBK_QQMLCOMPONENT_COMPILATIONMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QQmlComponent::Status >() { return SbkPySide6_QtQmlTypes[SBK_QQMLCOMPONENT_STATUS_IDX]; }
template<> inline PyTypeObject *SbkType< ::QQmlComponent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtQmlTypes[SBK_QQMLCOMPONENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QQmlContext >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtQmlTypes[SBK_QQMLCONTEXT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QQmlDebuggingEnabler::StartMode >() { return SbkPySide6_QtQmlTypes[SBK_QQMLDEBUGGINGENABLER_STARTMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QQmlDebuggingEnabler >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtQmlTypes[SBK_QQMLDEBUGGINGENABLER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QQmlEngine >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtQmlTypes[SBK_QQMLENGINE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QQmlError >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtQmlTypes[SBK_QQMLERROR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QQmlExpression >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtQmlTypes[SBK_QQMLEXPRESSION_IDX]); }
template<> inline PyTypeObject *SbkType< ::QQmlExtensionInterface >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtQmlTypes[SBK_QQMLEXTENSIONINTERFACE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QQmlExtensionPlugin >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtQmlTypes[SBK_QQMLEXTENSIONPLUGIN_IDX]); }
template<> inline PyTypeObject *SbkType< ::QQmlFile::Status >() { return SbkPySide6_QtQmlTypes[SBK_QQMLFILE_STATUS_IDX]; }
template<> inline PyTypeObject *SbkType< ::QQmlFile >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtQmlTypes[SBK_QQMLFILE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QQmlFileSelector >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtQmlTypes[SBK_QQMLFILESELECTOR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QQmlImageProviderBase::ImageType >() { return SbkPySide6_QtQmlTypes[SBK_QQMLIMAGEPROVIDERBASE_IMAGETYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QQmlImageProviderBase::Flag >() { return SbkPySide6_QtQmlTypes[SBK_QQMLIMAGEPROVIDERBASE_FLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QQmlImageProviderBase::Flag> >() { return SbkPySide6_QtQmlTypes[SBK_QFLAGS_QQMLIMAGEPROVIDERBASE_FLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QQmlImageProviderBase >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtQmlTypes[SBK_QQMLIMAGEPROVIDERBASE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QQmlIncubationController >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtQmlTypes[SBK_QQMLINCUBATIONCONTROLLER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QQmlIncubator::IncubationMode >() { return SbkPySide6_QtQmlTypes[SBK_QQMLINCUBATOR_INCUBATIONMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QQmlIncubator::Status >() { return SbkPySide6_QtQmlTypes[SBK_QQMLINCUBATOR_STATUS_IDX]; }
template<> inline PyTypeObject *SbkType< ::QQmlIncubator >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtQmlTypes[SBK_QQMLINCUBATOR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QQmlListReference >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtQmlTypes[SBK_QQMLLISTREFERENCE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QQmlNetworkAccessManagerFactory >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtQmlTypes[SBK_QQMLNETWORKACCESSMANAGERFACTORY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QQmlParserStatus >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtQmlTypes[SBK_QQMLPARSERSTATUS_IDX]); }
template<> inline PyTypeObject *SbkType< ::QQmlProperty::PropertyTypeCategory >() { return SbkPySide6_QtQmlTypes[SBK_QQMLPROPERTY_PROPERTYTYPECATEGORY_IDX]; }
template<> inline PyTypeObject *SbkType< ::QQmlProperty::Type >() { return SbkPySide6_QtQmlTypes[SBK_QQMLPROPERTY_TYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QQmlProperty >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtQmlTypes[SBK_QQMLPROPERTY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QQmlPropertyMap >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtQmlTypes[SBK_QQMLPROPERTYMAP_IDX]); }
template<> inline PyTypeObject *SbkType< ::QQmlPropertyValueSource >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtQmlTypes[SBK_QQMLPROPERTYVALUESOURCE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QQmlScriptString >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtQmlTypes[SBK_QQMLSCRIPTSTRING_IDX]); }
template<> inline PyTypeObject *SbkType< ::QQmlTypesExtensionInterface >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtQmlTypes[SBK_QQMLTYPESEXTENSIONINTERFACE_IDX]); }
QT_WARNING_POP

} // namespace Shiboken

#endif // SBK_QTQML_PYTHON_H

