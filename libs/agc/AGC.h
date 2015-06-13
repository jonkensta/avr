#ifndef AGC_H
#define AGC_H 1

template <typename T>
class ShiftAGC {
  public:
    // Constructor
    ShiftAGC(uint32_t attack_rate, uint8_t margin) 
        : _attack_rate(attack_rate)
        , _margin(margin)
    {
        _mask = (1 << sizeof(T)) - 1;
    }

    ~ShiftAGC(void);

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

    inline T calculate_mask(void)
    {
        return ~((1 << (sizeof(T) - (_shift + _margin))) - 1);
    }
};

#endif /* #ifndef AGC_H */
