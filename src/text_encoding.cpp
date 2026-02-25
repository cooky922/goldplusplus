// gold++ library <gold/src/text_encoding.cpp>

#include <gold/text_encoding>
#include <gold/basic_types>
#include <utility>
#include <algorithm>
#include <string>
#include <windows.h>

// maps code page id to gold::text_encoding and vice versa

namespace gold {

    namespace __text {

        // more info: https://learn.microsoft.com/en-us/windows/win32/api/winnls/ns-winnls-cpinfoexa

        // ref: https://learn.microsoft.com/en-us/windows/win32/intl/code-page-identifiers
        enum class CodePageID : gold::uint32_t {
            IBM037 = 37,
            IBM437 = 437,
            IBM500 = 500,
            ASMO708 = 708,
            DOS720  = 720,
            IBM737  = 737,
            IBM775  = 775,
            IBM850  = 850,
            IBM852  = 852,
            IBM855  = 855,
            IBM857  = 857,
            IBM858  = 858,
            IBM860  = 860,
            IBM861  = 861,
            DOS862  = 862,
            IBM863  = 863,
            IBM864  = 864,
            IBM865  = 865,
            CP866   = 866,
            IBM869  = 869,
            IBM870  = 870,
            Windows874 = 874,
            CP875 = 875,
            ShiftJIS = 932,
            GB2312 = 936,
            KSC56011987 = 949,
            Big5 = 950,
            IBM1026 = 1026,
            IBM1047 = 1047,
            IBM1140 = 1140,
            IBM1141 = 1141,
            IBM1142 = 1142,
            IBM1143 = 1143,
            IBM1144 = 1144,
            IBM1145 = 1145,
            IBM1146 = 1146,
            IBM1147 = 1147,
            IBM1148 = 1148,
            IBM1149 = 1149,
            UTF16   = 1200,
            UTF16LE = UTF16,
            UTF16BE = 1201,
            Windows1250 = 1250,
            Windows1251 = 1251,
            Windows1252 = 1252,
            Windows1253 = 1253,
            Windows1254 = 1254,
            Windows1255 = 1255,
            Windows1256 = 1256,
            Windows1257 = 1257,
            Windows1258 = 1258,
            Johab = 1361,
            Macintosh = 10'000,
            XMacJapanese = 10'001,
            XMacChineseTrad = 10'002,
            XMacKorean = 10'003,
            XMacArabic = 10'004,
            XMacHebrew = 10'005,
            XMacGreek  = 10'006,
            XMacCyrillic = 10'007,
            XMacChineseSimp = 10'008,
            XMacRomanian = 10'010,
            XMacUkrainian = 10'017,
            XMacThai = 10'021,
            XMacCE = 10'029,
            XMacIcelandic = 10'079,
            XMacTurkish = 10'081,
            XMacCroatian = 10'082,
            UTF32 = 12'000,
            UTF32LE = UTF32,
            UTF32BE = 12'001,
            XChineseCNS = 20'000,
            XCP20001 = 20'001,
            XChineseEten = 20'002,
            XCP20003 = 20'003,
            XCP20004 = 20'004,
            XCP20005 = 20'005,
            XIA5 = 20'105,
            XIA5German = 20'106,
            XIA5Swedish = 20'107,
            XIA5Norwegian = 20'108,
            USASCII = 20'127,
            XCP20261 = 20'261,
            XCP20269 = 20'269,
            IBM273 = 20'273,
            IBM277 = 20'277,
            IBM278 = 20'278,
            IBM280 = 20'280,
            IBM284 = 20'284,
            IBM285 = 20'285,
            IBM290 = 20'290,
            IBM297 = 20'297,
            IBM420 = 20'420,
            IBM423 = 20'423,
            IBM424 = 20'424,
            XKoreanExtended = 20'833,
            IBMThai = 20'838,
            KOI8R = 20'866,
            IBM871 = 20'871,
            IBM880 = 20'880,
            IBM905 = 20'905,
            IBM00924 = 20'924,
            XCP20936 = 20'936,
            XCP20949 = 20'949,
            CP1025 = 21'025,
            KOI8U = 21'866,
            ISOLatin1 = 28'591,
            ISOLatin2 = 28'592,
            ISOLatin3 = 28'593,
            ISOBaltic = 28'594,
            ISOCyrillic = 28'595,
            ISOArabic = 28'596,
            ISOGreek = 28'597,
            ISOVisualHebrew = 28'598,
            ISOTurkish = 28'599,
            ISOEstonian = 28'603,
            ISOLatin9 = 28'605,
            XEuropa = 29'001,
            ISOLogicalHebrew = 38'598,
            ISO2022JPWithNoHalfwidth = 50'220,
            ISO2022JPWithHalfwidth = 50'221,
            ISO2022JP = 50'222,
            ISO2022KR = 50'225,
            ISO2022SimpChinese = 50'227,
            ISO2022TradChinese = 50'229,
            EUCJP = 51'932,
            EUCCN = 51'936,
            EUCKR = 51'949,
            EUCCNTrad = 51'950,
            HZGB2312 = 52'936,
            GB18030 = 54'936,
            XISCIIDE = 57'002, // Devanagari
            XISCIIBE = 57'003, // Bangla
            XISCIITA = 57'004, // Tamil
            XISCIITE = 57'005, // Telugu
            XISCIIAS = 57'006, // Assamese
            XISCIIOR = 57'007, // Odia
            XISCIIKA = 57'008, // Kannada
            XISCIIMA = 57'009, // Malayalam
            XISCIIGU = 57'010, // Gujarati
            XISCIIPA = 57'011, // Punjabi
            UTF7 = 65'000,
            UTF8 = 65'001
        };

        using MIBenum = std::text_encoding::id;

        struct EncodingIDPair {
            __text::CodePageID cp;
            __text::MIBenum    mib;
        };

        // TODO: verify
        static constexpr EncodingIDPair encoding_id_pairs [] = {
            { CodePageID::IBM037,       MIBenum::IBM037       },
            { CodePageID::IBM437,       MIBenum::PC8CodePage437 },
            { CodePageID::IBM500,       MIBenum::IBM500       },
            { CodePageID::ASMO708,      MIBenum::ISO89ASMO449 },
            { CodePageID::DOS720,       MIBenum::windows1256  }, // Closest logical match; actual MIB may vary
            { CodePageID::IBM737,       MIBenum::windows1253  }, // Approximation
            { CodePageID::IBM775,       MIBenum::PC775Baltic  },
            { CodePageID::IBM850,       MIBenum::PC850Multilingual },
            { CodePageID::IBM852,       MIBenum::PCp852       },
            { CodePageID::IBM855,       MIBenum::IBM855       },
            { CodePageID::IBM857,       MIBenum::IBM857       },
            { CodePageID::IBM858,       MIBenum::IBM00858     },
            { CodePageID::IBM860,       MIBenum::IBM860       },
            { CodePageID::IBM861,       MIBenum::IBM861       },
            { CodePageID::DOS862,       MIBenum::PC862LatinHebrew },
            { CodePageID::IBM863,       MIBenum::IBM863       },
            { CodePageID::IBM864,       MIBenum::IBM864       },
            { CodePageID::IBM865,       MIBenum::IBM865       },
            { CodePageID::CP866,        MIBenum::IBM866       },
            { CodePageID::IBM869,       MIBenum::IBM869       },
            { CodePageID::IBM870,       MIBenum::IBM870       },
            { CodePageID::Windows874,   MIBenum::windows874   },
            { CodePageID::CP875,        MIBenum::other        },
            { CodePageID::ShiftJIS,     MIBenum::ShiftJIS     },
            { CodePageID::GB2312,       MIBenum::GB2312       },
            { CodePageID::KSC56011987,  MIBenum::KSC56011987  },
            { CodePageID::Big5,         MIBenum::Big5         },
            { CodePageID::IBM1026,      MIBenum::IBM1026      },
            { CodePageID::IBM1047,      MIBenum::IBM1047      },
            { CodePageID::IBM1140,      MIBenum::IBM01140     },
            { CodePageID::IBM1141,      MIBenum::IBM01141     },
            { CodePageID::IBM1142,      MIBenum::IBM01142     },
            { CodePageID::IBM1143,      MIBenum::IBM01143     },
            { CodePageID::IBM1144,      MIBenum::IBM01144     },
            { CodePageID::IBM1145,      MIBenum::IBM01145     },
            { CodePageID::IBM1146,      MIBenum::IBM01146     },
            { CodePageID::IBM1147,      MIBenum::IBM01147     },
            { CodePageID::IBM1148,      MIBenum::IBM01148     },
            { CodePageID::IBM1149,      MIBenum::IBM01149     },
            { CodePageID::UTF16,        MIBenum::UTF16        },
            { CodePageID::UTF16LE,      MIBenum::UTF16LE      },
            { CodePageID::UTF16BE,      MIBenum::UTF16BE      },
            { CodePageID::Windows1250,  MIBenum::windows1250  },
            { CodePageID::Windows1251,  MIBenum::windows1251  },
            { CodePageID::Windows1252,  MIBenum::windows1252  },
            { CodePageID::Windows1253,  MIBenum::windows1253  },
            { CodePageID::Windows1254,  MIBenum::windows1254  },
            { CodePageID::Windows1255,  MIBenum::windows1255  },
            { CodePageID::Windows1256,  MIBenum::windows1256  },
            { CodePageID::Windows1257,  MIBenum::windows1257  },
            { CodePageID::Windows1258,  MIBenum::windows1258  },
            { CodePageID::Johab,        MIBenum::other        },
            { CodePageID::Macintosh,    MIBenum::Macintosh    },
            { CodePageID::UTF32,        MIBenum::UTF32        },
            { CodePageID::UTF32LE,      MIBenum::UTF32LE      },
            { CodePageID::UTF32BE,      MIBenum::UTF32BE      },
            { CodePageID::USASCII,      MIBenum::ASCII        },
            { CodePageID::KOI8R,        MIBenum::KOI8R        },
            { CodePageID::KOI8U,        MIBenum::KOI8U        },
            { CodePageID::ISOLatin1,    MIBenum::ISOLatin1    },
            { CodePageID::ISOLatin2,    MIBenum::ISOLatin2    },
            { CodePageID::ISOLatin3,    MIBenum::ISOLatin3    },
            { CodePageID::ISOBaltic,    MIBenum::other        },
            { CodePageID::ISOCyrillic,  MIBenum::ISOLatinCyrillic },
            { CodePageID::ISOArabic,    MIBenum::ISOLatinArabic },
            { CodePageID::ISOGreek,     MIBenum::ISOLatinGreek },
            { CodePageID::ISOVisualHebrew, MIBenum::ISO88598I },
            { CodePageID::ISOTurkish,   MIBenum::windows1254  },
            { CodePageID::ISOEstonian,  MIBenum::other        },
            { CodePageID::ISOLatin9,    MIBenum::ISO885915    },
            { CodePageID::ISO2022JP,    MIBenum::ISO2022JP    },
            { CodePageID::ISO2022KR,    MIBenum::ISO2022KR    },
            { CodePageID::EUCJP,        MIBenum::EUCPkdFmtJapanese },
            { CodePageID::EUCCN,        MIBenum::GBK          },
            { CodePageID::EUCKR,        MIBenum::EUCKR        },
            { CodePageID::EUCCNTrad,    MIBenum::Big5         },
            { CodePageID::HZGB2312,     MIBenum::HZGB2312     },
            { CodePageID::GB18030,      MIBenum::GB18030      },
            { CodePageID::XISCIIDE,     MIBenum::other        },
            { CodePageID::XISCIIBE,     MIBenum::other        },
            { CodePageID::XISCIITA,     MIBenum::other        },
            { CodePageID::XISCIITE,     MIBenum::other        },
            { CodePageID::XISCIIAS,     MIBenum::other        },
            { CodePageID::XISCIIOR,     MIBenum::other        },
            { CodePageID::XISCIIKA,     MIBenum::other        },
            { CodePageID::XISCIIMA,     MIBenum::other        },
            { CodePageID::XISCIIGU,     MIBenum::other        },
            { CodePageID::XISCIIPA,     MIBenum::other        },
            { CodePageID::UTF7,         MIBenum::UTF7         },
            { CodePageID::UTF8,         MIBenum::UTF8         }
        };

        // get_current_code_page
        static __text::CodePageID get_current_code_page() noexcept {
            return __text::CodePageID(::GetACP());
        }

        // convert code page identifier to MIBenum value
        static __text::MIBenum to_MIBenum(__text::CodePageID id) noexcept {
            const auto it = std::ranges::lower_bound(
                __text::encoding_id_pairs, id, {}, &__text::EncodingIDPair::cp
            );
            __text::MIBenum result = __text::MIBenum::unknown;
            if (it != std::ranges::end(__text::encoding_id_pairs) && it->cp == id)
                result = it->mib;
            return result;
        }

    } // namespace __text

    text_encoding text_encoding::environment() {
        // NOTE: std::text_encoding is currently unusable under windows
        // so, it is implemented in terms of ::GetACP but its Code Page ID
        // is incompatible with MIBenum values. Hence, it needs to be mapped
        return text_encoding(__text::to_MIBenum(__text::get_current_code_page()));
    }

} // namespace gold
