#include "../../Assets.hpp"

namespace Assets {

  using namespace std;

  void pauseUntilKeyPressed(char k, string mes) {
    CLogger::Info(mes.c_str());
    fgetc(stdin);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
  }

  void pauseUntilKeyPressed(char k) {
    fgetc(stdin);
    cin.ignore(numeric_limits<streamsize>::max(), k);
  }

  void pauseUntilKeyPressed(string mes) {
    CLogger::Info(mes.c_str());
    fgetc(stdin);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
  }

  void pauseUntilKeyPressed() {
    fgetc(stdin);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
  }
};// namespace Assets
