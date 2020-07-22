#include <napi.h>
#include "group_by.h"

Napi::Object init(Napi::Env env, Napi::Object exports)
{
      exports.Set(Napi::String::New(env, "groupBy"), Napi::Function::New(env, GroupBy));
      return exports;
}

NODE_API_MODULE(collection_utils, init)
