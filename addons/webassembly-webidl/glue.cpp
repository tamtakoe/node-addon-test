
#include <emscripten.h>

extern "C" {

// Not using size_t for array indices as the values used by the javascript code are signed.

EM_JS(void, array_bounds_check_error, (size_t idx, size_t size), {
  throw 'Array index ' + idx + ' out of bounds: [0,' + size + ')';
});

void array_bounds_check(const int array_size, const int array_idx) {
  if (array_idx < 0 || array_idx >= array_size) {
    array_bounds_check_error(array_idx, array_size);
  }
}

// VoidPtr

void EMSCRIPTEN_KEEPALIVE emscripten_bind_VoidPtr___destroy___0(void** self) {
  delete self;
}

// CollectionUtils

CollectionUtils* EMSCRIPTEN_KEEPALIVE emscripten_bind_CollectionUtils_CollectionUtils_0() {
  return new CollectionUtils();
}

double EMSCRIPTEN_KEEPALIVE emscripten_bind_CollectionUtils_group_by_2(CollectionUtils* self, char* text, int value) {
  return self->group_by(text, value);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_CollectionUtils___destroy___0(CollectionUtils* self) {
  delete self;
}

}

