#ifndef AGC_H
#define AGC_H 1

#include <inttypes.h>

class ShiftAGC_I16 {
  public:
    // Constructor
    ShiftAGC_I16(uint32_t attack_rate, uint32_t backoff_rate)
        : _attack_rate(attack_rate)
        , _backoff_rate(backoff_rate)
        , _sizeof_type(8*sizeof(_overflow_level))
    {
        _overflow_level = 1 << (_sizeof_type - 1);
    }

    // Public methods
    inline bool update(int16_t value)
    {
        uint16_t abs_value = (value >= 0) ? (value) : (-value);

        if (abs_value < _overflow_level) {
            _attack_count++;

            if (_attack_count >= _attack_rate) {
                _attack_count = 0;
                _backoff_count = 0;

                if (_shift < (_sizeof_type - 1))
                    _shift++;
                _overflow_level = 1 << (_sizeof_type - _shift - 1);
            }
        }
        else {
            _backoff_count++;

            if (_backoff_count >= _backoff_rate) {
                _attack_count = 0;
                _backoff_count = 0;

                if (_shift > 0)
                    _shift--;
                _overflow_level = 1 << (_sizeof_type - _shift - 1);

                return true;
            }
        }

        return false;
    }

    inline int16_t apply(int16_t value)
    {
        const int16_t int16_max = 0x3FFF;
        uint16_t abs_value = (value >= 0) ? (value) : (-value);
        if (abs_value < _overflow_level) {
            return (value << _shift);
        }
        else {
            int8_t sign = (value > 0) ? 1 : -1;
            return sign * int16_max;
        }
    }

  private:
    uint8_t _shift;
    uint16_t _overflow_level;

    const uint8_t _sizeof_type;

    const uint32_t _attack_rate;
    uint32_t _attack_count;

    const uint32_t _backoff_rate;
    uint32_t _backoff_count;
};

class ShiftAGC_U16 {
  public:
    // Constructor
    ShiftAGC_U16(uint32_t attack_rate, uint32_t backoff_rate)
        : _attack_rate(attack_rate)
        , _backoff_rate(backoff_rate)
        , _sizeof_type(8*sizeof(_overflow_level))
    {
        _overflow_level = 1 << (_sizeof_type - 1);
    }

    // Public methods
    inline bool update(uint16_t value)
    {
        if (value < _overflow_level) {
            _attack_count++;

            if (_attack_count >= _attack_rate) {
                _attack_count = 0;
                _backoff_count = 0;

                if (_shift < (_sizeof_type - 1))
                    _shift++;
                _overflow_level = 1 << (_sizeof_type - _shift - 1);
            }
        }
        else {
            _backoff_count++;

            if (_backoff_count >= _backoff_rate) {
                _attack_count = 0;
                _backoff_count = 0;

                if (_shift > 0)
                    _shift--;
                _overflow_level = 1 << (_sizeof_type - _shift - 1);

                return true;
            }
        }

        return false;
    }

    inline uint16_t apply(uint16_t value)
    {
        const uint16_t uint16_max = 0xFFFF;
        if (value < _overflow_level)
            return (value << _shift);
        else
            return uint16_max;
    }

  private:
    uint8_t _shift;
    uint16_t _overflow_level;

    const uint8_t _sizeof_type;

    const uint32_t _attack_rate;
    uint32_t _attack_count;

    const uint32_t _backoff_rate;
    uint32_t _backoff_count;
};

class ShiftAGC_U8 {
  public:
    // Constructor
    ShiftAGC_U8(uint32_t attack_rate, uint32_t backoff_rate)
        : _attack_rate(attack_rate)
        , _backoff_rate(backoff_rate)
        , _sizeof_type(8*sizeof(_overflow_level))
    {
        _overflow_level = 1 << (_sizeof_type - 1);
    }

    // Public methods
    inline bool update(uint8_t value)
    {
        if (value < _overflow_level) {
            _attack_count++;

            if (_attack_count >= _attack_rate) {
                _attack_count = 0;
                _backoff_count = 0;

                if (_shift < (_sizeof_type - 1))
                    _shift++;
                _overflow_level = 1 << (_sizeof_type - _shift - 1);
            }
        }
        else {
            _backoff_count++;

            if (_backoff_count >= _backoff_rate) {
                _attack_count = 0;
                _backoff_count = 0;

                if (_shift > 0)
                    _shift--;
                _overflow_level = 1 << (_sizeof_type - _shift - 1);

                return true;
            }
        }

        return false;
    }

    inline uint8_t apply(uint8_t value)
    {
        const uint8_t uint8_max = 0xFF;
        if (value < _overflow_level)
            return (value << _shift);
        else
            return uint8_max;
    }

  private:
    uint8_t _shift;
    uint8_t _overflow_level;

    const uint8_t _sizeof_type;

    const uint32_t _attack_rate;
    uint32_t _attack_count;

    const uint32_t _backoff_rate;
    uint32_t _backoff_count;
};

#endif /* #ifndef AGC_H */
