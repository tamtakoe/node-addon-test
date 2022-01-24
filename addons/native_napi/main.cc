#include <napi.h>
#include "group_by_binding.h"

Napi::Object init(Napi::Env env, Napi::Object exports)
{
      exports.Set(Napi::String::New(env, "groupBy"), Napi::Function::New(env, GroupBy));
      return exports;
}

NODE_API_MODULE(native_napi, init)
