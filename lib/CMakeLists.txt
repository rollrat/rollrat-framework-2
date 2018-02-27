add_library(librollrat STATIC  
    DateTime.cpp
    File.cpp
    StringTools.cpp
    WString.cpp
    WStringBuffer.cpp
    WStringBuilder.cpp
    BigInteger/BigBase.cpp
    BigInteger/BigFraction.cpp
    BigInteger/BigInteger.cpp
    Crypto/Crypto.cpp
    Crypto/Sha256.c
    Encoding/Base64Encoding.cpp
    Encoding/UrlEncoding.cpp

    ${INCLUDE_DIRECTORIES}
    )