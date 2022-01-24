/* WebIDL example */

#include <string>

using namespace std;

class CollectionUtils {
  public:
  double group_by(char* text, int value) {
    string str = string(text); //Example how to convert char* to string
    printf("Text: '%s' Number: %d\n", text, value);
    return value * -1.7;
  }
};