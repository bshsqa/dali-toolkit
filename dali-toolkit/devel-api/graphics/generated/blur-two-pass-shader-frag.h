std::vector<uint32_t> SHADER_BLUR_TWO_PASS_SHADER_FRAG = {
	0x07230203,0x00010000,0x00080001,0x0000003a,0x00000000,0x00020011,0x00000001,0x0006000b,
	0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
	0x0007000f,0x00000004,0x00000004,0x6e69616d,0x00000000,0x00000020,0x00000038,0x00030010,
	0x00000004,0x00000007,0x00030003,0x00000002,0x000001ae,0x00040005,0x00000004,0x6e69616d,
	0x00000000,0x00040005,0x00000009,0x6f6c6f63,0x00000072,0x00030005,0x0000000e,0x00000069,
	0x00050005,0x00000016,0x5f4d554e,0x504d4153,0x0053454c,0x00050005,0x0000001c,0x78655473,
	0x65727574,0x00000000,0x00050005,0x00000020,0x78655476,0x726f6f43,0x00000064,0x00050005,
	0x00000024,0x72756c42,0x61746144,0x00000000,0x00070006,0x00000024,0x00000000,0x6d615375,
	0x4f656c70,0x65736666,0x00007374,0x00070006,0x00000024,0x00000001,0x6d615375,0x57656c70,
	0x68676965,0x00007374,0x00030005,0x00000026,0x00000000,0x00050005,0x00000038,0x67617266,
	0x6f6c6f43,0x00000072,0x00040047,0x00000016,0x00000001,0x00000000,0x00040047,0x0000001c,
	0x00000022,0x00000000,0x00040047,0x0000001c,0x00000021,0x00000002,0x00030047,0x00000020,
	0x00000000,0x00040047,0x00000020,0x0000001e,0x00000000,0x00030047,0x00000021,0x00000000,
	0x00040047,0x00000022,0x00000006,0x00000010,0x00040047,0x00000023,0x00000006,0x00000010,
	0x00050048,0x00000024,0x00000000,0x00000023,0x00000000,0x00050048,0x00000024,0x00000001,
	0x00000023,0x00000100,0x00030047,0x00000024,0x00000002,0x00040047,0x00000026,0x00000022,
	0x00000000,0x00040047,0x00000026,0x00000021,0x00000001,0x00040047,0x00000038,0x0000001e,
	0x00000000,0x00020013,0x00000002,0x00030021,0x00000003,0x00000002,0x00030016,0x00000006,
	0x00000020,0x00040017,0x00000007,0x00000006,0x00000004,0x00040020,0x00000008,0x00000007,
	0x00000007,0x0004002b,0x00000006,0x0000000a,0x00000000,0x0007002c,0x00000007,0x0000000b,
	0x0000000a,0x0000000a,0x0000000a,0x0000000a,0x00040015,0x0000000c,0x00000020,0x00000001,
	0x00040020,0x0000000d,0x00000007,0x0000000c,0x0004002b,0x0000000c,0x0000000f,0x00000000,
	0x00040032,0x0000000c,0x00000016,0x00000010,0x00020014,0x00000017,0x00090019,0x00000019,
	0x00000006,0x00000001,0x00000000,0x00000000,0x00000000,0x00000001,0x00000000,0x0003001b,
	0x0000001a,0x00000019,0x00040020,0x0000001b,0x00000000,0x0000001a,0x0004003b,0x0000001b,
	0x0000001c,0x00000000,0x00040017,0x0000001e,0x00000006,0x00000002,0x00040020,0x0000001f,
	0x00000001,0x0000001e,0x0004003b,0x0000001f,0x00000020,0x00000001,0x0004001c,0x00000022,
	0x0000001e,0x00000016,0x0004001c,0x00000023,0x00000006,0x00000016,0x0004001e,0x00000024,
	0x00000022,0x00000023,0x00040020,0x00000025,0x00000002,0x00000024,0x0004003b,0x00000025,
	0x00000026,0x00000002,0x00040020,0x00000028,0x00000002,0x0000001e,0x0004002b,0x0000000c,
	0x0000002d,0x00000001,0x00040020,0x0000002f,0x00000002,0x00000006,0x00040020,0x00000037,
	0x00000003,0x00000007,0x0004003b,0x00000037,0x00000038,0x00000003,0x00050036,0x00000002,
	0x00000004,0x00000000,0x00000003,0x000200f8,0x00000005,0x0004003b,0x00000008,0x00000009,
	0x00000007,0x0004003b,0x0000000d,0x0000000e,0x00000007,0x0003003e,0x00000009,0x0000000b,
	0x0003003e,0x0000000e,0x0000000f,0x000200f9,0x00000010,0x000200f8,0x00000010,0x000400f6,
	0x00000012,0x00000013,0x00000000,0x000200f9,0x00000014,0x000200f8,0x00000014,0x0004003d,
	0x0000000c,0x00000015,0x0000000e,0x000500b1,0x00000017,0x00000018,0x00000015,0x00000016,
	0x000400fa,0x00000018,0x00000011,0x00000012,0x000200f8,0x00000011,0x0004003d,0x0000001a,
	0x0000001d,0x0000001c,0x0004003d,0x0000001e,0x00000021,0x00000020,0x0004003d,0x0000000c,
	0x00000027,0x0000000e,0x00060041,0x00000028,0x00000029,0x00000026,0x0000000f,0x00000027,
	0x0004003d,0x0000001e,0x0000002a,0x00000029,0x00050081,0x0000001e,0x0000002b,0x00000021,
	0x0000002a,0x00050057,0x00000007,0x0000002c,0x0000001d,0x0000002b,0x0004003d,0x0000000c,
	0x0000002e,0x0000000e,0x00060041,0x0000002f,0x00000030,0x00000026,0x0000002d,0x0000002e,
	0x0004003d,0x00000006,0x00000031,0x00000030,0x0005008e,0x00000007,0x00000032,0x0000002c,
	0x00000031,0x0004003d,0x00000007,0x00000033,0x00000009,0x00050081,0x00000007,0x00000034,
	0x00000033,0x00000032,0x0003003e,0x00000009,0x00000034,0x000200f9,0x00000013,0x000200f8,
	0x00000013,0x0004003d,0x0000000c,0x00000035,0x0000000e,0x00050080,0x0000000c,0x00000036,
	0x00000035,0x0000002d,0x0003003e,0x0000000e,0x00000036,0x000200f9,0x00000010,0x000200f8,
	0x00000012,0x0004003d,0x00000007,0x00000039,0x00000009,0x0003003e,0x00000038,0x00000039,
	0x000100fd,0x00010038
};
