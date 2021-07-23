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


#ifndef SBK_QTOPENGL_PYTHON_H
#define SBK_QTOPENGL_PYTHON_H

#include <sbkpython.h>
#include <sbkconverter.h>
// Module Includes
#include <pyside6_qtcore_python.h>
#include <pyside6_qtgui_python.h>

// Bound library includes
#include <QtOpenGL/qopenglfunctions_1_0.h>
#include <QtOpenGL/qopenglbuffer.h>
#include <QtOpenGL/qopenglfunctions_4_0_core.h>
#include <QtOpenGL/qopengltimerquery.h>
#include <QtOpenGL/qopenglversionfunctions.h>
#include <QtOpenGL/qopenglfunctions_4_1_core.h>
#include <QtOpenGL/qopenglfunctions_4_3_compatibility.h>
#include <QtOpenGL/qopenglfunctions_3_2_core.h>
#include <QtOpenGL/qopenglfunctions_4_3_core.h>
#include <QtOpenGL/qopenglfunctions_3_1.h>
#include <QtOpenGL/qopenglfunctions_3_0.h>
#include <QtOpenGL/qopenglversionfunctionsfactory.h>
#include <QtOpenGL/qopengldebug.h>
#include <QtOpenGL/qopenglfunctions_4_0_compatibility.h>
#include <QtOpenGL/qopengltexture.h>
#include <QtOpenGL/qopenglfunctions_4_1_compatibility.h>
#include <QtOpenGL/qopenglfunctions_2_1.h>
#include <QtOpenGL/qopenglfunctions_4_2_compatibility.h>
#include <QtOpenGL/qopenglfunctions_1_1.h>
#include <QtOpenGL/qopenglfunctions_3_3_compatibility.h>
#include <QtOpenGL/qopenglpaintdevice.h>
#include <QtOpenGL/qopenglversionprofile.h>
#include <QtOpenGL/qopenglfunctions_1_3.h>
#include <QtOpenGL/qopenglvertexarrayobject.h>
#include <QtOpenGL/qopenglfunctions_1_2.h>
#include <QtOpenGL/qopenglfunctions_3_3_core.h>
#include <QtOpenGL/qopenglfunctions_4_5_core.h>
#include <QtOpenGL/qopenglfunctions_4_5_compatibility.h>
#include <QtOpenGL/qopenglfunctions_4_4_compatibility.h>
#include <QtOpenGL/qopenglshaderprogram.h>
#include <QtOpenGL/qopenglwindow.h>
#include <QtOpenGL/qopenglfunctions_4_2_core.h>
#include <QtOpenGL/qopenglpixeltransferoptions.h>
#include <QtOpenGL/qopenglfunctions_1_5.h>
#include <QtOpenGL/qopenglfunctions_2_0.h>
#include <QtOpenGL/qopenglfunctions_3_2_compatibility.h>
#include <QtOpenGL/qopengltextureblitter.h>
#include <QtOpenGL/qopenglframebufferobject.h>
#include <QtOpenGL/qopenglfunctions_1_4.h>
#include <QtOpenGL/qopenglfunctions_4_4_core.h>
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
    SBK_QABSTRACTOPENGLFUNCTIONS_IDX                         = 0,
    SBK_QOPENGLBUFFER_TYPE_IDX                               = 10,
    SBK_QOPENGLBUFFER_USAGEPATTERN_IDX                       = 11,
    SBK_QOPENGLBUFFER_ACCESS_IDX                             = 8,
    SBK_QOPENGLBUFFER_RANGEACCESSFLAG_IDX                    = 9,
    SBK_QFLAGS_QOPENGLBUFFER_RANGEACCESSFLAG_IDX             = 1,
    SBK_QOPENGLBUFFER_IDX                                    = 7,
    SBK_QOPENGLDEBUGLOGGER_LOGGINGMODE_IDX                   = 13,
    SBK_QOPENGLDEBUGLOGGER_IDX                               = 12,
    SBK_QOPENGLDEBUGMESSAGE_SOURCE_IDX                       = 16,
    SBK_QFLAGS_QOPENGLDEBUGMESSAGE_SOURCE_IDX                = 3,
    SBK_QOPENGLDEBUGMESSAGE_TYPE_IDX                         = 17,
    SBK_QFLAGS_QOPENGLDEBUGMESSAGE_TYPE_IDX                  = 4,
    SBK_QOPENGLDEBUGMESSAGE_SEVERITY_IDX                     = 15,
    SBK_QFLAGS_QOPENGLDEBUGMESSAGE_SEVERITY_IDX              = 2,
    SBK_QOPENGLDEBUGMESSAGE_IDX                              = 14,
    SBK_QOPENGLFRAMEBUFFEROBJECT_ATTACHMENT_IDX              = 19,
    SBK_QOPENGLFRAMEBUFFEROBJECT_FRAMEBUFFERRESTOREPOLICY_IDX = 20,
    SBK_QOPENGLFRAMEBUFFEROBJECT_IDX                         = 18,
    SBK_QOPENGLFRAMEBUFFEROBJECTFORMAT_IDX                   = 21,
    SBK_QOPENGLFUNCTIONS_1_0_IDX                             = 22,
    SBK_QOPENGLFUNCTIONS_1_1_IDX                             = 23,
    SBK_QOPENGLFUNCTIONS_1_2_IDX                             = 24,
    SBK_QOPENGLFUNCTIONS_1_3_IDX                             = 25,
    SBK_QOPENGLFUNCTIONS_1_4_IDX                             = 26,
    SBK_QOPENGLFUNCTIONS_1_5_IDX                             = 27,
    SBK_QOPENGLFUNCTIONS_2_0_IDX                             = 28,
    SBK_QOPENGLFUNCTIONS_2_1_IDX                             = 29,
    SBK_QOPENGLFUNCTIONS_3_0_IDX                             = 30,
    SBK_QOPENGLFUNCTIONS_3_1_IDX                             = 31,
    SBK_QOPENGLFUNCTIONS_3_2_COMPATIBILITY_IDX               = 32,
    SBK_QOPENGLFUNCTIONS_3_2_CORE_IDX                        = 33,
    SBK_QOPENGLFUNCTIONS_3_3_COMPATIBILITY_IDX               = 34,
    SBK_QOPENGLFUNCTIONS_3_3_CORE_IDX                        = 35,
    SBK_QOPENGLFUNCTIONS_4_0_COMPATIBILITY_IDX               = 36,
    SBK_QOPENGLFUNCTIONS_4_0_CORE_IDX                        = 37,
    SBK_QOPENGLFUNCTIONS_4_1_COMPATIBILITY_IDX               = 38,
    SBK_QOPENGLFUNCTIONS_4_1_CORE_IDX                        = 39,
    SBK_QOPENGLFUNCTIONS_4_2_COMPATIBILITY_IDX               = 40,
    SBK_QOPENGLFUNCTIONS_4_2_CORE_IDX                        = 41,
    SBK_QOPENGLFUNCTIONS_4_3_COMPATIBILITY_IDX               = 42,
    SBK_QOPENGLFUNCTIONS_4_3_CORE_IDX                        = 43,
    SBK_QOPENGLFUNCTIONS_4_4_COMPATIBILITY_IDX               = 44,
    SBK_QOPENGLFUNCTIONS_4_4_CORE_IDX                        = 45,
    SBK_QOPENGLFUNCTIONS_4_5_COMPATIBILITY_IDX               = 46,
    SBK_QOPENGLFUNCTIONS_4_5_CORE_IDX                        = 47,
    SBK_QOPENGLPAINTDEVICE_IDX                               = 49,
    SBK_QOPENGLPIXELTRANSFEROPTIONS_IDX                      = 50,
    SBK_QOPENGLSHADER_SHADERTYPEBIT_IDX                      = 52,
    SBK_QFLAGS_QOPENGLSHADER_SHADERTYPEBIT_IDX               = 5,
    SBK_QOPENGLSHADER_IDX                                    = 51,
    SBK_QOPENGLSHADERPROGRAM_IDX                             = 53,
    SBK_QOPENGLTEXTURE_TARGET_IDX                            = 68,
    SBK_QOPENGLTEXTURE_BINDINGTARGET_IDX                     = 55,
    SBK_QOPENGLTEXTURE_MIPMAPGENERATION_IDX                  = 63,
    SBK_QOPENGLTEXTURE_TEXTUREUNITRESET_IDX                  = 71,
    SBK_QOPENGLTEXTURE_TEXTUREFORMAT_IDX                     = 69,
    SBK_QOPENGLTEXTURE_TEXTUREFORMATCLASS_IDX                = 70,
    SBK_QOPENGLTEXTURE_CUBEMAPFACE_IDX                       = 59,
    SBK_QOPENGLTEXTURE_PIXELFORMAT_IDX                       = 64,
    SBK_QOPENGLTEXTURE_PIXELTYPE_IDX                         = 65,
    SBK_QOPENGLTEXTURE_SWIZZLECOMPONENT_IDX                  = 66,
    SBK_QOPENGLTEXTURE_SWIZZLEVALUE_IDX                      = 67,
    SBK_QOPENGLTEXTURE_WRAPMODE_IDX                          = 72,
    SBK_QOPENGLTEXTURE_COORDINATEDIRECTION_IDX               = 58,
    SBK_QOPENGLTEXTURE_FEATURE_IDX                           = 61,
    SBK_QFLAGS_QOPENGLTEXTURE_FEATURE_IDX                    = 6,
    SBK_QOPENGLTEXTURE_DEPTHSTENCILMODE_IDX                  = 60,
    SBK_QOPENGLTEXTURE_COMPARISONFUNCTION_IDX                = 56,
    SBK_QOPENGLTEXTURE_COMPARISONMODE_IDX                    = 57,
    SBK_QOPENGLTEXTURE_FILTER_IDX                            = 62,
    SBK_QOPENGLTEXTURE_IDX                                   = 54,
    SBK_QOPENGLTEXTUREBLITTER_ORIGIN_IDX                     = 74,
    SBK_QOPENGLTEXTUREBLITTER_IDX                            = 73,
    SBK_QOPENGLTIMEMONITOR_IDX                               = 75,
    SBK_QOPENGLTIMERQUERY_IDX                                = 76,
    SBK_QOPENGLVERSIONFUNCTIONSFACTORY_IDX                   = 77,
    SBK_QOPENGLVERSIONPROFILE_IDX                            = 78,
    SBK_QOPENGLVERTEXARRAYOBJECT_IDX                         = 79,
    SBK_QOPENGLVERTEXARRAYOBJECT_BINDER_IDX                  = 80,
    SBK_QOPENGLWINDOW_UPDATEBEHAVIOR_IDX                     = 82,
    SBK_QOPENGLWINDOW_IDX                                    = 81,
    SBK_QtOpenGL_IDX_COUNT                                   = 83
};
// This variable stores all Python types exported by this module.
extern PyTypeObject **SbkPySide6_QtOpenGLTypes;

// This variable stores the Python module object exported by this module.
extern PyObject *SbkPySide6_QtOpenGLModuleObject;

// This variable stores all type converters exported by this module.
extern SbkConverter **SbkPySide6_QtOpenGLTypeConverters;

// Converter indices
enum : int {
    SBK_QTOPENGL_QPAIR_INT_INT_IDX                           = 0, // QPair<int,int >
    SBK_QTOPENGL_QPAIR_FLOAT_FLOAT_IDX                       = 1, // QPair<float,float >
    SBK_QTOPENGL_QPAIR_QOPENGLTEXTURE_FILTER_QOPENGLTEXTURE_FILTER_IDX = 2, // QPair<QOpenGLTexture::Filter,QOpenGLTexture::Filter >
    SBK_QTOPENGL_QLIST_QSIZE_IDX                             = 3, // QList<QSize >
    SBK_QTOPENGL_QLIST_UNSIGNEDINT_IDX                       = 4, // QList<unsigned int >
    SBK_QTOPENGL_QLIST_QOBJECTPTR_IDX                        = 5, // QList<QObject* >
    SBK_QTOPENGL_QLIST_QBYTEARRAY_IDX                        = 6, // QList<QByteArray >
    SBK_QTOPENGL_QLIST_UNSIGNEDLONGLONG_IDX                  = 7, // QList<unsigned long long >
    SBK_QTOPENGL_QLIST_FLOAT_IDX                             = 8, // QList<float >
    SBK_QTOPENGL_QLIST_QOPENGLSHADERPTR_IDX                  = 9, // QList<QOpenGLShader* >
    SBK_QTOPENGL_QLIST_QOPENGLDEBUGMESSAGE_IDX               = 10, // QList<QOpenGLDebugMessage >
    SBK_QTOPENGL_QLIST_QVARIANT_IDX                          = 11, // QList<QVariant >
    SBK_QTOPENGL_QLIST_QSTRING_IDX                           = 12, // QList<QString >
    SBK_QTOPENGL_QMAP_QSTRING_QVARIANT_IDX                   = 13, // QMap<QString,QVariant >
    SBK_QtOpenGL_CONVERTERS_IDX_COUNT                        = 14
};
// Macros for type check

namespace Shiboken
{

// PyType functions, to get the PyObjectType for a type T
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
template<> inline PyTypeObject *SbkType< ::QAbstractOpenGLFunctions >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QABSTRACTOPENGLFUNCTIONS_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLBuffer::Type >() { return SbkPySide6_QtOpenGLTypes[SBK_QOPENGLBUFFER_TYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLBuffer::UsagePattern >() { return SbkPySide6_QtOpenGLTypes[SBK_QOPENGLBUFFER_USAGEPATTERN_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLBuffer::Access >() { return SbkPySide6_QtOpenGLTypes[SBK_QOPENGLBUFFER_ACCESS_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLBuffer::RangeAccessFlag >() { return SbkPySide6_QtOpenGLTypes[SBK_QOPENGLBUFFER_RANGEACCESSFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QOpenGLBuffer::RangeAccessFlag> >() { return SbkPySide6_QtOpenGLTypes[SBK_QFLAGS_QOPENGLBUFFER_RANGEACCESSFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLBuffer >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLBUFFER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLDebugLogger::LoggingMode >() { return SbkPySide6_QtOpenGLTypes[SBK_QOPENGLDEBUGLOGGER_LOGGINGMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLDebugLogger >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLDEBUGLOGGER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLDebugMessage::Source >() { return SbkPySide6_QtOpenGLTypes[SBK_QOPENGLDEBUGMESSAGE_SOURCE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QOpenGLDebugMessage::Source> >() { return SbkPySide6_QtOpenGLTypes[SBK_QFLAGS_QOPENGLDEBUGMESSAGE_SOURCE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLDebugMessage::Type >() { return SbkPySide6_QtOpenGLTypes[SBK_QOPENGLDEBUGMESSAGE_TYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QOpenGLDebugMessage::Type> >() { return SbkPySide6_QtOpenGLTypes[SBK_QFLAGS_QOPENGLDEBUGMESSAGE_TYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLDebugMessage::Severity >() { return SbkPySide6_QtOpenGLTypes[SBK_QOPENGLDEBUGMESSAGE_SEVERITY_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QOpenGLDebugMessage::Severity> >() { return SbkPySide6_QtOpenGLTypes[SBK_QFLAGS_QOPENGLDEBUGMESSAGE_SEVERITY_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLDebugMessage >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLDEBUGMESSAGE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLFramebufferObject::Attachment >() { return SbkPySide6_QtOpenGLTypes[SBK_QOPENGLFRAMEBUFFEROBJECT_ATTACHMENT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLFramebufferObject::FramebufferRestorePolicy >() { return SbkPySide6_QtOpenGLTypes[SBK_QOPENGLFRAMEBUFFEROBJECT_FRAMEBUFFERRESTOREPOLICY_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLFramebufferObject >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLFRAMEBUFFEROBJECT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLFramebufferObjectFormat >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLFRAMEBUFFEROBJECTFORMAT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLFunctions_1_0 >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLFUNCTIONS_1_0_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLFunctions_1_1 >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLFUNCTIONS_1_1_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLFunctions_1_2 >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLFUNCTIONS_1_2_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLFunctions_1_3 >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLFUNCTIONS_1_3_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLFunctions_1_4 >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLFUNCTIONS_1_4_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLFunctions_1_5 >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLFUNCTIONS_1_5_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLFunctions_2_0 >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLFUNCTIONS_2_0_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLFunctions_2_1 >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLFUNCTIONS_2_1_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLFunctions_3_0 >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLFUNCTIONS_3_0_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLFunctions_3_1 >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLFUNCTIONS_3_1_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLFunctions_3_2_Compatibility >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLFUNCTIONS_3_2_COMPATIBILITY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLFunctions_3_2_Core >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLFUNCTIONS_3_2_CORE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLFunctions_3_3_Compatibility >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLFUNCTIONS_3_3_COMPATIBILITY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLFunctions_3_3_Core >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLFUNCTIONS_3_3_CORE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLFunctions_4_0_Compatibility >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLFUNCTIONS_4_0_COMPATIBILITY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLFunctions_4_0_Core >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLFUNCTIONS_4_0_CORE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLFunctions_4_1_Compatibility >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLFUNCTIONS_4_1_COMPATIBILITY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLFunctions_4_1_Core >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLFUNCTIONS_4_1_CORE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLFunctions_4_2_Compatibility >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLFUNCTIONS_4_2_COMPATIBILITY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLFunctions_4_2_Core >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLFUNCTIONS_4_2_CORE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLFunctions_4_3_Compatibility >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLFUNCTIONS_4_3_COMPATIBILITY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLFunctions_4_3_Core >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLFUNCTIONS_4_3_CORE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLFunctions_4_4_Compatibility >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLFUNCTIONS_4_4_COMPATIBILITY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLFunctions_4_4_Core >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLFUNCTIONS_4_4_CORE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLFunctions_4_5_Compatibility >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLFUNCTIONS_4_5_COMPATIBILITY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLFunctions_4_5_Core >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLFUNCTIONS_4_5_CORE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLPaintDevice >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLPAINTDEVICE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLPixelTransferOptions >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLPIXELTRANSFEROPTIONS_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLShader::ShaderTypeBit >() { return SbkPySide6_QtOpenGLTypes[SBK_QOPENGLSHADER_SHADERTYPEBIT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QOpenGLShader::ShaderTypeBit> >() { return SbkPySide6_QtOpenGLTypes[SBK_QFLAGS_QOPENGLSHADER_SHADERTYPEBIT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLShader >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLSHADER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLShaderProgram >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLSHADERPROGRAM_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLTexture::Target >() { return SbkPySide6_QtOpenGLTypes[SBK_QOPENGLTEXTURE_TARGET_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLTexture::BindingTarget >() { return SbkPySide6_QtOpenGLTypes[SBK_QOPENGLTEXTURE_BINDINGTARGET_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLTexture::MipMapGeneration >() { return SbkPySide6_QtOpenGLTypes[SBK_QOPENGLTEXTURE_MIPMAPGENERATION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLTexture::TextureUnitReset >() { return SbkPySide6_QtOpenGLTypes[SBK_QOPENGLTEXTURE_TEXTUREUNITRESET_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLTexture::TextureFormat >() { return SbkPySide6_QtOpenGLTypes[SBK_QOPENGLTEXTURE_TEXTUREFORMAT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLTexture::TextureFormatClass >() { return SbkPySide6_QtOpenGLTypes[SBK_QOPENGLTEXTURE_TEXTUREFORMATCLASS_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLTexture::CubeMapFace >() { return SbkPySide6_QtOpenGLTypes[SBK_QOPENGLTEXTURE_CUBEMAPFACE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLTexture::PixelFormat >() { return SbkPySide6_QtOpenGLTypes[SBK_QOPENGLTEXTURE_PIXELFORMAT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLTexture::PixelType >() { return SbkPySide6_QtOpenGLTypes[SBK_QOPENGLTEXTURE_PIXELTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLTexture::SwizzleComponent >() { return SbkPySide6_QtOpenGLTypes[SBK_QOPENGLTEXTURE_SWIZZLECOMPONENT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLTexture::SwizzleValue >() { return SbkPySide6_QtOpenGLTypes[SBK_QOPENGLTEXTURE_SWIZZLEVALUE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLTexture::WrapMode >() { return SbkPySide6_QtOpenGLTypes[SBK_QOPENGLTEXTURE_WRAPMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLTexture::CoordinateDirection >() { return SbkPySide6_QtOpenGLTypes[SBK_QOPENGLTEXTURE_COORDINATEDIRECTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLTexture::Feature >() { return SbkPySide6_QtOpenGLTypes[SBK_QOPENGLTEXTURE_FEATURE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QOpenGLTexture::Feature> >() { return SbkPySide6_QtOpenGLTypes[SBK_QFLAGS_QOPENGLTEXTURE_FEATURE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLTexture::DepthStencilMode >() { return SbkPySide6_QtOpenGLTypes[SBK_QOPENGLTEXTURE_DEPTHSTENCILMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLTexture::ComparisonFunction >() { return SbkPySide6_QtOpenGLTypes[SBK_QOPENGLTEXTURE_COMPARISONFUNCTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLTexture::ComparisonMode >() { return SbkPySide6_QtOpenGLTypes[SBK_QOPENGLTEXTURE_COMPARISONMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLTexture::Filter >() { return SbkPySide6_QtOpenGLTypes[SBK_QOPENGLTEXTURE_FILTER_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLTexture >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLTEXTURE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLTextureBlitter::Origin >() { return SbkPySide6_QtOpenGLTypes[SBK_QOPENGLTEXTUREBLITTER_ORIGIN_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLTextureBlitter >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLTEXTUREBLITTER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLTimeMonitor >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLTIMEMONITOR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLTimerQuery >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLTIMERQUERY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLVersionFunctionsFactory >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLVERSIONFUNCTIONSFACTORY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLVersionProfile >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLVERSIONPROFILE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLVertexArrayObject >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLVERTEXARRAYOBJECT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLVertexArrayObject::Binder >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLVERTEXARRAYOBJECT_BINDER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLWindow::UpdateBehavior >() { return SbkPySide6_QtOpenGLTypes[SBK_QOPENGLWINDOW_UPDATEBEHAVIOR_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLWindow >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtOpenGLTypes[SBK_QOPENGLWINDOW_IDX]); }
QT_WARNING_POP

} // namespace Shiboken

#endif // SBK_QTOPENGL_PYTHON_H

