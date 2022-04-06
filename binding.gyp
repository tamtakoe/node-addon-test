{
  "targets": [
    {
      "target_name": "native_napi",
      "sources": [ "addons/native_napi/main.cc" ],
      "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ],
      "include_dirs" : [ "<!@(node -p \"require('node-addon-api').include\")" ],
      "dependencies": [ "<!(node -p \"require('node-addon-api').gyp\")" ],
      "cflags!": [ "-fno-exceptions", "-fno-rtti" ],
      "cflags_cc!": [ "-fno-exceptions", "-fno-rtti" ],
      "cflags": ["-O3"],
      "xcode_settings": {
        "GCC_ENABLE_CPP_RTTI": "YES",
        "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
        "CLANG_CXX_LIBRARY": "libc++",
        "MACOSX_DEPLOYMENT_TARGET": "12.3"
      },
      "msvs_settings": {
        "VCCLCompilerTool": {
          "AdditionalOptions": [ "-std:c++17" ],
          "ExceptionHandling": 1
        },
      }
    }
  ],
  "cflags": ["-O3"]
}
