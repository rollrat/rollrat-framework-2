//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  biginteger.h - 02/01/2016
//
//  Copyright (C) 2016-2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#ifndef _BIGINTEGER_
#define _BIGINTEGER_

#include <string>

#include "alu.h"

/*--------------------------------------------------------------------------------------------

    There are three ways to implement the biginteger.

    First, array of unsigned integer
    Second, array of character
    Last, array of bit

    Time Complexity Compration
    integer >>>>>>>> character >>>>>>>> bit

    Space Complexity Compration
    integer = binary >>>>>>>> bit
    or
    integer >>>>>>>> character = bit

    The library chose third way, using binary and second space complexity.
    So, it could make in only three days.
    
    --------------------------------------------------------------------------------------

    Reference:
    [1] http://transparenttape.tistory.com/
    [2] https://mattmccutchen.net/bigint/
    [3] http://blog.bab2min.pe.kr/
    [4] http://referencesource.microsoft.com/#System.Numerics/System/Numerics/BigInteger.cs
	[5] https://gmplib.org/

--------------------------------------------------------------------------------------------*/

template<int I>
class basic_integer
{
    typedef basic_integer<I> this_type;
    typedef alu<I, 4> integer_operation;
    typedef typename integer_operation::bits_type integer_bits;

    integer_operation engine;

public:

    basic_integer(const std::string str)
    {
        string_to_bigint(str);
    }

    basic_integer(const char* str)
        : basic_integer(std::string(str))
    {
    }

    basic_integer(pointer_type uiptr)
    {
        for (int i = 0; i < sizeof(pointer_type)*8; i++)
            engine.load(0)[i] = ((uiptr & (1 << i)) >> i) & 1;
    }

	basic_integer(integer_type iptr)
	{
		bool sign = iptr < 0;
		if ( sign ) 
			iptr = -iptr;
		for (int i = 0; i < sizeof(pointer_type)*8; i++)
			engine.load(0)[i] = ((iptr & (1 << i)) >> i) & 1;
        if ( sign )
            engine.nabs(0);
	}

    std::string operator*() const
    {
        return bigint_to_string(engine.load(0));
    }

    bit& operator[](size_t off)
    {
        return engine.load(0)[off];
    }

    bit operator[](size_t off) const
    {
        return engine.load(0)[off];
    }

    this_type& operator--()
    {
        engine.sub(0, integer_bits({1}));
        return *this;
    }

    this_type operator--(int)
    {
        this_type tmp = *this;
        --*this;
        return tmp;
    }
    
    this_type& operator++()
    {
        engine.add(0, integer_bits({1}));
    }

    this_type operator++(int)
    {
        this_type tmp = *this;
        ++*this;
        return tmp;
    }

    this_type& operator+=(const this_type& integer)
    {
        engine.add(0,integer.engine.load(0));
        return *this;
    }

    this_type operator+(const this_type& integer) const
    {
        this_type tmp = *this;
        return tmp += integer;
    }

    this_type& operator-=(const this_type& integer)
    {
        engine.sub(0,integer.engine.load(0));
        return *this;
    }

    this_type operator-(const this_type& integer) const
    {
        this_type tmp = *this;
        return tmp -= integer;
    }
    
    this_type& operator*=(const this_type& integer)
    {
        engine.mul(0,integer.engine.load(0));
        return *this;
    }

    this_type operator*(const this_type& integer) const
    {
        this_type tmp = *this;
        return tmp *= integer;
    }

    this_type& operator/=(const this_type& integer)
    {
        engine.div(0,integer.engine.load(0));
        return *this;
    }

    this_type operator/(const this_type& integer) const
    {
        this_type tmp = *this;
        return tmp /= integer;
    }

    this_type& operator&=(const this_type& integer)
    {
        engine.and(0,integer.engine.load(0));
        return *this;
    }

    this_type operator&(const this_type& integer) const
    {
        this_type tmp = *this;
        return tmp &= integer;
    }

    this_type& operator|=(const this_type& integer)
    {
        engine.or(0,integer.engine.load(0));
        return *this;
    }

    this_type operator|(const this_type& integer) const
    {
        this_type tmp = *this;
        return tmp |= integer;
    }

    this_type& operator^=(const this_type& integer)
    {
        engine.xor(0,integer.engine.load(0));
        return *this;
    }

    this_type operator^(const this_type& integer) const
    {
        this_type tmp = *this;
        return tmp ^= integer;
    }

    this_type& operator<<=(const this_type& integer)
    {
        engine.shl(0,integer.engien.load(0));
        return *this;
    }
    
    this_type operator<<(const this_type& integer) const
    {
        this_type tmp = *this;
        return tmp <<= integer;
    }
    
    this_type& operator>>=(const this_type& integer)
    {
        engine.shr(0,integer.engien.load(0));
        return *this;
    }
    
    this_type operator>>(const this_type& integer) const
    {
        this_type tmp = *this;
        return tmp >>= integer;
    }

    this_type& operator%=(const this_type& integer)
    {
        engine.mod(0,integer.engine.load(0));
        return *this;
    }

    this_type operator%(const this_type& integer) const
    {
        this_type tmp = *this;
        return tmp %= integer;
    }

    this_type operator~() const
    {
        this_type tmp = *this;
        tmp.engine.not(0);
        return tmp;
    }

    this_type& operator=(const this_type& integer)
    {
        engine.store(integer.engine.load(0),0);
        return *this;
    }

    this_type& operator=(const std::string str)
    {
        string_to_bigint(str);
		return *this;
    }

    this_type& operator=(pointer_type uiptr)
    {
        for (int i = 0; i < sizeof(pointer_type); i++)
            engine.load(0)[i] = ((uiptr & (1 << i)) >> i) & 1;
		return *this;
    }

    bool operator==(const this_type& integer) const
    {
        engine.eql(0,integer.engine.load(0),1);
        return engine.load(1).lsb() == on;
    }

    bool operator!=(const this_type& integer) const
    {
        return !(*this == integer);
    }

    bool operator<(const this_type& integer) const
    {
        engine.les(0,integer.engine.load(0),1);
        return engine.load(1).lsb() == on;
    }

    bool operator>(const this_type& integer) const
    {
        return integer < *this;
    }

    bool operator<=(const this_type& integer) const
    {
        return !(integer < *this);
    }

    bool operator>=(const this_type& integer) const
    {
        return !(*this < integer);
    }
    
    this_type gcd(const this_type& integer) const
    {
        this_type tmp = *this;
        tmp.engine.store(integer.engine.load(0),1);
        tmp.engine.les(0,1,2);
        if ( tmp.engine._register[2][0] == on )   
        {
            tmp.engine.move(0,3);
            tmp.engine.move(1,0);
            tmp.engine.move(3,1);
        }
        while ( true )
        {
            tmp.engine.gre(1,integer_bits({0}),2);
            if ( tmp.engine._register[2][0] == off )
                break;
            tmp.engine.eql(0,1,2);
            if ( tmp.engine._register[2][0] == on )
                break;
            tmp.engine.mod(0,1,2);
            tmp.engine.move(1,0);
            tmp.engine.move(2,1);
        }
        return tmp;
    }

    this_type lcm(const this_type& integer) const
    {
        this_type tmp = integer;
        tmp.engine.store(gcd(tmp).engine.load(0),3);
        tmp.engine.store(engine.load(0),0);
        tmp.engine.store(integer.engine.load(0),1);
        tmp.engine.mul(0,1,2);
        tmp.engine.div(2,3,0);
        return tmp;
    }
    
    this_type power(const this_type& integer) const
    {
        this_type tmp = *this;
        tmp.engine.move(0,2);
        tmp.engine.store(integer.engine.load(0),1);
        if (tmp.engine.load(1).zero() == off)
        {
            while(true)
            {
                tmp.engine.sub(1,integer_bits({1}));
                if (tmp.engine.load(1).zero() == on)
                    break;
                tmp.engine.mul(0,2,0);
            }
        }
        else
        {
            tmp.engine.store(integer_bits({0}),0);
        }
        return tmp;
    }

    this_type abs() const
    {
        this_type tmp = *this;
        tmp.engine.abs(0);
        return tmp;
    }
    
    this_type operator+() const
    {
        return *this;
    }
	
    this_type operator-() const
    {
        this_type tmp = *this;
        if (tmp.engine.load(0).msb() == on)
            tmp.engine.abs(0);
        else
            tmp.engine.nabs(0);
        return tmp;
    }

	bool sign() const
	{
		if (engine.load(0).msb() == on)
			return true;
		else
			return false;
	}

    void swap(this_type& refer)
    {
        refer.engine.store(engine.load(0),1);
        engine.store(refer.engine.load(0),0);
        refer.engine.move(1,0);
    }
    
    friend std::ostream& operator<<(std::ostream& os, const this_type& refer)
    {
        os << **refer;
        return os;
    }

    bool operator&&(const this_type&) const = delete;
    bool operator||(const this_type&) const = delete;

private:

    integer_bits string_to_bits(const std::string& str) const
    {
        integer_bits bits;
        
        for (int i = 0; i < str.length(); i++)
        {
            int c = str[str.length()-i-1] - '0';
            bits[(i << 2) + 0] = (c & 1) >> 0;
            bits[(i << 2) + 1] = (c & 2) >> 1;
            bits[(i << 2) + 2] = (c & 4) >> 2;
            bits[(i << 2) + 3] = (c & 8) >> 3;
        }
        
        return bits;
    }

    void string_to_bigint(std::string str)
    {
        bool sign = false;
        if ( str[0] == '-' )
        {
            str = str.substr(1);
            sign = true;
        }
        engine.dcb(string_to_bits(str),0);
        if (sign) engine.nabs(0);
    }

    std::string bits_to_string(integer_bits bits) const
    {
        std::string ak;
        int i = I;

        for ( ; i >= 0; i -= 4)
        {
            if (bits[i-1] == on || bits[i-2] == on ||
                bits[i-3] == on || bits[i-4] == on  )
            {
                break;
            }
        }

        if ( i <= 0 ) ak = '0';
        else for ( ; i > 0; i -= 4)
            ak += '0' + ( (bits[i - 1].var() << 3) | 
                          (bits[i - 2].var() << 2) | 
                          (bits[i - 3].var() << 1) | 
                          (bits[i - 4].var() << 0) );
        
        return ak;
    }

    std::string bigint_to_string(integer_bits bits) const
    {
        std::string sign;
        if (bits.msb() == on) sign = '-';
        engine.bcd(bits,1);
        return sign + bits_to_string(engine.load(1));
    }
    
};

typedef basic_integer<256> integer;

#endif