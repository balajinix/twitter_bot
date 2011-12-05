/*
 * JSONValue.h
 * Copyright (C) 2010 Mike Anchor <mikea@mjpa.co.uk>
 *
 * Part of the MJPA JSON Library - http://mjpa.co.uk/blog/view/A-simple-C-JSON-library/
 *
 * License: http://mjpa.co.uk/licenses/GPLv2/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _JSONVALUE_H_
#define _JSONVALUE_H_

#include <vector>
#include <string>

#include "JSON.h"

class JSON;

enum JSONType { JSONType_Null, JSONType_String, JSONType_Bool, JSONType_Number,/* JSONType_LongLong,*/ JSONType_Array, JSONType_Object };

class JSONValue
{
  friend class JSON;
  
  public:
    JSONValue(/*NULL*/);
    JSONValue(const char *m_char_value);
    JSONValue(std::string m_string_value);
    JSONValue(bool m_bool_value);
    JSONValue(double m_number_value);
    JSONValue(unsigned long long m_long_value);
    JSONValue(JSONArray m_array_value);
    JSONValue(JSONObject m_object_value);
    ~JSONValue();
    
    bool IsNull();
    bool IsString();
    bool IsBool();
    bool IsNumber();
    bool IsLongLong();
    bool IsArray();
    bool IsObject();
    
    std::string AsString();
    bool AsBool();
    double AsNumber();
    unsigned long long AsLongLong();
    JSONArray AsArray();
    JSONObject AsObject();
    
    std::string Stringify();
    
  protected:
    static JSONValue *Parse(const char **data);
  
  private:
    static std::string StringifyString(std::string str);
  
    JSONType type;
    std::string string_value;
    bool bool_value;
    double number_value;
    unsigned long long long_value;
    JSONArray array_value;
    JSONObject object_value;
};

#endif
