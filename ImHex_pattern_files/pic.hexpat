#pragma endian big

struct Header {
    u16 width;
    u16 height;
    u16 z; // maybe this is just padding
};

struct Pic {
    Header header;
};

Pic pic @ 0;