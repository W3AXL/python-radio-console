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


#ifndef SBK_QTGUI_PYTHON_H
#define SBK_QTGUI_PYTHON_H

#include <sbkpython.h>
#include <sbkconverter.h>
// Module Includes
#include <pyside6_qtcore_python.h>

// Bound library includes
#include <QtGui/qopenglfunctions.h>
#include <QtGui/qevent.h>
#include <QtGui/qmovie.h>
#include <QtGui/qstatictext.h>
#include <QtGui/qvalidator.h>
#include <QtGui/qpaintengine.h>
#include <QtGui/qeventpoint.h>
#include <QtGui/qdesktopservices.h>
#include <QtGui/qpixmap.h>
#include <QtGui/qbackingstore.h>
#include <QtGui/qcolor.h>
#include <QtGui/qpen.h>
#include <QtGui/qtexttable.h>
#include <QtGui/qcolortransform.h>
#include <QtGui/qimageiohandler.h>
#include <QtGui/qinputmethod.h>
#include <QtGui/qimagewriter.h>
#include <QtGui/qtransform.h>
#include <QtGui/qstandarditemmodel.h>
#include <QPainterPath>
#include <QtGui/qtextformat.h>
#include <QtGui/qbitmap.h>
#include <QtGui/qimage.h>
#include <QtGui/qtextdocumentwriter.h>
#include <QtGui/qvectornd.h>
#include <QtGui/qpixelformat.h>
#include <QtGui/qstylehints.h>
#include <QtGui/qundostack.h>
#include <QtGui/qmatrix4x4.h>
#include <QtGui/qpaintdevicewindow.h>
#include <QtGui/qinputdevice.h>
#include <QtGui/qgenericmatrix.h>
#include <QtGui/qaction.h>
#include <QPainter>
#include <QtGui/qpainter.h>
#include <QtGui/qrasterwindow.h>
#include <QtGui/qsessionmanager.h>
#include <QtGui/qrawfont.h>
#include <QTextLayout>
#include <QtGui/qkeysequence.h>
#include <QtGui/qpagelayout.h>
#include <QtGui/qabstracttextdocumentlayout.h>
#include <QtGui/qcolorspace.h>
#include <QtGui/qpicture.h>
#include <QtGui/qquaternion.h>
#include <QtGui/qtextdocumentfragment.h>
#include <QtGui/qsyntaxhighlighter.h>
#include <QtGui/qsurface.h>
#include <QtGui/qopenglextrafunctions.h>
#include <qpytextobject.h>
#include <QtGui/qtextdocument.h>
#include <QtGui/qtextlayout.h>
#include <QtGui/qfontmetrics.h>
#include <QtGui/qpainterpath.h>
#include <QtGui/qpointingdevice.h>
#include <QtGui/qdrag.h>
#include <QtGui/qwindow.h>
#include <QtGui/qpixmapcache.h>
#include <QTextFrame>
#include <QtGui/qoffscreensurface.h>
#include <QtGui/qsurfaceformat.h>
#include <QtGui/qopenglcontext.h>
#include <QtGui/qpalette.h>
#include <QtGui/qtextcursor.h>
#include <QtGui/qactiongroup.h>
#include <QtGui/qpolygon.h>
#include <QtGui/qregion.h>
#include <QtGui/qpdfwriter.h>
#include <QtGui/qfontdatabase.h>
#include <QtGui/qscreen.h>
#include <QtGui/qundogroup.h>
#include <QtGui/qpageranges.h>
#include <QtGui/qcursor.h>
#include <QtGui/qpaintdevice.h>
#include <QtGui/qfontinfo.h>
#include <QtGui/qiconengine.h>
#include <QTextBlock>
#include <QtGui/qaccessible.h>
#include <QtGui/qfont.h>
#include <QtGui/qbrush.h>
#include <QtGui/qguiapplication.h>
#include <QtGui/qtextoption.h>
#include <QtGui/qtextlist.h>
#include <QtGui/qpagedpaintdevice.h>
#include <QtGui/qshortcut.h>
#include <QtGui/qtextobject.h>
#include <QtGui/qglyphrun.h>
#include <QtGui/qpagesize.h>
#include <QAbstractTextDocumentLayout>
#include <QtGui/qclipboard.h>
#include <QInputMethodEvent>
#include <QtGui/qicon.h>
#include <QtGui/qabstractfileiconprovider.h>
#include <QtGui/qimagereader.h>
#include <QtGui/qaccessibleobject.h>
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
    SBK_QABSTRACTFILEICONPROVIDER_ICONTYPE_IDX               = 1,
    SBK_QABSTRACTFILEICONPROVIDER_OPTION_IDX                 = 2,
    SBK_QFLAGS_QABSTRACTFILEICONPROVIDER_OPTION_IDX          = 70,
    SBK_QABSTRACTFILEICONPROVIDER_IDX                        = 0,
    SBK_QABSTRACTTEXTDOCUMENTLAYOUT_IDX                      = 3,
    SBK_QABSTRACTTEXTDOCUMENTLAYOUT_PAINTCONTEXT_IDX         = 4,
    SBK_QABSTRACTTEXTDOCUMENTLAYOUT_SELECTION_IDX            = 5,
    SBK_QACCESSIBLE_EVENT_IDX                                = 7,
    SBK_QACCESSIBLE_ROLE_IDX                                 = 10,
    SBK_QACCESSIBLE_TEXT_IDX                                 = 12,
    SBK_QACCESSIBLE_RELATIONFLAG_IDX                         = 9,
    SBK_QFLAGS_QACCESSIBLE_RELATIONFLAG_IDX                  = 71,
    SBK_QACCESSIBLE_INTERFACETYPE_IDX                        = 8,
    SBK_QACCESSIBLE_TEXTBOUNDARYTYPE_IDX                     = 13,
    SBK_QACCESSIBLE_IDX                                      = 6,
    SBK_QACCESSIBLE_STATE_IDX                                = 11,
    SBK_QACCESSIBLEEDITABLETEXTINTERFACE_IDX                 = 14,
    SBK_QACCESSIBLEEVENT_IDX                                 = 15,
    SBK_QACCESSIBLEINTERFACE_IDX                             = 16,
    SBK_QACCESSIBLEOBJECT_IDX                                = 17,
    SBK_QACCESSIBLESTATECHANGEEVENT_IDX                      = 18,
    SBK_QACCESSIBLETABLECELLINTERFACE_IDX                    = 19,
    SBK_QACCESSIBLETABLEMODELCHANGEEVENT_MODELCHANGETYPE_IDX = 21,
    SBK_QACCESSIBLETABLEMODELCHANGEEVENT_IDX                 = 20,
    SBK_QACCESSIBLETEXTCURSOREVENT_IDX                       = 22,
    SBK_QACCESSIBLETEXTINSERTEVENT_IDX                       = 23,
    SBK_QACCESSIBLETEXTINTERFACE_IDX                         = 24,
    SBK_QACCESSIBLETEXTREMOVEEVENT_IDX                       = 25,
    SBK_QACCESSIBLETEXTSELECTIONEVENT_IDX                    = 26,
    SBK_QACCESSIBLETEXTUPDATEEVENT_IDX                       = 27,
    SBK_QACCESSIBLEVALUECHANGEEVENT_IDX                      = 28,
    SBK_QACCESSIBLEVALUEINTERFACE_IDX                        = 29,
    SBK_QACTION_MENUROLE_IDX                                 = 32,
    SBK_QACTION_PRIORITY_IDX                                 = 33,
    SBK_QACTION_ACTIONEVENT_IDX                              = 31,
    SBK_QACTION_IDX                                          = 30,
    SBK_QACTIONEVENT_IDX                                     = 34,
    SBK_QACTIONGROUP_EXCLUSIONPOLICY_IDX                     = 36,
    SBK_QACTIONGROUP_IDX                                     = 35,
    SBK_QBACKINGSTORE_IDX                                    = 37,
    SBK_QBITMAP_IDX                                          = 38,
    SBK_QBRUSH_IDX                                           = 39,
    SBK_QCLIPBOARD_MODE_IDX                                  = 41,
    SBK_QCLIPBOARD_IDX                                       = 40,
    SBK_QCLOSEEVENT_IDX                                      = 42,
    SBK_QCOLOR_SPEC_IDX                                      = 45,
    SBK_QCOLOR_NAMEFORMAT_IDX                                = 44,
    SBK_QCOLOR_IDX                                           = 43,
    SBK_QtGuiQCOLORCONSTANTS_IDX                             = 46,
    SBK_QtGuiQCOLORCONSTANTS_SVG_IDX                         = 47,
    SBK_QCOLORSPACE_NAMEDCOLORSPACE_IDX                      = 49,
    SBK_QCOLORSPACE_PRIMARIES_IDX                            = 50,
    SBK_QCOLORSPACE_TRANSFERFUNCTION_IDX                     = 51,
    SBK_QCOLORSPACE_IDX                                      = 48,
    SBK_QCOLORTRANSFORM_IDX                                  = 52,
    SBK_QCONICALGRADIENT_IDX                                 = 53,
    SBK_QCONTEXTMENUEVENT_REASON_IDX                         = 55,
    SBK_QCONTEXTMENUEVENT_IDX                                = 54,
    SBK_QCURSOR_IDX                                          = 56,
    SBK_QDESKTOPSERVICES_IDX                                 = 57,
    SBK_QDOUBLEVALIDATOR_NOTATION_IDX                        = 59,
    SBK_QDOUBLEVALIDATOR_IDX                                 = 58,
    SBK_QDRAG_IDX                                            = 60,
    SBK_QDRAGENTEREVENT_IDX                                  = 61,
    SBK_QDRAGLEAVEEVENT_IDX                                  = 62,
    SBK_QDRAGMOVEEVENT_IDX                                   = 63,
    SBK_QDROPEVENT_IDX                                       = 64,
    SBK_QENTEREVENT_IDX                                      = 65,
    SBK_QEVENTPOINT_STATE_IDX                                = 67,
    SBK_QEVENTPOINT_IDX                                      = 66,
    SBK_QEXPOSEEVENT_IDX                                     = 68,
    SBK_QFILEOPENEVENT_IDX                                   = 69,
    SBK_QFOCUSEVENT_IDX                                      = 90,
    SBK_QFONT_STYLEHINT_IDX                                  = 96,
    SBK_QFONT_STYLESTRATEGY_IDX                              = 97,
    SBK_QFONT_HINTINGPREFERENCE_IDX                          = 348,
    SBK_QFONT_WEIGHT_IDX                                     = 98,
    SBK_QFONT_STYLE_IDX                                      = 95,
    SBK_QFONT_STRETCH_IDX                                    = 94,
    SBK_QFONT_CAPITALIZATION_IDX                             = 92,
    SBK_QFONT_SPACINGTYPE_IDX                                = 93,
    SBK_QFONT_IDX                                            = 91,
    SBK_QFONTDATABASE_WRITINGSYSTEM_IDX                      = 101,
    SBK_QFONTDATABASE_SYSTEMFONT_IDX                         = 100,
    SBK_QFONTDATABASE_IDX                                    = 99,
    SBK_QFONTINFO_IDX                                        = 102,
    SBK_QFONTMETRICS_IDX                                     = 103,
    SBK_QFONTMETRICSF_IDX                                    = 104,
    SBK_QGLYPHRUN_GLYPHRUNFLAG_IDX                           = 106,
    SBK_QFLAGS_QGLYPHRUN_GLYPHRUNFLAG_IDX                    = 72,
    SBK_QGLYPHRUN_IDX                                        = 105,
    SBK_QGRADIENT_TYPE_IDX                                   = 112,
    SBK_QGRADIENT_SPREAD_IDX                                 = 111,
    SBK_QGRADIENT_COORDINATEMODE_IDX                         = 108,
    SBK_QGRADIENT_INTERPOLATIONMODE_IDX                      = 109,
    SBK_QGRADIENT_PRESET_IDX                                 = 110,
    SBK_QGRADIENT_IDX                                        = 107,
    SBK_QGUIAPPLICATION_IDX                                  = 113,
    SBK_QHELPEVENT_IDX                                       = 114,
    SBK_QHIDEEVENT_IDX                                       = 115,
    SBK_QHOVEREVENT_IDX                                      = 116,
    SBK_QICON_MODE_IDX                                       = 118,
    SBK_QICON_STATE_IDX                                      = 119,
    SBK_QICON_IDX                                            = 117,
    SBK_QICONDRAGEVENT_IDX                                   = 120,
    SBK_QICONENGINE_ICONENGINEHOOK_IDX                       = 122,
    SBK_QICONENGINE_IDX                                      = 121,
    SBK_QICONENGINE_SCALEDPIXMAPARGUMENT_IDX                 = 123,
    SBK_QIMAGE_INVERTMODE_IDX                                = 126,
    SBK_QIMAGE_FORMAT_IDX                                    = 125,
    SBK_QIMAGE_IDX                                           = 124,
    SBK_QIMAGEIOHANDLER_IMAGEOPTION_IDX                      = 128,
    SBK_QIMAGEIOHANDLER_TRANSFORMATION_IDX                   = 129,
    SBK_QFLAGS_QIMAGEIOHANDLER_TRANSFORMATION_IDX            = 73,
    SBK_QIMAGEIOHANDLER_IDX                                  = 127,
    SBK_QIMAGEREADER_IMAGEREADERERROR_IDX                    = 131,
    SBK_QIMAGEREADER_IDX                                     = 130,
    SBK_QIMAGEWRITER_IMAGEWRITERERROR_IDX                    = 133,
    SBK_QIMAGEWRITER_IDX                                     = 132,
    SBK_QINPUTDEVICE_DEVICETYPE_IDX                          = 136,
    SBK_QFLAGS_QINPUTDEVICE_DEVICETYPE_IDX                   = 75,
    SBK_QINPUTDEVICE_CAPABILITY_IDX                          = 135,
    SBK_QFLAGS_QINPUTDEVICE_CAPABILITY_IDX                   = 74,
    SBK_QINPUTDEVICE_IDX                                     = 134,
    SBK_QINPUTEVENT_IDX                                      = 137,
    SBK_QINPUTMETHOD_ACTION_IDX                              = 139,
    SBK_QINPUTMETHOD_IDX                                     = 138,
    SBK_QINPUTMETHODEVENT_ATTRIBUTETYPE_IDX                  = 142,
    SBK_QINPUTMETHODEVENT_IDX                                = 140,
    SBK_QINPUTMETHODEVENT_ATTRIBUTE_IDX                      = 141,
    SBK_QINPUTMETHODQUERYEVENT_IDX                           = 143,
    SBK_QINTVALIDATOR_IDX                                    = 144,
    SBK_QKEYEVENT_IDX                                        = 145,
    SBK_QKEYSEQUENCE_STANDARDKEY_IDX                         = 149,
    SBK_QKEYSEQUENCE_SEQUENCEFORMAT_IDX                      = 147,
    SBK_QKEYSEQUENCE_SEQUENCEMATCH_IDX                       = 148,
    SBK_QKEYSEQUENCE_IDX                                     = 146,
    SBK_QLINEARGRADIENT_IDX                                  = 150,
    SBK_QMATRIX2X2_IDX                                       = 151,
    SBK_QGENERICMATRIX_2_2_FLOAT_IDX                         = 151,
    SBK_QMATRIX2X3_IDX                                       = 152,
    SBK_QGENERICMATRIX_2_3_FLOAT_IDX                         = 152,
    SBK_QMATRIX2X4_IDX                                       = 153,
    SBK_QGENERICMATRIX_2_4_FLOAT_IDX                         = 153,
    SBK_QMATRIX3X2_IDX                                       = 154,
    SBK_QGENERICMATRIX_3_2_FLOAT_IDX                         = 154,
    SBK_QMATRIX3X3_IDX                                       = 155,
    SBK_QGENERICMATRIX_3_3_FLOAT_IDX                         = 155,
    SBK_QMATRIX3X4_IDX                                       = 156,
    SBK_QGENERICMATRIX_3_4_FLOAT_IDX                         = 156,
    SBK_QMATRIX4X2_IDX                                       = 157,
    SBK_QGENERICMATRIX_4_2_FLOAT_IDX                         = 157,
    SBK_QMATRIX4X3_IDX                                       = 158,
    SBK_QGENERICMATRIX_4_3_FLOAT_IDX                         = 158,
    SBK_QMATRIX4X4_FLAG_IDX                                  = 160,
    SBK_QFLAGS_QMATRIX4X4_FLAG_IDX                           = 76,
    SBK_QMATRIX4X4_IDX                                       = 159,
    SBK_QMOUSEEVENT_IDX                                      = 161,
    SBK_QMOVEEVENT_IDX                                       = 162,
    SBK_QMOVIE_MOVIESTATE_IDX                                = 165,
    SBK_QMOVIE_CACHEMODE_IDX                                 = 164,
    SBK_QMOVIE_IDX                                           = 163,
    SBK_QNATIVEGESTUREEVENT_IDX                              = 166,
    SBK_QOFFSCREENSURFACE_IDX                                = 167,
    SBK_QOPENGLCONTEXT_OPENGLMODULETYPE_IDX                  = 169,
    SBK_QOPENGLCONTEXT_IDX                                   = 168,
    SBK_QOPENGLCONTEXTGROUP_IDX                              = 170,
    SBK_QOPENGLEXTRAFUNCTIONS_IDX                            = 171,
    SBK_QOPENGLFUNCTIONS_OPENGLFEATURE_IDX                   = 173,
    SBK_QFLAGS_QOPENGLFUNCTIONS_OPENGLFEATURE_IDX            = 77,
    SBK_QOPENGLFUNCTIONS_IDX                                 = 172,
    SBK_QPAGELAYOUT_UNIT_IDX                                 = 177,
    SBK_QPAGELAYOUT_ORIENTATION_IDX                          = 176,
    SBK_QPAGELAYOUT_MODE_IDX                                 = 175,
    SBK_QPAGELAYOUT_IDX                                      = 174,
    SBK_QPAGERANGES_IDX                                      = 178,
    SBK_QPAGERANGES_RANGE_IDX                                = 179,
    SBK_QPAGESIZE_PAGESIZEID_IDX                             = 181,
    SBK_QPAGESIZE_UNIT_IDX                                   = 183,
    SBK_QPAGESIZE_SIZEMATCHPOLICY_IDX                        = 182,
    SBK_QPAGESIZE_IDX                                        = 180,
    SBK_QPAGEDPAINTDEVICE_PDFVERSION_IDX                     = 185,
    SBK_QPAGEDPAINTDEVICE_IDX                                = 184,
    SBK_QPAINTDEVICE_PAINTDEVICEMETRIC_IDX                   = 187,
    SBK_QPAINTDEVICE_IDX                                     = 186,
    SBK_QPAINTDEVICEWINDOW_IDX                               = 188,
    SBK_QPAINTENGINE_PAINTENGINEFEATURE_IDX                  = 191,
    SBK_QFLAGS_QPAINTENGINE_PAINTENGINEFEATURE_IDX           = 79,
    SBK_QPAINTENGINE_DIRTYFLAG_IDX                           = 190,
    SBK_QFLAGS_QPAINTENGINE_DIRTYFLAG_IDX                    = 78,
    SBK_QPAINTENGINE_POLYGONDRAWMODE_IDX                     = 192,
    SBK_QPAINTENGINE_TYPE_IDX                                = 193,
    SBK_QPAINTENGINE_IDX                                     = 189,
    SBK_QPAINTENGINESTATE_IDX                                = 194,
    SBK_QPAINTEVENT_IDX                                      = 195,
    SBK_QPAINTER_RENDERHINT_IDX                              = 200,
    SBK_QFLAGS_QPAINTER_RENDERHINT_IDX                       = 81,
    SBK_QPAINTER_PIXMAPFRAGMENTHINT_IDX                      = 199,
    SBK_QFLAGS_QPAINTER_PIXMAPFRAGMENTHINT_IDX               = 80,
    SBK_QPAINTER_COMPOSITIONMODE_IDX                         = 197,
    SBK_QPAINTER_IDX                                         = 196,
    SBK_QPAINTER_PIXMAPFRAGMENT_IDX                          = 198,
    SBK_QPAINTERPATH_ELEMENTTYPE_IDX                         = 203,
    SBK_QPAINTERPATH_IDX                                     = 201,
    SBK_QPAINTERPATH_ELEMENT_IDX                             = 202,
    SBK_QPAINTERPATHSTROKER_IDX                              = 204,
    SBK_QPALETTE_COLORGROUP_IDX                              = 206,
    SBK_QPALETTE_COLORROLE_IDX                               = 207,
    SBK_QPALETTE_IDX                                         = 205,
    SBK_QPDFWRITER_IDX                                       = 208,
    SBK_QPEN_IDX                                             = 209,
    SBK_QPICTURE_IDX                                         = 210,
    SBK_QPIXELFORMAT_COLORMODEL_IDX                          = 216,
    SBK_QPIXELFORMAT_ALPHAUSAGE_IDX                          = 214,
    SBK_QPIXELFORMAT_ALPHAPOSITION_IDX                       = 212,
    SBK_QPIXELFORMAT_ALPHAPREMULTIPLIED_IDX                  = 213,
    SBK_QPIXELFORMAT_TYPEINTERPRETATION_IDX                  = 217,
    SBK_QPIXELFORMAT_YUVLAYOUT_IDX                           = 218,
    SBK_QPIXELFORMAT_BYTEORDER_IDX                           = 215,
    SBK_QPIXELFORMAT_IDX                                     = 211,
    SBK_QPIXMAP_IDX                                          = 219,
    SBK_QPIXMAPCACHE_IDX                                     = 220,
    SBK_QPIXMAPCACHE_KEY_IDX                                 = 221,
    SBK_QPLATFORMSURFACEEVENT_SURFACEEVENTTYPE_IDX           = 223,
    SBK_QPLATFORMSURFACEEVENT_IDX                            = 222,
    SBK_QPOINTEREVENT_IDX                                    = 224,
    SBK_QPOINTINGDEVICE_POINTERTYPE_IDX                      = 227,
    SBK_QFLAGS_QPOINTINGDEVICE_POINTERTYPE_IDX               = 82,
    SBK_QPOINTINGDEVICE_GRABTRANSITION_IDX                   = 226,
    SBK_QPOINTINGDEVICE_IDX                                  = 225,
    SBK_QPOINTINGDEVICEUNIQUEID_IDX                          = 228,
    SBK_QPOLYGON_IDX                                         = 229,
    SBK_QPOLYGONF_IDX                                        = 230,
    SBK_QPYTEXTOBJECT_IDX                                    = 231,
    SBK_QQUATERNION_IDX                                      = 232,
    SBK_QRADIALGRADIENT_IDX                                  = 233,
    SBK_QRASTERWINDOW_IDX                                    = 234,
    SBK_QRAWFONT_ANTIALIASINGTYPE_IDX                        = 236,
    SBK_QRAWFONT_LAYOUTFLAG_IDX                              = 237,
    SBK_QFLAGS_QRAWFONT_LAYOUTFLAG_IDX                       = 83,
    SBK_QRAWFONT_IDX                                         = 235,
    SBK_QREGION_REGIONTYPE_IDX                               = 239,
    SBK_QREGION_IDX                                          = 238,
    SBK_QREGULAREXPRESSIONVALIDATOR_IDX                      = 240,
    SBK_QRESIZEEVENT_IDX                                     = 241,
    SBK_QSCREEN_IDX                                          = 242,
    SBK_QSCROLLEVENT_SCROLLSTATE_IDX                         = 244,
    SBK_QSCROLLEVENT_IDX                                     = 243,
    SBK_QSCROLLPREPAREEVENT_IDX                              = 245,
    SBK_QSESSIONMANAGER_RESTARTHINT_IDX                      = 247,
    SBK_QSESSIONMANAGER_IDX                                  = 246,
    SBK_QSHORTCUT_IDX                                        = 248,
    SBK_QSHORTCUTEVENT_IDX                                   = 249,
    SBK_QSHOWEVENT_IDX                                       = 250,
    SBK_QSINGLEPOINTEVENT_IDX                                = 251,
    SBK_QSTANDARDITEM_ITEMTYPE_IDX                           = 253,
    SBK_QSTANDARDITEM_IDX                                    = 252,
    SBK_QSTANDARDITEMMODEL_IDX                               = 254,
    SBK_QSTATICTEXT_PERFORMANCEHINT_IDX                      = 256,
    SBK_QSTATICTEXT_IDX                                      = 255,
    SBK_QSTATUSTIPEVENT_IDX                                  = 257,
    SBK_QSTYLEHINTS_IDX                                      = 258,
    SBK_QSURFACE_SURFACECLASS_IDX                            = 260,
    SBK_QSURFACE_SURFACETYPE_IDX                             = 261,
    SBK_QSURFACE_IDX                                         = 259,
    SBK_QSURFACEFORMAT_FORMATOPTION_IDX                      = 264,
    SBK_QFLAGS_QSURFACEFORMAT_FORMATOPTION_IDX               = 84,
    SBK_QSURFACEFORMAT_SWAPBEHAVIOR_IDX                      = 267,
    SBK_QSURFACEFORMAT_RENDERABLETYPE_IDX                    = 266,
    SBK_QSURFACEFORMAT_OPENGLCONTEXTPROFILE_IDX              = 265,
    SBK_QSURFACEFORMAT_COLORSPACE_IDX                        = 263,
    SBK_QSURFACEFORMAT_IDX                                   = 262,
    SBK_QSYNTAXHIGHLIGHTER_IDX                               = 268,
    SBK_QTABLETEVENT_IDX                                     = 269,
    SBK_QTEXTBLOCK_IDX                                       = 270,
    SBK_QTEXTBLOCK_ITERATOR_IDX                              = 271,
    SBK_QTEXTBLOCKFORMAT_LINEHEIGHTTYPES_IDX                 = 349,
    SBK_QTEXTBLOCKFORMAT_MARKERTYPE_IDX                      = 273,
    SBK_QTEXTBLOCKFORMAT_IDX                                 = 272,
    SBK_QTEXTBLOCKGROUP_IDX                                  = 274,
    SBK_QTEXTBLOCKUSERDATA_IDX                               = 275,
    SBK_QTEXTCHARFORMAT_VERTICALALIGNMENT_IDX                = 279,
    SBK_QTEXTCHARFORMAT_UNDERLINESTYLE_IDX                   = 278,
    SBK_QTEXTCHARFORMAT_FONTPROPERTIESINHERITANCEBEHAVIOR_IDX = 277,
    SBK_QTEXTCHARFORMAT_IDX                                  = 276,
    SBK_QTEXTCURSOR_MOVEMODE_IDX                             = 281,
    SBK_QTEXTCURSOR_MOVEOPERATION_IDX                        = 282,
    SBK_QTEXTCURSOR_SELECTIONTYPE_IDX                        = 283,
    SBK_QTEXTCURSOR_IDX                                      = 280,
    SBK_QTEXTDOCUMENT_METAINFORMATION_IDX                    = 287,
    SBK_QTEXTDOCUMENT_MARKDOWNFEATURE_IDX                    = 286,
    SBK_QFLAGS_QTEXTDOCUMENT_MARKDOWNFEATURE_IDX             = 86,
    SBK_QTEXTDOCUMENT_FINDFLAG_IDX                           = 285,
    SBK_QFLAGS_QTEXTDOCUMENT_FINDFLAG_IDX                    = 85,
    SBK_QTEXTDOCUMENT_RESOURCETYPE_IDX                       = 288,
    SBK_QTEXTDOCUMENT_STACKS_IDX                             = 289,
    SBK_QTEXTDOCUMENT_IDX                                    = 284,
    SBK_QTEXTDOCUMENTFRAGMENT_IDX                            = 290,
    SBK_QTEXTDOCUMENTWRITER_IDX                              = 291,
    SBK_QTEXTFORMAT_FORMATTYPE_IDX                           = 293,
    SBK_QTEXTFORMAT_PROPERTY_IDX                             = 296,
    SBK_QTEXTFORMAT_OBJECTTYPES_IDX                          = 294,
    SBK_QTEXTFORMAT_PAGEBREAKFLAG_IDX                        = 295,
    SBK_QFLAGS_QTEXTFORMAT_PAGEBREAKFLAG_IDX                 = 87,
    SBK_QTEXTFORMAT_IDX                                      = 292,
    SBK_QTEXTFRAGMENT_IDX                                    = 297,
    SBK_QTEXTFRAME_IDX                                       = 298,
    SBK_QTEXTFRAME_ITERATOR_IDX                              = 299,
    SBK_QTEXTFRAMEFORMAT_POSITION_IDX                        = 302,
    SBK_QTEXTFRAMEFORMAT_BORDERSTYLE_IDX                     = 301,
    SBK_QTEXTFRAMEFORMAT_IDX                                 = 300,
    SBK_QTEXTIMAGEFORMAT_IDX                                 = 303,
    SBK_QTEXTINLINEOBJECT_IDX                                = 304,
    SBK_QTEXTITEM_RENDERFLAG_IDX                             = 306,
    SBK_QFLAGS_QTEXTITEM_RENDERFLAG_IDX                      = 88,
    SBK_QTEXTITEM_IDX                                        = 305,
    SBK_QTEXTLAYOUT_CURSORMODE_IDX                           = 308,
    SBK_QTEXTLAYOUT_IDX                                      = 307,
    SBK_QTEXTLAYOUT_FORMATRANGE_IDX                          = 309,
    SBK_QTEXTLENGTH_TYPE_IDX                                 = 311,
    SBK_QTEXTLENGTH_IDX                                      = 310,
    SBK_QTEXTLINE_EDGE_IDX                                   = 314,
    SBK_QTEXTLINE_CURSORPOSITION_IDX                         = 313,
    SBK_QTEXTLINE_IDX                                        = 312,
    SBK_QTEXTLIST_IDX                                        = 315,
    SBK_QTEXTLISTFORMAT_STYLE_IDX                            = 317,
    SBK_QTEXTLISTFORMAT_IDX                                  = 316,
    SBK_QTEXTOBJECT_IDX                                      = 318,
    SBK_QTEXTOBJECTINTERFACE_IDX                             = 319,
    SBK_QTEXTOPTION_TABTYPE_IDX                              = 323,
    SBK_QTEXTOPTION_WRAPMODE_IDX                             = 324,
    SBK_QTEXTOPTION_FLAG_IDX                                 = 321,
    SBK_QFLAGS_QTEXTOPTION_FLAG_IDX                          = 89,
    SBK_QTEXTOPTION_IDX                                      = 320,
    SBK_QTEXTOPTION_TAB_IDX                                  = 322,
    SBK_QTEXTTABLE_IDX                                       = 325,
    SBK_QTEXTTABLECELL_IDX                                   = 326,
    SBK_QTEXTTABLECELLFORMAT_IDX                             = 327,
    SBK_QTEXTTABLEFORMAT_IDX                                 = 328,
    SBK_QTOOLBARCHANGEEVENT_IDX                              = 329,
    SBK_QTOUCHEVENT_IDX                                      = 330,
    SBK_QTRANSFORM_TRANSFORMATIONTYPE_IDX                    = 332,
    SBK_QTRANSFORM_IDX                                       = 331,
    SBK_QUNDOCOMMAND_IDX                                     = 333,
    SBK_QUNDOGROUP_IDX                                       = 334,
    SBK_QUNDOSTACK_IDX                                       = 335,
    SBK_QVALIDATOR_STATE_IDX                                 = 337,
    SBK_QVALIDATOR_IDX                                       = 336,
    SBK_QVECTOR2D_IDX                                        = 338,
    SBK_QVECTOR3D_IDX                                        = 339,
    SBK_QVECTOR4D_IDX                                        = 340,
    SBK_QWHATSTHISCLICKEDEVENT_IDX                           = 341,
    SBK_QWHEELEVENT_IDX                                      = 342,
    SBK_QWINDOW_VISIBILITY_IDX                               = 345,
    SBK_QWINDOW_ANCESTORMODE_IDX                             = 344,
    SBK_QWINDOW_IDX                                          = 343,
    SBK_QWINDOWSTATECHANGEEVENT_IDX                          = 346,
    SBK_QtGuiQT_IDX                                          = 347,
    SBK_QtGui_IDX_COUNT                                      = 350
};
// This variable stores all Python types exported by this module.
extern PyTypeObject **SbkPySide6_QtGuiTypes;

// This variable stores the Python module object exported by this module.
extern PyObject *SbkPySide6_QtGuiModuleObject;

// This variable stores all type converters exported by this module.
extern SbkConverter **SbkPySide6_QtGuiTypeConverters;

// Converter indices
enum : int {
    SBK_WID_IDX                                              = 0,
    SBK_QTGUI_QLIST_QREAL_IDX                                = 1, // QList<qreal >
    SBK_QTGUI_QLIST_QTEXTOPTION_TAB_IDX                      = 2, // QList<QTextOption::Tab >
    SBK_QTGUI_QLIST_QGLYPHRUN_IDX                            = 3, // QList<QGlyphRun >
    SBK_QTGUI_QLIST_QTEXTLENGTH_IDX                          = 4, // QList<QTextLength >
    SBK_QTGUI_QMAP_INT_QVARIANT_IDX                          = 5, // QMap<int,QVariant >
    SBK_QTGUI_QLIST_QTEXTLAYOUT_FORMATRANGE_IDX              = 6, // QList<QTextLayout::FormatRange >
    SBK_QTGUI_QPAIR_INT_INT_IDX                              = 7, // QPair<int,int >
    SBK_QTGUI_QLIST_QSTANDARDITEMPTR_IDX                     = 8, // QList<QStandardItem* >
    SBK_QTGUI_QLIST_QPOINTF_IDX                              = 9, // QList<QPointF >
    SBK_QTGUI_QLIST_QUINT32_IDX                              = 10, // QList<quint32 >
    SBK_QTGUI_QLIST_QFONTDATABASE_WRITINGSYSTEM_IDX          = 11, // QList<QFontDatabase::WritingSystem >
    SBK_QTGUI_QLIST_QPOINT_IDX                               = 12, // QList<QPoint >
    SBK_QTGUI_QLIST_QPOLYGONF_IDX                            = 13, // QList<QPolygonF >
    SBK_QTGUI_QLIST_QPAGERANGES_RANGE_IDX                    = 14, // QList<QPageRanges::Range >
    SBK_QTGUI_QLIST_FLOAT_IDX                                = 15, // QList<float >
    SBK_QTGUI_QLIST_QSIZE_IDX                                = 16, // QList<QSize >
    SBK_QTGUI_STD_PAIR_QREAL_QCOLOR_IDX                      = 17, // std::pair<qreal,QColor >
    SBK_QTGUI_QLIST_STD_PAIR_QREAL_QCOLOR_IDX                = 18, // QList<std::pair< qreal,QColor > >
    SBK_QTGUI_QLIST_INT_IDX                                  = 19, // QList<int >
    SBK_QTGUI_QLIST_QACCESSIBLEINTERFACEPTR_IDX              = 20, // QList<QAccessibleInterface* >
    SBK_QTGUI_QPAIR_QACCESSIBLEINTERFACEPTR_QFLAGS_QACCESSIBLE_RELATIONFLAG_IDX = 21, // QPair<QAccessibleInterface*,QFlags<QAccessible::RelationFlag> >
    SBK_QTGUI_QLIST_QPAIR_QACCESSIBLEINTERFACEPTR_QFLAGS_QACCESSIBLE_RELATIONFLAG_IDX = 22, // QList<QPair< QAccessibleInterface*,QFlags<QAccessible::RelationFlag> > >
    SBK_QTGUI_QLIST_UNSIGNEDINT_IDX                          = 23, // QList<unsigned int >
    SBK_QTGUI_QLIST_QLINE_IDX                                = 24, // QList<QLine >
    SBK_QTGUI_QLIST_QLINEF_IDX                               = 25, // QList<QLineF >
    SBK_QTGUI_QLIST_QRECT_IDX                                = 26, // QList<QRect >
    SBK_QTGUI_QLIST_QRECTF_IDX                               = 27, // QList<QRectF >
    SBK_QTGUI_QLIST_QKEYSEQUENCE_IDX                         = 28, // QList<QKeySequence >
    SBK_QTGUI_QLIST_QOBJECTPTR_IDX                           = 29, // QList<QObject* >
    SBK_QTGUI_QLIST_QBYTEARRAY_IDX                           = 30, // QList<QByteArray >
    SBK_QTGUI_QLIST_QUNDOSTACKPTR_IDX                        = 31, // QList<QUndoStack* >
    SBK_QTGUI_QLIST_QTEXTFRAMEPTR_IDX                        = 32, // QList<QTextFrame* >
    SBK_QTGUI_QLIST_QTEXTBLOCK_IDX                           = 33, // QList<QTextBlock >
    SBK_QTGUI_QLIST_QTEXTFORMAT_IDX                          = 34, // QList<QTextFormat >
    SBK_QTGUI_QLIST_QSCREENPTR_IDX                           = 35, // QList<QScreen* >
    SBK_QTGUI_QLIST_QOPENGLCONTEXTPTR_IDX                    = 36, // QList<QOpenGLContext* >
    SBK_QTGUI_QSET_QBYTEARRAY_IDX                            = 37, // QSet<QByteArray >
    SBK_QTGUI_QLIST_CONSTQINPUTDEVICEPTR_IDX                 = 38, // QList<const QInputDevice* >
    SBK_QTGUI_QLIST_QACTIONPTR_IDX                           = 39, // QList<QAction* >
    SBK_QTGUI_QLIST_QINPUTMETHODEVENT_ATTRIBUTE_IDX          = 40, // QList<QInputMethodEvent::Attribute >
    SBK_QTGUI_QLIST_QEVENTPOINT_IDX                          = 41, // QList<QEventPoint >
    SBK_QTGUI_QLIST_QWINDOWPTR_IDX                           = 42, // QList<QWindow* >
    SBK_QTGUI_QLIST_QPERSISTENTMODELINDEX_IDX                = 43, // QList<QPersistentModelIndex >
    SBK_QTGUI_QHASH_INT_QBYTEARRAY_IDX                       = 44, // QHash<int,QByteArray >
    SBK_QTGUI_QLIST_QVARIANT_IDX                             = 45, // QList<QVariant >
    SBK_QTGUI_QLIST_QSTRING_IDX                              = 46, // QList<QString >
    SBK_QTGUI_QMAP_QSTRING_QVARIANT_IDX                      = 47, // QMap<QString,QVariant >
    SBK_QtGui_CONVERTERS_IDX_COUNT                           = 48
};
// Macros for type check

namespace Shiboken
{

// PyType functions, to get the PyObjectType for a type T
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
template<> inline PyTypeObject *SbkType< ::QAbstractFileIconProvider::IconType >() { return SbkPySide6_QtGuiTypes[SBK_QABSTRACTFILEICONPROVIDER_ICONTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAbstractFileIconProvider::Option >() { return SbkPySide6_QtGuiTypes[SBK_QABSTRACTFILEICONPROVIDER_OPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QAbstractFileIconProvider::Option> >() { return SbkPySide6_QtGuiTypes[SBK_QFLAGS_QABSTRACTFILEICONPROVIDER_OPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAbstractFileIconProvider >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QABSTRACTFILEICONPROVIDER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAbstractTextDocumentLayout >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QABSTRACTTEXTDOCUMENTLAYOUT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAbstractTextDocumentLayout::PaintContext >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QABSTRACTTEXTDOCUMENTLAYOUT_PAINTCONTEXT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAbstractTextDocumentLayout::Selection >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QABSTRACTTEXTDOCUMENTLAYOUT_SELECTION_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAccessible::Event >() { return SbkPySide6_QtGuiTypes[SBK_QACCESSIBLE_EVENT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAccessible::Role >() { return SbkPySide6_QtGuiTypes[SBK_QACCESSIBLE_ROLE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAccessible::Text >() { return SbkPySide6_QtGuiTypes[SBK_QACCESSIBLE_TEXT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAccessible::RelationFlag >() { return SbkPySide6_QtGuiTypes[SBK_QACCESSIBLE_RELATIONFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QAccessible::RelationFlag> >() { return SbkPySide6_QtGuiTypes[SBK_QFLAGS_QACCESSIBLE_RELATIONFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAccessible::InterfaceType >() { return SbkPySide6_QtGuiTypes[SBK_QACCESSIBLE_INTERFACETYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAccessible::TextBoundaryType >() { return SbkPySide6_QtGuiTypes[SBK_QACCESSIBLE_TEXTBOUNDARYTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAccessible >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QACCESSIBLE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAccessible::State >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QACCESSIBLE_STATE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAccessibleEditableTextInterface >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QACCESSIBLEEDITABLETEXTINTERFACE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAccessibleEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QACCESSIBLEEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAccessibleInterface >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QACCESSIBLEINTERFACE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAccessibleObject >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QACCESSIBLEOBJECT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAccessibleStateChangeEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QACCESSIBLESTATECHANGEEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAccessibleTableCellInterface >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QACCESSIBLETABLECELLINTERFACE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAccessibleTableModelChangeEvent::ModelChangeType >() { return SbkPySide6_QtGuiTypes[SBK_QACCESSIBLETABLEMODELCHANGEEVENT_MODELCHANGETYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAccessibleTableModelChangeEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QACCESSIBLETABLEMODELCHANGEEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAccessibleTextCursorEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QACCESSIBLETEXTCURSOREVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAccessibleTextInsertEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QACCESSIBLETEXTINSERTEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAccessibleTextInterface >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QACCESSIBLETEXTINTERFACE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAccessibleTextRemoveEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QACCESSIBLETEXTREMOVEEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAccessibleTextSelectionEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QACCESSIBLETEXTSELECTIONEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAccessibleTextUpdateEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QACCESSIBLETEXTUPDATEEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAccessibleValueChangeEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QACCESSIBLEVALUECHANGEEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAccessibleValueInterface >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QACCESSIBLEVALUEINTERFACE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAction::MenuRole >() { return SbkPySide6_QtGuiTypes[SBK_QACTION_MENUROLE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAction::Priority >() { return SbkPySide6_QtGuiTypes[SBK_QACTION_PRIORITY_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAction::ActionEvent >() { return SbkPySide6_QtGuiTypes[SBK_QACTION_ACTIONEVENT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAction >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QACTION_IDX]); }
template<> inline PyTypeObject *SbkType< ::QActionEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QACTIONEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QActionGroup::ExclusionPolicy >() { return SbkPySide6_QtGuiTypes[SBK_QACTIONGROUP_EXCLUSIONPOLICY_IDX]; }
template<> inline PyTypeObject *SbkType< ::QActionGroup >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QACTIONGROUP_IDX]); }
template<> inline PyTypeObject *SbkType< ::QBackingStore >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QBACKINGSTORE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QBitmap >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QBITMAP_IDX]); }
template<> inline PyTypeObject *SbkType< ::QBrush >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QBRUSH_IDX]); }
template<> inline PyTypeObject *SbkType< ::QClipboard::Mode >() { return SbkPySide6_QtGuiTypes[SBK_QCLIPBOARD_MODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QClipboard >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QCLIPBOARD_IDX]); }
template<> inline PyTypeObject *SbkType< ::QCloseEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QCLOSEEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QColor::Spec >() { return SbkPySide6_QtGuiTypes[SBK_QCOLOR_SPEC_IDX]; }
template<> inline PyTypeObject *SbkType< ::QColor::NameFormat >() { return SbkPySide6_QtGuiTypes[SBK_QCOLOR_NAMEFORMAT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QColor >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QCOLOR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QColorSpace::NamedColorSpace >() { return SbkPySide6_QtGuiTypes[SBK_QCOLORSPACE_NAMEDCOLORSPACE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QColorSpace::Primaries >() { return SbkPySide6_QtGuiTypes[SBK_QCOLORSPACE_PRIMARIES_IDX]; }
template<> inline PyTypeObject *SbkType< ::QColorSpace::TransferFunction >() { return SbkPySide6_QtGuiTypes[SBK_QCOLORSPACE_TRANSFERFUNCTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QColorSpace >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QCOLORSPACE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QColorTransform >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QCOLORTRANSFORM_IDX]); }
template<> inline PyTypeObject *SbkType< ::QConicalGradient >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QCONICALGRADIENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QContextMenuEvent::Reason >() { return SbkPySide6_QtGuiTypes[SBK_QCONTEXTMENUEVENT_REASON_IDX]; }
template<> inline PyTypeObject *SbkType< ::QContextMenuEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QCONTEXTMENUEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QCursor >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QCURSOR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDesktopServices >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QDESKTOPSERVICES_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDoubleValidator::Notation >() { return SbkPySide6_QtGuiTypes[SBK_QDOUBLEVALIDATOR_NOTATION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QDoubleValidator >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QDOUBLEVALIDATOR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDrag >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QDRAG_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDragEnterEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QDRAGENTEREVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDragLeaveEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QDRAGLEAVEEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDragMoveEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QDRAGMOVEEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDropEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QDROPEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QEnterEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QENTEREVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QEventPoint::State >() { return SbkPySide6_QtGuiTypes[SBK_QEVENTPOINT_STATE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QEventPoint >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QEVENTPOINT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QExposeEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QEXPOSEEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QFileOpenEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QFILEOPENEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QFocusEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QFOCUSEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QFont::StyleHint >() { return SbkPySide6_QtGuiTypes[SBK_QFONT_STYLEHINT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFont::StyleStrategy >() { return SbkPySide6_QtGuiTypes[SBK_QFONT_STYLESTRATEGY_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFont::HintingPreference >() { return SbkPySide6_QtGuiTypes[SBK_QFONT_HINTINGPREFERENCE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFont::Weight >() { return SbkPySide6_QtGuiTypes[SBK_QFONT_WEIGHT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFont::Style >() { return SbkPySide6_QtGuiTypes[SBK_QFONT_STYLE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFont::Stretch >() { return SbkPySide6_QtGuiTypes[SBK_QFONT_STRETCH_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFont::Capitalization >() { return SbkPySide6_QtGuiTypes[SBK_QFONT_CAPITALIZATION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFont::SpacingType >() { return SbkPySide6_QtGuiTypes[SBK_QFONT_SPACINGTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFont >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QFONT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QFontDatabase::WritingSystem >() { return SbkPySide6_QtGuiTypes[SBK_QFONTDATABASE_WRITINGSYSTEM_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFontDatabase::SystemFont >() { return SbkPySide6_QtGuiTypes[SBK_QFONTDATABASE_SYSTEMFONT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFontDatabase >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QFONTDATABASE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QFontInfo >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QFONTINFO_IDX]); }
template<> inline PyTypeObject *SbkType< ::QFontMetrics >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QFONTMETRICS_IDX]); }
template<> inline PyTypeObject *SbkType< ::QFontMetricsF >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QFONTMETRICSF_IDX]); }
template<> inline PyTypeObject *SbkType< ::QGlyphRun::GlyphRunFlag >() { return SbkPySide6_QtGuiTypes[SBK_QGLYPHRUN_GLYPHRUNFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QGlyphRun::GlyphRunFlag> >() { return SbkPySide6_QtGuiTypes[SBK_QFLAGS_QGLYPHRUN_GLYPHRUNFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QGlyphRun >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QGLYPHRUN_IDX]); }
template<> inline PyTypeObject *SbkType< ::QGradient::Type >() { return SbkPySide6_QtGuiTypes[SBK_QGRADIENT_TYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QGradient::Spread >() { return SbkPySide6_QtGuiTypes[SBK_QGRADIENT_SPREAD_IDX]; }
template<> inline PyTypeObject *SbkType< ::QGradient::CoordinateMode >() { return SbkPySide6_QtGuiTypes[SBK_QGRADIENT_COORDINATEMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QGradient::InterpolationMode >() { return SbkPySide6_QtGuiTypes[SBK_QGRADIENT_INTERPOLATIONMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QGradient::Preset >() { return SbkPySide6_QtGuiTypes[SBK_QGRADIENT_PRESET_IDX]; }
template<> inline PyTypeObject *SbkType< ::QGradient >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QGRADIENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QGuiApplication >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QGUIAPPLICATION_IDX]); }
template<> inline PyTypeObject *SbkType< ::QHelpEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QHELPEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QHideEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QHIDEEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QHoverEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QHOVEREVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QIcon::Mode >() { return SbkPySide6_QtGuiTypes[SBK_QICON_MODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QIcon::State >() { return SbkPySide6_QtGuiTypes[SBK_QICON_STATE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QIcon >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QICON_IDX]); }
template<> inline PyTypeObject *SbkType< ::QIconDragEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QICONDRAGEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QIconEngine::IconEngineHook >() { return SbkPySide6_QtGuiTypes[SBK_QICONENGINE_ICONENGINEHOOK_IDX]; }
template<> inline PyTypeObject *SbkType< ::QIconEngine >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QICONENGINE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QIconEngine::ScaledPixmapArgument >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QICONENGINE_SCALEDPIXMAPARGUMENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QImage::InvertMode >() { return SbkPySide6_QtGuiTypes[SBK_QIMAGE_INVERTMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QImage::Format >() { return SbkPySide6_QtGuiTypes[SBK_QIMAGE_FORMAT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QImage >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QIMAGE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QImageIOHandler::ImageOption >() { return SbkPySide6_QtGuiTypes[SBK_QIMAGEIOHANDLER_IMAGEOPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QImageIOHandler::Transformation >() { return SbkPySide6_QtGuiTypes[SBK_QIMAGEIOHANDLER_TRANSFORMATION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QImageIOHandler::Transformation> >() { return SbkPySide6_QtGuiTypes[SBK_QFLAGS_QIMAGEIOHANDLER_TRANSFORMATION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QImageIOHandler >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QIMAGEIOHANDLER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QImageReader::ImageReaderError >() { return SbkPySide6_QtGuiTypes[SBK_QIMAGEREADER_IMAGEREADERERROR_IDX]; }
template<> inline PyTypeObject *SbkType< ::QImageReader >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QIMAGEREADER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QImageWriter::ImageWriterError >() { return SbkPySide6_QtGuiTypes[SBK_QIMAGEWRITER_IMAGEWRITERERROR_IDX]; }
template<> inline PyTypeObject *SbkType< ::QImageWriter >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QIMAGEWRITER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QInputDevice::DeviceType >() { return SbkPySide6_QtGuiTypes[SBK_QINPUTDEVICE_DEVICETYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QInputDevice::DeviceType> >() { return SbkPySide6_QtGuiTypes[SBK_QFLAGS_QINPUTDEVICE_DEVICETYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QInputDevice::Capability >() { return SbkPySide6_QtGuiTypes[SBK_QINPUTDEVICE_CAPABILITY_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QInputDevice::Capability> >() { return SbkPySide6_QtGuiTypes[SBK_QFLAGS_QINPUTDEVICE_CAPABILITY_IDX]; }
template<> inline PyTypeObject *SbkType< ::QInputDevice >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QINPUTDEVICE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QInputEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QINPUTEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QInputMethod::Action >() { return SbkPySide6_QtGuiTypes[SBK_QINPUTMETHOD_ACTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QInputMethod >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QINPUTMETHOD_IDX]); }
template<> inline PyTypeObject *SbkType< ::QInputMethodEvent::AttributeType >() { return SbkPySide6_QtGuiTypes[SBK_QINPUTMETHODEVENT_ATTRIBUTETYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QInputMethodEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QINPUTMETHODEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QInputMethodEvent::Attribute >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QINPUTMETHODEVENT_ATTRIBUTE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QInputMethodQueryEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QINPUTMETHODQUERYEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QIntValidator >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QINTVALIDATOR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QKeyEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QKEYEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QKeySequence::StandardKey >() { return SbkPySide6_QtGuiTypes[SBK_QKEYSEQUENCE_STANDARDKEY_IDX]; }
template<> inline PyTypeObject *SbkType< ::QKeySequence::SequenceFormat >() { return SbkPySide6_QtGuiTypes[SBK_QKEYSEQUENCE_SEQUENCEFORMAT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QKeySequence::SequenceMatch >() { return SbkPySide6_QtGuiTypes[SBK_QKEYSEQUENCE_SEQUENCEMATCH_IDX]; }
template<> inline PyTypeObject *SbkType< ::QKeySequence >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QKEYSEQUENCE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QLinearGradient >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QLINEARGRADIENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QMatrix2x2 >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QMATRIX2X2_IDX]); }
template<> inline PyTypeObject *SbkType< ::QMatrix2x3 >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QMATRIX2X3_IDX]); }
template<> inline PyTypeObject *SbkType< ::QMatrix2x4 >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QMATRIX2X4_IDX]); }
template<> inline PyTypeObject *SbkType< ::QMatrix3x2 >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QMATRIX3X2_IDX]); }
template<> inline PyTypeObject *SbkType< ::QMatrix3x3 >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QMATRIX3X3_IDX]); }
template<> inline PyTypeObject *SbkType< ::QMatrix3x4 >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QMATRIX3X4_IDX]); }
template<> inline PyTypeObject *SbkType< ::QMatrix4x2 >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QMATRIX4X2_IDX]); }
template<> inline PyTypeObject *SbkType< ::QMatrix4x3 >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QMATRIX4X3_IDX]); }
template<> inline PyTypeObject *SbkType< ::QMatrix4x4::Flag >() { return SbkPySide6_QtGuiTypes[SBK_QMATRIX4X4_FLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QMatrix4x4::Flag> >() { return SbkPySide6_QtGuiTypes[SBK_QFLAGS_QMATRIX4X4_FLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QMatrix4x4 >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QMATRIX4X4_IDX]); }
template<> inline PyTypeObject *SbkType< ::QMouseEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QMOUSEEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QMoveEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QMOVEEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QMovie::MovieState >() { return SbkPySide6_QtGuiTypes[SBK_QMOVIE_MOVIESTATE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QMovie::CacheMode >() { return SbkPySide6_QtGuiTypes[SBK_QMOVIE_CACHEMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QMovie >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QMOVIE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QNativeGestureEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QNATIVEGESTUREEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOffscreenSurface >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QOFFSCREENSURFACE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLContext::OpenGLModuleType >() { return SbkPySide6_QtGuiTypes[SBK_QOPENGLCONTEXT_OPENGLMODULETYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLContext >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QOPENGLCONTEXT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLContextGroup >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QOPENGLCONTEXTGROUP_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLExtraFunctions >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QOPENGLEXTRAFUNCTIONS_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOpenGLFunctions::OpenGLFeature >() { return SbkPySide6_QtGuiTypes[SBK_QOPENGLFUNCTIONS_OPENGLFEATURE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QOpenGLFunctions::OpenGLFeature> >() { return SbkPySide6_QtGuiTypes[SBK_QFLAGS_QOPENGLFUNCTIONS_OPENGLFEATURE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOpenGLFunctions >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QOPENGLFUNCTIONS_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPageLayout::Unit >() { return SbkPySide6_QtGuiTypes[SBK_QPAGELAYOUT_UNIT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QPageLayout::Orientation >() { return SbkPySide6_QtGuiTypes[SBK_QPAGELAYOUT_ORIENTATION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QPageLayout::Mode >() { return SbkPySide6_QtGuiTypes[SBK_QPAGELAYOUT_MODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QPageLayout >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QPAGELAYOUT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPageRanges >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QPAGERANGES_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPageRanges::Range >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QPAGERANGES_RANGE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPageSize::PageSizeId >() { return SbkPySide6_QtGuiTypes[SBK_QPAGESIZE_PAGESIZEID_IDX]; }
template<> inline PyTypeObject *SbkType< ::QPageSize::Unit >() { return SbkPySide6_QtGuiTypes[SBK_QPAGESIZE_UNIT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QPageSize::SizeMatchPolicy >() { return SbkPySide6_QtGuiTypes[SBK_QPAGESIZE_SIZEMATCHPOLICY_IDX]; }
template<> inline PyTypeObject *SbkType< ::QPageSize >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QPAGESIZE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPagedPaintDevice::PdfVersion >() { return SbkPySide6_QtGuiTypes[SBK_QPAGEDPAINTDEVICE_PDFVERSION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QPagedPaintDevice >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QPAGEDPAINTDEVICE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPaintDevice::PaintDeviceMetric >() { return SbkPySide6_QtGuiTypes[SBK_QPAINTDEVICE_PAINTDEVICEMETRIC_IDX]; }
template<> inline PyTypeObject *SbkType< ::QPaintDevice >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QPAINTDEVICE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPaintDeviceWindow >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QPAINTDEVICEWINDOW_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPaintEngine::PaintEngineFeature >() { return SbkPySide6_QtGuiTypes[SBK_QPAINTENGINE_PAINTENGINEFEATURE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QPaintEngine::PaintEngineFeature> >() { return SbkPySide6_QtGuiTypes[SBK_QFLAGS_QPAINTENGINE_PAINTENGINEFEATURE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QPaintEngine::DirtyFlag >() { return SbkPySide6_QtGuiTypes[SBK_QPAINTENGINE_DIRTYFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QPaintEngine::DirtyFlag> >() { return SbkPySide6_QtGuiTypes[SBK_QFLAGS_QPAINTENGINE_DIRTYFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QPaintEngine::PolygonDrawMode >() { return SbkPySide6_QtGuiTypes[SBK_QPAINTENGINE_POLYGONDRAWMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QPaintEngine::Type >() { return SbkPySide6_QtGuiTypes[SBK_QPAINTENGINE_TYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QPaintEngine >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QPAINTENGINE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPaintEngineState >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QPAINTENGINESTATE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPaintEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QPAINTEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPainter::RenderHint >() { return SbkPySide6_QtGuiTypes[SBK_QPAINTER_RENDERHINT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QPainter::RenderHint> >() { return SbkPySide6_QtGuiTypes[SBK_QFLAGS_QPAINTER_RENDERHINT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QPainter::PixmapFragmentHint >() { return SbkPySide6_QtGuiTypes[SBK_QPAINTER_PIXMAPFRAGMENTHINT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QPainter::PixmapFragmentHint> >() { return SbkPySide6_QtGuiTypes[SBK_QFLAGS_QPAINTER_PIXMAPFRAGMENTHINT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QPainter::CompositionMode >() { return SbkPySide6_QtGuiTypes[SBK_QPAINTER_COMPOSITIONMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QPainter >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QPAINTER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPainter::PixmapFragment >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QPAINTER_PIXMAPFRAGMENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPainterPath::ElementType >() { return SbkPySide6_QtGuiTypes[SBK_QPAINTERPATH_ELEMENTTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QPainterPath >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QPAINTERPATH_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPainterPath::Element >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QPAINTERPATH_ELEMENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPainterPathStroker >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QPAINTERPATHSTROKER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPalette::ColorGroup >() { return SbkPySide6_QtGuiTypes[SBK_QPALETTE_COLORGROUP_IDX]; }
template<> inline PyTypeObject *SbkType< ::QPalette::ColorRole >() { return SbkPySide6_QtGuiTypes[SBK_QPALETTE_COLORROLE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QPalette >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QPALETTE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPdfWriter >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QPDFWRITER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPen >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QPEN_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPicture >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QPICTURE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPixelFormat::ColorModel >() { return SbkPySide6_QtGuiTypes[SBK_QPIXELFORMAT_COLORMODEL_IDX]; }
template<> inline PyTypeObject *SbkType< ::QPixelFormat::AlphaUsage >() { return SbkPySide6_QtGuiTypes[SBK_QPIXELFORMAT_ALPHAUSAGE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QPixelFormat::AlphaPosition >() { return SbkPySide6_QtGuiTypes[SBK_QPIXELFORMAT_ALPHAPOSITION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QPixelFormat::AlphaPremultiplied >() { return SbkPySide6_QtGuiTypes[SBK_QPIXELFORMAT_ALPHAPREMULTIPLIED_IDX]; }
template<> inline PyTypeObject *SbkType< ::QPixelFormat::TypeInterpretation >() { return SbkPySide6_QtGuiTypes[SBK_QPIXELFORMAT_TYPEINTERPRETATION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QPixelFormat::YUVLayout >() { return SbkPySide6_QtGuiTypes[SBK_QPIXELFORMAT_YUVLAYOUT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QPixelFormat::ByteOrder >() { return SbkPySide6_QtGuiTypes[SBK_QPIXELFORMAT_BYTEORDER_IDX]; }
template<> inline PyTypeObject *SbkType< ::QPixelFormat >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QPIXELFORMAT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPixmap >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QPIXMAP_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPixmapCache >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QPIXMAPCACHE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPixmapCache::Key >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QPIXMAPCACHE_KEY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPlatformSurfaceEvent::SurfaceEventType >() { return SbkPySide6_QtGuiTypes[SBK_QPLATFORMSURFACEEVENT_SURFACEEVENTTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QPlatformSurfaceEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QPLATFORMSURFACEEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPointerEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QPOINTEREVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPointingDevice::PointerType >() { return SbkPySide6_QtGuiTypes[SBK_QPOINTINGDEVICE_POINTERTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QPointingDevice::PointerType> >() { return SbkPySide6_QtGuiTypes[SBK_QFLAGS_QPOINTINGDEVICE_POINTERTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QPointingDevice::GrabTransition >() { return SbkPySide6_QtGuiTypes[SBK_QPOINTINGDEVICE_GRABTRANSITION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QPointingDevice >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QPOINTINGDEVICE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPointingDeviceUniqueId >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QPOINTINGDEVICEUNIQUEID_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPolygon >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QPOLYGON_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPolygonF >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QPOLYGONF_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPyTextObject >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QPYTEXTOBJECT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QQuaternion >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QQUATERNION_IDX]); }
template<> inline PyTypeObject *SbkType< ::QRadialGradient >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QRADIALGRADIENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QRasterWindow >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QRASTERWINDOW_IDX]); }
template<> inline PyTypeObject *SbkType< ::QRawFont::AntialiasingType >() { return SbkPySide6_QtGuiTypes[SBK_QRAWFONT_ANTIALIASINGTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QRawFont::LayoutFlag >() { return SbkPySide6_QtGuiTypes[SBK_QRAWFONT_LAYOUTFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QRawFont::LayoutFlag> >() { return SbkPySide6_QtGuiTypes[SBK_QFLAGS_QRAWFONT_LAYOUTFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QRawFont >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QRAWFONT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QRegion::RegionType >() { return SbkPySide6_QtGuiTypes[SBK_QREGION_REGIONTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QRegion >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QREGION_IDX]); }
template<> inline PyTypeObject *SbkType< ::QRegularExpressionValidator >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QREGULAREXPRESSIONVALIDATOR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QResizeEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QRESIZEEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QScreen >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QSCREEN_IDX]); }
template<> inline PyTypeObject *SbkType< ::QScrollEvent::ScrollState >() { return SbkPySide6_QtGuiTypes[SBK_QSCROLLEVENT_SCROLLSTATE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QScrollEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QSCROLLEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QScrollPrepareEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QSCROLLPREPAREEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QSessionManager::RestartHint >() { return SbkPySide6_QtGuiTypes[SBK_QSESSIONMANAGER_RESTARTHINT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QSessionManager >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QSESSIONMANAGER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QShortcut >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QSHORTCUT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QShortcutEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QSHORTCUTEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QShowEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QSHOWEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QSinglePointEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QSINGLEPOINTEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QStandardItem::ItemType >() { return SbkPySide6_QtGuiTypes[SBK_QSTANDARDITEM_ITEMTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QStandardItem >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QSTANDARDITEM_IDX]); }
template<> inline PyTypeObject *SbkType< ::QStandardItemModel >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QSTANDARDITEMMODEL_IDX]); }
template<> inline PyTypeObject *SbkType< ::QStaticText::PerformanceHint >() { return SbkPySide6_QtGuiTypes[SBK_QSTATICTEXT_PERFORMANCEHINT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QStaticText >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QSTATICTEXT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QStatusTipEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QSTATUSTIPEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QStyleHints >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QSTYLEHINTS_IDX]); }
template<> inline PyTypeObject *SbkType< ::QSurface::SurfaceClass >() { return SbkPySide6_QtGuiTypes[SBK_QSURFACE_SURFACECLASS_IDX]; }
template<> inline PyTypeObject *SbkType< ::QSurface::SurfaceType >() { return SbkPySide6_QtGuiTypes[SBK_QSURFACE_SURFACETYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QSurface >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QSURFACE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QSurfaceFormat::FormatOption >() { return SbkPySide6_QtGuiTypes[SBK_QSURFACEFORMAT_FORMATOPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QSurfaceFormat::FormatOption> >() { return SbkPySide6_QtGuiTypes[SBK_QFLAGS_QSURFACEFORMAT_FORMATOPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QSurfaceFormat::SwapBehavior >() { return SbkPySide6_QtGuiTypes[SBK_QSURFACEFORMAT_SWAPBEHAVIOR_IDX]; }
template<> inline PyTypeObject *SbkType< ::QSurfaceFormat::RenderableType >() { return SbkPySide6_QtGuiTypes[SBK_QSURFACEFORMAT_RENDERABLETYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QSurfaceFormat::OpenGLContextProfile >() { return SbkPySide6_QtGuiTypes[SBK_QSURFACEFORMAT_OPENGLCONTEXTPROFILE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QSurfaceFormat::ColorSpace >() { return SbkPySide6_QtGuiTypes[SBK_QSURFACEFORMAT_COLORSPACE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QSurfaceFormat >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QSURFACEFORMAT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QSyntaxHighlighter >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QSYNTAXHIGHLIGHTER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTabletEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTABLETEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextBlock >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTBLOCK_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextBlock::iterator >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTBLOCK_ITERATOR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextBlockFormat::LineHeightTypes >() { return SbkPySide6_QtGuiTypes[SBK_QTEXTBLOCKFORMAT_LINEHEIGHTTYPES_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextBlockFormat::MarkerType >() { return SbkPySide6_QtGuiTypes[SBK_QTEXTBLOCKFORMAT_MARKERTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextBlockFormat >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTBLOCKFORMAT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextBlockGroup >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTBLOCKGROUP_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextBlockUserData >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTBLOCKUSERDATA_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextCharFormat::VerticalAlignment >() { return SbkPySide6_QtGuiTypes[SBK_QTEXTCHARFORMAT_VERTICALALIGNMENT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextCharFormat::UnderlineStyle >() { return SbkPySide6_QtGuiTypes[SBK_QTEXTCHARFORMAT_UNDERLINESTYLE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextCharFormat::FontPropertiesInheritanceBehavior >() { return SbkPySide6_QtGuiTypes[SBK_QTEXTCHARFORMAT_FONTPROPERTIESINHERITANCEBEHAVIOR_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextCharFormat >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTCHARFORMAT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextCursor::MoveMode >() { return SbkPySide6_QtGuiTypes[SBK_QTEXTCURSOR_MOVEMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextCursor::MoveOperation >() { return SbkPySide6_QtGuiTypes[SBK_QTEXTCURSOR_MOVEOPERATION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextCursor::SelectionType >() { return SbkPySide6_QtGuiTypes[SBK_QTEXTCURSOR_SELECTIONTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextCursor >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTCURSOR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextDocument::MetaInformation >() { return SbkPySide6_QtGuiTypes[SBK_QTEXTDOCUMENT_METAINFORMATION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextDocument::MarkdownFeature >() { return SbkPySide6_QtGuiTypes[SBK_QTEXTDOCUMENT_MARKDOWNFEATURE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QTextDocument::MarkdownFeature> >() { return SbkPySide6_QtGuiTypes[SBK_QFLAGS_QTEXTDOCUMENT_MARKDOWNFEATURE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextDocument::FindFlag >() { return SbkPySide6_QtGuiTypes[SBK_QTEXTDOCUMENT_FINDFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QTextDocument::FindFlag> >() { return SbkPySide6_QtGuiTypes[SBK_QFLAGS_QTEXTDOCUMENT_FINDFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextDocument::ResourceType >() { return SbkPySide6_QtGuiTypes[SBK_QTEXTDOCUMENT_RESOURCETYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextDocument::Stacks >() { return SbkPySide6_QtGuiTypes[SBK_QTEXTDOCUMENT_STACKS_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextDocument >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTDOCUMENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextDocumentFragment >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTDOCUMENTFRAGMENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextDocumentWriter >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTDOCUMENTWRITER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextFormat::FormatType >() { return SbkPySide6_QtGuiTypes[SBK_QTEXTFORMAT_FORMATTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextFormat::Property >() { return SbkPySide6_QtGuiTypes[SBK_QTEXTFORMAT_PROPERTY_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextFormat::ObjectTypes >() { return SbkPySide6_QtGuiTypes[SBK_QTEXTFORMAT_OBJECTTYPES_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextFormat::PageBreakFlag >() { return SbkPySide6_QtGuiTypes[SBK_QTEXTFORMAT_PAGEBREAKFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QTextFormat::PageBreakFlag> >() { return SbkPySide6_QtGuiTypes[SBK_QFLAGS_QTEXTFORMAT_PAGEBREAKFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextFormat >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTFORMAT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextFragment >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTFRAGMENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextFrame >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTFRAME_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextFrame::iterator >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTFRAME_ITERATOR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextFrameFormat::Position >() { return SbkPySide6_QtGuiTypes[SBK_QTEXTFRAMEFORMAT_POSITION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextFrameFormat::BorderStyle >() { return SbkPySide6_QtGuiTypes[SBK_QTEXTFRAMEFORMAT_BORDERSTYLE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextFrameFormat >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTFRAMEFORMAT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextImageFormat >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTIMAGEFORMAT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextInlineObject >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTINLINEOBJECT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextItem::RenderFlag >() { return SbkPySide6_QtGuiTypes[SBK_QTEXTITEM_RENDERFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QTextItem::RenderFlag> >() { return SbkPySide6_QtGuiTypes[SBK_QFLAGS_QTEXTITEM_RENDERFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextItem >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTITEM_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextLayout::CursorMode >() { return SbkPySide6_QtGuiTypes[SBK_QTEXTLAYOUT_CURSORMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextLayout >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTLAYOUT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextLayout::FormatRange >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTLAYOUT_FORMATRANGE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextLength::Type >() { return SbkPySide6_QtGuiTypes[SBK_QTEXTLENGTH_TYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextLength >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTLENGTH_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextLine::Edge >() { return SbkPySide6_QtGuiTypes[SBK_QTEXTLINE_EDGE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextLine::CursorPosition >() { return SbkPySide6_QtGuiTypes[SBK_QTEXTLINE_CURSORPOSITION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextLine >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTLINE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextList >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTLIST_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextListFormat::Style >() { return SbkPySide6_QtGuiTypes[SBK_QTEXTLISTFORMAT_STYLE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextListFormat >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTLISTFORMAT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextObject >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTOBJECT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextObjectInterface >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTOBJECTINTERFACE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextOption::TabType >() { return SbkPySide6_QtGuiTypes[SBK_QTEXTOPTION_TABTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextOption::WrapMode >() { return SbkPySide6_QtGuiTypes[SBK_QTEXTOPTION_WRAPMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextOption::Flag >() { return SbkPySide6_QtGuiTypes[SBK_QTEXTOPTION_FLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QTextOption::Flag> >() { return SbkPySide6_QtGuiTypes[SBK_QFLAGS_QTEXTOPTION_FLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextOption >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTOPTION_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextOption::Tab >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTOPTION_TAB_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextTable >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTTABLE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextTableCell >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTTABLECELL_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextTableCellFormat >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTTABLECELLFORMAT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextTableFormat >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTEXTTABLEFORMAT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QToolBarChangeEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTOOLBARCHANGEEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTouchEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTOUCHEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTransform::TransformationType >() { return SbkPySide6_QtGuiTypes[SBK_QTRANSFORM_TRANSFORMATIONTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTransform >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QTRANSFORM_IDX]); }
template<> inline PyTypeObject *SbkType< ::QUndoCommand >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QUNDOCOMMAND_IDX]); }
template<> inline PyTypeObject *SbkType< ::QUndoGroup >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QUNDOGROUP_IDX]); }
template<> inline PyTypeObject *SbkType< ::QUndoStack >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QUNDOSTACK_IDX]); }
template<> inline PyTypeObject *SbkType< ::QValidator::State >() { return SbkPySide6_QtGuiTypes[SBK_QVALIDATOR_STATE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QValidator >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QVALIDATOR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QVector2D >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QVECTOR2D_IDX]); }
template<> inline PyTypeObject *SbkType< ::QVector3D >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QVECTOR3D_IDX]); }
template<> inline PyTypeObject *SbkType< ::QVector4D >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QVECTOR4D_IDX]); }
template<> inline PyTypeObject *SbkType< ::QWhatsThisClickedEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QWHATSTHISCLICKEDEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QWheelEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QWHEELEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QWindow::Visibility >() { return SbkPySide6_QtGuiTypes[SBK_QWINDOW_VISIBILITY_IDX]; }
template<> inline PyTypeObject *SbkType< ::QWindow::AncestorMode >() { return SbkPySide6_QtGuiTypes[SBK_QWINDOW_ANCESTORMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QWindow >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QWINDOW_IDX]); }
template<> inline PyTypeObject *SbkType< ::QWindowStateChangeEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtGuiTypes[SBK_QWINDOWSTATECHANGEEVENT_IDX]); }
QT_WARNING_POP

} // namespace Shiboken

#endif // SBK_QTGUI_PYTHON_H

