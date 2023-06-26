#include <cmci.h>
#include <native.h>
#include <unistd.h>
#include <cimc/cimcdt.h>

extern char *value2Chars(CMPIType type, CMPIValue * value);

void showObjectPath( CMPIObjectPath * objectpath );
void showInstance( CMPIInstance *instance );
void showClass( CMPIConstClass * in_class );
void showProperty( CMPIData data, char *name );

void showCClass( const CIMCClass * in_class );
void showCObjectPath( CIMCObjectPath * objectpath );
