#ifndef AGC_H
#define AGC_H 1

#include <inttypes.h>

template <typename T>
class ShiftAGC {
  public:
    // Constructor
    ShiftAGC(uint32_t attack_rate, uint8_t margin) 
        : _attack_rate(attack_rate)
        , _margin(margin)
    {
    }

    // Public methods
    inline bool update(T value)
    {
        T abs_value = (value >= 0) ? (value) : (-value);

        if (!(abs_value & _mask)) {
            _attack_count++;

            if (_attack_count >= _attack_rate) {
                _attack_count = 0;
                _shift++;
                _mask = calculate_mask(_shift, _margin);
            }

            return false;
        }
        else {
            _shift--;
            _mask = calculate_mask(_shift, _margin);

            return true;
        }
    }

    inline T apply(T value)
    {
        return (value << _shift);
    }

  private:
    uint8_t _shift;
    uint8_t _margin;
    T _mask;

    uint32_t _attack_rate;
    uint32_t _attack_count;

    static inline T calculate_mask(uint8_t shift, uint8_t margin)
    {
        return ~((1 << ((sizeof(T) - 1) - (shift + margin))) - 1);
    }
};

typedef ShiftAGC<int8_t>  ShiftAGC8;
typedef ShiftAGC<int16_t> ShiftAGC16;
typedef ShiftAGC<int32_t> ShiftAGC32;

#endif /* #ifndef AGC_H */
