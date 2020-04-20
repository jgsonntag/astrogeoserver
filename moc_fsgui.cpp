/****************************************************************************
** Fsgui meta object code from reading C++ file 'fsgui.h'
**
** Created: Sun Nov 16 11:10:52 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "fsgui.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Fsgui::className() const
{
    return "Fsgui";
}

QMetaObject *Fsgui::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Fsgui( "Fsgui", &Fsgui::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Fsgui::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Fsgui", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Fsgui::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Fsgui", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Fsgui::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QVBox::staticMetaObject();
    static const QUMethod slot_0 = {"about", 0, 0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"slotNewGPS", 1, param_slot_1 };
    static const QUMethod slot_2 = {"slotGPSClosed", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"slotGPSError", 1, param_slot_3 };
    static const QUMethod slot_4 = {"slotAstroUpdate", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "about()", &slot_0, QMetaData::Private },
	{ "slotNewGPS(QString)", &slot_1, QMetaData::Private },
	{ "slotGPSClosed()", &slot_2, QMetaData::Private },
	{ "slotGPSError(int)", &slot_3, QMetaData::Private },
	{ "slotAstroUpdate()", &slot_4, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"Fsgui", parentObject,
	slot_tbl, 5,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Fsgui.setMetaObject( metaObj );
    return metaObj;
}

void* Fsgui::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Fsgui" ) )
	return this;
    return QVBox::qt_cast( clname );
}

bool Fsgui::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: about(); break;
    case 1: slotNewGPS((QString)static_QUType_QString.get(_o+1)); break;
    case 2: slotGPSClosed(); break;
    case 3: slotGPSError((int)static_QUType_int.get(_o+1)); break;
    case 4: slotAstroUpdate(); break;
    default:
	return QVBox::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Fsgui::qt_emit( int _id, QUObject* _o )
{
    return QVBox::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool Fsgui::qt_property( int id, int f, QVariant* v)
{
    return QVBox::qt_property( id, f, v);
}

bool Fsgui::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
