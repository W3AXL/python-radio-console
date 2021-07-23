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


#ifndef SBK_QTDATAVISUALIZATION_PYTHON_H
#define SBK_QTDATAVISUALIZATION_PYTHON_H

#include <sbkpython.h>
#include <sbkconverter.h>
// Module Includes
#include <pyside6_qtgui_python.h>
#include <pyside6_qtcore_python.h>

// Bound library includes
#include <QtDataVisualization/qsurfacedataproxy.h>
#include <QtDataVisualization/qbardataitem.h>
#include <QtDataVisualization/qsurface3dseries.h>
#include <QtDataVisualization/qitemmodelbardataproxy.h>
#include <QtDataVisualization/qcustom3ditem.h>
#include <QtDataVisualization/qitemmodelscatterdataproxy.h>
#include <QtDataVisualization/q3dscatter.h>
#include <QtDataVisualization/qabstract3dseries.h>
#include <QtDataVisualization/q3dscene.h>
#include <QtDataVisualization/qcustom3dvolume.h>
#include <QtDataVisualization/qitemmodelsurfacedataproxy.h>
#include <QtDataVisualization/qscatterdataitem.h>
#include <QtDataVisualization/q3dsurface.h>
#include <QtDataVisualization/q3dcamera.h>
#include <QtDataVisualization/qbardataproxy.h>
#include <QtDataVisualization/qvalue3daxis.h>
#include <QtDataVisualization/qvalue3daxisformatter.h>
#include <QtDataVisualization/q3dinputhandler.h>
#include <QtDataVisualization/q3dlight.h>
#include <QtDataVisualization/qabstract3daxis.h>
#include <QtDataVisualization/qcustom3dlabel.h>
#include <QtDataVisualization/qtouch3dinputhandler.h>
#include <QtDataVisualization/qheightmapsurfacedataproxy.h>
#include <QtDataVisualization/qscatterdataproxy.h>
#include <QtDataVisualization/qbar3dseries.h>
#include <QtDataVisualization/qabstract3dinputhandler.h>
#include <QtDataVisualization/qabstract3dgraph.h>
#include <QtDataVisualization/qcategory3daxis.h>
#include <QtDataVisualization/q3dtheme.h>
#include <QtDataVisualization/qabstractdataproxy.h>
#include <QtDataVisualization/qscatter3dseries.h>
#include <QtDataVisualization/qsurfacedataitem.h>
#include <QtDataVisualization/q3dobject.h>
#include <QtDataVisualization/qlogvalue3daxisformatter.h>
#include <QtDataVisualization/q3dbars.h>
// Conversion Includes - Primitive Types
#include <wtypes.h>
#include <qbardataproxy.h>
#include <qabstractitemmodel.h>
#include <QString>
#include <QStringList>
#include <QStringView>
#include <qsurfacedataproxy.h>
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
    SBK_Q3DBARS_IDX                                          = 0,
    SBK_Q3DCAMERA_CAMERAPRESET_IDX                           = 2,
    SBK_Q3DCAMERA_IDX                                        = 1,
    SBK_Q3DINPUTHANDLER_IDX                                  = 3,
    SBK_Q3DLIGHT_IDX                                         = 4,
    SBK_Q3DOBJECT_IDX                                        = 5,
    SBK_Q3DSCATTER_IDX                                       = 6,
    SBK_Q3DSCENE_IDX                                         = 7,
    SBK_Q3DSURFACE_IDX                                       = 8,
    SBK_Q3DTHEME_COLORSTYLE_IDX                              = 10,
    SBK_Q3DTHEME_THEME_IDX                                   = 11,
    SBK_Q3DTHEME_IDX                                         = 9,
    SBK_QABSTRACT3DAXIS_AXISORIENTATION_IDX                  = 13,
    SBK_QABSTRACT3DAXIS_AXISTYPE_IDX                         = 14,
    SBK_QABSTRACT3DAXIS_IDX                                  = 12,
    SBK_QABSTRACT3DGRAPH_SELECTIONFLAG_IDX                   = 18,
    SBK_QFLAGS_QABSTRACT3DGRAPH_SELECTIONFLAG_IDX            = 35,
    SBK_QABSTRACT3DGRAPH_SHADOWQUALITY_IDX                   = 19,
    SBK_QABSTRACT3DGRAPH_ELEMENTTYPE_IDX                     = 16,
    SBK_QABSTRACT3DGRAPH_OPTIMIZATIONHINT_IDX                = 17,
    SBK_QFLAGS_QABSTRACT3DGRAPH_OPTIMIZATIONHINT_IDX         = 34,
    SBK_QABSTRACT3DGRAPH_IDX                                 = 15,
    SBK_QABSTRACT3DINPUTHANDLER_INPUTVIEW_IDX                = 21,
    SBK_QABSTRACT3DINPUTHANDLER_IDX                          = 20,
    SBK_QABSTRACT3DSERIES_SERIESTYPE_IDX                     = 24,
    SBK_QABSTRACT3DSERIES_MESH_IDX                           = 23,
    SBK_QABSTRACT3DSERIES_IDX                                = 22,
    SBK_QABSTRACTDATAPROXY_DATATYPE_IDX                      = 26,
    SBK_QABSTRACTDATAPROXY_IDX                               = 25,
    SBK_QBAR3DSERIES_IDX                                     = 27,
    SBK_QBARDATAITEM_IDX                                     = 28,
    SBK_QBARDATAPROXY_IDX                                    = 29,
    SBK_QCATEGORY3DAXIS_IDX                                  = 30,
    SBK_QCUSTOM3DITEM_IDX                                    = 31,
    SBK_QCUSTOM3DLABEL_IDX                                   = 32,
    SBK_QCUSTOM3DVOLUME_IDX                                  = 33,
    SBK_QHEIGHTMAPSURFACEDATAPROXY_IDX                       = 37,
    SBK_QITEMMODELBARDATAPROXY_MULTIMATCHBEHAVIOR_IDX        = 39,
    SBK_QITEMMODELBARDATAPROXY_IDX                           = 38,
    SBK_QITEMMODELSCATTERDATAPROXY_IDX                       = 40,
    SBK_QITEMMODELSURFACEDATAPROXY_MULTIMATCHBEHAVIOR_IDX    = 42,
    SBK_QITEMMODELSURFACEDATAPROXY_IDX                       = 41,
    SBK_QLOGVALUE3DAXISFORMATTER_IDX                         = 43,
    SBK_QSCATTER3DSERIES_IDX                                 = 44,
    SBK_QSCATTERDATAITEM_IDX                                 = 45,
    SBK_QSCATTERDATAPROXY_IDX                                = 46,
    SBK_QSURFACE3DSERIES_DRAWFLAG_IDX                        = 48,
    SBK_QFLAGS_QSURFACE3DSERIES_DRAWFLAG_IDX                 = 36,
    SBK_QSURFACE3DSERIES_IDX                                 = 47,
    SBK_QSURFACEDATAITEM_IDX                                 = 49,
    SBK_QSURFACEDATAPROXY_IDX                                = 50,
    SBK_QTOUCH3DINPUTHANDLER_IDX                             = 51,
    SBK_QVALUE3DAXIS_IDX                                     = 52,
    SBK_QVALUE3DAXISFORMATTER_IDX                            = 53,
    SBK_QtDataVisualization_IDX_COUNT                        = 54
};
// This variable stores all Python types exported by this module.
extern PyTypeObject **SbkPySide6_QtDataVisualizationTypes;

// This variable stores the Python module object exported by this module.
extern PyObject *SbkPySide6_QtDataVisualizationModuleObject;

// This variable stores all type converters exported by this module.
extern SbkConverter **SbkPySide6_QtDataVisualizationTypeConverters;

// Converter indices
enum : int {
    SBK_QBARDATAARRAY_IDX                                    = 0,
    SBK_QSURFACEDATAARRAY_IDX                                = 1,
    SBK_QTDATAVISUALIZATION_QLIST_QCUSTOM3DITEMPTR_IDX       = 2, // QList<QCustom3DItem* >
    SBK_QTDATAVISUALIZATION_QLIST_QABSTRACT3DINPUTHANDLERPTR_IDX = 3, // QList<QAbstract3DInputHandler* >
    SBK_QTDATAVISUALIZATION_QLIST_Q3DTHEMEPTR_IDX            = 4, // QList<Q3DTheme* >
    SBK_QTDATAVISUALIZATION_QLIST_QVALUE3DAXISPTR_IDX        = 5, // QList<QValue3DAxis* >
    SBK_QTDATAVISUALIZATION_QLIST_QSURFACE3DSERIESPTR_IDX    = 6, // QList<QSurface3DSeries* >
    SBK_QTDATAVISUALIZATION_QLIST_QSCATTER3DSERIESPTR_IDX    = 7, // QList<QScatter3DSeries* >
    SBK_QTDATAVISUALIZATION_QLIST_QABSTRACT3DAXISPTR_IDX     = 8, // QList<QAbstract3DAxis* >
    SBK_QTDATAVISUALIZATION_QLIST_QBAR3DSERIESPTR_IDX        = 9, // QList<QBar3DSeries* >
    SBK_QTDATAVISUALIZATION_QLIST_QOBJECTPTR_IDX             = 10, // QList<QObject* >
    SBK_QTDATAVISUALIZATION_QLIST_QBYTEARRAY_IDX             = 11, // QList<QByteArray >
    SBK_QTDATAVISUALIZATION_QLIST_FLOAT_IDX                  = 12, // QList<float >
    SBK_QTDATAVISUALIZATION_QLIST_UCHAR_IDX                  = 13, // QList<uchar >
    SBK_QTDATAVISUALIZATION_QLIST_UNSIGNEDINT_IDX            = 14, // QList<unsigned int >
    SBK_QTDATAVISUALIZATION_QLIST_QIMAGEPTR_IDX              = 15, // QList<QImage* >
    SBK_QTDATAVISUALIZATION_QLIST_QSURFACEDATAITEM_IDX       = 16, // QList<QSurfaceDataItem >
    SBK_QTDATAVISUALIZATION_QLIST_QSCATTERDATAITEM_IDX       = 17, // QList<QScatterDataItem >
    SBK_QTDATAVISUALIZATION_QLIST_QBARDATAITEM_IDX           = 18, // QList<QBarDataItem >
    SBK_QTDATAVISUALIZATION_QLIST_QCOLOR_IDX                 = 19, // QList<QColor >
    SBK_QTDATAVISUALIZATION_QLIST_QLINEARGRADIENT_IDX        = 20, // QList<QLinearGradient >
    SBK_QTDATAVISUALIZATION_QLIST_QVARIANT_IDX               = 21, // QList<QVariant >
    SBK_QTDATAVISUALIZATION_QLIST_QSTRING_IDX                = 22, // QList<QString >
    SBK_QTDATAVISUALIZATION_QMAP_QSTRING_QVARIANT_IDX        = 23, // QMap<QString,QVariant >
    SBK_QtDataVisualization_CONVERTERS_IDX_COUNT             = 24
};
// Macros for type check

namespace Shiboken
{

// PyType functions, to get the PyObjectType for a type T
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
template<> inline PyTypeObject *SbkType< ::Q3DBars >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_Q3DBARS_IDX]); }
template<> inline PyTypeObject *SbkType< ::Q3DCamera::CameraPreset >() { return SbkPySide6_QtDataVisualizationTypes[SBK_Q3DCAMERA_CAMERAPRESET_IDX]; }
template<> inline PyTypeObject *SbkType< ::Q3DCamera >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_Q3DCAMERA_IDX]); }
template<> inline PyTypeObject *SbkType< ::Q3DInputHandler >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_Q3DINPUTHANDLER_IDX]); }
template<> inline PyTypeObject *SbkType< ::Q3DLight >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_Q3DLIGHT_IDX]); }
template<> inline PyTypeObject *SbkType< ::Q3DObject >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_Q3DOBJECT_IDX]); }
template<> inline PyTypeObject *SbkType< ::Q3DScatter >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_Q3DSCATTER_IDX]); }
template<> inline PyTypeObject *SbkType< ::Q3DScene >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_Q3DSCENE_IDX]); }
template<> inline PyTypeObject *SbkType< ::Q3DSurface >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_Q3DSURFACE_IDX]); }
template<> inline PyTypeObject *SbkType< ::Q3DTheme::ColorStyle >() { return SbkPySide6_QtDataVisualizationTypes[SBK_Q3DTHEME_COLORSTYLE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Q3DTheme::Theme >() { return SbkPySide6_QtDataVisualizationTypes[SBK_Q3DTHEME_THEME_IDX]; }
template<> inline PyTypeObject *SbkType< ::Q3DTheme >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_Q3DTHEME_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAbstract3DAxis::AxisOrientation >() { return SbkPySide6_QtDataVisualizationTypes[SBK_QABSTRACT3DAXIS_AXISORIENTATION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAbstract3DAxis::AxisType >() { return SbkPySide6_QtDataVisualizationTypes[SBK_QABSTRACT3DAXIS_AXISTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAbstract3DAxis >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_QABSTRACT3DAXIS_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAbstract3DGraph::SelectionFlag >() { return SbkPySide6_QtDataVisualizationTypes[SBK_QABSTRACT3DGRAPH_SELECTIONFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QAbstract3DGraph::SelectionFlag> >() { return SbkPySide6_QtDataVisualizationTypes[SBK_QFLAGS_QABSTRACT3DGRAPH_SELECTIONFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAbstract3DGraph::ShadowQuality >() { return SbkPySide6_QtDataVisualizationTypes[SBK_QABSTRACT3DGRAPH_SHADOWQUALITY_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAbstract3DGraph::ElementType >() { return SbkPySide6_QtDataVisualizationTypes[SBK_QABSTRACT3DGRAPH_ELEMENTTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAbstract3DGraph::OptimizationHint >() { return SbkPySide6_QtDataVisualizationTypes[SBK_QABSTRACT3DGRAPH_OPTIMIZATIONHINT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QAbstract3DGraph::OptimizationHint> >() { return SbkPySide6_QtDataVisualizationTypes[SBK_QFLAGS_QABSTRACT3DGRAPH_OPTIMIZATIONHINT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAbstract3DGraph >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_QABSTRACT3DGRAPH_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAbstract3DInputHandler::InputView >() { return SbkPySide6_QtDataVisualizationTypes[SBK_QABSTRACT3DINPUTHANDLER_INPUTVIEW_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAbstract3DInputHandler >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_QABSTRACT3DINPUTHANDLER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAbstract3DSeries::SeriesType >() { return SbkPySide6_QtDataVisualizationTypes[SBK_QABSTRACT3DSERIES_SERIESTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAbstract3DSeries::Mesh >() { return SbkPySide6_QtDataVisualizationTypes[SBK_QABSTRACT3DSERIES_MESH_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAbstract3DSeries >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_QABSTRACT3DSERIES_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAbstractDataProxy::DataType >() { return SbkPySide6_QtDataVisualizationTypes[SBK_QABSTRACTDATAPROXY_DATATYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAbstractDataProxy >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_QABSTRACTDATAPROXY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QBar3DSeries >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_QBAR3DSERIES_IDX]); }
template<> inline PyTypeObject *SbkType< ::QBarDataItem >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_QBARDATAITEM_IDX]); }
template<> inline PyTypeObject *SbkType< ::QBarDataProxy >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_QBARDATAPROXY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QCategory3DAxis >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_QCATEGORY3DAXIS_IDX]); }
template<> inline PyTypeObject *SbkType< ::QCustom3DItem >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_QCUSTOM3DITEM_IDX]); }
template<> inline PyTypeObject *SbkType< ::QCustom3DLabel >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_QCUSTOM3DLABEL_IDX]); }
template<> inline PyTypeObject *SbkType< ::QCustom3DVolume >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_QCUSTOM3DVOLUME_IDX]); }
template<> inline PyTypeObject *SbkType< ::QHeightMapSurfaceDataProxy >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_QHEIGHTMAPSURFACEDATAPROXY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QItemModelBarDataProxy::MultiMatchBehavior >() { return SbkPySide6_QtDataVisualizationTypes[SBK_QITEMMODELBARDATAPROXY_MULTIMATCHBEHAVIOR_IDX]; }
template<> inline PyTypeObject *SbkType< ::QItemModelBarDataProxy >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_QITEMMODELBARDATAPROXY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QItemModelScatterDataProxy >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_QITEMMODELSCATTERDATAPROXY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QItemModelSurfaceDataProxy::MultiMatchBehavior >() { return SbkPySide6_QtDataVisualizationTypes[SBK_QITEMMODELSURFACEDATAPROXY_MULTIMATCHBEHAVIOR_IDX]; }
template<> inline PyTypeObject *SbkType< ::QItemModelSurfaceDataProxy >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_QITEMMODELSURFACEDATAPROXY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QLogValue3DAxisFormatter >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_QLOGVALUE3DAXISFORMATTER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QScatter3DSeries >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_QSCATTER3DSERIES_IDX]); }
template<> inline PyTypeObject *SbkType< ::QScatterDataItem >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_QSCATTERDATAITEM_IDX]); }
template<> inline PyTypeObject *SbkType< ::QScatterDataProxy >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_QSCATTERDATAPROXY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QSurface3DSeries::DrawFlag >() { return SbkPySide6_QtDataVisualizationTypes[SBK_QSURFACE3DSERIES_DRAWFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QSurface3DSeries::DrawFlag> >() { return SbkPySide6_QtDataVisualizationTypes[SBK_QFLAGS_QSURFACE3DSERIES_DRAWFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QSurface3DSeries >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_QSURFACE3DSERIES_IDX]); }
template<> inline PyTypeObject *SbkType< ::QSurfaceDataItem >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_QSURFACEDATAITEM_IDX]); }
template<> inline PyTypeObject *SbkType< ::QSurfaceDataProxy >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_QSURFACEDATAPROXY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTouch3DInputHandler >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_QTOUCH3DINPUTHANDLER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QValue3DAxis >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_QVALUE3DAXIS_IDX]); }
template<> inline PyTypeObject *SbkType< ::QValue3DAxisFormatter >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtDataVisualizationTypes[SBK_QVALUE3DAXISFORMATTER_IDX]); }
QT_WARNING_POP

} // namespace Shiboken

#endif // SBK_QTDATAVISUALIZATION_PYTHON_H

