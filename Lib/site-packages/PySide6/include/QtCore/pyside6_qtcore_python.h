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


#ifndef SBK_QTCORE_PYTHON_H
#define SBK_QTCORE_PYTHON_H

#include <sbkpython.h>
#include <sbkconverter.h>
// Bound library includes
#include <qpropertyanimation.h>
#include <qresource.h>
#include <qsignalmapper.h>
#include <qstringconverter.h>
#include <qtransposeproxymodel.h>
#include <qsocketnotifier.h>
#include <qtimezone.h>
#include <qcborstreamreader.h>
#include <qfutureinterface.h>
#include <qsemaphore.h>
#include <qlocale.h>
#include <qsortfilterproxymodel.h>
#include <qidentityproxymodel.h>
#include <qjsonarray.h>
#include <qabstractnativeeventfilter.h>
#include <qlibraryinfo.h>
#include <qabstracteventdispatcher.h>
#include <qmetaobject.h>
#include <qtemporarydir.h>
#include <qjsonvalue.h>
#include <qmessageauthenticationcode.h>
#include <qfileinfo.h>
#include <qtextstream.h>
#include <qstorageinfo.h>
#include <QtCore/qbuffer.h>
#include <qpoint.h>
#include <qfile.h>
#include <qcalendar.h>
#include <qfileselector.h>
#include <qmimetype.h>
#include <qpauseanimation.h>
#include <qurlquery.h>
#include <qcommandlineoption.h>
#include <qdatastream.h>
#include <qbitarray.h>
#include <qbasictimer.h>
#include <qdeadlinetimer.h>
#include <qanimationgroup.h>
#include <qtextboundaryfinder.h>
#include <qwineventnotifier.h>
#include <qbytearraymatcher.h>
#include <qlogging.h>
#include <qreadwritelock.h>
#include <qmargins.h>
#include <qcborstream.h>
#include <quuid.h>
#include <qeventloop.h>
#include <qstringlistmodel.h>
#include <qdatetime.h>
#include <QList>
#include <qcollator.h>
#include <qsettings.h>
#include <qmimedatabase.h>
#include <qabstractitemmodel.h>
#include <qcommandlineparser.h>
#include <qwaitcondition.h>
#include <qdir.h>
#include <qabstractanimation.h>
#include <qrunnable.h>
#include <qlockfile.h>
#include <qtcorehelper.h>
#include <qabstractproxymodel.h>
#include <qrandom.h>
#include <qprocess.h>
#include <qtemporaryfile.h>
#include <qcryptographichash.h>
#include <qfiledevice.h>
#include <qtimer.h>
#include <qsequentialanimationgroup.h>
#include <qbytearray.h>
#include <qeasingcurve.h>
#include <qnamespace.h>
#include <qcoreevent.h>
#include <qvariantanimation.h>
#include <qfilesystemwatcher.h>
#include <qsavefile.h>
#include <qxmlstream.h>
#include <qstandardpaths.h>
#include <qobject.h>
#include <qline.h>
#include <qlibrary.h>
#include <qpluginloader.h>
#include <qiodevice.h>
#include <qfactoryinterface.h>
#include <qitemselectionmodel.h>
#include <qtimeline.h>
#include <qcoreapplication.h>
#include <qrect.h>
#include <qelapsedtimer.h>
#include <qdiriterator.h>
#include <qurl.h>
#include <qthread.h>
#include <qmetatype.h>
#include <qmutex.h>
#include <qmimedata.h>
#include <qversionnumber.h>
#include <qcbormap.h>
#include <qsystemsemaphore.h>
#include <qtranslator.h>
#include <qiodevicebase.h>
#include <qthreadpool.h>
#include <qcborvalue.h>
#include <qcborcommon.h>
#include <qconcatenatetablesproxymodel.h>
#include <qoperatingsystemversion.h>
#include <qobjectdefs.h>
#include <qsysinfo.h>
#include <qcborarray.h>
#include <qparallelanimationgroup.h>
#include <qjsondocument.h>
#include <qregularexpression.h>
#include <qsize.h>
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
    SBK_QABSTRACTANIMATION_DIRECTION_IDX                     = 2,
    SBK_QABSTRACTANIMATION_STATE_IDX                         = 3,
    SBK_QABSTRACTANIMATION_DELETIONPOLICY_IDX                = 1,
    SBK_QABSTRACTANIMATION_IDX                               = 0,
    SBK_QABSTRACTEVENTDISPATCHER_IDX                         = 4,
    SBK_QABSTRACTEVENTDISPATCHER_TIMERINFO_IDX               = 5,
    SBK_QABSTRACTITEMMODEL_LAYOUTCHANGEHINT_IDX              = 8,
    SBK_QABSTRACTITEMMODEL_CHECKINDEXOPTION_IDX              = 7,
    SBK_QFLAGS_QABSTRACTITEMMODEL_CHECKINDEXOPTION_IDX       = 90,
    SBK_QABSTRACTITEMMODEL_IDX                               = 6,
    SBK_QABSTRACTLISTMODEL_IDX                               = 9,
    SBK_QABSTRACTNATIVEEVENTFILTER_IDX                       = 10,
    SBK_QABSTRACTPROXYMODEL_IDX                              = 11,
    SBK_QABSTRACTTABLEMODEL_IDX                              = 12,
    SBK_QANIMATIONGROUP_IDX                                  = 13,
    SBK_QBASICMUTEX_IDX                                      = 14,
    SBK_QBASICTIMER_IDX                                      = 15,
    SBK_QBITARRAY_IDX                                        = 16,
    SBK_QBUFFER_IDX                                          = 17,
    SBK_QBYTEARRAY_BASE64OPTION_IDX                          = 20,
    SBK_QFLAGS_QBYTEARRAY_BASE64OPTION_IDX                   = 91,
    SBK_QBYTEARRAY_BASE64DECODINGSTATUS_IDX                  = 19,
    SBK_QBYTEARRAY_IDX                                       = 18,
    SBK_QBYTEARRAY_FROMBASE64RESULT_IDX                      = 21,
    SBK_QBYTEARRAYMATCHER_IDX                                = 22,
    SBK_QCALENDAR_SYSTEM_IDX                                 = 24,
    SBK_QCALENDAR_IDX                                        = 23,
    SBK_QCALENDAR_YEARMONTHDAY_IDX                           = 25,
    SBK_QCBORARRAY_IDX                                       = 26,
    SBK_QCBORERROR_CODE_IDX                                  = 28,
    SBK_QCBORERROR_IDX                                       = 27,
    SBK_QCBORMAP_IDX                                         = 30,
    SBK_QCBORPARSERERROR_IDX                                 = 31,
    SBK_QCBORSTREAMREADER_TYPE_IDX                           = 35,
    SBK_QCBORSTREAMREADER_STRINGRESULTCODE_IDX               = 34,
    SBK_QCBORSTREAMREADER_IDX                                = 33,
    SBK_QCBORSTREAMWRITER_IDX                                = 36,
    SBK_QCBORSTRINGRESULTBYTEARRAY_IDX                       = 37,
    SBK_QCBORSTREAMREADER_STRINGRESULT_QBYTEARRAY_IDX        = 37,
    SBK_QCBORSTRINGRESULTSTRING_IDX                          = 38,
    SBK_QCBORSTREAMREADER_STRINGRESULT_QSTRING_IDX           = 38,
    SBK_QCBORVALUE_ENCODINGOPTION_IDX                        = 41,
    SBK_QFLAGS_QCBORVALUE_ENCODINGOPTION_IDX                 = 93,
    SBK_QCBORVALUE_DIAGNOSTICNOTATIONOPTION_IDX              = 40,
    SBK_QFLAGS_QCBORVALUE_DIAGNOSTICNOTATIONOPTION_IDX       = 92,
    SBK_QCBORVALUE_TYPE_IDX                                  = 42,
    SBK_QCBORVALUE_IDX                                       = 39,
    SBK_QCHILDEVENT_IDX                                      = 43,
    SBK_QCOLLATOR_IDX                                        = 44,
    SBK_QCOLLATORSORTKEY_IDX                                 = 45,
    SBK_QCOMMANDLINEOPTION_FLAG_IDX                          = 47,
    SBK_QFLAGS_QCOMMANDLINEOPTION_FLAG_IDX                   = 94,
    SBK_QCOMMANDLINEOPTION_IDX                               = 46,
    SBK_QCOMMANDLINEPARSER_SINGLEDASHWORDOPTIONMODE_IDX      = 50,
    SBK_QCOMMANDLINEPARSER_OPTIONSAFTERPOSITIONALARGUMENTSMODE_IDX = 49,
    SBK_QCOMMANDLINEPARSER_IDX                               = 48,
    SBK_QCONCATENATETABLESPROXYMODEL_IDX                     = 51,
    SBK_QCOREAPPLICATION_APPLICATIONFLAGS_IDX                = 414,
    SBK_QCOREAPPLICATION_IDX                                 = 52,
    SBK_QCRYPTOGRAPHICHASH_ALGORITHM_IDX                     = 54,
    SBK_QCRYPTOGRAPHICHASH_IDX                               = 53,
    SBK_QDATASTREAM_VERSION_IDX                              = 59,
    SBK_QDATASTREAM_BYTEORDER_IDX                            = 56,
    SBK_QDATASTREAM_STATUS_IDX                               = 58,
    SBK_QDATASTREAM_FLOATINGPOINTPRECISION_IDX               = 57,
    SBK_QDATASTREAM_IDX                                      = 55,
    SBK_QDATE_IDX                                            = 60,
    SBK_QDATETIME_YEARRANGE_IDX                              = 62,
    SBK_QDATETIME_IDX                                        = 61,
    SBK_QDEADLINETIMER_FOREVERCONSTANT_IDX                   = 64,
    SBK_QDEADLINETIMER_IDX                                   = 63,
    SBK_QDIR_FILTER_IDX                                      = 66,
    SBK_QFLAGS_QDIR_FILTER_IDX                               = 95,
    SBK_QDIR_SORTFLAG_IDX                                    = 67,
    SBK_QFLAGS_QDIR_SORTFLAG_IDX                             = 96,
    SBK_QDIR_IDX                                             = 65,
    SBK_QDIRITERATOR_ITERATORFLAG_IDX                        = 69,
    SBK_QFLAGS_QDIRITERATOR_ITERATORFLAG_IDX                 = 97,
    SBK_QDIRITERATOR_IDX                                     = 68,
    SBK_QDYNAMICPROPERTYCHANGEEVENT_IDX                      = 70,
    SBK_QEASINGCURVE_TYPE_IDX                                = 72,
    SBK_QEASINGCURVE_IDX                                     = 71,
    SBK_QELAPSEDTIMER_CLOCKTYPE_IDX                          = 74,
    SBK_QELAPSEDTIMER_IDX                                    = 73,
    SBK_QEVENT_TYPE_IDX                                      = 76,
    SBK_QEVENT_IDX                                           = 75,
    SBK_QEVENTLOOP_PROCESSEVENTSFLAG_IDX                     = 78,
    SBK_QFLAGS_QEVENTLOOP_PROCESSEVENTSFLAG_IDX              = 98,
    SBK_QEVENTLOOP_IDX                                       = 77,
    SBK_QFACTORYINTERFACE_IDX                                = 79,
    SBK_QFILE_IDX                                            = 80,
    SBK_QFILEDEVICE_FILEERROR_IDX                            = 82,
    SBK_QFILEDEVICE_FILETIME_IDX                             = 84,
    SBK_QFILEDEVICE_PERMISSION_IDX                           = 86,
    SBK_QFLAGS_QFILEDEVICE_PERMISSION_IDX                    = 101,
    SBK_QFILEDEVICE_FILEHANDLEFLAG_IDX                       = 83,
    SBK_QFLAGS_QFILEDEVICE_FILEHANDLEFLAG_IDX                = 99,
    SBK_QFILEDEVICE_MEMORYMAPFLAG_IDX                        = 85,
    SBK_QFLAGS_QFILEDEVICE_MEMORYMAPFLAG_IDX                 = 100,
    SBK_QFILEDEVICE_IDX                                      = 81,
    SBK_QFILEINFO_IDX                                        = 87,
    SBK_QFILESELECTOR_IDX                                    = 88,
    SBK_QFILESYSTEMWATCHER_IDX                               = 89,
    SBK_QFUTUREINTERFACEBASE_STATE_IDX                       = 142,
    SBK_QFUTUREINTERFACEBASE_IDX                             = 141,
    SBK_QGENERICARGUMENT_IDX                                 = 143,
    SBK_QGENERICRETURNARGUMENT_IDX                           = 144,
    SBK_QIODEVICE_IDX                                        = 145,
    SBK_QIODEVICEBASE_OPENMODEFLAG_IDX                       = 147,
    SBK_QFLAGS_QIODEVICEBASE_OPENMODEFLAG_IDX                = 102,
    SBK_QIODEVICEBASE_IDX                                    = 146,
    SBK_QIDENTITYPROXYMODEL_IDX                              = 148,
    SBK_QITEMSELECTION_IDX                                   = 149,
    SBK_QITEMSELECTIONMODEL_SELECTIONFLAG_IDX                = 151,
    SBK_QFLAGS_QITEMSELECTIONMODEL_SELECTIONFLAG_IDX         = 103,
    SBK_QITEMSELECTIONMODEL_IDX                              = 150,
    SBK_QITEMSELECTIONRANGE_IDX                              = 152,
    SBK_QJSONARRAY_IDX                                       = 153,
    SBK_QJSONDOCUMENT_JSONFORMAT_IDX                         = 155,
    SBK_QJSONDOCUMENT_IDX                                    = 154,
    SBK_QJSONPARSEERROR_PARSEERROR_IDX                       = 157,
    SBK_QJSONPARSEERROR_IDX                                  = 156,
    SBK_QJSONVALUE_TYPE_IDX                                  = 159,
    SBK_QJSONVALUE_IDX                                       = 158,
    SBK_QKEYCOMBINATION_IDX                                  = 160,
    SBK_QLIBRARY_LOADHINT_IDX                                = 162,
    SBK_QFLAGS_QLIBRARY_LOADHINT_IDX                         = 104,
    SBK_QLIBRARY_IDX                                         = 161,
    SBK_QLIBRARYINFO_LIBRARYPATH_IDX                         = 164,
    SBK_QLIBRARYINFO_IDX                                     = 163,
    SBK_QLINE_IDX                                            = 165,
    SBK_QLINEF_INTERSECTIONTYPE_IDX                          = 167,
    SBK_QLINEF_IDX                                           = 166,
    SBK_QLOCALE_LANGUAGE_IDX                                 = 173,
    SBK_QLOCALE_SCRIPT_IDX                                   = 417,
    SBK_QLOCALE_COUNTRY_IDX                                  = 169,
    SBK_QLOCALE_MEASUREMENTSYSTEM_IDX                        = 174,
    SBK_QLOCALE_FORMATTYPE_IDX                               = 172,
    SBK_QLOCALE_NUMBEROPTION_IDX                             = 175,
    SBK_QFLAGS_QLOCALE_NUMBEROPTION_IDX                      = 106,
    SBK_QLOCALE_FLOATINGPOINTPRECISIONOPTION_IDX             = 171,
    SBK_QLOCALE_CURRENCYSYMBOLFORMAT_IDX                     = 415,
    SBK_QLOCALE_DATASIZEFORMAT_IDX                           = 170,
    SBK_QFLAGS_QLOCALE_DATASIZEFORMAT_IDX                    = 105,
    SBK_QLOCALE_QUOTATIONSTYLE_IDX                           = 416,
    SBK_QLOCALE_IDX                                          = 168,
    SBK_QLOCKFILE_LOCKERROR_IDX                              = 177,
    SBK_QLOCKFILE_IDX                                        = 176,
    SBK_QMARGINS_IDX                                         = 178,
    SBK_QMARGINSF_IDX                                        = 179,
    SBK_QMESSAGEAUTHENTICATIONCODE_IDX                       = 180,
    SBK_QMESSAGELOGCONTEXT_IDX                               = 181,
    SBK_QMETACLASSINFO_IDX                                   = 182,
    SBK_QMETAENUM_IDX                                        = 183,
    SBK_QMETAMETHOD_ACCESS_IDX                               = 185,
    SBK_QMETAMETHOD_METHODTYPE_IDX                           = 186,
    SBK_QMETAMETHOD_IDX                                      = 184,
    SBK_QMETAOBJECT_CALL_IDX                                 = 188,
    SBK_QMETAOBJECT_IDX                                      = 187,
    SBK_QMETAOBJECT_CONNECTION_IDX                           = 189,
    SBK_QMETAPROPERTY_IDX                                    = 190,
    SBK_QMETATYPE_TYPE_IDX                                   = 192,
    SBK_QMETATYPE_TYPEFLAG_IDX                               = 193,
    SBK_QFLAGS_QMETATYPE_TYPEFLAG_IDX                        = 107,
    SBK_QMETATYPE_IDX                                        = 191,
    SBK_QMIMEDATA_IDX                                        = 194,
    SBK_QMIMEDATABASE_MATCHMODE_IDX                          = 196,
    SBK_QMIMEDATABASE_IDX                                    = 195,
    SBK_QMIMETYPE_IDX                                        = 197,
    SBK_QMODELINDEX_IDX                                      = 198,
    SBK_QMODELROLEDATA_IDX                                   = 199,
    SBK_QMUTEX_IDX                                           = 200,
    SBK_QOBJECT_IDX                                          = 201,
    SBK_QOPERATINGSYSTEMVERSION_OSTYPE_IDX                   = 203,
    SBK_QOPERATINGSYSTEMVERSION_IDX                          = 202,
    SBK_QPARALLELANIMATIONGROUP_IDX                          = 204,
    SBK_QPAUSEANIMATION_IDX                                  = 205,
    SBK_QPERSISTENTMODELINDEX_IDX                            = 206,
    SBK_QPLUGINLOADER_IDX                                    = 207,
    SBK_QPOINT_IDX                                           = 208,
    SBK_QPOINTF_IDX                                          = 209,
    SBK_QPROCESS_PROCESSERROR_IDX                            = 215,
    SBK_QPROCESS_PROCESSSTATE_IDX                            = 216,
    SBK_QPROCESS_PROCESSCHANNEL_IDX                          = 213,
    SBK_QPROCESS_PROCESSCHANNELMODE_IDX                      = 214,
    SBK_QPROCESS_INPUTCHANNELMODE_IDX                        = 212,
    SBK_QPROCESS_EXITSTATUS_IDX                              = 211,
    SBK_QPROCESS_IDX                                         = 210,
    SBK_QPROCESSENVIRONMENT_IDX                              = 217,
    SBK_QPROPERTYANIMATION_IDX                               = 218,
    SBK_QRANDOMGENERATOR_IDX                                 = 219,
    SBK_QRANDOMGENERATOR64_IDX                               = 220,
    SBK_QREADLOCKER_IDX                                      = 221,
    SBK_QREADWRITELOCK_RECURSIONMODE_IDX                     = 223,
    SBK_QREADWRITELOCK_IDX                                   = 222,
    SBK_QRECT_IDX                                            = 224,
    SBK_QRECTF_IDX                                           = 225,
    SBK_QRECURSIVEMUTEX_IDX                                  = 226,
    SBK_QREGULAREXPRESSION_PATTERNOPTION_IDX                 = 230,
    SBK_QFLAGS_QREGULAREXPRESSION_PATTERNOPTION_IDX          = 109,
    SBK_QREGULAREXPRESSION_MATCHTYPE_IDX                     = 229,
    SBK_QREGULAREXPRESSION_MATCHOPTION_IDX                   = 228,
    SBK_QFLAGS_QREGULAREXPRESSION_MATCHOPTION_IDX            = 108,
    SBK_QREGULAREXPRESSION_WILDCARDCONVERSIONOPTION_IDX      = 231,
    SBK_QFLAGS_QREGULAREXPRESSION_WILDCARDCONVERSIONOPTION_IDX = 110,
    SBK_QREGULAREXPRESSION_IDX                               = 227,
    SBK_QREGULAREXPRESSIONMATCH_IDX                          = 232,
    SBK_QREGULAREXPRESSIONMATCHITERATOR_IDX                  = 233,
    SBK_QRESOURCE_COMPRESSION_IDX                            = 235,
    SBK_QRESOURCE_IDX                                        = 234,
    SBK_QRUNNABLE_IDX                                        = 236,
    SBK_QSAVEFILE_IDX                                        = 237,
    SBK_QSEMAPHORE_IDX                                       = 238,
    SBK_QSEMAPHORERELEASER_IDX                               = 239,
    SBK_QSEQUENTIALANIMATIONGROUP_IDX                        = 240,
    SBK_QSETTINGS_STATUS_IDX                                 = 244,
    SBK_QSETTINGS_FORMAT_IDX                                 = 242,
    SBK_QSETTINGS_SCOPE_IDX                                  = 243,
    SBK_QSETTINGS_IDX                                        = 241,
    SBK_QSIGNALBLOCKER_IDX                                   = 245,
    SBK_QSIGNALMAPPER_IDX                                    = 246,
    SBK_QSIZE_IDX                                            = 247,
    SBK_QSIZEF_IDX                                           = 248,
    SBK_QSOCKETDESCRIPTOR_IDX                                = 249,
    SBK_QSOCKETNOTIFIER_TYPE_IDX                             = 251,
    SBK_QSOCKETNOTIFIER_IDX                                  = 250,
    SBK_QSORTFILTERPROXYMODEL_IDX                            = 252,
    SBK_QSTANDARDPATHS_STANDARDLOCATION_IDX                  = 255,
    SBK_QSTANDARDPATHS_LOCATEOPTION_IDX                      = 254,
    SBK_QFLAGS_QSTANDARDPATHS_LOCATEOPTION_IDX               = 111,
    SBK_QSTANDARDPATHS_IDX                                   = 253,
    SBK_QSTORAGEINFO_IDX                                     = 256,
    SBK_QSTRINGCONVERTER_ENCODING_IDX                        = 258,
    SBK_QSTRINGCONVERTER_IDX                                 = 257,
    SBK_QSTRINGCONVERTERBASE_FLAG_IDX                        = 260,
    SBK_QFLAGS_QSTRINGCONVERTERBASE_FLAG_IDX                 = 112,
    SBK_QSTRINGCONVERTERBASE_IDX                             = 259,
    SBK_QSTRINGCONVERTERBASE_STATE_IDX                       = 261,
    SBK_QSTRINGDECODER_IDX                                   = 262,
    SBK_QSTRINGENCODER_IDX                                   = 263,
    SBK_QSTRINGLISTMODEL_IDX                                 = 264,
    SBK_QSYSINFO_SIZES_IDX                                   = 267,
    SBK_QSYSINFO_ENDIAN_IDX                                  = 266,
    SBK_QSYSINFO_IDX                                         = 265,
    SBK_QSYSTEMSEMAPHORE_ACCESSMODE_IDX                      = 269,
    SBK_QSYSTEMSEMAPHORE_SYSTEMSEMAPHOREERROR_IDX            = 270,
    SBK_QSYSTEMSEMAPHORE_IDX                                 = 268,
    SBK_QTEMPORARYDIR_IDX                                    = 271,
    SBK_QTEMPORARYFILE_IDX                                   = 272,
    SBK_QTEXTBOUNDARYFINDER_BOUNDARYTYPE_IDX                 = 275,
    SBK_QTEXTBOUNDARYFINDER_BOUNDARYREASON_IDX               = 274,
    SBK_QFLAGS_QTEXTBOUNDARYFINDER_BOUNDARYREASON_IDX        = 113,
    SBK_QTEXTBOUNDARYFINDER_IDX                              = 273,
    SBK_QTEXTSTREAM_REALNUMBERNOTATION_IDX                   = 279,
    SBK_QTEXTSTREAM_FIELDALIGNMENT_IDX                       = 277,
    SBK_QTEXTSTREAM_STATUS_IDX                               = 280,
    SBK_QTEXTSTREAM_NUMBERFLAG_IDX                           = 278,
    SBK_QFLAGS_QTEXTSTREAM_NUMBERFLAG_IDX                    = 114,
    SBK_QTEXTSTREAM_IDX                                      = 276,
    SBK_QTEXTSTREAMMANIPULATOR_IDX                           = 281,
    SBK_QTHREAD_PRIORITY_IDX                                 = 283,
    SBK_QTHREAD_IDX                                          = 282,
    SBK_QTHREADPOOL_IDX                                      = 284,
    SBK_QTIME_IDX                                            = 285,
    SBK_QTIMELINE_STATE_IDX                                  = 288,
    SBK_QTIMELINE_DIRECTION_IDX                              = 287,
    SBK_QTIMELINE_IDX                                        = 286,
    SBK_QTIMEZONE_TIMETYPE_IDX                               = 292,
    SBK_QTIMEZONE_NAMETYPE_IDX                               = 290,
    SBK_QTIMEZONE_IDX                                        = 289,
    SBK_QTIMEZONE_OFFSETDATA_IDX                             = 291,
    SBK_QTIMER_IDX                                           = 293,
    SBK_QTIMEREVENT_IDX                                      = 294,
    SBK_QTRANSLATOR_IDX                                      = 295,
    SBK_QTRANSPOSEPROXYMODEL_IDX                             = 296,
    SBK_QURL_PARSINGMODE_IDX                                 = 299,
    SBK_QURL_URLFORMATTINGOPTION_IDX                         = 300,
    SBK_QURL_COMPONENTFORMATTINGOPTION_IDX                   = 298,
    SBK_QFLAGS_QURL_COMPONENTFORMATTINGOPTION_IDX            = 115,
    SBK_QURL_USERINPUTRESOLUTIONOPTION_IDX                   = 301,
    SBK_QFLAGS_QURL_USERINPUTRESOLUTIONOPTION_IDX            = 117,
    SBK_QURL_IDX                                             = 297,
    SBK_QURLQUERY_IDX                                        = 302,
    SBK_QUUID_VARIANT_IDX                                    = 305,
    SBK_QUUID_VERSION_IDX                                    = 306,
    SBK_QUUID_STRINGFORMAT_IDX                               = 304,
    SBK_QUUID_IDX                                            = 303,
    SBK_QVARIANTANIMATION_IDX                                = 307,
    SBK_QVERSIONNUMBER_IDX                                   = 308,
    SBK_QWAITCONDITION_IDX                                   = 309,
    SBK_QWINEVENTNOTIFIER_IDX                                = 310,
    SBK_QWRITELOCKER_IDX                                     = 311,
    SBK_QXMLSTREAMATTRIBUTE_IDX                              = 312,
    SBK_QXMLSTREAMATTRIBUTES_IDX                             = 313,
    SBK_QXMLSTREAMENTITYDECLARATION_IDX                      = 314,
    SBK_QXMLSTREAMENTITYRESOLVER_IDX                         = 315,
    SBK_QXMLSTREAMNAMESPACEDECLARATION_IDX                   = 316,
    SBK_QXMLSTREAMNOTATIONDECLARATION_IDX                    = 317,
    SBK_QXMLSTREAMREADER_TOKENTYPE_IDX                       = 321,
    SBK_QXMLSTREAMREADER_READELEMENTTEXTBEHAVIOUR_IDX        = 320,
    SBK_QXMLSTREAMREADER_ERROR_IDX                           = 319,
    SBK_QXMLSTREAMREADER_IDX                                 = 318,
    SBK_QXMLSTREAMWRITER_IDX                                 = 322,
    SBK_QT_GLOBALCOLOR_IDX                                   = 357,
    SBK_QT_MOUSEBUTTON_IDX                                   = 373,
    SBK_QFLAGS_QT_MOUSEBUTTON_IDX                            = 131,
    SBK_QT_ORIENTATION_IDX                                   = 378,
    SBK_QFLAGS_QT_ORIENTATION_IDX                            = 133,
    SBK_QT_FOCUSPOLICY_IDX                                   = 352,
    SBK_QT_TABFOCUSBEHAVIOR_IDX                              = 391,
    SBK_QT_SORTORDER_IDX                                     = 389,
    SBK_QT_SPLITBEHAVIORFLAGS_IDX                            = 390,
    SBK_QFLAGS_QT_SPLITBEHAVIORFLAGS_IDX                     = 135,
    SBK_QT_TILERULE_IDX                                      = 396,
    SBK_QT_ALIGNMENTFLAG_IDX                                 = 324,
    SBK_QFLAGS_QT_ALIGNMENTFLAG_IDX                          = 118,
    SBK_QT_TEXTFLAG_IDX                                      = 393,
    SBK_QT_TEXTELIDEMODE_IDX                                 = 392,
    SBK_QT_WHITESPACEMODE_IDX                                = 405,
    SBK_QT_HITTESTACCURACY_IDX                               = 359,
    SBK_QT_WINDOWTYPE_IDX                                    = 410,
    SBK_QFLAGS_QT_WINDOWTYPE_IDX                             = 140,
    SBK_QT_WINDOWSTATE_IDX                                   = 409,
    SBK_QFLAGS_QT_WINDOWSTATE_IDX                            = 139,
    SBK_QT_APPLICATIONSTATE_IDX                              = 327,
    SBK_QFLAGS_QT_APPLICATIONSTATE_IDX                       = 119,
    SBK_QT_SCREENORIENTATION_IDX                             = 383,
    SBK_QFLAGS_QT_SCREENORIENTATION_IDX                      = 134,
    SBK_QT_WIDGETATTRIBUTE_IDX                               = 406,
    SBK_QT_APPLICATIONATTRIBUTE_IDX                          = 326,
    SBK_QT_IMAGECONVERSIONFLAG_IDX                           = 360,
    SBK_QFLAGS_QT_IMAGECONVERSIONFLAG_IDX                    = 125,
    SBK_QT_BGMODE_IDX                                        = 331,
    SBK_QT_KEY_IDX                                           = 367,
    SBK_QT_KEYBOARDMODIFIER_IDX                              = 368,
    SBK_QFLAGS_QT_KEYBOARDMODIFIER_IDX                       = 129,
    SBK_QT_MODIFIER_IDX                                      = 372,
    SBK_QT_ARROWTYPE_IDX                                     = 328,
    SBK_QT_PENSTYLE_IDX                                      = 381,
    SBK_QT_PENCAPSTYLE_IDX                                   = 379,
    SBK_QT_PENJOINSTYLE_IDX                                  = 380,
    SBK_QT_BRUSHSTYLE_IDX                                    = 332,
    SBK_QT_SIZEMODE_IDX                                      = 388,
    SBK_QT_UIEFFECT_IDX                                      = 404,
    SBK_QT_CURSORSHAPE_IDX                                   = 341,
    SBK_QT_TEXTFORMAT_IDX                                    = 394,
    SBK_QT_ASPECTRATIOMODE_IDX                               = 329,
    SBK_QT_DOCKWIDGETAREA_IDX                                = 344,
    SBK_QFLAGS_QT_DOCKWIDGETAREA_IDX                         = 120,
    SBK_QT_DOCKWIDGETAREASIZES_IDX                           = 345,
    SBK_QT_TOOLBARAREA_IDX                                   = 399,
    SBK_QFLAGS_QT_TOOLBARAREA_IDX                            = 137,
    SBK_QT_TOOLBARAREASIZES_IDX                              = 400,
    SBK_QT_DATEFORMAT_IDX                                    = 342,
    SBK_QT_TIMESPEC_IDX                                      = 397,
    SBK_QT_DAYOFWEEK_IDX                                     = 343,
    SBK_QT_SCROLLBARPOLICY_IDX                               = 384,
    SBK_QT_CASESENSITIVITY_IDX                               = 333,
    SBK_QT_CORNER_IDX                                        = 340,
    SBK_QT_EDGE_IDX                                          = 347,
    SBK_QFLAGS_QT_EDGE_IDX                                   = 122,
    SBK_QT_CONNECTIONTYPE_IDX                                = 337,
    SBK_QT_SHORTCUTCONTEXT_IDX                               = 386,
    SBK_QT_FILLRULE_IDX                                      = 350,
    SBK_QT_MASKMODE_IDX                                      = 370,
    SBK_QT_CLIPOPERATION_IDX                                 = 336,
    SBK_QT_ITEMSELECTIONMODE_IDX                             = 365,
    SBK_QT_ITEMSELECTIONOPERATION_IDX                        = 366,
    SBK_QT_TRANSFORMATIONMODE_IDX                            = 403,
    SBK_QT_AXIS_IDX                                          = 330,
    SBK_QT_FOCUSREASON_IDX                                   = 353,
    SBK_QT_CONTEXTMENUPOLICY_IDX                             = 338,
    SBK_QT_INPUTMETHODQUERY_IDX                              = 362,
    SBK_QFLAGS_QT_INPUTMETHODQUERY_IDX                       = 127,
    SBK_QT_INPUTMETHODHINT_IDX                               = 361,
    SBK_QFLAGS_QT_INPUTMETHODHINT_IDX                        = 126,
    SBK_QT_ENTERKEYTYPE_IDX                                  = 348,
    SBK_QT_TOOLBUTTONSTYLE_IDX                               = 401,
    SBK_QT_LAYOUTDIRECTION_IDX                               = 369,
    SBK_QT_ANCHORPOINT_IDX                                   = 325,
    SBK_QT_FINDCHILDOPTION_IDX                               = 351,
    SBK_QFLAGS_QT_FINDCHILDOPTION_IDX                        = 123,
    SBK_QT_DROPACTION_IDX                                    = 346,
    SBK_QFLAGS_QT_DROPACTION_IDX                             = 121,
    SBK_QT_CHECKSTATE_IDX                                    = 334,
    SBK_QT_ITEMDATAROLE_IDX                                  = 363,
    SBK_QT_ITEMFLAG_IDX                                      = 364,
    SBK_QFLAGS_QT_ITEMFLAG_IDX                               = 128,
    SBK_QT_MATCHFLAG_IDX                                     = 371,
    SBK_QFLAGS_QT_MATCHFLAG_IDX                              = 130,
    SBK_QT_WINDOWMODALITY_IDX                                = 408,
    SBK_QT_TEXTINTERACTIONFLAG_IDX                           = 395,
    SBK_QFLAGS_QT_TEXTINTERACTIONFLAG_IDX                    = 136,
    SBK_QT_EVENTPRIORITY_IDX                                 = 349,
    SBK_QT_SIZEHINT_IDX                                      = 387,
    SBK_QT_WINDOWFRAMESECTION_IDX                            = 407,
    SBK_QT_COORDINATESYSTEM_IDX                              = 339,
    SBK_QT_TOUCHPOINTSTATE_IDX                               = 402,
    SBK_QFLAGS_QT_TOUCHPOINTSTATE_IDX                        = 138,
    SBK_QT_GESTURESTATE_IDX                                  = 355,
    SBK_QT_GESTURETYPE_IDX                                   = 356,
    SBK_QT_GESTUREFLAG_IDX                                   = 354,
    SBK_QFLAGS_QT_GESTUREFLAG_IDX                            = 124,
    SBK_QT_NATIVEGESTURETYPE_IDX                             = 376,
    SBK_QT_NAVIGATIONMODE_IDX                                = 377,
    SBK_QT_CURSORMOVESTYLE_IDX                               = 418,
    SBK_QT_TIMERTYPE_IDX                                     = 398,
    SBK_QT_SCROLLPHASE_IDX                                   = 385,
    SBK_QT_MOUSEEVENTSOURCE_IDX                              = 375,
    SBK_QT_MOUSEEVENTFLAG_IDX                                = 374,
    SBK_QFLAGS_QT_MOUSEEVENTFLAG_IDX                         = 132,
    SBK_QT_CHECKSUMTYPE_IDX                                  = 335,
    SBK_QT_HIGHDPISCALEFACTORROUNDINGPOLICY_IDX              = 358,
    SBK_QT_RETURNBYVALUECONSTANT_IDX                         = 382,
    SBK_QtCoreQT_IDX                                         = 323,
    SBK_QTCOREHELPER_QMUTEXLOCKER_IDX                        = 412,
    SBK_QCBORKNOWNTAGS_IDX                                   = 29,
    SBK_QCBORSIMPLETYPE_IDX                                  = 32,
    SBK_QTMSGTYPE_IDX                                        = 413,
    SBK_QtCore_IDX_COUNT                                     = 419
};
// This variable stores all Python types exported by this module.
extern PyTypeObject **SbkPySide6_QtCoreTypes;

// This variable stores the Python module object exported by this module.
extern PyObject *SbkPySide6_QtCoreModuleObject;

// This variable stores all type converters exported by this module.
extern SbkConverter **SbkPySide6_QtCoreTypeConverters;

// Converter indices
enum : int {
    SBK_HWND_IDX                                             = 0,
    SBK_QBYTEARRAYVIEW_IDX                                   = 1,
    SBK_QCHAR_IDX                                            = 2,
    SBK_QJSONOBJECT_IDX                                      = 3,
    SBK_QMODELINDEXLIST_IDX                                  = 4,
    SBK_QSTRING_IDX                                          = 5,
    SBK_QSTRINGLIST_IDX                                      = 6,
    SBK_QSTRINGVIEW_IDX                                      = 7,
    SBK_QVARIANT_IDX                                         = 8,
    SBK_BOOL_IDX                                             = 9,
    SBK_QINTPTR_IDX                                          = 10,
    SBK_QPTRDIFF_IDX                                         = 11,
    SBK_QUINTPTR_IDX                                         = 12,
    SBK_QTCORE_QLIST_QXMLSTREAMNAMESPACEDECLARATION_IDX      = 13, // QList<QXmlStreamNamespaceDeclaration >
    SBK_QTCORE_QLIST_QXMLSTREAMENTITYDECLARATION_IDX         = 14, // QList<QXmlStreamEntityDeclaration >
    SBK_QTCORE_QLIST_QXMLSTREAMNOTATIONDECLARATION_IDX       = 15, // QList<QXmlStreamNotationDeclaration >
    SBK_QTCORE_QLIST_QXMLSTREAMATTRIBUTE_IDX                 = 16, // QList<QXmlStreamAttribute >
    SBK_QTCORE_QLIST_INT_IDX                                 = 17, // QList<int >
    SBK_QTCORE_QPAIR_QSTRING_QSTRING_IDX                     = 18, // QPair<QString,QString >
    SBK_QTCORE_QLIST_QPAIR_QSTRING_QSTRING_IDX               = 19, // QList<QPair< QString,QString > >
    SBK_QTCORE_QLIST_QSTORAGEINFO_IDX                        = 20, // QList<QStorageInfo >
    SBK_QTCORE_QLIST_QMIMETYPE_IDX                           = 21, // QList<QMimeType >
    SBK_QTCORE_QLIST_QLOCALE_COUNTRY_IDX                     = 22, // QList<QLocale::Country >
    SBK_QTCORE_QLIST_QLOCALE_IDX                             = 23, // QList<QLocale >
    SBK_QTCORE_QLIST_QT_DAYOFWEEK_IDX                        = 24, // QList<Qt::DayOfWeek >
    SBK_QTCORE_QLIST_QBYTEARRAY_IDX                          = 25, // QList<QByteArray >
    SBK_QTCORE_QLIST_QTIMEZONE_OFFSETDATA_IDX                = 26, // QList<QTimeZone::OffsetData >
    SBK_QTCORE_QLIST_QVARIANT_IDX                            = 27, // QList<QVariant >
    SBK_QTCORE_QLIST_QITEMSELECTIONRANGE_IDX                 = 28, // QList<QItemSelectionRange >
    SBK_QTCORE_QLIST_QOBJECTPTR_IDX                          = 29, // QList<QObject* >
    SBK_QTCORE_QLIST_QURL_IDX                                = 30, // QList<QUrl >
    SBK_QTCORE_QLIST_QPOINTF_IDX                             = 31, // QList<QPointF >
    SBK_QTCORE_QLIST_QFILEINFO_IDX                           = 32, // QList<QFileInfo >
    SBK_QTCORE_QPAIR_QINT64_UNSIGNEDINT_IDX                  = 33, // QPair<qint64,unsigned int >
    SBK_QTCORE_QLIST_QCOMMANDLINEOPTION_IDX                  = 34, // QList<QCommandLineOption >
    SBK_QTCORE_QHASH_QSTRING_QVARIANT_IDX                    = 35, // QHash<QString,QVariant >
    SBK_QTCORE_QMAP_QSTRING_QVARIANT_IDX                     = 36, // QMap<QString,QVariant >
    SBK_QTCORE_QLIST_QCBORVALUE_IDX                          = 37, // QList<QCborValue >
    SBK_QTCORE_QMAP_INT_QVARIANT_IDX                         = 38, // QMap<int,QVariant >
    SBK_QTCORE_QLIST_QPERSISTENTMODELINDEX_IDX               = 39, // QList<QPersistentModelIndex >
    SBK_QTCORE_QHASH_INT_QBYTEARRAY_IDX                      = 40, // QHash<int,QByteArray >
    SBK_QTCORE_QLIST_QABSTRACTITEMMODELPTR_IDX               = 41, // QList<QAbstractItemModel* >
    SBK_QTCORE_QLIST_QABSTRACTEVENTDISPATCHER_TIMERINFO_IDX  = 42, // QList<QAbstractEventDispatcher::TimerInfo >
    SBK_QTCORE_STD_PAIR_QREAL_QVARIANT_IDX                   = 43, // std::pair<qreal,QVariant >
    SBK_QTCORE_QLIST_STD_PAIR_QREAL_QVARIANT_IDX             = 44, // QList<std::pair< qreal,QVariant > >
    SBK_QTCORE_QLIST_QSTRING_IDX                             = 45, // QList<QString >
    SBK_QtCore_CONVERTERS_IDX_COUNT                          = 46
};

// typedef entries
using QCborStringResultByteArray = QCborStreamReader::StringResult<QByteArray>;
using QCborStringResultString = QCborStreamReader::StringResult<QString>;

// Macros for type check

namespace Shiboken
{

// PyType functions, to get the PyObjectType for a type T
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
template<> inline PyTypeObject *SbkType< ::QCborKnownTags >() { return SbkPySide6_QtCoreTypes[SBK_QCBORKNOWNTAGS_IDX]; }
template<> inline PyTypeObject *SbkType< ::QCborSimpleType >() { return SbkPySide6_QtCoreTypes[SBK_QCBORSIMPLETYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QtMsgType >() { return SbkPySide6_QtCoreTypes[SBK_QTMSGTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAbstractAnimation::Direction >() { return SbkPySide6_QtCoreTypes[SBK_QABSTRACTANIMATION_DIRECTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAbstractAnimation::State >() { return SbkPySide6_QtCoreTypes[SBK_QABSTRACTANIMATION_STATE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAbstractAnimation::DeletionPolicy >() { return SbkPySide6_QtCoreTypes[SBK_QABSTRACTANIMATION_DELETIONPOLICY_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAbstractAnimation >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QABSTRACTANIMATION_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAbstractEventDispatcher >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QABSTRACTEVENTDISPATCHER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAbstractEventDispatcher::TimerInfo >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QABSTRACTEVENTDISPATCHER_TIMERINFO_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAbstractItemModel::LayoutChangeHint >() { return SbkPySide6_QtCoreTypes[SBK_QABSTRACTITEMMODEL_LAYOUTCHANGEHINT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAbstractItemModel::CheckIndexOption >() { return SbkPySide6_QtCoreTypes[SBK_QABSTRACTITEMMODEL_CHECKINDEXOPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QAbstractItemModel::CheckIndexOption> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QABSTRACTITEMMODEL_CHECKINDEXOPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QAbstractItemModel >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QABSTRACTITEMMODEL_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAbstractListModel >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QABSTRACTLISTMODEL_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAbstractNativeEventFilter >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QABSTRACTNATIVEEVENTFILTER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAbstractProxyModel >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QABSTRACTPROXYMODEL_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAbstractTableModel >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QABSTRACTTABLEMODEL_IDX]); }
template<> inline PyTypeObject *SbkType< ::QAnimationGroup >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QANIMATIONGROUP_IDX]); }
template<> inline PyTypeObject *SbkType< ::QBasicMutex >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QBASICMUTEX_IDX]); }
template<> inline PyTypeObject *SbkType< ::QBasicTimer >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QBASICTIMER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QBitArray >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QBITARRAY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QBuffer >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QBUFFER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QByteArray::Base64Option >() { return SbkPySide6_QtCoreTypes[SBK_QBYTEARRAY_BASE64OPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QByteArray::Base64Option> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QBYTEARRAY_BASE64OPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QByteArray::Base64DecodingStatus >() { return SbkPySide6_QtCoreTypes[SBK_QBYTEARRAY_BASE64DECODINGSTATUS_IDX]; }
template<> inline PyTypeObject *SbkType< ::QByteArray >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QBYTEARRAY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QByteArray::FromBase64Result >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QBYTEARRAY_FROMBASE64RESULT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QByteArrayMatcher >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QBYTEARRAYMATCHER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QCalendar::System >() { return SbkPySide6_QtCoreTypes[SBK_QCALENDAR_SYSTEM_IDX]; }
template<> inline PyTypeObject *SbkType< ::QCalendar >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QCALENDAR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QCalendar::YearMonthDay >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QCALENDAR_YEARMONTHDAY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QCborArray >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QCBORARRAY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QCborError::Code >() { return SbkPySide6_QtCoreTypes[SBK_QCBORERROR_CODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QCborError >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QCBORERROR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QCborMap >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QCBORMAP_IDX]); }
template<> inline PyTypeObject *SbkType< ::QCborParserError >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QCBORPARSERERROR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QCborStreamReader::Type >() { return SbkPySide6_QtCoreTypes[SBK_QCBORSTREAMREADER_TYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QCborStreamReader::StringResultCode >() { return SbkPySide6_QtCoreTypes[SBK_QCBORSTREAMREADER_STRINGRESULTCODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QCborStreamReader >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QCBORSTREAMREADER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QCborStreamWriter >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QCBORSTREAMWRITER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QCborStringResultByteArray >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QCBORSTRINGRESULTBYTEARRAY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QCborStringResultString >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QCBORSTRINGRESULTSTRING_IDX]); }
template<> inline PyTypeObject *SbkType< ::QCborValue::EncodingOption >() { return SbkPySide6_QtCoreTypes[SBK_QCBORVALUE_ENCODINGOPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QCborValue::EncodingOption> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QCBORVALUE_ENCODINGOPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QCborValue::DiagnosticNotationOption >() { return SbkPySide6_QtCoreTypes[SBK_QCBORVALUE_DIAGNOSTICNOTATIONOPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QCborValue::DiagnosticNotationOption> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QCBORVALUE_DIAGNOSTICNOTATIONOPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QCborValue::Type >() { return SbkPySide6_QtCoreTypes[SBK_QCBORVALUE_TYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QCborValue >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QCBORVALUE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QChildEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QCHILDEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QCollator >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QCOLLATOR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QCollatorSortKey >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QCOLLATORSORTKEY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QCommandLineOption::Flag >() { return SbkPySide6_QtCoreTypes[SBK_QCOMMANDLINEOPTION_FLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QCommandLineOption::Flag> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QCOMMANDLINEOPTION_FLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QCommandLineOption >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QCOMMANDLINEOPTION_IDX]); }
template<> inline PyTypeObject *SbkType< ::QCommandLineParser::SingleDashWordOptionMode >() { return SbkPySide6_QtCoreTypes[SBK_QCOMMANDLINEPARSER_SINGLEDASHWORDOPTIONMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QCommandLineParser::OptionsAfterPositionalArgumentsMode >() { return SbkPySide6_QtCoreTypes[SBK_QCOMMANDLINEPARSER_OPTIONSAFTERPOSITIONALARGUMENTSMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QCommandLineParser >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QCOMMANDLINEPARSER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QConcatenateTablesProxyModel >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QCONCATENATETABLESPROXYMODEL_IDX]); }
template<> inline PyTypeObject *SbkType< ::QCoreApplication >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QCOREAPPLICATION_IDX]); }
template<> inline PyTypeObject *SbkType< ::QCryptographicHash::Algorithm >() { return SbkPySide6_QtCoreTypes[SBK_QCRYPTOGRAPHICHASH_ALGORITHM_IDX]; }
template<> inline PyTypeObject *SbkType< ::QCryptographicHash >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QCRYPTOGRAPHICHASH_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDataStream::Version >() { return SbkPySide6_QtCoreTypes[SBK_QDATASTREAM_VERSION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QDataStream::ByteOrder >() { return SbkPySide6_QtCoreTypes[SBK_QDATASTREAM_BYTEORDER_IDX]; }
template<> inline PyTypeObject *SbkType< ::QDataStream::Status >() { return SbkPySide6_QtCoreTypes[SBK_QDATASTREAM_STATUS_IDX]; }
template<> inline PyTypeObject *SbkType< ::QDataStream::FloatingPointPrecision >() { return SbkPySide6_QtCoreTypes[SBK_QDATASTREAM_FLOATINGPOINTPRECISION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QDataStream >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QDATASTREAM_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDate >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QDATE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDateTime::YearRange >() { return SbkPySide6_QtCoreTypes[SBK_QDATETIME_YEARRANGE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QDateTime >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QDATETIME_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDeadlineTimer::ForeverConstant >() { return SbkPySide6_QtCoreTypes[SBK_QDEADLINETIMER_FOREVERCONSTANT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QDeadlineTimer >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QDEADLINETIMER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDir::Filter >() { return SbkPySide6_QtCoreTypes[SBK_QDIR_FILTER_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QDir::Filter> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QDIR_FILTER_IDX]; }
template<> inline PyTypeObject *SbkType< ::QDir::SortFlag >() { return SbkPySide6_QtCoreTypes[SBK_QDIR_SORTFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QDir::SortFlag> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QDIR_SORTFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QDir >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QDIR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDirIterator::IteratorFlag >() { return SbkPySide6_QtCoreTypes[SBK_QDIRITERATOR_ITERATORFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QDirIterator::IteratorFlag> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QDIRITERATOR_ITERATORFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QDirIterator >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QDIRITERATOR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QDynamicPropertyChangeEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QDYNAMICPROPERTYCHANGEEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QEasingCurve::Type >() { return SbkPySide6_QtCoreTypes[SBK_QEASINGCURVE_TYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QEasingCurve >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QEASINGCURVE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QElapsedTimer::ClockType >() { return SbkPySide6_QtCoreTypes[SBK_QELAPSEDTIMER_CLOCKTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QElapsedTimer >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QELAPSEDTIMER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QEvent::Type >() { return SbkPySide6_QtCoreTypes[SBK_QEVENT_TYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QEVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QEventLoop::ProcessEventsFlag >() { return SbkPySide6_QtCoreTypes[SBK_QEVENTLOOP_PROCESSEVENTSFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QEventLoop::ProcessEventsFlag> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QEVENTLOOP_PROCESSEVENTSFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QEventLoop >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QEVENTLOOP_IDX]); }
template<> inline PyTypeObject *SbkType< ::QFactoryInterface >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QFACTORYINTERFACE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QFile >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QFILE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QFileDevice::FileError >() { return SbkPySide6_QtCoreTypes[SBK_QFILEDEVICE_FILEERROR_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFileDevice::FileTime >() { return SbkPySide6_QtCoreTypes[SBK_QFILEDEVICE_FILETIME_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFileDevice::Permission >() { return SbkPySide6_QtCoreTypes[SBK_QFILEDEVICE_PERMISSION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QFileDevice::Permission> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QFILEDEVICE_PERMISSION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFileDevice::FileHandleFlag >() { return SbkPySide6_QtCoreTypes[SBK_QFILEDEVICE_FILEHANDLEFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QFileDevice::FileHandleFlag> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QFILEDEVICE_FILEHANDLEFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFileDevice::MemoryMapFlag >() { return SbkPySide6_QtCoreTypes[SBK_QFILEDEVICE_MEMORYMAPFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QFileDevice::MemoryMapFlag> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QFILEDEVICE_MEMORYMAPFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFileDevice >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QFILEDEVICE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QFileInfo >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QFILEINFO_IDX]); }
template<> inline PyTypeObject *SbkType< ::QFileSelector >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QFILESELECTOR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QFileSystemWatcher >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QFILESYSTEMWATCHER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QFutureInterfaceBase::State >() { return SbkPySide6_QtCoreTypes[SBK_QFUTUREINTERFACEBASE_STATE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFutureInterfaceBase >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QFUTUREINTERFACEBASE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QGenericArgument >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QGENERICARGUMENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QGenericReturnArgument >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QGENERICRETURNARGUMENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QIODevice >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QIODEVICE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QIODeviceBase::OpenModeFlag >() { return SbkPySide6_QtCoreTypes[SBK_QIODEVICEBASE_OPENMODEFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QIODeviceBase::OpenModeFlag> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QIODEVICEBASE_OPENMODEFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QIODeviceBase >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QIODEVICEBASE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QIdentityProxyModel >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QIDENTITYPROXYMODEL_IDX]); }
template<> inline PyTypeObject *SbkType< ::QItemSelection >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QITEMSELECTION_IDX]); }
template<> inline PyTypeObject *SbkType< ::QItemSelectionModel::SelectionFlag >() { return SbkPySide6_QtCoreTypes[SBK_QITEMSELECTIONMODEL_SELECTIONFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QItemSelectionModel::SelectionFlag> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QITEMSELECTIONMODEL_SELECTIONFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QItemSelectionModel >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QITEMSELECTIONMODEL_IDX]); }
template<> inline PyTypeObject *SbkType< ::QItemSelectionRange >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QITEMSELECTIONRANGE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QJsonArray >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QJSONARRAY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QJsonDocument::JsonFormat >() { return SbkPySide6_QtCoreTypes[SBK_QJSONDOCUMENT_JSONFORMAT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QJsonDocument >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QJSONDOCUMENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QJsonParseError::ParseError >() { return SbkPySide6_QtCoreTypes[SBK_QJSONPARSEERROR_PARSEERROR_IDX]; }
template<> inline PyTypeObject *SbkType< ::QJsonParseError >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QJSONPARSEERROR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QJsonValue::Type >() { return SbkPySide6_QtCoreTypes[SBK_QJSONVALUE_TYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QJsonValue >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QJSONVALUE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QKeyCombination >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QKEYCOMBINATION_IDX]); }
template<> inline PyTypeObject *SbkType< ::QLibrary::LoadHint >() { return SbkPySide6_QtCoreTypes[SBK_QLIBRARY_LOADHINT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QLibrary::LoadHint> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QLIBRARY_LOADHINT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QLibrary >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QLIBRARY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QLibraryInfo::LibraryPath >() { return SbkPySide6_QtCoreTypes[SBK_QLIBRARYINFO_LIBRARYPATH_IDX]; }
template<> inline PyTypeObject *SbkType< ::QLibraryInfo >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QLIBRARYINFO_IDX]); }
template<> inline PyTypeObject *SbkType< ::QLine >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QLINE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QLineF::IntersectionType >() { return SbkPySide6_QtCoreTypes[SBK_QLINEF_INTERSECTIONTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QLineF >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QLINEF_IDX]); }
template<> inline PyTypeObject *SbkType< ::QLocale::Language >() { return SbkPySide6_QtCoreTypes[SBK_QLOCALE_LANGUAGE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QLocale::Script >() { return SbkPySide6_QtCoreTypes[SBK_QLOCALE_SCRIPT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QLocale::Country >() { return SbkPySide6_QtCoreTypes[SBK_QLOCALE_COUNTRY_IDX]; }
template<> inline PyTypeObject *SbkType< ::QLocale::MeasurementSystem >() { return SbkPySide6_QtCoreTypes[SBK_QLOCALE_MEASUREMENTSYSTEM_IDX]; }
template<> inline PyTypeObject *SbkType< ::QLocale::FormatType >() { return SbkPySide6_QtCoreTypes[SBK_QLOCALE_FORMATTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QLocale::NumberOption >() { return SbkPySide6_QtCoreTypes[SBK_QLOCALE_NUMBEROPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QLocale::NumberOption> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QLOCALE_NUMBEROPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QLocale::FloatingPointPrecisionOption >() { return SbkPySide6_QtCoreTypes[SBK_QLOCALE_FLOATINGPOINTPRECISIONOPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QLocale::CurrencySymbolFormat >() { return SbkPySide6_QtCoreTypes[SBK_QLOCALE_CURRENCYSYMBOLFORMAT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QLocale::DataSizeFormat >() { return SbkPySide6_QtCoreTypes[SBK_QLOCALE_DATASIZEFORMAT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QLocale::DataSizeFormat> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QLOCALE_DATASIZEFORMAT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QLocale::QuotationStyle >() { return SbkPySide6_QtCoreTypes[SBK_QLOCALE_QUOTATIONSTYLE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QLocale >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QLOCALE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QLockFile::LockError >() { return SbkPySide6_QtCoreTypes[SBK_QLOCKFILE_LOCKERROR_IDX]; }
template<> inline PyTypeObject *SbkType< ::QLockFile >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QLOCKFILE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QMargins >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QMARGINS_IDX]); }
template<> inline PyTypeObject *SbkType< ::QMarginsF >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QMARGINSF_IDX]); }
template<> inline PyTypeObject *SbkType< ::QMessageAuthenticationCode >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QMESSAGEAUTHENTICATIONCODE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QMessageLogContext >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QMESSAGELOGCONTEXT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QMetaClassInfo >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QMETACLASSINFO_IDX]); }
template<> inline PyTypeObject *SbkType< ::QMetaEnum >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QMETAENUM_IDX]); }
template<> inline PyTypeObject *SbkType< ::QMetaMethod::Access >() { return SbkPySide6_QtCoreTypes[SBK_QMETAMETHOD_ACCESS_IDX]; }
template<> inline PyTypeObject *SbkType< ::QMetaMethod::MethodType >() { return SbkPySide6_QtCoreTypes[SBK_QMETAMETHOD_METHODTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QMetaMethod >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QMETAMETHOD_IDX]); }
template<> inline PyTypeObject *SbkType< ::QMetaObject::Call >() { return SbkPySide6_QtCoreTypes[SBK_QMETAOBJECT_CALL_IDX]; }
template<> inline PyTypeObject *SbkType< ::QMetaObject >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QMETAOBJECT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QMetaObject::Connection >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QMETAOBJECT_CONNECTION_IDX]); }
template<> inline PyTypeObject *SbkType< ::QMetaProperty >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QMETAPROPERTY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QMetaType::Type >() { return SbkPySide6_QtCoreTypes[SBK_QMETATYPE_TYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QMetaType::TypeFlag >() { return SbkPySide6_QtCoreTypes[SBK_QMETATYPE_TYPEFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QMetaType::TypeFlag> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QMETATYPE_TYPEFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QMetaType >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QMETATYPE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QMimeData >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QMIMEDATA_IDX]); }
template<> inline PyTypeObject *SbkType< ::QMimeDatabase::MatchMode >() { return SbkPySide6_QtCoreTypes[SBK_QMIMEDATABASE_MATCHMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QMimeDatabase >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QMIMEDATABASE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QMimeType >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QMIMETYPE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QModelIndex >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QMODELINDEX_IDX]); }
template<> inline PyTypeObject *SbkType< ::QModelRoleData >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QMODELROLEDATA_IDX]); }
template<> inline PyTypeObject *SbkType< ::QMutex >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QMUTEX_IDX]); }
template<> inline PyTypeObject *SbkType< ::QObject >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QOBJECT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QOperatingSystemVersion::OSType >() { return SbkPySide6_QtCoreTypes[SBK_QOPERATINGSYSTEMVERSION_OSTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QOperatingSystemVersion >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QOPERATINGSYSTEMVERSION_IDX]); }
template<> inline PyTypeObject *SbkType< ::QParallelAnimationGroup >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QPARALLELANIMATIONGROUP_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPauseAnimation >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QPAUSEANIMATION_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPersistentModelIndex >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QPERSISTENTMODELINDEX_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPluginLoader >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QPLUGINLOADER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPoint >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QPOINT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPointF >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QPOINTF_IDX]); }
template<> inline PyTypeObject *SbkType< ::QProcess::ProcessError >() { return SbkPySide6_QtCoreTypes[SBK_QPROCESS_PROCESSERROR_IDX]; }
template<> inline PyTypeObject *SbkType< ::QProcess::ProcessState >() { return SbkPySide6_QtCoreTypes[SBK_QPROCESS_PROCESSSTATE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QProcess::ProcessChannel >() { return SbkPySide6_QtCoreTypes[SBK_QPROCESS_PROCESSCHANNEL_IDX]; }
template<> inline PyTypeObject *SbkType< ::QProcess::ProcessChannelMode >() { return SbkPySide6_QtCoreTypes[SBK_QPROCESS_PROCESSCHANNELMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QProcess::InputChannelMode >() { return SbkPySide6_QtCoreTypes[SBK_QPROCESS_INPUTCHANNELMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QProcess::ExitStatus >() { return SbkPySide6_QtCoreTypes[SBK_QPROCESS_EXITSTATUS_IDX]; }
template<> inline PyTypeObject *SbkType< ::QProcess >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QPROCESS_IDX]); }
template<> inline PyTypeObject *SbkType< ::QProcessEnvironment >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QPROCESSENVIRONMENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QPropertyAnimation >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QPROPERTYANIMATION_IDX]); }
template<> inline PyTypeObject *SbkType< ::QRandomGenerator >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QRANDOMGENERATOR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QRandomGenerator64 >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QRANDOMGENERATOR64_IDX]); }
template<> inline PyTypeObject *SbkType< ::QReadLocker >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QREADLOCKER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QReadWriteLock::RecursionMode >() { return SbkPySide6_QtCoreTypes[SBK_QREADWRITELOCK_RECURSIONMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QReadWriteLock >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QREADWRITELOCK_IDX]); }
template<> inline PyTypeObject *SbkType< ::QRect >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QRECT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QRectF >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QRECTF_IDX]); }
template<> inline PyTypeObject *SbkType< ::QRecursiveMutex >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QRECURSIVEMUTEX_IDX]); }
template<> inline PyTypeObject *SbkType< ::QRegularExpression::PatternOption >() { return SbkPySide6_QtCoreTypes[SBK_QREGULAREXPRESSION_PATTERNOPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QRegularExpression::PatternOption> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QREGULAREXPRESSION_PATTERNOPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QRegularExpression::MatchType >() { return SbkPySide6_QtCoreTypes[SBK_QREGULAREXPRESSION_MATCHTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QRegularExpression::MatchOption >() { return SbkPySide6_QtCoreTypes[SBK_QREGULAREXPRESSION_MATCHOPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QRegularExpression::MatchOption> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QREGULAREXPRESSION_MATCHOPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QRegularExpression::WildcardConversionOption >() { return SbkPySide6_QtCoreTypes[SBK_QREGULAREXPRESSION_WILDCARDCONVERSIONOPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QRegularExpression::WildcardConversionOption> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QREGULAREXPRESSION_WILDCARDCONVERSIONOPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QRegularExpression >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QREGULAREXPRESSION_IDX]); }
template<> inline PyTypeObject *SbkType< ::QRegularExpressionMatch >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QREGULAREXPRESSIONMATCH_IDX]); }
template<> inline PyTypeObject *SbkType< ::QRegularExpressionMatchIterator >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QREGULAREXPRESSIONMATCHITERATOR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QResource::Compression >() { return SbkPySide6_QtCoreTypes[SBK_QRESOURCE_COMPRESSION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QResource >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QRESOURCE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QRunnable >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QRUNNABLE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QSaveFile >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QSAVEFILE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QSemaphore >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QSEMAPHORE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QSemaphoreReleaser >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QSEMAPHORERELEASER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QSequentialAnimationGroup >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QSEQUENTIALANIMATIONGROUP_IDX]); }
template<> inline PyTypeObject *SbkType< ::QSettings::Status >() { return SbkPySide6_QtCoreTypes[SBK_QSETTINGS_STATUS_IDX]; }
template<> inline PyTypeObject *SbkType< ::QSettings::Format >() { return SbkPySide6_QtCoreTypes[SBK_QSETTINGS_FORMAT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QSettings::Scope >() { return SbkPySide6_QtCoreTypes[SBK_QSETTINGS_SCOPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QSettings >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QSETTINGS_IDX]); }
template<> inline PyTypeObject *SbkType< ::QSignalBlocker >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QSIGNALBLOCKER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QSignalMapper >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QSIGNALMAPPER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QSize >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QSIZE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QSizeF >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QSIZEF_IDX]); }
template<> inline PyTypeObject *SbkType< ::QSocketDescriptor >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QSOCKETDESCRIPTOR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QSocketNotifier::Type >() { return SbkPySide6_QtCoreTypes[SBK_QSOCKETNOTIFIER_TYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QSocketNotifier >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QSOCKETNOTIFIER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QSortFilterProxyModel >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QSORTFILTERPROXYMODEL_IDX]); }
template<> inline PyTypeObject *SbkType< ::QStandardPaths::StandardLocation >() { return SbkPySide6_QtCoreTypes[SBK_QSTANDARDPATHS_STANDARDLOCATION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QStandardPaths::LocateOption >() { return SbkPySide6_QtCoreTypes[SBK_QSTANDARDPATHS_LOCATEOPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QStandardPaths::LocateOption> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QSTANDARDPATHS_LOCATEOPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QStandardPaths >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QSTANDARDPATHS_IDX]); }
template<> inline PyTypeObject *SbkType< ::QStorageInfo >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QSTORAGEINFO_IDX]); }
template<> inline PyTypeObject *SbkType< ::QStringConverter::Encoding >() { return SbkPySide6_QtCoreTypes[SBK_QSTRINGCONVERTER_ENCODING_IDX]; }
template<> inline PyTypeObject *SbkType< ::QStringConverter >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QSTRINGCONVERTER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QStringConverterBase::Flag >() { return SbkPySide6_QtCoreTypes[SBK_QSTRINGCONVERTERBASE_FLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QStringConverterBase::Flag> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QSTRINGCONVERTERBASE_FLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QStringConverterBase >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QSTRINGCONVERTERBASE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QStringConverterBase::State >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QSTRINGCONVERTERBASE_STATE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QStringDecoder >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QSTRINGDECODER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QStringEncoder >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QSTRINGENCODER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QStringListModel >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QSTRINGLISTMODEL_IDX]); }
template<> inline PyTypeObject *SbkType< ::QSysInfo::Sizes >() { return SbkPySide6_QtCoreTypes[SBK_QSYSINFO_SIZES_IDX]; }
template<> inline PyTypeObject *SbkType< ::QSysInfo::Endian >() { return SbkPySide6_QtCoreTypes[SBK_QSYSINFO_ENDIAN_IDX]; }
template<> inline PyTypeObject *SbkType< ::QSysInfo >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QSYSINFO_IDX]); }
template<> inline PyTypeObject *SbkType< ::QSystemSemaphore::AccessMode >() { return SbkPySide6_QtCoreTypes[SBK_QSYSTEMSEMAPHORE_ACCESSMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QSystemSemaphore::SystemSemaphoreError >() { return SbkPySide6_QtCoreTypes[SBK_QSYSTEMSEMAPHORE_SYSTEMSEMAPHOREERROR_IDX]; }
template<> inline PyTypeObject *SbkType< ::QSystemSemaphore >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QSYSTEMSEMAPHORE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTemporaryDir >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QTEMPORARYDIR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTemporaryFile >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QTEMPORARYFILE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextBoundaryFinder::BoundaryType >() { return SbkPySide6_QtCoreTypes[SBK_QTEXTBOUNDARYFINDER_BOUNDARYTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextBoundaryFinder::BoundaryReason >() { return SbkPySide6_QtCoreTypes[SBK_QTEXTBOUNDARYFINDER_BOUNDARYREASON_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QTextBoundaryFinder::BoundaryReason> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QTEXTBOUNDARYFINDER_BOUNDARYREASON_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextBoundaryFinder >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QTEXTBOUNDARYFINDER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextStream::RealNumberNotation >() { return SbkPySide6_QtCoreTypes[SBK_QTEXTSTREAM_REALNUMBERNOTATION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextStream::FieldAlignment >() { return SbkPySide6_QtCoreTypes[SBK_QTEXTSTREAM_FIELDALIGNMENT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextStream::Status >() { return SbkPySide6_QtCoreTypes[SBK_QTEXTSTREAM_STATUS_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextStream::NumberFlag >() { return SbkPySide6_QtCoreTypes[SBK_QTEXTSTREAM_NUMBERFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QTextStream::NumberFlag> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QTEXTSTREAM_NUMBERFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTextStream >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QTEXTSTREAM_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTextStreamManipulator >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QTEXTSTREAMMANIPULATOR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QThread::Priority >() { return SbkPySide6_QtCoreTypes[SBK_QTHREAD_PRIORITY_IDX]; }
template<> inline PyTypeObject *SbkType< ::QThread >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QTHREAD_IDX]); }
template<> inline PyTypeObject *SbkType< ::QThreadPool >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QTHREADPOOL_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTime >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QTIME_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTimeLine::State >() { return SbkPySide6_QtCoreTypes[SBK_QTIMELINE_STATE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTimeLine::Direction >() { return SbkPySide6_QtCoreTypes[SBK_QTIMELINE_DIRECTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTimeLine >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QTIMELINE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTimeZone::TimeType >() { return SbkPySide6_QtCoreTypes[SBK_QTIMEZONE_TIMETYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTimeZone::NameType >() { return SbkPySide6_QtCoreTypes[SBK_QTIMEZONE_NAMETYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QTimeZone >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QTIMEZONE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTimeZone::OffsetData >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QTIMEZONE_OFFSETDATA_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTimer >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QTIMER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTimerEvent >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QTIMEREVENT_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTranslator >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QTRANSLATOR_IDX]); }
template<> inline PyTypeObject *SbkType< ::QTransposeProxyModel >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QTRANSPOSEPROXYMODEL_IDX]); }
template<> inline PyTypeObject *SbkType< ::QUrl::ParsingMode >() { return SbkPySide6_QtCoreTypes[SBK_QURL_PARSINGMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QUrl::UrlFormattingOption >() { return SbkPySide6_QtCoreTypes[SBK_QURL_URLFORMATTINGOPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QUrl::ComponentFormattingOption >() { return SbkPySide6_QtCoreTypes[SBK_QURL_COMPONENTFORMATTINGOPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QUrl::ComponentFormattingOption> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QURL_COMPONENTFORMATTINGOPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QUrl::UserInputResolutionOption >() { return SbkPySide6_QtCoreTypes[SBK_QURL_USERINPUTRESOLUTIONOPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<QUrl::UserInputResolutionOption> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QURL_USERINPUTRESOLUTIONOPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QUrl >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QURL_IDX]); }
template<> inline PyTypeObject *SbkType< ::QUrlQuery >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QURLQUERY_IDX]); }
template<> inline PyTypeObject *SbkType< ::QUuid::Variant >() { return SbkPySide6_QtCoreTypes[SBK_QUUID_VARIANT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QUuid::Version >() { return SbkPySide6_QtCoreTypes[SBK_QUUID_VERSION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QUuid::StringFormat >() { return SbkPySide6_QtCoreTypes[SBK_QUUID_STRINGFORMAT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QUuid >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QUUID_IDX]); }
template<> inline PyTypeObject *SbkType< ::QVariantAnimation >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QVARIANTANIMATION_IDX]); }
template<> inline PyTypeObject *SbkType< ::QVersionNumber >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QVERSIONNUMBER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QWaitCondition >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QWAITCONDITION_IDX]); }
template<> inline PyTypeObject *SbkType< ::QWinEventNotifier >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QWINEVENTNOTIFIER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QWriteLocker >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QWRITELOCKER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QXmlStreamAttribute >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QXMLSTREAMATTRIBUTE_IDX]); }
template<> inline PyTypeObject *SbkType< ::QXmlStreamAttributes >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QXMLSTREAMATTRIBUTES_IDX]); }
template<> inline PyTypeObject *SbkType< ::QXmlStreamEntityDeclaration >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QXMLSTREAMENTITYDECLARATION_IDX]); }
template<> inline PyTypeObject *SbkType< ::QXmlStreamEntityResolver >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QXMLSTREAMENTITYRESOLVER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QXmlStreamNamespaceDeclaration >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QXMLSTREAMNAMESPACEDECLARATION_IDX]); }
template<> inline PyTypeObject *SbkType< ::QXmlStreamNotationDeclaration >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QXMLSTREAMNOTATIONDECLARATION_IDX]); }
template<> inline PyTypeObject *SbkType< ::QXmlStreamReader::TokenType >() { return SbkPySide6_QtCoreTypes[SBK_QXMLSTREAMREADER_TOKENTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QXmlStreamReader::ReadElementTextBehaviour >() { return SbkPySide6_QtCoreTypes[SBK_QXMLSTREAMREADER_READELEMENTTEXTBEHAVIOUR_IDX]; }
template<> inline PyTypeObject *SbkType< ::QXmlStreamReader::Error >() { return SbkPySide6_QtCoreTypes[SBK_QXMLSTREAMREADER_ERROR_IDX]; }
template<> inline PyTypeObject *SbkType< ::QXmlStreamReader >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QXMLSTREAMREADER_IDX]); }
template<> inline PyTypeObject *SbkType< ::QXmlStreamWriter >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QXMLSTREAMWRITER_IDX]); }
template<> inline PyTypeObject *SbkType< ::Qt::GlobalColor >() { return SbkPySide6_QtCoreTypes[SBK_QT_GLOBALCOLOR_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::MouseButton >() { return SbkPySide6_QtCoreTypes[SBK_QT_MOUSEBUTTON_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<Qt::MouseButton> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QT_MOUSEBUTTON_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::Orientation >() { return SbkPySide6_QtCoreTypes[SBK_QT_ORIENTATION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<Qt::Orientation> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QT_ORIENTATION_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::FocusPolicy >() { return SbkPySide6_QtCoreTypes[SBK_QT_FOCUSPOLICY_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::TabFocusBehavior >() { return SbkPySide6_QtCoreTypes[SBK_QT_TABFOCUSBEHAVIOR_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::SortOrder >() { return SbkPySide6_QtCoreTypes[SBK_QT_SORTORDER_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::SplitBehaviorFlags >() { return SbkPySide6_QtCoreTypes[SBK_QT_SPLITBEHAVIORFLAGS_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<Qt::SplitBehaviorFlags> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QT_SPLITBEHAVIORFLAGS_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::TileRule >() { return SbkPySide6_QtCoreTypes[SBK_QT_TILERULE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::AlignmentFlag >() { return SbkPySide6_QtCoreTypes[SBK_QT_ALIGNMENTFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<Qt::AlignmentFlag> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QT_ALIGNMENTFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::TextFlag >() { return SbkPySide6_QtCoreTypes[SBK_QT_TEXTFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::TextElideMode >() { return SbkPySide6_QtCoreTypes[SBK_QT_TEXTELIDEMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::WhiteSpaceMode >() { return SbkPySide6_QtCoreTypes[SBK_QT_WHITESPACEMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::HitTestAccuracy >() { return SbkPySide6_QtCoreTypes[SBK_QT_HITTESTACCURACY_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::WindowType >() { return SbkPySide6_QtCoreTypes[SBK_QT_WINDOWTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<Qt::WindowType> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QT_WINDOWTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::WindowState >() { return SbkPySide6_QtCoreTypes[SBK_QT_WINDOWSTATE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<Qt::WindowState> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QT_WINDOWSTATE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::ApplicationState >() { return SbkPySide6_QtCoreTypes[SBK_QT_APPLICATIONSTATE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<Qt::ApplicationState> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QT_APPLICATIONSTATE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::ScreenOrientation >() { return SbkPySide6_QtCoreTypes[SBK_QT_SCREENORIENTATION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<Qt::ScreenOrientation> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QT_SCREENORIENTATION_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::WidgetAttribute >() { return SbkPySide6_QtCoreTypes[SBK_QT_WIDGETATTRIBUTE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::ApplicationAttribute >() { return SbkPySide6_QtCoreTypes[SBK_QT_APPLICATIONATTRIBUTE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::ImageConversionFlag >() { return SbkPySide6_QtCoreTypes[SBK_QT_IMAGECONVERSIONFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<Qt::ImageConversionFlag> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QT_IMAGECONVERSIONFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::BGMode >() { return SbkPySide6_QtCoreTypes[SBK_QT_BGMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::Key >() { return SbkPySide6_QtCoreTypes[SBK_QT_KEY_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::KeyboardModifier >() { return SbkPySide6_QtCoreTypes[SBK_QT_KEYBOARDMODIFIER_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<Qt::KeyboardModifier> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QT_KEYBOARDMODIFIER_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::Modifier >() { return SbkPySide6_QtCoreTypes[SBK_QT_MODIFIER_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::ArrowType >() { return SbkPySide6_QtCoreTypes[SBK_QT_ARROWTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::PenStyle >() { return SbkPySide6_QtCoreTypes[SBK_QT_PENSTYLE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::PenCapStyle >() { return SbkPySide6_QtCoreTypes[SBK_QT_PENCAPSTYLE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::PenJoinStyle >() { return SbkPySide6_QtCoreTypes[SBK_QT_PENJOINSTYLE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::BrushStyle >() { return SbkPySide6_QtCoreTypes[SBK_QT_BRUSHSTYLE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::SizeMode >() { return SbkPySide6_QtCoreTypes[SBK_QT_SIZEMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::UIEffect >() { return SbkPySide6_QtCoreTypes[SBK_QT_UIEFFECT_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::CursorShape >() { return SbkPySide6_QtCoreTypes[SBK_QT_CURSORSHAPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::TextFormat >() { return SbkPySide6_QtCoreTypes[SBK_QT_TEXTFORMAT_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::AspectRatioMode >() { return SbkPySide6_QtCoreTypes[SBK_QT_ASPECTRATIOMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::DockWidgetArea >() { return SbkPySide6_QtCoreTypes[SBK_QT_DOCKWIDGETAREA_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<Qt::DockWidgetArea> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QT_DOCKWIDGETAREA_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::DockWidgetAreaSizes >() { return SbkPySide6_QtCoreTypes[SBK_QT_DOCKWIDGETAREASIZES_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::ToolBarArea >() { return SbkPySide6_QtCoreTypes[SBK_QT_TOOLBARAREA_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<Qt::ToolBarArea> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QT_TOOLBARAREA_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::ToolBarAreaSizes >() { return SbkPySide6_QtCoreTypes[SBK_QT_TOOLBARAREASIZES_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::DateFormat >() { return SbkPySide6_QtCoreTypes[SBK_QT_DATEFORMAT_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::TimeSpec >() { return SbkPySide6_QtCoreTypes[SBK_QT_TIMESPEC_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::DayOfWeek >() { return SbkPySide6_QtCoreTypes[SBK_QT_DAYOFWEEK_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::ScrollBarPolicy >() { return SbkPySide6_QtCoreTypes[SBK_QT_SCROLLBARPOLICY_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::CaseSensitivity >() { return SbkPySide6_QtCoreTypes[SBK_QT_CASESENSITIVITY_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::Corner >() { return SbkPySide6_QtCoreTypes[SBK_QT_CORNER_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::Edge >() { return SbkPySide6_QtCoreTypes[SBK_QT_EDGE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<Qt::Edge> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QT_EDGE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::ConnectionType >() { return SbkPySide6_QtCoreTypes[SBK_QT_CONNECTIONTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::ShortcutContext >() { return SbkPySide6_QtCoreTypes[SBK_QT_SHORTCUTCONTEXT_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::FillRule >() { return SbkPySide6_QtCoreTypes[SBK_QT_FILLRULE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::MaskMode >() { return SbkPySide6_QtCoreTypes[SBK_QT_MASKMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::ClipOperation >() { return SbkPySide6_QtCoreTypes[SBK_QT_CLIPOPERATION_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::ItemSelectionMode >() { return SbkPySide6_QtCoreTypes[SBK_QT_ITEMSELECTIONMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::ItemSelectionOperation >() { return SbkPySide6_QtCoreTypes[SBK_QT_ITEMSELECTIONOPERATION_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::TransformationMode >() { return SbkPySide6_QtCoreTypes[SBK_QT_TRANSFORMATIONMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::Axis >() { return SbkPySide6_QtCoreTypes[SBK_QT_AXIS_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::FocusReason >() { return SbkPySide6_QtCoreTypes[SBK_QT_FOCUSREASON_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::ContextMenuPolicy >() { return SbkPySide6_QtCoreTypes[SBK_QT_CONTEXTMENUPOLICY_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::InputMethodQuery >() { return SbkPySide6_QtCoreTypes[SBK_QT_INPUTMETHODQUERY_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<Qt::InputMethodQuery> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QT_INPUTMETHODQUERY_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::InputMethodHint >() { return SbkPySide6_QtCoreTypes[SBK_QT_INPUTMETHODHINT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<Qt::InputMethodHint> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QT_INPUTMETHODHINT_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::EnterKeyType >() { return SbkPySide6_QtCoreTypes[SBK_QT_ENTERKEYTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::ToolButtonStyle >() { return SbkPySide6_QtCoreTypes[SBK_QT_TOOLBUTTONSTYLE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::LayoutDirection >() { return SbkPySide6_QtCoreTypes[SBK_QT_LAYOUTDIRECTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::AnchorPoint >() { return SbkPySide6_QtCoreTypes[SBK_QT_ANCHORPOINT_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::FindChildOption >() { return SbkPySide6_QtCoreTypes[SBK_QT_FINDCHILDOPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<Qt::FindChildOption> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QT_FINDCHILDOPTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::DropAction >() { return SbkPySide6_QtCoreTypes[SBK_QT_DROPACTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<Qt::DropAction> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QT_DROPACTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::CheckState >() { return SbkPySide6_QtCoreTypes[SBK_QT_CHECKSTATE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::ItemDataRole >() { return SbkPySide6_QtCoreTypes[SBK_QT_ITEMDATAROLE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::ItemFlag >() { return SbkPySide6_QtCoreTypes[SBK_QT_ITEMFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<Qt::ItemFlag> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QT_ITEMFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::MatchFlag >() { return SbkPySide6_QtCoreTypes[SBK_QT_MATCHFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<Qt::MatchFlag> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QT_MATCHFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::WindowModality >() { return SbkPySide6_QtCoreTypes[SBK_QT_WINDOWMODALITY_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::TextInteractionFlag >() { return SbkPySide6_QtCoreTypes[SBK_QT_TEXTINTERACTIONFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<Qt::TextInteractionFlag> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QT_TEXTINTERACTIONFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::EventPriority >() { return SbkPySide6_QtCoreTypes[SBK_QT_EVENTPRIORITY_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::SizeHint >() { return SbkPySide6_QtCoreTypes[SBK_QT_SIZEHINT_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::WindowFrameSection >() { return SbkPySide6_QtCoreTypes[SBK_QT_WINDOWFRAMESECTION_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::CoordinateSystem >() { return SbkPySide6_QtCoreTypes[SBK_QT_COORDINATESYSTEM_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::TouchPointState >() { return SbkPySide6_QtCoreTypes[SBK_QT_TOUCHPOINTSTATE_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<Qt::TouchPointState> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QT_TOUCHPOINTSTATE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::GestureState >() { return SbkPySide6_QtCoreTypes[SBK_QT_GESTURESTATE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::GestureType >() { return SbkPySide6_QtCoreTypes[SBK_QT_GESTURETYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::GestureFlag >() { return SbkPySide6_QtCoreTypes[SBK_QT_GESTUREFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<Qt::GestureFlag> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QT_GESTUREFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::NativeGestureType >() { return SbkPySide6_QtCoreTypes[SBK_QT_NATIVEGESTURETYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::NavigationMode >() { return SbkPySide6_QtCoreTypes[SBK_QT_NAVIGATIONMODE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::CursorMoveStyle >() { return SbkPySide6_QtCoreTypes[SBK_QT_CURSORMOVESTYLE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::TimerType >() { return SbkPySide6_QtCoreTypes[SBK_QT_TIMERTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::ScrollPhase >() { return SbkPySide6_QtCoreTypes[SBK_QT_SCROLLPHASE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::MouseEventSource >() { return SbkPySide6_QtCoreTypes[SBK_QT_MOUSEEVENTSOURCE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::MouseEventFlag >() { return SbkPySide6_QtCoreTypes[SBK_QT_MOUSEEVENTFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::QFlags<Qt::MouseEventFlag> >() { return SbkPySide6_QtCoreTypes[SBK_QFLAGS_QT_MOUSEEVENTFLAG_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::ChecksumType >() { return SbkPySide6_QtCoreTypes[SBK_QT_CHECKSUMTYPE_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::HighDpiScaleFactorRoundingPolicy >() { return SbkPySide6_QtCoreTypes[SBK_QT_HIGHDPISCALEFACTORROUNDINGPOLICY_IDX]; }
template<> inline PyTypeObject *SbkType< ::Qt::ReturnByValueConstant >() { return SbkPySide6_QtCoreTypes[SBK_QT_RETURNBYVALUECONSTANT_IDX]; }
template<> inline PyTypeObject *SbkType< ::QtCoreHelper::QMutexLocker >() { return reinterpret_cast<PyTypeObject *>(SbkPySide6_QtCoreTypes[SBK_QTCOREHELPER_QMUTEXLOCKER_IDX]); }
QT_WARNING_POP

} // namespace Shiboken

#endif // SBK_QTCORE_PYTHON_H

