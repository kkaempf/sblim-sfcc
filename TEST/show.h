#include <cmci.h>
#include <native.h>
#include <unistd.h>

extern char *value2Chars(CMPIType type, CMPIValue * value);

void showObjectPath( CMPIObjectPath * objectpath );
void showInstance( CMPIInstance *instance );
void showClass( CMPIConstClass * class );


