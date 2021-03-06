#ifndef BufferParsing
#define BufferParsing

#include <aSubRecord.h>

#ifdef __cplusplus
extern "C" {
#endif

    long ParseReadingsForValue(aSubRecord *prec);
    long ParseReadingsForUnit(aSubRecord *prec);

#ifdef __cplusplus
}
#endif 

#endif// !BufferParsing
