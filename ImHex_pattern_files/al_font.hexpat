#pragma endian big

bitfield AL_bitfield {
    proportioal: 1;
    baseline: 7;
};

struct XHdata {
    u16 XW;
    u8 HD;
    u8 XH;
};

struct AL {
    u16 height;
    AL_bitfield;
    u8 maxWidth;
    u16 pointers[0o377]; // 256
};

AL al @ 0x0;

XHdata xhdata_dummy @ 0x220;