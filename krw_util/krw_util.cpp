#include <stdio.h>
#include <stdint.h>

#include <localUtil.h>

#include <kernel_block.h>
#include <kern_img.h>

#ifdef __APPLE__
#include <TargetConditionals.h>
#elif defined _WIN32
#include <Windows.h>
#include <ntkern_tools.h>
#endif

#include "krw_util.h"

#ifdef __METALKIT__
extern kernel_metalkit* g_kernblock;
#else
	#if !defined(__linux__)
	#include <kern_static.h>
	#endif
// #include <kern_dynamic.h>
// extern kern_dynamic* g_kernblock;
#endif

size_t kSlideTarg(size_t targ)
{
	size_t resultTarg = -1;
	size_t kernSlideTmp = 0;
	SAFE_BAIL(kernel_slide(&kernSlideTmp) == -1);

	resultTarg = targ + kernSlideTmp;
fail:
	return resultTarg;
}

size_t kBaseTarg(size_t targ)
{
	size_t resultTarg = -1;
	size_t kernBaseTmp = 0;
	SAFE_BAIL(kernel_base(&kernBaseTmp) == -1);

	resultTarg = targ + kernBaseTmp;
fail:
	return resultTarg;
}
