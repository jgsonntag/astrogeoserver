/****************************************************************************
** Setupgui meta object code from reading C++ file 'setupgui.h'
**
** Created: Sun Nov 16 10:55:15 2014
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "setupgui.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Setupgui::className() const
{
    return "Setupgui";
}

QMetaObject *Setupgui::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Setupgui( "Setupgui", &Setupgui::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Setupgui::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Setupgui", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Setupgui::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Setupgui", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Setupgui::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"testInputs", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "testInputs()", &slot_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"Setupgui", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Setupgui.setMetaObject( metaObj );
    return metaObj;
}

void* Setupgui::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Setupgui" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool Setupgui::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: testInputs(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Setupgui::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool Setupgui::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool Setupgui::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
