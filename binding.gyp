{
  "targets": [
    {
      "target_name": "collection_utils",
      "sources": [ "addons/collection_utils/main.cc" ],
      "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ],
      "include_dirs" : [ "<!@(node -p \"require('node-addon-api').include\")" ],
      "dependencies": [ "<!(node -p \"require('node-addon-api').gyp\")" ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "cflags": ["-O3"],
      "xcode_settings": {
        "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
        "CLANG_CXX_LIBRARY": "libc++",
        "MACOSX_DEPLOYMENT_TARGET": "10.7"
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
