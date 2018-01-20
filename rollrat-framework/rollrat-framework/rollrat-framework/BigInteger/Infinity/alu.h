//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  alu.h - 01/30/2016
//
//  Copyright (C) 2016-2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#ifndef _ALU_
#define _ALU_

/*--------------------------------------------------------------------------------------------

    ALU Structure

    The ALU has a dependent relationship with the register. 
    It is independent with the register of CPU, but are mutually dependent.

                 +---------------------+    +---------------------+
                 |                     | -> |                     |
    In/Out ------|        ALU          |    |     Register        |
                 |                     | <- |                     |
                 +---------------------+    +---------------------+
                                                     |
                                                     |
                                                     |

                                                   CU, IR

--------------------------------------------------------------------------------------------*/

#include <memory.h>
#include <stdint.h>
#include <vector>

#if defined(__x86_64__) || defined(__ia64__) || defined(_M_AMD64) || defined(_M_IA64) \
 || defined(_WIN64) || defined(__alpha__) || defined(__s390__)
#define _X64_MODE
typedef uint64_t	pointer_type;
typedef int64_t     integer_type;
#else
typedef uint32_t	pointer_type;
typedef int32_t     integer_type;
#endif

typedef enum bit_state : uint8_t { off=0, on=1 } state;

#pragma pack (push, 1)

/*--------------------------------------------------------------------------------------------

    Standard Logic Algorithm(STD LA)
    
    And : And Gate, a and b, If a == b Then 1 else 0
    The gate of the two input values​​ is equal, the output is 1 output.

    Or : Or Gate, a or b, If a | b Then 1 else 0
    If either one of two inputs to 1 output.
    
    Not : Not Gat, ~a, ~a
    The opposite of the input values ​​to output values​​.
    
    Xor : Excusive Or Gate, a xor b, If a != b Then 1 else 0
    This gate is different from the value of the two inputs, the output is 1.
    ※ a Xor b = (a And ~b) Or (~a And b)

--------------------------------------------------------------------------------------------*/

state reverse_manager[2] = {on, off};
class bit { // real in 1 byte
    state s;
public:

    bit() : s(state::off) {}
    bit(int i) : s((state)i) {}

    void on() { s = state::on; }
    void off() { s = state::off; }
    void reverse() { s = reverse_manager[s]; }

    bit not() const { return bit(reverse_manager[s]); }
    bit and(const bit& o)const  { return bit(s & o.s & 1); }
    bit or(const bit& o) const { return bit(s | o.s & 1); }
    bit xor(const bit& o) const { return bit(s ^ o.s); }

    bit operator&(const bit& o) const { return and(o); }
    bit operator|(const bit& o) const { return or(o); }
    bit operator^(const bit& o) const { return xor(o); }
    bit operator~() const { return not(); }

    bit operator=(const bit& o) { s = o.s; return *this; }

    state var() const { return s; }
    bool operator==(const bit& o) { return s == o.s; }
    bool operator!=(const bit& o) { return s != o.s; }
};

#define BIT_FOREACH(kss,iss) for ( int i=0; i < kss; i++) { iss }
#define BIT_FOREACHI(iss) BIT_FOREACH(I, iss)

template<int I>
class bits {
    typedef bits<I> this_type;

    template<int k>
    class internal_placeholder { };

    typedef internal_placeholder<0> placeholder_not;
    typedef internal_placeholder<1> placeholder_and;
    typedef internal_placeholder<2> placeholder_or;
    typedef internal_placeholder<3> placeholder_xor;

    bits(const this_type& k, placeholder_not) {BIT_FOREACHI( s[i] = ~k.s[i]; )}
    bits(const this_type& k0, const this_type& k1, placeholder_and) {BIT_FOREACHI( s[i] = k0.s[i] & k1.s[i]; )}
    bits(const this_type& k0, const this_type& k1, placeholder_or) {BIT_FOREACHI( s[i] = k0.s[i] | k1.s[i]; )}
    bits(const this_type& k0, const this_type& k1, placeholder_xor) {BIT_FOREACHI( s[i] = k0.s[i] ^ k1.s[i]; )}

public:

    bit s[I] = { off, };

    bits(const this_type& k) { memcpy(s,k.s,I*sizeof(bit)); }

    template<typename... Args>
    bits(Args... args) : s{ bit(args)... } { static_assert(sizeof...(Args) <= I, "Error!"); }

    this_type not() const { return this_type(*this, placeholder_not()); }
    this_type and(const this_type& o) const { return this_type(*this, o, placeholder_and()); }
    this_type or(const this_type& o) const { return this_type(*this, o, placeholder_or()); }
    this_type xor(const this_type& o) const { return this_type(*this, o, placeholder_xor()); }
    
    this_type operator&(const this_type& o) const { return and(o); }
    this_type operator|(const this_type& o) const { return or(o); }
    this_type operator^(const this_type& o) const { return xor(o); }
    this_type operator~() const { return not(); }
    
    this_type operator=(const this_type& o) { memcpy(s,o.s,I*sizeof(bit)); return *this;  }
    
    bit operator[](size_t i) const { return s[i]; }
    bit& operator[](size_t i) { return s[i]; }

    bit msb() const { return s[I-1]; }
    bit lsb() const { return s[0]; }

    bit zero() const {bit r; BIT_FOREACHI( r = r | s[i]; ) return r.not();}
};


/*--------------------------------------------------------------------------------------------

    FLAGS Structure
    
                                      +=================+
                                      | 5| 4| 3| 2| 1| 0|
                                      |OF|SF|ZF|AF|PF|CF|
                                      +=================+
    
    FLAGS Status | On, Off
    
    Status Flag List
        Carry Flag(CF)    : get Carry Out
        Parity Flag(PF)   : If First bit = 1
        Adjust Flag(AF)   : If (P 3 Bit) as Carry set! -> BCD Algorithm
        Zero Flag(ZF)     : If Thing = 0
        Sign Flag(SF)     : If Most Significant Byte(MSB) = 1 -> Adder-Subtractor
        Overflow Flag(OF) : If Thing(without MSB) > Max

--------------------------------------------------------------------------------------------*/

typedef bits<6> flags;

#define FLAGS_NONE                          bits<6> (off,off,off,off,off,off)
#define FLAGS_ADJUST                        bits<6> (off,off,on,off,off,off)
#define FLAGS_CARRY                         bits<6> (on,off,off,off,off,off)
#define FLAGS_OVERFLOW                      bits<6> (off,off,off,off,off,on)
#define FLAGS_PARITY                        bits<6> (off,on,off,off,off,off)
#define FLAGS_SIGN                          bits<6> (off,off,off,off,on,off)
#define FLAGS_ZERO                          bits<6> (off,off,off,on,off,off)


/*--------------------------------------------------------------------------------------------

    Addition and Carry Bit Example( ex in bits 32 )
                            
                            Binary                              Hex      Decimal
    
           |    1011 0010 1001 0011 1111 1010 1101 1100(2) = 0B293FADC, 2996042460
           |+   0111 1010 1101 1001 0111 0101 0000 0001(2) = 07AD97501, 2061071617
           +------------------------------------------------------------------------
    Add       1 0010 1101 0110 1101 0110 1111 1101 1101(2) = 12D6D6FDD, 5057114077      Over Flow
    Carry     0 1111 0010 1001 0011 1111 0000 0000 0000(2) = 0F293F000, 4069781504
    
    Secure Bit = 
    32 bit, so reality used area is 31bit. But unsigned signiture limit to use of 32bit
    , so unsgined used are is 32bit.
    
    a = Bit X, b = Bit Y, c = Carry bit(First calc is zero.)
    
    Add Bit = (a xor(^) b) | c
    Carry Bit = (a & b) | c
        
    --------------------------------------------------------------------------------------
    
    Subtraction And Complement ways
    
    Thought of Subtraction
    
                            Binary                                Hex     Decimal

           |    1111 1011 0010 1101 1001 0110 0101 1100(2) =   FB2D965C, 4214068828
           |-   0110 1010 0101 0001 1101 1011 1010 1010(2) =   6A51DBAA, 1783749546
           +------------------------------------------------------------------------
    Add       1 0110 0101 0111 1111 0111 0010 0000 0110(2) = 1 657F7206, 5997818374
    Sub         1001 0000 1101 1011 1011 1010 1011 0010(2) =   90DBBAB2, 2430319282 <- Calculated by Calculator
            +------------------------------------------------------------------------
    Not Y       1001 0101 1010 1110 0010 0100 0101 0101(2) =   95AE2455, 2511217749 <- One's Complement
    Not Y+1     1001 0101 1010 1110 0010 0100 0101 0110(2) =   95AE2456, 2511217750 <= Two's Complement
            +------------------------------------------------------------------------
            |   1111 1011 0010 1101 1001 0110 0101 1100(2) =   FB2D965C, 4214068828
    Not Y+1 |+  1001 0101 1010 1110 0010 0100 0101 0110(2) =   95AE2456, 2511217750
            +------------------------------------------------------------------------
              1 1001 0000 1101 1011 1011 1010 1011 0010(2) = 1 90DBBAB2, 6725286578
              |                                       |
             MSB                                     LSB

    One's Complement = 32 bit Not Calc
    Two's Complement = One's Complement + Carry (LSB)1
    
    --------------------------------------------------------------------------------------
    
    Multiplication For Decimal
    
                            Binary                                Hex     Decimal
    
           |    0000 0000 0000 0000 1001 0110 0101 1100(2) =   0000965C, 0000038492
           |*   0000 0000 0000 0000 1101 1011 1010 1010(2) =   0000DBAA, 0000056234
           +------------------------------------------------------------------------
    Mul         1000 0001 0000 0100 1000 1101 0001 1000(2) =   81048D18, 2164559128
    
                0000 0000 0000 0000 1101 1011 1010 1010     value
                31   27   23   19   15   11   7    3          bit
    
     0(0)   |                       0000 0000 0000 0000    =   00000000, 0000000000
     1(1)   |                     1 0010 1100 1011 100     =   00012CB8, 0000076984
     2(0)   |                    00 0000 0000 0000 00      =   00000000, 0000000000
     3(1)   |                   100 1011 0010 1110 0       =   0004B2E0, 0000307936
     4(0)   |                  0000 0000 0000 0000         =   00000000, 0000000000
     5(1)   |                1 0010 1100 1011 100          =   0012CB80, 0001231744
     6(0)   |               00 0000 0000 0000 00           =   00000000, 0000000000
     7(1)   |              100 1011 0010 1110 0            =   004B2E00, 0004926976
     8(1)   |             1001 0110 0101 1100              =   00965C00, 0009853952
     9(1)   |           1 0010 1100 1011 100               =   012CB800, 0019707904
    10(0)   |          00 0000 0000 0000 00                =   00000000, 0000000000
    11(1)   |         100 1011 0010 1110 0                 =   04B2E000, 0078831616
    12(1)   |        1001 0110 0101 1100                   =   0965C000, 0157663232
    13(0)   |      0 0000 0000 0000 000                    =   00000000, 0000000000
    14(1)   |     10 0101 1001 0111 00                     =   25970000, 0630652928
    15(1)   |+   100 1011 0010 1110 0                      =   4B2E0000, 1261305856
            +------------------------------------------------------------------------
                1000 0001 0000 0100 1000 1101 0001 1000(2) =   81048D18, 2164559128
                
    --------------------------------------------------------------------------------------
    
    Binary To BCD

                        BCD            Binary

                0000 0000 0000 0000 1101100111110(2) =   00006974
                0000 0000 0000 0001 1011001111100 <<
                0000 0000 0000 0011 0110011111000 << 
                0000 0000 0000 0110 1100111110000 <<
                0000 0000 0000 1001 1100111110000 + 3(0)
                0000 0000 0001 0011 1001111100000 <<
                0000 0000 0010 0111 0011111000000 <<
                0000 0000 0010 1010 0011111000000 + 3(0)
                0000 0000 0101 0100 0111110000000 <<
                0000 0000 1000 0100 0111110000000 + 3(1)
                0000 0000 1000 0100 0111110000000 <<
                0000 0001 0000 1000 1111100000000 <<
                0000 0001 0000 1011 1111100000000 + 3(0)
                0000 0010 0001 0111 1111000000000 <<
                0000 0010 0001 1010 1111000000000 + 3(0)
                0000 0100 0011 0101 1110000000000 <<
                0000 0100 0011 1000 1110000000000 + 3(0)
                0000 1000 0111 0001 1100000000000 <<
                0000 1011 1010 0001 1100000000000 + 3(2,1)
                0001 0111 0100 0011 1000000000000 <<
                0001 1010 0100 0011 1000000000000 + 3(2)
                0011 0100 1000 0111 0000000000000 <<
                0011 0100 1011 1010 0000000000000 + 3(1,0)
                0110 1001 0111 0100 0000000000000 <<

    It is possible to represent all of the numbers are performed in alu operations 
    independently, just leave to be easy to use. Conversion function included in 
    this class, are implemented using "Double dabble" algorithm.
    Usually introduced method is as follows.

    [1] Repeat shift msb bit on new binary set until hit [2] condition or all bits shifted.
    [2] If each the interval by 4 column is larger equal than 5, adds 3 each column.
    [3] Continue [1] semantics.

    The core is the addition of 3 when it is shifted is convinced that is 10 or more, 
    because, when 101 is shifted, is 1010 but this cannot represented by bcd, 
    but by shift-and-adding 101 to 3 to 10000. That is, even in the case of 111, 
    can be the number of up to 4 to ensure fixed.

--------------------------------------------------------------------------------------------*/
template<
    unsigned int I,
    unsigned int R,
    unsigned int blocks = I / (sizeof(pointer_type)*8),
    unsigned int bcdblocks = I / 4
>   // I >= 4, R >= 1, I % 4 == 0, bcdblocks > 0, remain < 4
class alu {
public:
    static_assert ( bcdblocks > 0 && (bcdblocks == I / 4) , "ALU Bit Setting Error!" );
    static_assert ( blocks > 0 && (blocks == I / (sizeof(pointer_type)*8)) , "ALU Bit Setting Error!" );
    static_assert ( (I % (sizeof(pointer_type)*8)) == 0 , "Bits must be a multiple of pointer type." );

    typedef bits<I> bits_type;

    mutable flags flags;
    mutable bits_type _register[R+4];

    void store(const bits_type o, int index) const // operand
    {
        _register[index] = o;
    }

    bits_type load(int index) const
    {
        return _register[index];
    }

    bits_type& load(int index)
    {
        return _register[index];
    }

    void move(int source, int destination) const
    {
        _register[destination] = _register[source];
    }
    
    void and(const bits_type o1, int index) const { and(o1, _register[index], index); }
    void and(const bits_type o1, int index1, int index) const { and(o1, _register[index1], index); }
    void and(int index, const bits_type o1) const { and(_register[index], o1, index); }
    void and(int index1, const bits_type o1, int index) const { and(_register[index1], o1, index); }
    void and(int index1, int index2) const { and(_register[index1], _register[index2], index1); }
    void and(int index1, int index2, int index) const { and(_register[index1], _register[index2], index); }
    
    void or(const bits_type o1, int index) const { or(o1, _register[index], index); }
    void or(const bits_type o1, int index1, int index) const { or(o1, _register[index1], index); }
    void or(int index, const bits_type o1) const { or(_register[index], o1, index); }
    void or(int index1, const bits_type o1, int index) const { or(_register[index1], o1, index); }
    void or(int index1, int index2) const { or(_register[index1], _register[index2], index1); }
    void or(int index1, int index2, int index) const { or(_register[index1], _register[index2], index); }
    
    void xor(const bits_type o1, int index) const { xor(o1, _register[index], index); }
    void xor(const bits_type o1, int index1, int index) const { xor(o1, _register[index1], index); }
    void xor(int index, const bits_type o1) const { xor(_register[index], o1, index); }
    void xor(int index1, const bits_type o1, int index) const { xor(_register[index1], o1, index); }
    void xor(int index1, int index2) const { xor(_register[index1], _register[index2], index1); }
    void xor(int index1, int index2, int index) const { xor(_register[index1], _register[index2], index); }
    
    void not(const bits_type o1, int index) const { _not(o1, index); }
    void not(int index1) const { _not(_register[index1], index1); }
    void not(int index1, int index) const { _not(_register[index1], index); }

    void add(const bits_type o1, int index) const { add(o1, _register[index], index); }
    void add(const bits_type o1, int index1, int index) const { add(o1, _register[index1], index); }
    void add(int index, const bits_type o1) const { add(_register[index], o1, index); }
    void add(int index1, const bits_type o1, int index) const { add(_register[index1], o1, index); }
    void add(int index1, int index2) const { add(_register[index1], _register[index2], index1); }
    void add(int index1, int index2, int index) const { add(_register[index1], _register[index2], index); }
    
    void sub(const bits_type o1, int index) const { sub(o1, _register[index], index); }
    void sub(const bits_type o1, int index1, int index) const { sub(o1, _register[index1], index); }
    void sub(int index, const bits_type o1) const { sub(_register[index], o1, index); }
    void sub(int index1, const bits_type o1, int index) const { sub(_register[index1], o1, index); }
    void sub(int index1, int index2) const { sub(_register[index1], _register[index2], index1); }
    void sub(int index1, int index2, int index) const { sub(_register[index1], _register[index2], index); }
    
    void shl(const bits_type o1, int index) const { shl(o1, _register[index], index); }
    void shl(const bits_type o1, int index1, int index) const { shl(o1, _register[index1], index); }
    void shl(int index, const bits_type o1) const { shl(_register[index], o1, index); }
    void shl(int index1, const bits_type o1, int index) const { shl(_register[index1], o1, index); }
    void shl(int index1, int index2) const { shl(_register[index1], _register[index2], index1); }
    void shl(int index1, int index2, int index) const { shl(_register[index1], _register[index2], index); }
    
    void shr(const bits_type o1, int index) const { shr(o1, _register[index], index); }
    void shr(const bits_type o1, int index1, int index) const { shr(o1, _register[index1], index); }
    void shr(int index, const bits_type o1) const { shr(_register[index], o1, index); }
    void shr(int index1, const bits_type o1, int index) const { shr(_register[index1], o1, index); }
    void shr(int index1, int index2) const { shr(_register[index1], _register[index2], index1); }
    void shr(int index1, int index2, int index) const { shr(_register[index1], _register[index2], index); }
    
    void mul(const bits_type o1, int index) const { mul(o1, _register[index], index); }
    void mul(const bits_type o1, int index1, int index) const { mul(o1, _register[index1], index); }
    void mul(int index, const bits_type o1) const { mul(_register[index], o1, index); }
    void mul(int index1, const bits_type o1, int index) const { mul(_register[index1], o1, index); }
    void mul(int index1, int index2) const { mul(_register[index1], _register[index2], index1); }
    void mul(int index1, int index2, int index) const { mul(_register[index1], _register[index2], index); }
    
    void div(const bits_type o1, int index) const { div(o1, _register[index], index); }
    void div(const bits_type o1, int index1, int index) const { div(o1, _register[index1], index); }
    void div(int index, const bits_type o1) const { div(_register[index], o1, index); }
    void div(int index1, const bits_type o1, int index) const { div(_register[index1], o1, index); }
    void div(int index1, int index2) const { div(_register[index1], _register[index2], index1); }
    void div(int index1, int index2, int index) const { div(_register[index1], _register[index2], index); }
    
    void mod(const bits_type o1, int index) const { mod(o1, _register[index], index); }
    void mod(const bits_type o1, int index1, int index) const { mod(o1, _register[index1], index); }
    void mod(int index, const bits_type o1) const { mod(_register[index], o1, index); }
    void mod(int index1, const bits_type o1, int index) const { mod(_register[index1], o1, index); }
    void mod(int index1, int index2) const { mod(_register[index1], _register[index2], index1); }
    void mod(int index1, int index2, int index) const { mod(_register[index1], _register[index2], index); }

    void les(const bits_type o1, int index1, int index) const { less(o1, _register[index1], index); }
    void les(int index1, const bits_type o1, int index) const { less(_register[index1], o1, index); }
    void les(int index1, int index2, int index) const { less(_register[index1], _register[index2], index); }

    void leq(const bits_type o1, int index1, int index) const { lessequal(o1, _register[index1], index); }
    void leq(int index1, const bits_type o1, int index) const { lessequal(_register[index1], o1, index); }
    void leq(int index1, int index2, int index) const { lessequal(_register[index1], _register[index2], index); }
    
    void gre(const bits_type o1, int index1, int index) const { greater(o1, _register[index1], index); }
    void gre(int index1, const bits_type o1, int index) const { greater(_register[index1], o1, index); }
    void gre(int index1, int index2, int index) const { greater(_register[index1], _register[index2], index); }
    
    void geq(const bits_type o1, int index1, int index) const { greaterequal(o1, _register[index1], index); }
    void geq(int index1, const bits_type o1, int index) const { greaterequal(_register[index1], o1, index); }
    void geq(int index1, int index2, int index) const { greaterequal(_register[index1], _register[index2], index); }
    
    void eql(const bits_type o1, int index1, int index) const { equal(o1, _register[index1], index); }
    void eql(int index1, const bits_type o1, int index) const { equal(_register[index1], o1, index); }
    void eql(int index1, int index2, int index) const { equal(_register[index1], _register[index2], index); }

    void bcd(const bits_type o, int index) const { b2d(o, index); }
    void bcd(int index1, int index) const { b2d(_register[index1], index); }
    void bcd(int index1) const { b2d(_register[index1], index1); }

    void dcb(const bits_type o, int index) const { d2b(o, index); }
    void dcb(int index1, int index) const { d2b(_register[index1], index); }
    void dcb(int index1) const { d2b(_register[index1], index1); }

    void clz(const bits_type o, int index) const { count_leading_zero(o, index); }
    void clz(int index1, int index) const { count_leading_zero(_register[index1], index); }
    void clz(int index1) const { count_leading_zero(_register[index1], index1); }

    void ctz(const bits_type o, int index) const { count_tailing_zero(o, index); }
    void ctz(int index1, int index) const { count_tailing_zero(_register[index1], index); }
    void ctz(int index1) const { count_leading_zero(_register[index1], index1); }
    
    void abs(const bits_type o, int index) const { absolute(o, index); }
    void abs(int index1, int index) const { absolute(_register[index1], index); }
    void abs(int index1) const { absolute(_register[index1], index1); }

    void nabs(const bits_type o, int index) const { notabsolute(o, index); }
    void nabs(int index1, int index) const { notabsolute(_register[index1], index); }
    void nabs(int index1) const { notabsolute(_register[index1], index1); }
    
    void bon(const bits_type o1, int index) const { bit_on(o1, _register[index], index); }
    void bon(const bits_type o1, int index1, int index) const { bit_on(o1, _register[index1], index); }
    void bon(int index, const bits_type o1) const { bit_on(_register[index], o1, index); }
    void bon(int index1, const bits_type o1, int index) const { bit_on(_register[index1], o1, index); }
    void bon(int index1, int index2) const { bit_on(_register[index1], _register[index2], index1); }
    void bon(int index1, int index2, int index) const { bit_on(_register[index1], _register[index2], index); }
    
    void boff(const bits_type o1, int index) const { bit_off(o1, _register[index], index); }
    void boff(const bits_type o1, int index1, int index) const { bit_off(o1, _register[index1], index); }
    void boff(int index, const bits_type o1) const { bit_off(_register[index], o1, index); }
    void boff(int index1, const bits_type o1, int index) const { bit_off(_register[index1], o1, index); }
    void boff(int index1, int index2) const { bit_off(_register[index1], _register[index2], index1); }
    void boff(int index1, int index2, int index) const { bit_off(_register[index1], _register[index2], index); }

    void inc(int index1) const { _inc(index1); }
    void dec(int index1) const { _dec(index1); }

private:

    void and(const bits_type o1, const bits_type o2, int index) const
    {
        _register[index] = o1 & o2;
    }
    
    void or(const bits_type o1, const bits_type o2, int index) const
    {
        _register[index] = o1 | o2;
    }
    
    void xor(const bits_type o1, const bits_type o2, int index) const
    {
        _register[index] = o1 ^ o2;
    }
    
    void _not(const bits_type o, int index) const
    {
        _register[index] = ~o;
    }

    void add(const bits_type o1, const bits_type o2, int index) const
    {
        bits_type reg;
        bit carry;

        flags_clear();

        for (int i = 0; i < I; i++)
        {
            bit halfX = o1[i] ^ o2[i];
            bit halfA = o1[i] & o2[i];
            
            reg[i] = halfX ^ carry;
            carry = (halfX & carry) | halfA;
        }

        if (carry == on) flag_cf(), flag_of();
        if (reg.zero() == on) flag_zf();
        _register[index] = reg;
    }

    void sub(const bits_type o1, const bits_type o2, int index) const
    {
        _register[index] = ~o2;
        add(bits_type({1}), index);
        add(o1, index);
        if ( _register[index][I-1] == on )
            flag_sf();
    }
    
    void _inc(int index) const
    {
        bit carry;

        flags_clear();
        if ( _register[index][0] == off )
            _register[index][0] = on;
        else
        {
            _register[index][0] = off;
            carry = on;
            for (int i = 1; i < I; i++)
            {
                if ( _register[index][i] == off )
                {
                    carry = off;
                    _register[index][i] = on;
                    break;
                }
                else
                {
                    _register[index][i] = off;
                }
            }
        }
        
        if (carry == on) flag_cf(), flag_of();
    }
    
    void _dec(int index) const
    {
        flags_clear();
        if ( _register[index][0] == on )
        {
            _register[index][0] = off;
            if (_register[index].zero() == on) flag_zf();
        }
        else
        {
            _register[index][0] = on;
            for (int i = 1; i < I; i++)
            {
                if ( _register[index][i] == on )
                {
                    _register[index][i] = off;
                    break;
                }
                else
                {
                    _register[index][i] = on;
                }
            }
        }

        if ( _register[index][I-1] == on )
            flag_sf();
    }

    void shl(const bits_type o1, const bits_type o2, int index) const
    {
        bits_type reg = o1;
        
        flags_clear();

        if (o2.zero() == off) 
        {
            _register[index] = o2;
            for (int i = 0; flag_zero() == off && i < I; i++)
            {
                for (int j = I-1; j > i; j--)
                    reg[j] = reg[j-1];
				reg[i] = off;
                _dec(index);
            }
        }
		
        flags_clear();
        
        if (reg.zero() == on) flag_zero();
        _register[index] = reg;
    }
    
    void shr(const bits_type o1, const bits_type o2, int index) const
    {
        bits_type reg = o1;
        
        flags_clear();
        
        if (o2.zero() == off) 
        {
            _register[index] = o2;
            for (int i = 1; flag_zero() == off && i < I; i++)
            {
                for (int j = i; j < I; j++)
                    reg[j-1] = reg[j];
				reg[I-i] = off;
                _dec(index);
            }
        }

        flags_clear();
        
        if (reg.zero() == on) flag_zero();
        _register[index] = reg;
    }

    void mul(const bits_type o1, const bits_type o2, int index) const
    {
        bit sign;
        bits_type no1, no2;

        if ( o1.msb() == on )
        {
            sign.reverse();
            _register[index] = o1;
            _dec(index);
            no1 = ~_register[index];
        }
        else
            no1 = o1;
        if ( o2.msb() == on )
        {
            sign.reverse();
            _register[index] = o2;
            _dec(index);
            no2 = ~_register[index];
        }
        else
            no2 = o2;

        flags_clear();

        _register[index] = _register[index] ^ _register[index];
        _register[R] = no2;
        for (int i = 0; i < I; i++)
        {
            if (no1[i] == on)
            {
                add(index,R,index);
            }
            shl(R,bits_type({1}),R);
        }

        if (sign == on)
        {
            _register[index] = ~_register[index];
            _inc(index);
        }
    }

    void div(const bits_type o1, const bits_type o2, int index) const
    {
        bit sign;
        bits_type no1, no2;

        if ( o1.msb() == on )
        {
            sign.reverse();
            _register[index] = o1;
            _dec(index);
            no1 = ~_register[index];
        }
        else
            no1 = o1;
        if ( o2.msb() == on )
        {
            sign.reverse();
            _register[index] = o2;
            _dec(index);
            no2 = ~_register[index];
        }
        else
            no2 = o2;

        flags_clear();

        lessequal(no1, no2, R);
        if (_register[R][0] == on)
        {
            equal(no1, no2, R);
            if (_register[R][0] == on)
                _register[index] = bits_type({1});
            else
                _register[index] = bits_type({0}), flag_zf();
        }
        else
        {
            _register[index] = bits_type({0});

            count_leading_zero(no1, R);
            count_leading_zero(no2, R+1);

            sub(R+1,R,R+1);
            _register[R] = no2;
            shl(R,R+1,R);
            _register[R+2] = no1;
            
            flags_clear();
            for (int i = 0; flag_sign() == off && i < I; i++)
            {
                geq(R+2,R,R+3);
                if (_register[R+3][0] == on)
                {
                    sub(R+2,R,R+2);
                    bon(index,R+1,index);
                }

                shr(R,bits_type({1}),R);
                _dec(R+1);
            }
        }

        if (sign == on)
        {
            _register[index] = ~_register[index];
            _inc(index);
        }
    }

    void mod(const bits_type o1, const bits_type o2, int index) const
    {
        flags_clear();
        bit sign;
        bits_type no1, no2;

        if ( o1.msb() == on )
        {
            sign.reverse();
            _register[index] = o1;
            _dec(index);
            no1 = ~_register[index];
        }
        else
            no1 = o1;
        if ( o2.msb() == on )
        {
            _register[index] = o2;
            _dec(index);
            no2 = ~_register[index];
        }
        else
            no2 = o2;
        
        lessequal(no1, no2, R);
        if (_register[R][0] == on)
        {
            _register[index] = bits_type({0});
           flag_zf();
        }
        else
        {
            _register[index] = bits_type({0});

            count_leading_zero(no1, R);
            count_leading_zero(no2, R+1);

            sub(R+1,R,R+1);
            _register[R] = no2;
            shl(R,R+1,R);
            _register[R+2] = no1;
            
            flags_clear();
            for (int i = 0; flag_sign() == off && i < I; i++)
            {
                geq(R+2,R,R+3);
                if (_register[R+3][0] == on)
                    sub(R+2,R,R+2);

                shr(R,bits_type({1}),R);
                _dec(R+1);
            }
            _register[index] = _register[R+2];
        }

        if (sign == on)
        {
            _register[index] = ~_register[index];
            _inc(index);
        }
    }

    void less(const bits_type o1, const bits_type o2, int index) const
    {
        bits_type no1, no2;

        if (o1.msb() == on)
        {
            if (o2.msb() == on)
            {
                _register[index] = o1;
                _dec(index);
                no1 = ~_register[index];
                _register[index] = o2;
                _dec(index);
                no2 = ~_register[index];
            }
            else
            {
                _register[index] = bits_type({1});
            }
        }
        else if (o2.msb() == on)
        {
            _register[index] = bits_type({0});
        }
        else
        {
            no1 = o1; no2 = o2;
        }

        for ( int i = I-1; i >= 0; i-- )
        {
            if ( no1[i] != no2[i] )
				if ( no2[i] == on )
                {
                    _register[index] = bits_type({1});
                    return;
                }
				else
					break;
        }
        _register[index] = bits_type({0});
    }
    
    void lessequal(const bits_type o1, const bits_type o2, int index) const
    {
        bits_type no1, no2;

        if (o1.msb() == on)
        {
            if (o2.msb() == on)
            {
                _register[index] = o1;
                _dec(index);
                no1 = ~_register[index];
                _register[index] = o2;
                _dec(index);
                no2 = ~_register[index];
            }
            else
            {
                _register[index] = bits_type({1});
            }
        }
        else if (o2.msb() == on)
        {
            _register[index] = bits_type({0});
        }
        else
        {
            no1 = o1; no2 = o2;
        }

        for ( int i = I-1; i >= 0; i-- )
        {
            if ( no1[i] != no2[i] )
                if ( no1[i] == on )
                {
                    _register[index] = bits_type({0});
                    return;
                }
                else
                {
                    _register[index] = bits_type({1});
                    return;
                }
        }
        _register[index] = bits_type({1});
    }

    void greater(const bits_type o1, const bits_type o2, int index) const
    {
        bits_type no1, no2;

        if (o1.msb() == on)
        {
            if (o2.msb() == on)
            {
                _register[index] = o1;
                _dec(index);
                no1 = ~_register[index];
                _register[index] = o2;
                _dec(index);
                no2 = ~_register[index];
            }
            else
            {
                _register[index] = bits_type({0});
            }
        }
        else if (o2.msb() == on)
        {
            _register[index] = bits_type({1});
        }
        else
        {
            no1 = o1; no2 = o2;
        }

        for ( int i = I-1; i >= 0; i-- )
        {
            if ( no1[i] != no2[i] )
				if ( no1[i] == on )
                {
                    _register[index] = bits_type({1});
                    return;
                }
				else
					break;
        }
        _register[index] = bits_type({0});
    }
    
    void greaterequal(const bits_type o1, const bits_type o2, int index) const
    {
        bits_type no1, no2;

        if (o1.msb() == on)
        {
            if (o2.msb() == on)
            {
                _register[index] = o1;
                _dec(index);
                no1 = ~_register[index];
                _register[index] = o2;
                _dec(index);
                no2 = ~_register[index];
            }
            else
            {
                _register[index] = bits_type({0});
            }
        }
        else if (o2.msb() == on)
        {
            _register[index] = bits_type({1});
        }
        else
        {
            no1 = o1; no2 = o2;
        }

        for ( int i = I-1; i >= 0; i-- )
        {
            if ( no1[i] != no2[i] )
                if ( no2[i] == on )
                {
                    _register[index] = bits_type({0});
                    return;
                }
                else
                {
                    _register[index] = bits_type({1});
                    return;
                }
        }
        _register[index] = bits_type({1});
    }

    void equal(const bits_type o1, const bits_type o2, int index) const
    {
        _register[index] = memcmp(o1.s, o2.s, sizeof(bit)*I) == 0;
    }

    void absolute(const bits_type o, int index) const
    {
        _register[index] = o;
        if (o.msb() == on)
        {
            _dec(index);
            _register[index] = ~_register[index];
        }
    }

    void notabsolute(const bits_type o, int index) const
    {
        _register[index] = o;
        if (o.msb() == off)
        {
            _register[index] = ~_register[index];
            _inc(index);
        }
    }

    void b2d(const bits_type o, int index) const
    {
        bits_type reg;
        
        if (o.msb() == on)
        {
            _register[index] = o;
            _dec(index);
            _register[R+1] = ~_register[index];
        }
        else
            _register[R+1] = o;

        for ( int i = I-1; i > 0; i-- )
        {
            reg[0] = _register[R+1][i];

            for ( int i = bcdblocks; i > 0; i-- )
            {
                bool larger_101 = false;
                if ( reg[i*4-1] == on )
                    larger_101 = true;
                else if ( reg[i*4-2] == on && 
                    ( reg[i*4-3] == on || reg[i*4-4] == on ) )
                    larger_101 = true;

                if (larger_101)
                {
                    bits_type add_m;
                    add_m[i*4-3] = add_m[i*4-4] = on;
                    add(add_m, reg, R);
                    reg = _register[R];
                }
            }
            
            shl(reg, bits_type({1}), R);
            reg = _register[R];
        }


        reg[0] = o[0];

        _register[index] = reg;
    }
    
    void d2b(const bits_type o, int index) const
    {
        bits_type reg;

        if (o.msb() == on)
        {
            _register[index] = o;
            _dec(index);
            _register[R+1] = ~_register[index];
        }
        else
            _register[R+1] = o;

        for ( int i = 0; i < I; i++ )
        {
            shr(reg, bits_type({1}), R);
            reg = _register[R];
            reg[I-1] = _register[R+1][0];

            shr(R+1, bits_type({1}), R+1);

            for ( int i = bcdblocks; i > 0; i-- )
            {
                bool larger_101 = false;
                if ( _register[R+1][i*4-1] == on )
                    larger_101 = true;
                else if ( _register[R+1][i*4-2] == on && 
                    ( _register[R+1][i*4-3] == on || _register[R+1][i*4-4] == on ) )
                    larger_101 = true;

                if (larger_101)
                {
                    bits_type sub_m;
                    sub_m[i*4-3] = sub_m[i*4-4] = on;
                    sub(R+1, sub_m, R+1);
                }
            }
        }

        _register[index] = reg;
    }

    void count_leading_zero(const bits_type o, int index) const
    {
        _register[index] = bits_type({0});

        for ( int i = I-1; i >= 0; i-- )
        {
            if ( o[i] == on )
                break;
            _inc(index);
        }
    }
    
    void count_tailing_zero(const bits_type o, int index) const
    {
        _register[index] = bits_type({0});

        for ( int i = 0; i < I; i++ )
        {
            if ( o[i] == on )
                break;
            _inc(index);
        }
    }

    void bit_on(const bits_type o1, const bits_type o2, int index) const
    {
        _register[index] = o2;

        int i = 0;
        for ( int j = 0; j < I; j++)
        {
            if (_register[index].zero() == on)
                break;
            _dec(index);
            i++;
        }
        _register[index] = o1;
        _register[index][i] = on;
    }
    
    void bit_off(const bits_type o1, const bits_type o2, int index) const
    {
        _register[index] = o2;

        int i = 0;
        for ( int j = 0; j < I; j++)
        {
            if (_register[index].zero() == on)
                break;
            _dec(index);
            i++;
        }
        _register[index] = o1;
        _register[index][i] = off;
    }

    inline pointer_type _getblock(const bits_type& o, int offset) const
    {
        return ((pointer_type *)o.s)[offset];
    }

    /*inline int _findleadingzero(const bits_type& o) const
    {
        for ( int i = blocks-1; i >= 0; i++ )
            if ( _getblock(o,i) != 0 )
            {

            }
        return 0;
    }*/

    void flag_sf() const { flags = flags | FLAGS_SIGN; }
    void flag_cf() const { flags = flags | FLAGS_CARRY; }
    void flag_of() const { flags = flags | FLAGS_OVERFLOW; }
    void flag_zf() const { flags = flags | FLAGS_ZERO; }
    void flags_clear() const { flags = FLAGS_NONE; }

    bit flag_carry() const { return flags[0]; }
    bit flag_sign() const { return flags[4]; }
    bit flag_overflow() const { return flags[5]; }
    bit flag_zero() const { return flags[3]; }
};
#pragma pack (pop)

#endif