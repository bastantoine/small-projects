a = """
                           ==Phrack Inc.==

              Volume 0x0b, Issue 0x3d, Phile #0x0b of 0x0f

|=------------=[ Building IA32 'Unicode-Proof' Shellcodes ]=-------------=|
|=-----------------------------------------------------------------------=|
|=------------=[ obscou <obscou@dr.com||wishkah@chek.com> ]=-------------=|

--[ 0 - The Unicode Standard

While   exploiting   buffer   overflows,   we  sometime face a difficulty :
character transformations. In fact, the exploited program may have modified
our buffer, by setting it to lower/upper case, or by getting rid of
non-alphanumeric characters, thus stopping the attack as our shellcode
usually can't run anymore. The transformation we are dealing here with is
the transformation of a C-type string (common zero terminated string) to a
Unicode string.
"""

if __name__ == '__main__':
    data = a.split("\n")
