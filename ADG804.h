#ifndef ADG804_H
#define ADG804_H

template <typename pin_EN, typename pin_A0, typename pin_A1>
struct ADG804
{
  static void initialize(){
    pin_EN::clear();
    pin_A0::clear();
    pin_A1::clear();
    pin_EN::output();
    pin_A0::output();
    pin_A1::output();
  }

  static void enable(const bool value = true){
      pin_EN::set(value);
  }

  static void disable(){
    enable(false);
  }

  static void set_channel(const uint8_t channel_no){
    if (channel_no >= 4)
      return;
    pin_A0::set(channel_no & (1 << 0));
    pin_A1::set(channel_no & (1 << 1));
  }
};

#endif