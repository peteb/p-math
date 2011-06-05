#include <platform/window_manager.h>

extern void showWin32ErrorReport(HWND hWnd); // in another file

typedef HWND (*Win32GetHwnd)();
typedef void (*MessageBox)(const char *);

// meningen med det här är att kringgå WindowManager-interfacet.

class GameDelegate : public WindowManager::Delegate {
public:
  GameDelegate() {
    _wm = services.get<WindowManager>();
    
  }
  
  // en funktion kan returnera en facade också. eller andra saker med medlems-
  // funktioner. skissa på det också. TODO
  
  void reportError() {
    // kanske kan wrappa in funptr-typen och ext-namnet i något?
    
    // alternativ #1
    Win32GetHwnd* getHwnd = (Win32GetHwnd*)_wm->getExtension("win32GetHwnd");
    if (getHwnd) {
      HWND hwnd = getHwnd();
      showWin32ErrorReport(hwnd);
    }
  }
  
  void showMsg(const std::string& msg) {
    MessageBox* msgBox = (MessageBox*)_wm->getExtension("MessageBox");
    if (msgBox) {
      msgBox(msg.c_str());
    }
  }
private:
  WindowManager* _wm;
};

int main() {
  WindowManager* wm = services.get<WindowManager>(); // might be win32
  
  wm->run<GameDelegate>();
}