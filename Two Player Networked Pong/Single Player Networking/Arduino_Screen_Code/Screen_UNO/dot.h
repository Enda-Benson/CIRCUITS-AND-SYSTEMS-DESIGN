
class Dot {
public:
  uint8_t x_position; // X position of the dot object (used for animations)
  uint8_t y_position; // Y position of the dot object (used for animations)
  Dot(){}
  Dot(int Xnumber, int Ynumber){
    x_position = Xnumber;
    y_position = Ynumber;
  }

};              // close dor class
Dot dot;  //initialise an instance of the dot
