#ifndef PIN_H
#define PIN_H

template <typename Port, int Pin>
struct pin
{
  static void set(const bool value = true)
  {
    if (value)
      Port::port(Port::port() | (1 << Pin));
    else
      Port::port(Port::port() & ~(1 << Pin));
  }
  static void clear(){ set(false); };

  static void output(const bool value = true){
    if (value)
      Port::dir(Port::dir() | (1 << Pin));
    else
      Port::port(Port::port() & ~(1 << Pin));
      
  }
  static void input(const bool value = true){ output(false); }

  static void toggle() { Port::port(Port::port() ^ (1 << Pin)); }
  static bool read() { return (Port::port() & (1 << Pin)) != 0; }

};

#endif