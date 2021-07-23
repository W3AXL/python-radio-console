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


#ifndef SBK_QTCHARTS_PYTHON_H
#define SBK_QTCHARTS_PYTHON_H

#include <sbkpython.h>
#include <sbkconverter.h>
// Module Includes
#include <pyside6_qtwidgets_python.h>
#include <pyside6_qtgui_python.h>
#include <pyside6_qtcore_python.h>

// Bound library includes
#include <QtCharts/qhbarmodelmapper.h>
#include <QtCharts/qvcandlestickmodelmapper.h>
#include <QtCharts/qvxymodelmapper.h>
#include <QtCharts/qboxplotmodelmapper.h>
#include <QtCharts/qhorizontalstackedbarseries.h>
#include <QtCharts/qhorizontalbarseries.h>
#include <QtCharts/qvalueaxis.h>
#include <QtCharts/qabstractbarseries.h>
#include <QtCharts/qpolarchart.h>
#include <QtCharts/qbarlegendmarker.h>
#include <QtCharts/qcandlestickmodelmapper.h>
#include <QtCharts/qpieslice.h>
#include <QtCharts/qsplineseries.h>
#include <QtCharts/qxylegendmarker.h>
#include <pyside_numpy.h>
#include <QtCharts/qchart.h>
#include <QtCharts/qpieseries.h>
#include <QtCharts/qhpiemodelmapper.h>
#include <QtCharts/qlogvalueaxis.h>
#include <QtCharts/qvbarmodelmapper.h>
#include <QtCharts/qpielegendmarker.h>
#include <QtCharts/qareaseries.h>
#include <QtCharts/qvpiemodelmapper.h>
#include <QtCharts/qboxset.h>
#include <QtCharts/qchartview.h>
#include <QtCharts/qvboxplotmodelmapper.h>
#include <QtCharts/qbarseries.h>
#include <QtCharts/qdatetimeaxis.h>
#include <QtCharts/qabstractseries.h>
#include <QtCharts/qpercentbarseries.h>
#include <QtCharts/qstackedbarseries.h>
#include <QtCharts/qcandlestickseries.h>
#include <QtCharts/qcandlestickset.h>
#include <QtCharts/qcandlesticklegendmarker.h>
#include <QtCharts/qlegend.h>
#include <QtCharts/qabstractaxis.h>
#include <QtCharts/qbarset.h>
#include <QtCharts/qboxplotlegendmarker.h>
#include <QtCharts/qarealegendmarker.h>
#include <QtCharts/qbarcategoryaxis.h>
#include <QtCharts/qboxplotseries.h>
#include <QtCharts/qbarmodelmapper.h>
#include <QtCharts/qscatterseries.h>
#include <QtCharts/qhboxplotmodelmapper.h>
#include <QtCharts/qlegendmarker.h>
#include <QtCharts/qpiemodelmapper.h>
#include <QtCharts/qxymodelmapper.h>
#include <QtCharts/qcategoryaxis.h>
#include <QtCharts/qhxymodelmapper.h>
#include <QtCharts/qhcandlestickmodelmapper.h>
#include <QtCharts/qhorizontalpercentbarseries.h>
#include <QtCharts/qlineseries.h>
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
    SBK_QABSTRACTAXIS_AXISTYPE_IDX                           = 1,
    SBK_QABSTRACTAXIS_IDX                                    = 0,
    SBK_QABSTRACTBARSERIES_LABELSPOSITION_IDX                = 3,
    SBK_QABSTRACTBARSERIES_IDX                               = 2,
    SBK_QABSTRACTSERIES_SERIESTYPE_IDX                       = 5,
    SBK_QABSTRACTSERIES_IDX                                  = 4,
    SBK_QAREALEGENDMARKER_IDX                                = 6,
    SBK_QAREASERIES_IDX                                      = 7,
    SBK_QBARCATEGORYAXIS_IDX                                 = 8,
    SBK_QBARLEGENDMARKER_IDX                                 = 9,
    SBK_QBARMODELMAPPER_IDX                                  = 10,
    SBK_QBARSERIES_IDX                                       = 11,
    SBK_QBARSET_IDX                                          = 12,
    SBK_QBOXPLOTLEGENDMARKER_IDX                             = 13,
    SBK_QBOXPLOTMODELMAPPER_IDX                              = 14,
    SBK_QBOXPLOTSERIES_IDX                                   = 15,
    SBK_QBOXSET_VALUEPOSITIONS_IDX                           = 17,
    SBK_QBOXSET_IDX                                          = 16,
    SBK_QCANDLESTICKLEGENDMARKER_IDX                         = 18,
    SBK_QCANDLESTICKMODELMAPPER_IDX                          = 19,
    SBK_QCANDLESTICKSERIES_IDX                               = 20,
    SBK_QCANDLESTICKSET_IDX                                  = 21,
    SBK_QCATEGORYAXIS_AXISLABELSPOSITION_IDX                 = 23,
    SBK_QCATEGORYAXIS_IDX                                    = 22,
    SBK_QCHART_CHARTTYPE_IDX                                 = 27,
    SBK_QCHART_CHARTTHEME_IDX                                = 26,
    SBK_QCHART_ANIMATIONOPTION_IDX                           = 25,
    SBK_QFLAGS_QCHART_ANIMATIONOPTION_IDX                    = 31,
    SBK_QCHART_IDX                                           = 24,
    SBK_QCHARTVIEW_RUBBERBAND_IDX                            = 29,
    SBK_QFLAGS_QCHARTVIEW_RUBBERBAND_IDX                     = 32,
    SBK_QCHARTVIEW_IDX                                       = 28,
    SBK_QDATETIMEAXIS_IDX                                    = 30,
    SBK_QHBARMODELMAPPER_IDX                                 = 34,
    SBK_QHBOXPLOTMODELMAPPER_IDX                             = 35,
    SBK_QHCANDLESTICKMODELMAPPER_IDX                         = 36,
    SBK_QHPIEMODELMAPPER_IDX                                 = 37,
    SBK_QHXYMODELMAPPER_IDX                                  = 38,
    SBK_QHORIZONTALBARSERIES_IDX                             = 39,
    SBK_QHORIZONTALPERCENTBARSERIES_IDX                      = 40,
    SBK_QHORIZONTALSTACKEDBARSERIES_IDX                      = 41,
    SBK_QLEGEND_MARKERSHAPE_IDX                              = 43,
    SBK_QLEGEND_IDX                                          = 42,
    SBK_QLEGENDMARKER_LEGENDMARKERTYPE_IDX                   = 45,
    SBK_QLEGENDMARKER_IDX                                    = 44,
    SBK_QLINESERIES_IDX                                      = 46,
    SBK_QLOGVALUEAXIS_IDX                                    = 47,
    SBK_QPERCENTBARSERIES_IDX                                = 48,
    SBK_QPIELEGENDMARKER_IDX                                 = 49,
    SBK_QPIEMODELMAPPER_IDX                                  = 50,
    SBK_QPIESERIES_IDX                                       = 51,
    SBK_QPIESLICE_LABELPOSITION_IDX                          = 53,
    SBK_QPIESLICE_IDX                                        = 52,
    SBK_QPOLARCHART_POLARORIENTATION_IDX                     = 55,
    SBK_QFLAGS_QPOLARCHART_POLARORIENTATION_IDX              = 33,
    SBK_QPOLARCHART_IDX                                      = 54,
    SBK_QSCATTERSERIES_MARKERSHAPE_IDX                       = 57,
    SBK_QSCATTERSERIES_IDX                                   = 56,
    SBK_QSPLINESERIES_IDX                                    = 58,
    SBK_QSTACKEDBARSERIES_IDX                                = 59,
    SBK_QVBARMODELMAPPER_IDX                                 = 60,
    SBK_QVBOXPLOTMODELMAPPER_IDX                             = 61,
    SBK_QVCANDLESTICKMODELMAPPER_IDX                         = 62,
    SBK_QVPIEMODELMAPPER_IDX                                 = 63,
    SBK_QVXYMODELMAPPER_IDX                                  = 64,
    SBK_QVALUEAXIS_TICKTYPE_IDX                              = 66,
    SBK_QVALUEAXIS_IDX                                       = 65,
    SBK_QXYLEGENDMARKER_IDX                                  = 67,
    SBK_QXYMODELMAPPER_IDX                                   = 68,
    SBK_QXYSERIES_IDX                                        = 69,
    SBK_QtCharts_IDX_COUNT                                   = 70
};
// This variable stores all Python types exported by this module.
extern PyTypeObject **SbkPySide6_QtChartsTypes;

// This variable stores the Python module object exported by this module.
extern PyObject *SbkPySide6_QtChartsModuleObject;

// This variable stores all type converters exported by this module.
extern SbkConverter **SbkPySide6_QtChartsTypeConverters;

// Converter indices
enum : int {
    SBK_QTCHARTS_QLIST_QOBJECTPTR_IDX                        = 0, // QList<QObject* >
    SBK_QTCHARTS_QLIST_QBYTEARRAY_IDX                        = 1, // QList<QByteArray >
    SBK_QTCHARTS_QLIST_QACTIONPTR_IDX                        = 2, // QList<QAction* >
    SBK_QTCHARTS_QLIST_QLEGENDMARKERPTR_IDX                  = 3, // QList<QLegendMarker* >
    SBK_QTCHARTS_QLIST_QGRAPHICSITEMPTR_IDX                  = 4, // QList<QGraphicsItem* >
    SBK_QTCHARTS_QLIST_QRECTF_IDX                            = 5, // QList<QRectF >
    SBK_QTCHARTS_QLIST_QABSTRACTAXISPTR_IDX                  = 6, // QList<QAbstractAxis* >
    SBK_QTCHARTS_QLIST_QABSTRACTSERIESPTR_IDX                = 7, // QList<QAbstractSeries* >
    SBK_QTCHARTS_QLIST_QREAL_IDX                             = 8, // QList<qreal >
    SBK_QTCHARTS_QLIST_QPOINTF_IDX                           = 9, // QList<QPointF >
    SBK_QTCHARTS_QLIST_QPIESLICEPTR_IDX                      = 10, // QList<QPieSlice* >
    SBK_QTCHARTS_QLIST_QCANDLESTICKSETPTR_IDX                = 11, // QList<QCandlestickSet* >
    SBK_QTCHARTS_QLIST_QBOXSETPTR_IDX                        = 12, // QList<QBoxSet* >
    SBK_QTCHARTS_QLIST_QBARSETPTR_IDX                        = 13, // QList<QBarSet* >
    SBK_QTCHARTS_QLIST_QVARIANT_IDX                          = 14, // QList<QVariant >
    SBK_QTCHARTS_QLIST_QSTRING_IDX                           = 15, // QList<QString >
    SBK_QTCHARTS_QMAP_QSTRING_QVARIANT_IDX                   = 16, // QMap<QString,QVariant >
    SBK_QtCharts_CONVERTERS_IDX_COUNT                        = 17
};
// Macros for type check

namespace Shiboken
{

// PyType functions, to get the PyObjectType for a type T
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
template<> inline PyTypeObject *SbkType< ::QAbstractAxis::AxisType >() { return SbkPySide6_QtChartsTypes[SBK_QABSTRACTAXIS_AXISTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAbstractAxis >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QABSTRACTAXIS_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAbstractBarSeries::LabelsPosition >() { return SbkPySide6_QtChartsTypes[SBK_QABSTRACTBARSERIES_LABELSPOSITION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAbstractBarSeries >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QABSTRACTBARSERIES_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAbstractSeries::SeriesType >() { return SbkPySide6_QtChartsTypes[SBK_QABSTRACTSERIES_SERIESTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAbstractSeries >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QABSTRACTSERIES_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAreaLegendMarker >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QAREALEGENDMARKER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAreaSeries >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QAREASERIES_IDX]); }
template<> inline PyTypeObject *SbkType< ::QBarCategoryAxis >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QBARCATEGORYAXIS_IDX]); }
template<> inline PyTypeObject *SbkType< ::QBarLegendMarker >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QBARLEGENDMARKER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QBarModelMapper >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QBARMODELMAPPER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QBarSeries >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QBARSERIES_IDX]); }
template<> inline PyTypeObject *SbkType< ::QBarSet >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QBARSET_IDX]); }
template<> inline PyTypeObject *SbkType< ::QBoxPlotLegendMarker >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QBOXPLOTLEGENDMARKER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QBoxPlotModelMapper >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QBOXPLOTMODELMAPPER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QBoxPlotSeries >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QBOXPLOTSERIES_IDX]); }
template<> inline PyTypeObject *SbkType< ::QBoxSet::ValuePositions >() { return SbkPySide6_QtChartsTypes[SBK_QBOXSET_VALUEPOSITIONS_IDX]; }
template<> inline PyTypeObject *SbkType< ::QBoxSet >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QBOXSET_IDX]); }
template<> inline PyTypeObject *SbkType< ::QCandlestickLegendMarker >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QCANDLESTICKLEGENDMARKER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QCandlestickModelMapper >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QCANDLESTICKMODELMAPPER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QCandlestickSeries >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QCANDLESTICKSERIES_IDX]); }
template<> inline PyTypeObject *SbkType< ::QCandlestickSet >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QCANDLESTICKSET_IDX]); }
template<> inline PyTypeObject *SbkType< ::QCategoryAxis::AxisLabelsPosition >() { return SbkPySide6_QtChartsTypes[SBK_QCATEGORYAXIS_AXISLABELSPOSITION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QCategoryAxis >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QCATEGORYAXIS_IDX]); }
template<> inline PyTypeObject *SbkType< ::QChart::ChartType >() { return SbkPySide6_QtChartsTypes[SBK_QCHART_CHARTTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QChart::ChartTheme >() { return SbkPySide6_QtChartsTypes[SBK_QCHART_CHARTTHEME_IDX]; }
template<> inline PyTypeObject *SbkType< ::QChart::AnimationOption >() { return SbkPySide6_QtChartsTypes[SBK_QCHART_ANIMATIONOPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QChart::AnimationOption> >() { return SbkPySide6_QtChartsTypes[SBK_QFLAGS_QCHART_ANIMATIONOPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QChart >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QCHART_IDX]); }
template<> inline PyTypeObject *SbkType< ::QChartView::RubberBand >() { return SbkPySide6_QtChartsTypes[SBK_QCHARTVIEW_RUBBERBAND_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QChartView::RubberBand> >() { return SbkPySide6_QtChartsTypes[SBK_QFLAGS_QCHARTVIEW_RUBBERBAND_IDX]; }
template<> inline PyTypeObject *SbkType< ::QChartView >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QCHARTVIEW_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDateTimeAxis >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QDATETIMEAXIS_IDX]); }
template<> inline PyTypeObject *SbkType< ::QHBarModelMapper >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QHBARMODELMAPPER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QHBoxPlotModelMapper >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QHBOXPLOTMODELMAPPER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QHCandlestickModelMapper >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QHCANDLESTICKMODELMAPPER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QHPieModelMapper >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QHPIEMODELMAPPER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QHXYModelMapper >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QHXYMODELMAPPER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QHorizontalBarSeries >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QHORIZONTALBARSERIES_IDX]); }
template<> inline PyTypeObject *SbkType< ::QHorizontalPercentBarSeries >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QHORIZONTALPERCENTBARSERIES_IDX]); }
template<> inline PyTypeObject *SbkType< ::QHorizontalStackedBarSeries >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QHORIZONTALSTACKEDBARSERIES_IDX]); }
template<> inline PyTypeObject *SbkType< ::QLegend::MarkerShape >() { return SbkPySide6_QtChartsTypes[SBK_QLEGEND_MARKERSHAPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QLegend >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QLEGEND_IDX]); }
template<> inline PyTypeObject *SbkType< ::QLegendMarker::LegendMarkerType >() { return SbkPySide6_QtChartsTypes[SBK_QLEGENDMARKER_LEGENDMARKERTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QLegendMarker >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QLEGENDMARKER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QLineSeries >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QLINESERIES_IDX]); }
template<> inline PyTypeObject *SbkType< ::QLogValueAxis >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QLOGVALUEAXIS_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPercentBarSeries >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QPERCENTBARSERIES_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPieLegendMarker >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QPIELEGENDMARKER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPieModelMapper >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QPIEMODELMAPPER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPieSeries >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QPIESERIES_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPieSlice::LabelPosition >() { return SbkPySide6_QtChartsTypes[SBK_QPIESLICE_LABELPOSITION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QPieSlice >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QPIESLICE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPolarChart::PolarOrientation >() { return SbkPySide6_QtChartsTypes[SBK_QPOLARCHART_POLARORIENTATION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QPolarChart::PolarOrientation> >() { return SbkPySide6_QtChartsTypes[SBK_QFLAGS_QPOLARCHART_POLARORIENTATION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QPolarChart >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QPOLARCHART_IDX]); }
template<> inline PyTypeObject *SbkType< ::QScatterSeries::MarkerShape >() { return SbkPySide6_QtChartsTypes[SBK_QSCATTERSERIES_MARKERSHAPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QScatterSeries >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QSCATTERSERIES_IDX]); }
template<> inline PyTypeObject *SbkType< ::QSplineSeries >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QSPLINESERIES_IDX]); }
template<> inline PyTypeObject *SbkType< ::QStackedBarSeries >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QSTACKEDBARSERIES_IDX]); }
template<> inline PyTypeObject *SbkType< ::QVBarModelMapper >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QVBARMODELMAPPER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QVBoxPlotModelMapper >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QVBOXPLOTMODELMAPPER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QVCandlestickModelMapper >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QVCANDLESTICKMODELMAPPER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QVPieModelMapper >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QVPIEMODELMAPPER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QVXYModelMapper >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QVXYMODELMAPPER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QValueAxis::TickType >() { return SbkPySide6_QtChartsTypes[SBK_QVALUEAXIS_TICKTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QValueAxis >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QVALUEAXIS_IDX]); }
template<> inline PyTypeObject *SbkType< ::QXYLegendMarker >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QXYLEGENDMARKER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QXYModelMapper >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QXYMODELMAPPER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QXYSeries >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtChartsTypes[SBK_QXYSERIES_IDX]); }
QT_WARNING_POP

} // namespace Shiboken

#endif // SBK_QTCHARTS_PYTHON_H

