#ifndef _SCREEN_H
#define _SCREEN_H
class Screen {
   public:
      Screen(int pixelWidth, int pixelHeight, Vector3& cameraLocation, double left, double right, double top, double bottom);
      Vector3 * pixelToScreen(int x, int y);

   protected:
      double pixelToScreenX(int x);
      double pixelToScreenY(int y);
   private:
      int _pixelWidth, _pixelHeight;
      double _left, _right, _top, _bottom, _near;
      Vector3 _cameraLocation;
};

Screen::Screen(int pixelWidth, int pixelHeight, Vector3& cameraLocation, double left, double right, double top, double bottom) {
   _pixelWidth = pixelWidth;
   _pixelHeight = pixelHeight;
   _left = left;
   _right = right;
   _top = top;
   _bottom = bottom;
   _cameraLocation = Vector3(cameraLocation);
}

Vector3 * Screen::pixelToScreen(int x, int y) {
   return new Vector3(pixelToScreenX(x), pixelToScreenY(y), _cameraLocation.z - 1);
}

double Screen::pixelToScreenX(int x) {
   double screenWidth = _right - _left;

   // handle a weird condition
   if(_pixelWidth == 1) {
      return _right + screenWidth / 2.0;
   }
   return _left + screenWidth * (x + 0.5) / (_pixelWidth - 1);
}

double Screen::pixelToScreenY(int y) {
   double screenHeight = _top - _bottom;

   if(_pixelHeight == 1) {
      return _bottom + screenHeight;
   }

   return _top - screenHeight * (y + 0.5) / (_pixelHeight);
}
#endif // _SCREEN_H
