/*
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
  *
  * Additional permission under GNU GPL version 3 section 7
  *
  * If you modify this Program, or any covered work, by linking or combining
  * it with OpenSSL (or a modified version of that library), containing parts
  * covered by the terms of OpenSSL License and SSLeay License, the licensors
  * of this Program grant you additional permission to convey the resulting work.
  *
  */

#include "crypto/cryptonight_aesni.h"

#include "xmrstak/misc/console.hpp"
#include "xmrstak/backend/iBackend.hpp"
#include "xmrstak/backend//globalStates.hpp"
#include "xmrstak/misc/configEditor.hpp"
#include "xmrstak/params.hpp"
#include "jconf.hpp"

#include "xmrstak/misc/executor.hpp"
#include "minethd.hpp"
#include "xmrstak/jconf.hpp"

#include "hwlocMemory.hpp"
#include "xmrstak/backend/miner_work.hpp"

#ifndef CONF_NO_HWLOC
#   include "autoAdjustHwloc.hpp"
#else
#   include "autoAdjust.hpp"
#endif

#include <assert.h>
#include <cmath>
#include <chrono>
#include <cstring>
#include <thread>
#include <bitset>

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>

#if defined(__APPLE__)
#include <mach/thread_policy.h>
#include <mach/thread_act.h>
#define SYSCTL_CORE_COUNT   "machdep.cpu.core_count"
#elif defined(__FreeBSD__)
#include <pthread_np.h>
#endif //__APPLE__

#endif //_WIN32

namespace xmrstak
{
namespace cpu
{

bool minethd::thd_setaffinity(std::thread::native_handle_type h, uint64_t cpu_id)
{
#if defined(_WIN32)
	return SetThreadAffinityMask(h, 1ULL << cpu_id) != 0;
#elif defined(__APPLE__)
	thread_port_t mach_thread;
	thread_affinity_policy_data_t policy = { static_cast<integer_t>(cpu_id) };
	mach_thread = pthread_mach_thread_np(h);
	return thread_policy_set(mach_thread, THREAD_AFFINITY_POLICY, (thread_policy_t)&policy, 1) == KERN_SUCCESS;
#elif defined(__FreeBSD__)
	cpuset_t mn;
	CPU_ZERO(&mn);
	CPU_SET(cpu_id, &mn);
	return pthread_setaffinity_np(h, sizeof(cpuset_t), &mn) == 0;
#else
	cpu_set_t mn;
	CPU_ZERO(&mn);
	CPU_SET(cpu_id, &mn);
	return pthread_setaffinity_np(h, sizeof(cpu_set_t), &mn) == 0;
#endif
}

minethd::minethd(miner_work& pWork, size_t iNo, int iMultiway, bool no_prefetch, int64_t affinity)
{
	this->backendType = iBackend::CPU;
	oWork = pWork;
	bQuit = 0;
	iThreadNo = (uint8_t)iNo;
	iJobNo = 0;
	bNoPrefetch = no_prefetch;
	this->affinity = affinity;

	std::unique_lock<std::mutex> lck(thd_aff_set);
	std::future<void> order_guard = order_fix.get_future();

	switch (iMultiway)
	{
		case 1:
			oWorkThd = std::thread(&minethd::x1_work_main, this);
			break;
		case 2:
			oWorkThd = std::thread(&minethd::x2_work_main, this);
			break;
		case 3:
			oWorkThd = std::thread(&minethd::x3_work_main, this);
			break;
		case 4:
			oWorkThd = std::thread(&minethd::x4_work_main, this);
			break;
		case 5:
			oWorkThd = std::thread(&minethd::x5_work_main, this);
			break;
		case 6:
			oWorkThd = std::thread(&minethd::x6_work_main, this);
			break;
		case 7:
			oWorkThd = std::thread(&minethd::x7_work_main, this);
			break;
		case 8:
			oWorkThd = std::thread(&minethd::x8_work_main, this);
			break;
		case 9:
			oWorkThd = std::thread(&minethd::x9_work_main, this);
			break;
		case 10:
			oWorkThd = std::thread(&minethd::x10_work_main, this);
			break;
		case 11:
			oWorkThd = std::thread(&minethd::x11_work_main, this);
			break;
		case 12:
			oWorkThd = std::thread(&minethd::x12_work_main, this);
			break;
		case 13:
			oWorkThd = std::thread(&minethd::x13_work_main, this);
			break;
		case 14:
			oWorkThd = std::thread(&minethd::x14_work_main, this);
			break;
		case 15:
			oWorkThd = std::thread(&minethd::x15_work_main, this);
			break;
		case 16:
			oWorkThd = std::thread(&minethd::x16_work_main, this);
			break;
		case 17:
			oWorkThd = std::thread(&minethd::x17_work_main, this);
			break;
		case 18:
			oWorkThd = std::thread(&minethd::x18_work_main, this);
			break;
		case 19:
			oWorkThd = std::thread(&minethd::x19_work_main, this);
			break;
		case 20:
			oWorkThd = std::thread(&minethd::x20_work_main, this);
			break;
		case 21:
			oWorkThd = std::thread(&minethd::x21_work_main, this);
			break;
		case 22:
			oWorkThd = std::thread(&minethd::x22_work_main, this);
			break;
		case 23:
			oWorkThd = std::thread(&minethd::x23_work_main, this);
			break;
		case 24:
			oWorkThd = std::thread(&minethd::x24_work_main, this);
			break;
		case 25:
			oWorkThd = std::thread(&minethd::x25_work_main, this);
			break;
		case 26:
			oWorkThd = std::thread(&minethd::x26_work_main, this);
			break;
		case 27:
			oWorkThd = std::thread(&minethd::x27_work_main, this);
			break;
		case 28:
			oWorkThd = std::thread(&minethd::x28_work_main, this);
			break;
		case 29:
			oWorkThd = std::thread(&minethd::x29_work_main, this);
			break;
		case 30:
			oWorkThd = std::thread(&minethd::x30_work_main, this);
			break;
		case 31:
			oWorkThd = std::thread(&minethd::x31_work_main, this);
			break;
		case 32:
			oWorkThd = std::thread(&minethd::x32_work_main, this);
			break;
		case 33:
			oWorkThd = std::thread(&minethd::x33_work_main, this);
			break;
		case 34:
			oWorkThd = std::thread(&minethd::x34_work_main, this);
			break;
		case 35:
			oWorkThd = std::thread(&minethd::x35_work_main, this);
			break;
		case 36:
			oWorkThd = std::thread(&minethd::x36_work_main, this);
			break;
		case 37:
			oWorkThd = std::thread(&minethd::x37_work_main, this);
			break;
		case 38:
			oWorkThd = std::thread(&minethd::x38_work_main, this);
			break;
		case 39:
			oWorkThd = std::thread(&minethd::x39_work_main, this);
			break;
		case 40:
			oWorkThd = std::thread(&minethd::x40_work_main, this);
			break;
		case 41:
			oWorkThd = std::thread(&minethd::x41_work_main, this);
			break;
		case 42:
			oWorkThd = std::thread(&minethd::x42_work_main, this);
			break;
		case 43:
			oWorkThd = std::thread(&minethd::x43_work_main, this);
			break;
		case 44:
			oWorkThd = std::thread(&minethd::x44_work_main, this);
			break;
		case 45:
			oWorkThd = std::thread(&minethd::x45_work_main, this);
			break;
		case 46:
			oWorkThd = std::thread(&minethd::x46_work_main, this);
			break;
		case 47:
			oWorkThd = std::thread(&minethd::x47_work_main, this);
			break;
		case 48:
			oWorkThd = std::thread(&minethd::x48_work_main, this);
			break;
		case 49:
			oWorkThd = std::thread(&minethd::x49_work_main, this);
			break;
		case 50:
			oWorkThd = std::thread(&minethd::x50_work_main, this);
			break;
		case 51:
			oWorkThd = std::thread(&minethd::x51_work_main, this);
			break;
		case 52:
			oWorkThd = std::thread(&minethd::x52_work_main, this);
			break;
		case 53:
			oWorkThd = std::thread(&minethd::x53_work_main, this);
			break;
		case 54:
			oWorkThd = std::thread(&minethd::x54_work_main, this);
			break;
		case 55:
			oWorkThd = std::thread(&minethd::x55_work_main, this);
			break;
		case 56:
			oWorkThd = std::thread(&minethd::x56_work_main, this);
			break;
		case 57:
			oWorkThd = std::thread(&minethd::x57_work_main, this);
			break;
		case 58:
			oWorkThd = std::thread(&minethd::x58_work_main, this);
			break;
		case 59:
			oWorkThd = std::thread(&minethd::x59_work_main, this);
			break;
		case 60:
			oWorkThd = std::thread(&minethd::x60_work_main, this);
			break;
		case 61:
			oWorkThd = std::thread(&minethd::x61_work_main, this);
			break;
		case 62:
			oWorkThd = std::thread(&minethd::x62_work_main, this);
			break;
		case 63:
			oWorkThd = std::thread(&minethd::x63_work_main, this);
			break;
		case 64:
			oWorkThd = std::thread(&minethd::x64_work_main, this);
			break;
		case 65:
			oWorkThd = std::thread(&minethd::x65_work_main, this);
			break;
		case 66:
			oWorkThd = std::thread(&minethd::x66_work_main, this);
			break;
		case 67:
			oWorkThd = std::thread(&minethd::x67_work_main, this);
			break;
		case 68:
			oWorkThd = std::thread(&minethd::x68_work_main, this);
			break;
		case 69:
			oWorkThd = std::thread(&minethd::x69_work_main, this);
			break;
		case 70:
			oWorkThd = std::thread(&minethd::x70_work_main, this);
			break;
		case 71:
			oWorkThd = std::thread(&minethd::x71_work_main, this);
			break;
		case 72:
			oWorkThd = std::thread(&minethd::x72_work_main, this);
			break;
		case 73:
			oWorkThd = std::thread(&minethd::x73_work_main, this);
			break;
		case 74:
			oWorkThd = std::thread(&minethd::x74_work_main, this);
			break;
		case 75:
			oWorkThd = std::thread(&minethd::x75_work_main, this);
			break;
		case 76:
			oWorkThd = std::thread(&minethd::x76_work_main, this);
			break;
		case 77:
			oWorkThd = std::thread(&minethd::x77_work_main, this);
			break;
		case 78:
			oWorkThd = std::thread(&minethd::x78_work_main, this);
			break;
		case 79:
			oWorkThd = std::thread(&minethd::x79_work_main, this);
			break;
		case 80:
			oWorkThd = std::thread(&minethd::x80_work_main, this);
			break;
		case 81:
			oWorkThd = std::thread(&minethd::x81_work_main, this);
			break;
		case 82:
			oWorkThd = std::thread(&minethd::x82_work_main, this);
			break;
		case 83:
			oWorkThd = std::thread(&minethd::x83_work_main, this);
			break;
		case 84:
			oWorkThd = std::thread(&minethd::x84_work_main, this);
			break;
		case 85:
			oWorkThd = std::thread(&minethd::x85_work_main, this);
			break;
		case 86:
			oWorkThd = std::thread(&minethd::x86_work_main, this);
			break;
		case 87:
			oWorkThd = std::thread(&minethd::x87_work_main, this);
			break;
		case 88:
			oWorkThd = std::thread(&minethd::x88_work_main, this);
			break;
		case 89:
			oWorkThd = std::thread(&minethd::x89_work_main, this);
			break;
		case 90:
			oWorkThd = std::thread(&minethd::x90_work_main, this);
			break;
		case 91:
			oWorkThd = std::thread(&minethd::x91_work_main, this);
			break;
		case 92:
			oWorkThd = std::thread(&minethd::x92_work_main, this);
			break;
		case 93:
			oWorkThd = std::thread(&minethd::x93_work_main, this);
			break;
		case 94:
			oWorkThd = std::thread(&minethd::x94_work_main, this);
			break;
		case 95:
			oWorkThd = std::thread(&minethd::x95_work_main, this);
			break;
		case 96:
			oWorkThd = std::thread(&minethd::x96_work_main, this);
			break;
		case 97:
			oWorkThd = std::thread(&minethd::x97_work_main, this);
			break;
		case 98:
			oWorkThd = std::thread(&minethd::x98_work_main, this);
			break;
		case 99:
			oWorkThd = std::thread(&minethd::x99_work_main, this);
			break;
		case 100:
			oWorkThd = std::thread(&minethd::x100_work_main, this);
			break;
		case 101:
			oWorkThd = std::thread(&minethd::x101_work_main, this);
			break;
		case 102:
			oWorkThd = std::thread(&minethd::x102_work_main, this);
			break;
		case 103:
			oWorkThd = std::thread(&minethd::x103_work_main, this);
			break;
		case 104:
			oWorkThd = std::thread(&minethd::x104_work_main, this);
			break;
		case 105:
			oWorkThd = std::thread(&minethd::x105_work_main, this);
			break;
		case 106:
			oWorkThd = std::thread(&minethd::x106_work_main, this);
			break;
		case 107:
			oWorkThd = std::thread(&minethd::x107_work_main, this);
			break;
		case 108:
			oWorkThd = std::thread(&minethd::x108_work_main, this);
			break;
		case 109:
			oWorkThd = std::thread(&minethd::x109_work_main, this);
			break;
		case 110:
			oWorkThd = std::thread(&minethd::x110_work_main, this);
			break;
		case 111:
			oWorkThd = std::thread(&minethd::x111_work_main, this);
			break;
		case 112:
			oWorkThd = std::thread(&minethd::x112_work_main, this);
			break;
		case 113:
			oWorkThd = std::thread(&minethd::x113_work_main, this);
			break;
		case 114:
			oWorkThd = std::thread(&minethd::x114_work_main, this);
			break;
		case 115:
			oWorkThd = std::thread(&minethd::x115_work_main, this);
			break;
		case 116:
			oWorkThd = std::thread(&minethd::x116_work_main, this);
			break;
		case 117:
			oWorkThd = std::thread(&minethd::x117_work_main, this);
			break;
		case 118:
			oWorkThd = std::thread(&minethd::x118_work_main, this);
			break;
		case 119:
			oWorkThd = std::thread(&minethd::x119_work_main, this);
			break;
		case 120:
			oWorkThd = std::thread(&minethd::x120_work_main, this);
			break;
		case 121:
			oWorkThd = std::thread(&minethd::x121_work_main, this);
			break;
		case 122:
			oWorkThd = std::thread(&minethd::x122_work_main, this);
			break;
		case 123:
			oWorkThd = std::thread(&minethd::x123_work_main, this);
			break;
		case 124:
			oWorkThd = std::thread(&minethd::x124_work_main, this);
			break;
		case 125:
			oWorkThd = std::thread(&minethd::x125_work_main, this);
			break;
		case 126:
			oWorkThd = std::thread(&minethd::x126_work_main, this);
			break;
		case 127:
			oWorkThd = std::thread(&minethd::x127_work_main, this);
			break;
		case 128:
			oWorkThd = std::thread(&minethd::x128_work_main, this);
			break;
		default:
			oWorkThd = std::thread(&minethd::work_main, this);
			break;
	}

	order_guard.wait();

	if(affinity >= 0) //-1 means no affinity
		if(!thd_setaffinity(oWorkThd.native_handle(), affinity))
			printer::inst()->print_msg(L1, "WARNING setting affinity failed.");
}

cryptonight_ctx* minethd::minethd_alloc_ctx()
{
	cryptonight_ctx* ctx;
	alloc_msg msg = { 0 };

	switch (::jconf::inst()->GetSlowMemSetting())
	{
	case ::jconf::never_use:
		ctx = cryptonight_alloc_ctx(1, 1, &msg);
		if (ctx == NULL)
			printer::inst()->print_msg(L0, "MEMORY ALLOC FAILED: %s", msg.warning);
		return ctx;

	case ::jconf::no_mlck:
		ctx = cryptonight_alloc_ctx(1, 0, &msg);
		if (ctx == NULL)
			printer::inst()->print_msg(L0, "MEMORY ALLOC FAILED: %s", msg.warning);
		return ctx;

	case ::jconf::print_warning:
		ctx = cryptonight_alloc_ctx(1, 1, &msg);
		if (msg.warning != NULL)
			printer::inst()->print_msg(L0, "MEMORY ALLOC FAILED: %s", msg.warning);
		if (ctx == NULL)
			ctx = cryptonight_alloc_ctx(0, 0, NULL);
		return ctx;

	case ::jconf::always_use:
		return cryptonight_alloc_ctx(0, 0, NULL);

	case ::jconf::unknown_value:
		return NULL; //Shut up compiler
	}

	return nullptr; //Should never happen
}

static constexpr size_t MAX_N = 130;
bool minethd::self_test()
{
	alloc_msg msg = { 0 };
	size_t res;
	bool fatal = false;

	switch (::jconf::inst()->GetSlowMemSetting())
	{
	case ::jconf::never_use:
		res = cryptonight_init(1, 1, &msg);
		fatal = true;
		break;

	case ::jconf::no_mlck:
		res = cryptonight_init(1, 0, &msg);
		fatal = true;
		break;

	case ::jconf::print_warning:
		res = cryptonight_init(1, 1, &msg);
		break;

	case ::jconf::always_use:
		res = cryptonight_init(0, 0, &msg);
		break;

	case ::jconf::unknown_value:
	default:
		return false; //Shut up compiler
	}

	if(msg.warning != nullptr)
		printer::inst()->print_msg(L0, "MEMORY INIT ERROR: %s", msg.warning);

	if(res == 0 && fatal)
		return false;

	cryptonight_ctx *ctx[MAX_N] = {0};
	for (int i = 0; i < MAX_N; i++)
	{
		if ((ctx[i] = minethd_alloc_ctx()) == nullptr)
		{
			for (int j = 0; j < i; j++)
				cryptonight_free_ctx(ctx[j]);
			return false;
		}
	}

	bool bResult = true;

	bool mineMonero = ::jconf::inst()->IsCurrencyMonero();
	if(mineMonero)
	{
		unsigned char out[32 * MAX_N];
		cn_hash_fun hashf;
		cn_hash_fun_multi hashf_multi;

		hashf = func_selector(::jconf::inst()->HaveHardwareAes(), false, mineMonero);
		hashf("This is a test", 14, out, ctx[0]);
		bResult = memcmp(out, "\xa0\x84\xf0\x1d\x14\x37\xa0\x9c\x69\x85\x40\x1b\x60\xd4\x35\x54\xae\x10\x58\x02\xc5\xf5\xd8\xa9\xb3\x25\x36\x49\xc0\xbe\x66\x05", 32) == 0;

		hashf = func_selector(::jconf::inst()->HaveHardwareAes(), true, mineMonero);
		hashf("This is a test", 14, out, ctx[0]);
		bResult &= memcmp(out, "\xa0\x84\xf0\x1d\x14\x37\xa0\x9c\x69\x85\x40\x1b\x60\xd4\x35\x54\xae\x10\x58\x02\xc5\xf5\xd8\xa9\xb3\x25\x36\x49\xc0\xbe\x66\x05", 32) == 0;

		hashf_multi = func_multi_selector(2, ::jconf::inst()->HaveHardwareAes(), false, mineMonero);
		hashf_multi("The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy log", 43, out, ctx);
		bResult &= memcmp(out, "\x3e\xbb\x7f\x9f\x7d\x27\x3d\x7c\x31\x8d\x86\x94\x77\x55\x0c\xc8\x00\xcf\xb1\x1b\x0c\xad\xb7\xff\xbd\xf6\xf8\x9f\x3a\x47\x1c\x59"
				"\xb4\x77\xd5\x02\xe4\xd8\x48\x7f\x42\xdf\xe3\x8e\xed\x73\x81\x7a\xda\x91\xb7\xe2\x63\xd2\x91\x71\xb6\x5c\x44\x3a\x01\x2a\x41\x22", 64) == 0;

		hashf_multi = func_multi_selector(2, ::jconf::inst()->HaveHardwareAes(), true, mineMonero);
		hashf_multi("The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy log", 43, out, ctx);
		bResult &= memcmp(out, "\x3e\xbb\x7f\x9f\x7d\x27\x3d\x7c\x31\x8d\x86\x94\x77\x55\x0c\xc8\x00\xcf\xb1\x1b\x0c\xad\xb7\xff\xbd\xf6\xf8\x9f\x3a\x47\x1c\x59"
				"\xb4\x77\xd5\x02\xe4\xd8\x48\x7f\x42\xdf\xe3\x8e\xed\x73\x81\x7a\xda\x91\xb7\xe2\x63\xd2\x91\x71\xb6\x5c\x44\x3a\x01\x2a\x41\x22", 64) == 0;

		hashf_multi = func_multi_selector(3, ::jconf::inst()->HaveHardwareAes(), false, mineMonero);
		hashf_multi("This is a testThis is a testThis is a test", 14, out, ctx);
		bResult &= memcmp(out, "\xa0\x84\xf0\x1d\x14\x37\xa0\x9c\x69\x85\x40\x1b\x60\xd4\x35\x54\xae\x10\x58\x02\xc5\xf5\xd8\xa9\xb3\x25\x36\x49\xc0\xbe\x66\x05"
				"\xa0\x84\xf0\x1d\x14\x37\xa0\x9c\x69\x85\x40\x1b\x60\xd4\x35\x54\xae\x10\x58\x02\xc5\xf5\xd8\xa9\xb3\x25\x36\x49\xc0\xbe\x66\x05"
				"\xa0\x84\xf0\x1d\x14\x37\xa0\x9c\x69\x85\x40\x1b\x60\xd4\x35\x54\xae\x10\x58\x02\xc5\xf5\xd8\xa9\xb3\x25\x36\x49\xc0\xbe\x66\x05", 96) == 0;

		hashf_multi = func_multi_selector(4, ::jconf::inst()->HaveHardwareAes(), false, mineMonero);
		hashf_multi("This is a testThis is a testThis is a testThis is a test", 14, out, ctx);
		bResult &= memcmp(out, "\xa0\x84\xf0\x1d\x14\x37\xa0\x9c\x69\x85\x40\x1b\x60\xd4\x35\x54\xae\x10\x58\x02\xc5\xf5\xd8\xa9\xb3\x25\x36\x49\xc0\xbe\x66\x05"
				"\xa0\x84\xf0\x1d\x14\x37\xa0\x9c\x69\x85\x40\x1b\x60\xd4\x35\x54\xae\x10\x58\x02\xc5\xf5\xd8\xa9\xb3\x25\x36\x49\xc0\xbe\x66\x05"
				"\xa0\x84\xf0\x1d\x14\x37\xa0\x9c\x69\x85\x40\x1b\x60\xd4\x35\x54\xae\x10\x58\x02\xc5\xf5\xd8\xa9\xb3\x25\x36\x49\xc0\xbe\x66\x05"
				"\xa0\x84\xf0\x1d\x14\x37\xa0\x9c\x69\x85\x40\x1b\x60\xd4\x35\x54\xae\x10\x58\x02\xc5\xf5\xd8\xa9\xb3\x25\x36\x49\xc0\xbe\x66\x05", 128) == 0;

		hashf_multi = func_multi_selector(5, ::jconf::inst()->HaveHardwareAes(), false, mineMonero);
		hashf_multi("This is a testThis is a testThis is a testThis is a testThis is a test", 14, out, ctx);
		bResult &= memcmp(out, "\xa0\x84\xf0\x1d\x14\x37\xa0\x9c\x69\x85\x40\x1b\x60\xd4\x35\x54\xae\x10\x58\x02\xc5\xf5\xd8\xa9\xb3\x25\x36\x49\xc0\xbe\x66\x05"
				"\xa0\x84\xf0\x1d\x14\x37\xa0\x9c\x69\x85\x40\x1b\x60\xd4\x35\x54\xae\x10\x58\x02\xc5\xf5\xd8\xa9\xb3\x25\x36\x49\xc0\xbe\x66\x05"
				"\xa0\x84\xf0\x1d\x14\x37\xa0\x9c\x69\x85\x40\x1b\x60\xd4\x35\x54\xae\x10\x58\x02\xc5\xf5\xd8\xa9\xb3\x25\x36\x49\xc0\xbe\x66\x05"
				"\xa0\x84\xf0\x1d\x14\x37\xa0\x9c\x69\x85\x40\x1b\x60\xd4\x35\x54\xae\x10\x58\x02\xc5\xf5\xd8\xa9\xb3\x25\x36\x49\xc0\xbe\x66\x05"
				"\xa0\x84\xf0\x1d\x14\x37\xa0\x9c\x69\x85\x40\x1b\x60\xd4\x35\x54\xae\x10\x58\x02\xc5\xf5\xd8\xa9\xb3\x25\x36\x49\xc0\xbe\x66\x05", 160) == 0;
	}

	for (int i = 0; i < MAX_N; i++)
		cryptonight_free_ctx(ctx[i]);

	if(!bResult)
		printer::inst()->print_msg(L0,
			"Cryptonight hash self-test failed. This might be caused by bad compiler optimizations.");

	return bResult;
}

std::vector<iBackend*> minethd::thread_starter(uint32_t threadOffset, miner_work& pWork)
{
	std::vector<iBackend*> pvThreads;

	if(!configEditor::file_exist(params::inst().configFileCPU))
	{
		autoAdjust adjust;
		if(!adjust.printConfig())
			return pvThreads;
	}

	if(!jconf::inst()->parse_config())
	{
		win_exit();
	}


	//Launch the requested number of single and double threads, to distribute
	//load evenly we need to alternate single and double threads
	size_t i, n = jconf::inst()->GetThreadCount();
	pvThreads.reserve(n);

	jconf::thd_cfg cfg;
	for (i = 0; i < n; i++)
	{
		jconf::inst()->GetThreadConfig(i, cfg);

		if(cfg.iCpuAff >= 0)
		{
#if defined(__APPLE__)
			printer::inst()->print_msg(L1, "WARNING on MacOS thread affinity is only advisory.");
#endif

			printer::inst()->print_msg(L1, "Starting %dx thread, affinity: %d.", cfg.iMultiway, (int)cfg.iCpuAff);
		}
		else
			printer::inst()->print_msg(L1, "Starting %dx thread, no affinity.", cfg.iMultiway);
		
		minethd* thd = new minethd(pWork, i + threadOffset, cfg.iMultiway, cfg.bNoPrefetch, cfg.iCpuAff);
		pvThreads.push_back(thd);
	}

	return pvThreads;
}

void minethd::consume_work()
{
	memcpy(&oWork, &globalStates::inst().inst().oGlobalWork, sizeof(miner_work));
	iJobNo++;
	globalStates::inst().inst().iConsumeCnt++;
}

minethd::cn_hash_fun minethd::func_selector(bool bHaveAes, bool bNoPrefetch, bool mineMonero)
{
	// We have two independent flag bits in the functions
	// therefore we will build a binary digit and select the
	// function as a two digit binary
	// Digit order SOFT_AES, NO_PREFETCH, MINER_ALGO

	static const cn_hash_fun func_table[] = {
		/* there will be 8 function entries if `CONF_NO_MONERO` and `CONF_NO_AEON`
		 * is not defined. If one is defined there will be 4 entries.
		 */
#ifndef CONF_NO_MONERO
		cryptonight_hash<MONERO_MASK, MONERO_ITER, MONERO_MEMORY, false, false>,
		cryptonight_hash<MONERO_MASK, MONERO_ITER, MONERO_MEMORY, false, true>,
		cryptonight_hash<MONERO_MASK, MONERO_ITER, MONERO_MEMORY, true, false>,
		cryptonight_hash<MONERO_MASK, MONERO_ITER, MONERO_MEMORY, true, true>
#endif
#if (!defined(CONF_NO_AEON)) && (!defined(CONF_NO_MONERO))
		// comma will be added only if Monero and Aeon is build
		,
#endif
#ifndef CONF_NO_AEON
		cryptonight_hash<AEON_MASK, AEON_ITER, AEON_MEMORY, false, false>,
		cryptonight_hash<AEON_MASK, AEON_ITER, AEON_MEMORY, false, true>,
		cryptonight_hash<AEON_MASK, AEON_ITER, AEON_MEMORY, true, false>,
		cryptonight_hash<AEON_MASK, AEON_ITER, AEON_MEMORY, true, true>
#endif
	};

	std::bitset<3> digit;
	digit.set(0, !bNoPrefetch);
	digit.set(1, !bHaveAes);

	// define aeon settings
#if defined(CONF_NO_AEON) || defined(CONF_NO_MONERO)
	// ignore 3rd bit if only one currency is active
	digit.set(2, 0);
#else
	digit.set(2, !mineMonero);
#endif

	return func_table[digit.to_ulong()];
}

void minethd::work_main()
{
	if(affinity >= 0) //-1 means no affinity
		bindMemoryToNUMANode(affinity);

	order_fix.set_value();
	std::unique_lock<std::mutex> lck(thd_aff_set);
	lck.release();
	std::this_thread::yield();

	cn_hash_fun hash_fun;
	cryptonight_ctx* ctx;
	uint64_t iCount = 0;
	uint64_t* piHashVal;
	uint32_t* piNonce;
	job_result result;

	hash_fun = func_selector(::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero());
	ctx = minethd_alloc_ctx();

	piHashVal = (uint64_t*)(result.bResult + 24);
	piNonce = (uint32_t*)(oWork.bWorkBlob + 39);
	globalStates::inst().inst().iConsumeCnt++;
	result.iThreadId = iThreadNo;

	while (bQuit == 0)
	{
		if (oWork.bStall)
		{
			/* We are stalled here because the executor didn't find a job for us yet,
			 * either because of network latency, or a socket problem. Since we are
			 * raison d'etre of this software it us sensible to just wait until we have something
			 */

			while (globalStates::inst().iGlobalJobNo.load(std::memory_order_relaxed) == iJobNo)
				std::this_thread::sleep_for(std::chrono::milliseconds(100));

			consume_work();
			continue;
		}

		size_t nonce_ctr = 0;
		constexpr size_t nonce_chunk = 4096; // Needs to be a power of 2

		assert(sizeof(job_result::sJobID) == sizeof(pool_job::sJobID));
		memcpy(result.sJobID, oWork.sJobID, sizeof(job_result::sJobID));

		if(oWork.bNiceHash)
			result.iNonce = *piNonce;

		while(globalStates::inst().iGlobalJobNo.load(std::memory_order_relaxed) == iJobNo)
		{
			if ((iCount++ & 0xF) == 0) //Store stats every 16 hashes
			{
				using namespace std::chrono;
				uint64_t iStamp = time_point_cast<milliseconds>(high_resolution_clock::now()).time_since_epoch().count();
				iHashCount.store(iCount, std::memory_order_relaxed);
				iTimestamp.store(iStamp, std::memory_order_relaxed);
			}

			if((nonce_ctr++ & (nonce_chunk-1)) == 0)
			{
				globalStates::inst().calc_start_nonce(result.iNonce, oWork.bNiceHash, nonce_chunk);
			}

			*piNonce = ++result.iNonce;

			hash_fun(oWork.bWorkBlob, oWork.iWorkSize, result.bResult, ctx);

			if (*piHashVal < oWork.iTarget)
				executor::inst()->push_event(ex_event(result, oWork.iPoolId));

			std::this_thread::yield();
		}

		consume_work();
	}

	cryptonight_free_ctx(ctx);
}

minethd::cn_hash_fun_multi minethd::func_multi_selector(size_t N, bool bHaveAes, bool bNoPrefetch, bool mineMonero)
{
	// We have two independent flag bits in the functions
	// therefore we will build a binary digit and select the
	// function as a two digit binary
	// Digit order SOFT_AES, NO_PREFETCH

	static const cn_hash_fun_multi func_table[] = {
		/* there will be 8*(MAX_N-1) function entries if `CONF_NO_MONERO` and `CONF_NO_AEON`
		 * is not defined. If one is defined there will be 4*(MAX_N-1) entries.
		 */
#ifndef CONF_NO_MONERO
		cryptonight_double_hash<MONERO_MASK, MONERO_ITER, MONERO_MEMORY, false, false>,
		cryptonight_double_hash<MONERO_MASK, MONERO_ITER, MONERO_MEMORY, false, true>,
		cryptonight_double_hash<MONERO_MASK, MONERO_ITER, MONERO_MEMORY, true, false>,
		cryptonight_double_hash<MONERO_MASK, MONERO_ITER, MONERO_MEMORY, true, true>,
		cryptonight_triple_hash<MONERO_MASK, MONERO_ITER, MONERO_MEMORY, false, false>,
		cryptonight_triple_hash<MONERO_MASK, MONERO_ITER, MONERO_MEMORY, false, true>,
		cryptonight_triple_hash<MONERO_MASK, MONERO_ITER, MONERO_MEMORY, true, false>,
		cryptonight_triple_hash<MONERO_MASK, MONERO_ITER, MONERO_MEMORY, true, true>,
		cryptonight_quad_hash<MONERO_MASK, MONERO_ITER, MONERO_MEMORY, false, false>,
		cryptonight_quad_hash<MONERO_MASK, MONERO_ITER, MONERO_MEMORY, false, true>,
		cryptonight_quad_hash<MONERO_MASK, MONERO_ITER, MONERO_MEMORY, true, false>,
		cryptonight_quad_hash<MONERO_MASK, MONERO_ITER, MONERO_MEMORY, true, true>,
		cryptonight_penta_hash<MONERO_MASK, MONERO_ITER, MONERO_MEMORY, false, false>,
		cryptonight_penta_hash<MONERO_MASK, MONERO_ITER, MONERO_MEMORY, false, true>,
		cryptonight_penta_hash<MONERO_MASK, MONERO_ITER, MONERO_MEMORY, true, false>,
		cryptonight_penta_hash<MONERO_MASK, MONERO_ITER, MONERO_MEMORY, true, true>
#endif
#if (!defined(CONF_NO_AEON)) && (!defined(CONF_NO_MONERO))
		// comma will be added only if Monero and Aeon is build
		,
#endif
#ifndef CONF_NO_AEON
		cryptonight_double_hash<AEON_MASK, AEON_ITER, AEON_MEMORY, false, false>,
		cryptonight_double_hash<AEON_MASK, AEON_ITER, AEON_MEMORY, false, true>,
		cryptonight_double_hash<AEON_MASK, AEON_ITER, AEON_MEMORY, true, false>,
		cryptonight_double_hash<AEON_MASK, AEON_ITER, AEON_MEMORY, true, true>,
		cryptonight_triple_hash<AEON_MASK, AEON_ITER, AEON_MEMORY, false, false>,
		cryptonight_triple_hash<AEON_MASK, AEON_ITER, AEON_MEMORY, false, true>,
		cryptonight_triple_hash<AEON_MASK, AEON_ITER, AEON_MEMORY, true, false>,
		cryptonight_triple_hash<AEON_MASK, AEON_ITER, AEON_MEMORY, true, true>,
		cryptonight_quad_hash<AEON_MASK, AEON_ITER, AEON_MEMORY, false, false>,
		cryptonight_quad_hash<AEON_MASK, AEON_ITER, AEON_MEMORY, false, true>,
		cryptonight_quad_hash<AEON_MASK, AEON_ITER, AEON_MEMORY, true, false>,
		cryptonight_quad_hash<AEON_MASK, AEON_ITER, AEON_MEMORY, true, true>,
		cryptonight_penta_hash<AEON_MASK, AEON_ITER, AEON_MEMORY, false, false>,
		cryptonight_penta_hash<AEON_MASK, AEON_ITER, AEON_MEMORY, false, true>,
		cryptonight_penta_hash<AEON_MASK, AEON_ITER, AEON_MEMORY, true, false>,
		cryptonight_penta_hash<AEON_MASK, AEON_ITER, AEON_MEMORY, true, true>
#endif
	};

	std::bitset<2> digit;
	digit.set(0, !bNoPrefetch);
	digit.set(1, !bHaveAes);

	// define aeon settings
#if defined(CONF_NO_AEON) || defined(CONF_NO_MONERO)
	// ignore miner algo if only one currency is active
	size_t miner_algo_base = 0;
#else
	size_t miner_algo_base = mineMonero ? 0 : 4*(MAX_N-1);
#endif

	N = (N<2) ? 2 : (N>MAX_N) ? MAX_N : N;
	return func_table[miner_algo_base + 4*(N-2) + digit.to_ulong()];
}

void minethd::double_work_main()
{
	multiway_work_main<2>(func_multi_selector(2, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero()));
}

void minethd::triple_work_main()
{
	multiway_work_main<3>(func_multi_selector(3, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero()));
}

void minethd::quad_work_main()
{
	multiway_work_main<4>(func_multi_selector(4, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero()));
}

void minethd::penta_work_main()
{
	multiway_work_main<5>(func_multi_selector(5, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero()));
}

void minethd::x1_work_main(){
	multiway_work_main<1>(func_multi_selector(1, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x2_work_main(){
	multiway_work_main<2>(func_multi_selector(2, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x3_work_main(){
	multiway_work_main<3>(func_multi_selector(3, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x4_work_main(){
	multiway_work_main<4>(func_multi_selector(4, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x5_work_main(){
	multiway_work_main<5>(func_multi_selector(5, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x6_work_main(){
	multiway_work_main<6>(func_multi_selector(6, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x7_work_main(){
	multiway_work_main<7>(func_multi_selector(7, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x8_work_main(){
	multiway_work_main<8>(func_multi_selector(8, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x9_work_main(){
	multiway_work_main<9>(func_multi_selector(9, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x10_work_main(){
	multiway_work_main<10>(func_multi_selector(10, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x11_work_main(){
	multiway_work_main<11>(func_multi_selector(11, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x12_work_main(){
	multiway_work_main<12>(func_multi_selector(12, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x13_work_main(){
	multiway_work_main<13>(func_multi_selector(13, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x14_work_main(){
	multiway_work_main<14>(func_multi_selector(14, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x15_work_main(){
	multiway_work_main<15>(func_multi_selector(15, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x16_work_main(){
	multiway_work_main<16>(func_multi_selector(16, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x17_work_main(){
	multiway_work_main<17>(func_multi_selector(17, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x18_work_main(){
	multiway_work_main<18>(func_multi_selector(18, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x19_work_main(){
	multiway_work_main<19>(func_multi_selector(19, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x20_work_main(){
	multiway_work_main<20>(func_multi_selector(20, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x21_work_main(){
	multiway_work_main<21>(func_multi_selector(21, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x22_work_main(){
	multiway_work_main<22>(func_multi_selector(22, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x23_work_main(){
	multiway_work_main<23>(func_multi_selector(23, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x24_work_main(){
	multiway_work_main<24>(func_multi_selector(24, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x25_work_main(){
	multiway_work_main<25>(func_multi_selector(25, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x26_work_main(){
	multiway_work_main<26>(func_multi_selector(26, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x27_work_main(){
	multiway_work_main<27>(func_multi_selector(27, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x28_work_main(){
	multiway_work_main<28>(func_multi_selector(28, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x29_work_main(){
	multiway_work_main<29>(func_multi_selector(29, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x30_work_main(){
	multiway_work_main<30>(func_multi_selector(30, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x31_work_main(){
	multiway_work_main<31>(func_multi_selector(31, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x32_work_main(){
	multiway_work_main<32>(func_multi_selector(32, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x33_work_main(){
	multiway_work_main<33>(func_multi_selector(33, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x34_work_main(){
	multiway_work_main<34>(func_multi_selector(34, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x35_work_main(){
	multiway_work_main<35>(func_multi_selector(35, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x36_work_main(){
	multiway_work_main<36>(func_multi_selector(36, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x37_work_main(){
	multiway_work_main<37>(func_multi_selector(37, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x38_work_main(){
	multiway_work_main<38>(func_multi_selector(38, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x39_work_main(){
	multiway_work_main<39>(func_multi_selector(39, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x40_work_main(){
	multiway_work_main<40>(func_multi_selector(40, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x41_work_main(){
	multiway_work_main<41>(func_multi_selector(41, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x42_work_main(){
	multiway_work_main<42>(func_multi_selector(42, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x43_work_main(){
	multiway_work_main<43>(func_multi_selector(43, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x44_work_main(){
	multiway_work_main<44>(func_multi_selector(44, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x45_work_main(){
	multiway_work_main<45>(func_multi_selector(45, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x46_work_main(){
	multiway_work_main<46>(func_multi_selector(46, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x47_work_main(){
	multiway_work_main<47>(func_multi_selector(47, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x48_work_main(){
	multiway_work_main<48>(func_multi_selector(48, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x49_work_main(){
	multiway_work_main<49>(func_multi_selector(49, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x50_work_main(){
	multiway_work_main<50>(func_multi_selector(50, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x51_work_main(){
	multiway_work_main<51>(func_multi_selector(51, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x52_work_main(){
	multiway_work_main<52>(func_multi_selector(52, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x53_work_main(){
	multiway_work_main<53>(func_multi_selector(53, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x54_work_main(){
	multiway_work_main<54>(func_multi_selector(54, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x55_work_main(){
	multiway_work_main<55>(func_multi_selector(55, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x56_work_main(){
	multiway_work_main<56>(func_multi_selector(56, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x57_work_main(){
	multiway_work_main<57>(func_multi_selector(57, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x58_work_main(){
	multiway_work_main<58>(func_multi_selector(58, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x59_work_main(){
	multiway_work_main<59>(func_multi_selector(59, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x60_work_main(){
	multiway_work_main<60>(func_multi_selector(60, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x61_work_main(){
	multiway_work_main<61>(func_multi_selector(61, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x62_work_main(){
	multiway_work_main<62>(func_multi_selector(62, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x63_work_main(){
	multiway_work_main<63>(func_multi_selector(63, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x64_work_main(){
	multiway_work_main<64>(func_multi_selector(64, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x65_work_main(){
	multiway_work_main<65>(func_multi_selector(65, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x66_work_main(){
	multiway_work_main<66>(func_multi_selector(66, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x67_work_main(){
	multiway_work_main<67>(func_multi_selector(67, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x68_work_main(){
	multiway_work_main<68>(func_multi_selector(68, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x69_work_main(){
	multiway_work_main<69>(func_multi_selector(69, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x70_work_main(){
	multiway_work_main<70>(func_multi_selector(70, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x71_work_main(){
	multiway_work_main<71>(func_multi_selector(71, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x72_work_main(){
	multiway_work_main<72>(func_multi_selector(72, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x73_work_main(){
	multiway_work_main<73>(func_multi_selector(73, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x74_work_main(){
	multiway_work_main<74>(func_multi_selector(74, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x75_work_main(){
	multiway_work_main<75>(func_multi_selector(75, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x76_work_main(){
	multiway_work_main<76>(func_multi_selector(76, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x77_work_main(){
	multiway_work_main<77>(func_multi_selector(77, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x78_work_main(){
	multiway_work_main<78>(func_multi_selector(78, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x79_work_main(){
	multiway_work_main<79>(func_multi_selector(79, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x80_work_main(){
	multiway_work_main<80>(func_multi_selector(80, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x81_work_main(){
	multiway_work_main<81>(func_multi_selector(81, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x82_work_main(){
	multiway_work_main<82>(func_multi_selector(82, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x83_work_main(){
	multiway_work_main<83>(func_multi_selector(83, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x84_work_main(){
	multiway_work_main<84>(func_multi_selector(84, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x85_work_main(){
	multiway_work_main<85>(func_multi_selector(85, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x86_work_main(){
	multiway_work_main<86>(func_multi_selector(86, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x87_work_main(){
	multiway_work_main<87>(func_multi_selector(87, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x88_work_main(){
	multiway_work_main<88>(func_multi_selector(88, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x89_work_main(){
	multiway_work_main<89>(func_multi_selector(89, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x90_work_main(){
	multiway_work_main<90>(func_multi_selector(90, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x91_work_main(){
	multiway_work_main<91>(func_multi_selector(91, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x92_work_main(){
	multiway_work_main<92>(func_multi_selector(92, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x93_work_main(){
	multiway_work_main<93>(func_multi_selector(93, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x94_work_main(){
	multiway_work_main<94>(func_multi_selector(94, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x95_work_main(){
	multiway_work_main<95>(func_multi_selector(95, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x96_work_main(){
	multiway_work_main<96>(func_multi_selector(96, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x97_work_main(){
	multiway_work_main<97>(func_multi_selector(97, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x98_work_main(){
	multiway_work_main<98>(func_multi_selector(98, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x99_work_main(){
	multiway_work_main<99>(func_multi_selector(99, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x100_work_main(){
	multiway_work_main<100>(func_multi_selector(100, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x101_work_main(){
	multiway_work_main<101>(func_multi_selector(101, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x102_work_main(){
	multiway_work_main<102>(func_multi_selector(102, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x103_work_main(){
	multiway_work_main<103>(func_multi_selector(103, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x104_work_main(){
	multiway_work_main<104>(func_multi_selector(104, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x105_work_main(){
	multiway_work_main<105>(func_multi_selector(105, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x106_work_main(){
	multiway_work_main<106>(func_multi_selector(106, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x107_work_main(){
	multiway_work_main<107>(func_multi_selector(107, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x108_work_main(){
	multiway_work_main<108>(func_multi_selector(108, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x109_work_main(){
	multiway_work_main<109>(func_multi_selector(109, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x110_work_main(){
	multiway_work_main<110>(func_multi_selector(110, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x111_work_main(){
	multiway_work_main<111>(func_multi_selector(111, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x112_work_main(){
	multiway_work_main<112>(func_multi_selector(112, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x113_work_main(){
	multiway_work_main<113>(func_multi_selector(113, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x114_work_main(){
	multiway_work_main<114>(func_multi_selector(114, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x115_work_main(){
	multiway_work_main<115>(func_multi_selector(115, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x116_work_main(){
	multiway_work_main<116>(func_multi_selector(116, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x117_work_main(){
	multiway_work_main<117>(func_multi_selector(117, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x118_work_main(){
	multiway_work_main<118>(func_multi_selector(118, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x119_work_main(){
	multiway_work_main<119>(func_multi_selector(119, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x120_work_main(){
	multiway_work_main<120>(func_multi_selector(120, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x121_work_main(){
	multiway_work_main<121>(func_multi_selector(121, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x122_work_main(){
	multiway_work_main<122>(func_multi_selector(122, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x123_work_main(){
	multiway_work_main<123>(func_multi_selector(123, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x124_work_main(){
	multiway_work_main<124>(func_multi_selector(124, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x125_work_main(){
	multiway_work_main<125>(func_multi_selector(125, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x126_work_main(){
	multiway_work_main<126>(func_multi_selector(126, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x127_work_main(){
	multiway_work_main<127>(func_multi_selector(127, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

void minethd::x128_work_main(){
	multiway_work_main<128>(func_multi_selector(128, ::jconf::inst()->HaveHardwareAes(), bNoPrefetch, ::jconf::inst()->IsCurrencyMonero())); 
}

template<size_t N>
void minethd::prep_multiway_work(uint8_t *bWorkBlob, uint32_t **piNonce)
{
	for (size_t i = 0; i < N; i++)
	{
		memcpy(bWorkBlob + oWork.iWorkSize * i, oWork.bWorkBlob, oWork.iWorkSize);
		if (i > 0)
			piNonce[i] = (uint32_t*)(bWorkBlob + oWork.iWorkSize * i + 39);
	}
}

template<size_t N>
void minethd::multiway_work_main(cn_hash_fun_multi hash_fun_multi)
{
	if(affinity >= 0) //-1 means no affinity
		bindMemoryToNUMANode(affinity);

	order_fix.set_value();
	std::unique_lock<std::mutex> lck(thd_aff_set);
	lck.release();
	std::this_thread::yield();

	cryptonight_ctx *ctx[MAX_N];
	uint64_t iCount = 0;
	uint64_t *piHashVal[MAX_N];
	uint32_t *piNonce[MAX_N];
	uint8_t bHashOut[MAX_N * 32];
	uint8_t bWorkBlob[sizeof(miner_work::bWorkBlob) * MAX_N];
	uint32_t iNonce;
	job_result res;

	for (size_t i = 0; i < N; i++)
	{
		ctx[i] = minethd_alloc_ctx();
		piHashVal[i] = (uint64_t*)(bHashOut + 32 * i + 24);
		piNonce[i] = (i == 0) ? (uint32_t*)(bWorkBlob + 39) : nullptr;
	}

	if(!oWork.bStall)
		prep_multiway_work<N>(bWorkBlob, piNonce);

	globalStates::inst().iConsumeCnt++;

	while (bQuit == 0)
	{
		if (oWork.bStall)
		{
			/*	We are stalled here because the executor didn't find a job for us yet,
			either because of network latency, or a socket problem. Since we are
			raison d'etre of this software it us sensible to just wait until we have something*/

			while (globalStates::inst().iGlobalJobNo.load(std::memory_order_relaxed) == iJobNo)
				std::this_thread::sleep_for(std::chrono::milliseconds(100));

			consume_work();
			prep_multiway_work<N>(bWorkBlob, piNonce);
			continue;
		}

		constexpr uint32_t nonce_chunk = 4096;
		int64_t nonce_ctr = 0;

		assert(sizeof(job_result::sJobID) == sizeof(pool_job::sJobID));

		if(oWork.bNiceHash)
			iNonce = *piNonce[0];

		while (globalStates::inst().iGlobalJobNo.load(std::memory_order_relaxed) == iJobNo)
		{
			if ((iCount++ & 0x7) == 0)  //Store stats every 8*N hashes
			{
				using namespace std::chrono;
				uint64_t iStamp = time_point_cast<milliseconds>(high_resolution_clock::now()).time_since_epoch().count();
				iHashCount.store(iCount * N, std::memory_order_relaxed);
				iTimestamp.store(iStamp, std::memory_order_relaxed);
			}

			nonce_ctr -= N;
			if(nonce_ctr <= 0)
			{
				globalStates::inst().calc_start_nonce(iNonce, oWork.bNiceHash, nonce_chunk);
				nonce_ctr = nonce_chunk;
			}

			for (size_t i = 0; i < N; i++)
				*piNonce[i] = ++iNonce;

			hash_fun_multi(bWorkBlob, oWork.iWorkSize, bHashOut, ctx);

			for (size_t i = 0; i < N; i++)
			{
				if (*piHashVal[i] < oWork.iTarget)
				{
					executor::inst()->push_event(ex_event(job_result(oWork.sJobID, iNonce - N + 1 + i, bHashOut + 32 * i, iThreadNo), oWork.iPoolId));
				}
			}

			std::this_thread::yield();
		}

		consume_work();
		prep_multiway_work<N>(bWorkBlob, piNonce);
	}

	for (int i = 0; i < N; i++)
		cryptonight_free_ctx(ctx[i]);
}

} // namespace cpu
} // namepsace xmrstak
