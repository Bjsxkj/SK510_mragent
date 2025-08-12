
#pragma once

#include <stdint.h>
//#include <


#define BASE_PROPERTY_GET(type, name, defval, propid) \
        type get ## name () const { \
            IBaseProperty<type> _ ## name; \
            int rc = getProperty(propid, &(_ ## name)); \
            return rc==0? _ ## name .get() : defval; \
        }

#define BOOL_PROPERTY_GET(type, name, defval, propid) \
        bool is ## name () const { \
            IBaseProperty<type> _ ## name; \
            int rc = getProperty(propid, &(_ ## name)); \
            return (rc==0? _ ## name .get() : defval) == 0; \
        }

#define BASE_PROPERTY_SET(type, name, errcode, propid) \
        int set ## name ( type val) { \
            IBaseProperty<type> _ ## name(val); \
            int rc = setProperty(propid, &(_ ## name)); \
            return rc==0? rc: errcode; \
        }

#define BOOL_PROPERTY_SET(type, name, errcode, propid) \
        int set ## name (bool val) { \
            IBaseProperty<type> _ ## name((type)val); \
            int rc = setProperty(propid, &(_ ## name)); \
            return rc==0? rc: errcode; \
        }

        // getProperty( propid, &(_ ## name)); \

namespace imimr {

    

}