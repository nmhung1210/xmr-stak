#pragma once

#include "crypto/cryptonight.h"
#include "xmrstak/backend/miner_work.hpp"
#include "xmrstak/backend/iBackend.hpp"

#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <future>

namespace xmrstak
{
namespace cpu
{

class minethd : public iBackend
{
public:
	static std::vector<iBackend*> thread_starter(uint32_t threadOffset, miner_work& pWork);
	static bool self_test();

	typedef void (*cn_hash_fun)(const void*, size_t, void*, cryptonight_ctx*);

	static cn_hash_fun func_selector(bool bHaveAes, bool bNoPrefetch, bool mineMonero);
	static bool thd_setaffinity(std::thread::native_handle_type h, uint64_t cpu_id);

	static cryptonight_ctx* minethd_alloc_ctx();

private:
	typedef void (*cn_hash_fun_multi)(const void*, size_t, void*, cryptonight_ctx**);
	static cn_hash_fun_multi func_multi_selector(size_t N, bool bHaveAes, bool bNoPrefetch, bool mineMonero);

	minethd(miner_work& pWork, size_t iNo, int iMultiway, bool no_prefetch, int64_t affinity);

	template<size_t N>
	void multiway_work_main(cn_hash_fun_multi hash_fun_multi);

	template<size_t N>
	void prep_multiway_work(uint8_t *bWorkBlob, uint32_t **piNonce);

	void work_main();
	void double_work_main();
	void triple_work_main();
	void quad_work_main();
	void penta_work_main();
	
	void x1_work_main();
	void x2_work_main();
	void x3_work_main();
	void x4_work_main();
	void x5_work_main();
	void x6_work_main();
	void x7_work_main();
	void x8_work_main();
	void x9_work_main();
	void x10_work_main();
	void x11_work_main();
	void x12_work_main();
	void x13_work_main();
	void x14_work_main();
	void x15_work_main();
	void x16_work_main();
	void x17_work_main();
	void x18_work_main();
	void x19_work_main();
	void x20_work_main();
	void x21_work_main();
	void x22_work_main();
	void x23_work_main();
	void x24_work_main();
	void x25_work_main();
	void x26_work_main();
	void x27_work_main();
	void x28_work_main();
	void x29_work_main();
	void x30_work_main();
	void x31_work_main();
	void x32_work_main();
	void x33_work_main();
	void x34_work_main();
	void x35_work_main();
	void x36_work_main();
	void x37_work_main();
	void x38_work_main();
	void x39_work_main();
	void x40_work_main();
	void x41_work_main();
	void x42_work_main();
	void x43_work_main();
	void x44_work_main();
	void x45_work_main();
	void x46_work_main();
	void x47_work_main();
	void x48_work_main();
	void x49_work_main();
	void x50_work_main();
	void x51_work_main();
	void x52_work_main();
	void x53_work_main();
	void x54_work_main();
	void x55_work_main();
	void x56_work_main();
	void x57_work_main();
	void x58_work_main();
	void x59_work_main();
	void x60_work_main();
	void x61_work_main();
	void x62_work_main();
	void x63_work_main();
	void x64_work_main();
	void x65_work_main();
	void x66_work_main();
	void x67_work_main();
	void x68_work_main();
	void x69_work_main();
	void x70_work_main();
	void x71_work_main();
	void x72_work_main();
	void x73_work_main();
	void x74_work_main();
	void x75_work_main();
	void x76_work_main();
	void x77_work_main();
	void x78_work_main();
	void x79_work_main();
	void x80_work_main();
	void x81_work_main();
	void x82_work_main();
	void x83_work_main();
	void x84_work_main();
	void x85_work_main();
	void x86_work_main();
	void x87_work_main();
	void x88_work_main();
	void x89_work_main();
	void x90_work_main();
	void x91_work_main();
	void x92_work_main();
	void x93_work_main();
	void x94_work_main();
	void x95_work_main();
	void x96_work_main();
	void x97_work_main();
	void x98_work_main();
	void x99_work_main();
	void x100_work_main();
	void x101_work_main();
	void x102_work_main();
	void x103_work_main();
	void x104_work_main();
	void x105_work_main();
	void x106_work_main();
	void x107_work_main();
	void x108_work_main();
	void x109_work_main();
	void x110_work_main();
	void x111_work_main();
	void x112_work_main();
	void x113_work_main();
	void x114_work_main();
	void x115_work_main();
	void x116_work_main();
	void x117_work_main();
	void x118_work_main();
	void x119_work_main();
	void x120_work_main();
	void x121_work_main();
	void x122_work_main();
	void x123_work_main();
	void x124_work_main();
	void x125_work_main();
	void x126_work_main();
	void x127_work_main();
	void x128_work_main();

	void consume_work();

	uint64_t iJobNo;

	static miner_work oGlobalWork;
	miner_work oWork;

	std::promise<void> order_fix;
	std::mutex thd_aff_set;

	std::thread oWorkThd;
	int64_t affinity;

	bool bQuit;
	bool bNoPrefetch;
};

} // namespace cpu
} // namepsace xmrstak
