/**
 * \brief  Lx_emul support to register Linux kernel framebuffer
 * \author Stefan Kalkowski
 * \date   2021-05-17
 */

/*
 * Copyright (C) 2021 Genode Labs GmbH
 *
 * This file is distributed under the terms of the GNU General Public License
 * version 2.
 */

#ifndef _LX_EMUL__FB_H_
#define _LX_EMUL__FB_H_

#ifdef __cplusplus
extern "C" {
#endif

void lx_emul_framebuffer_ready(void * base, unsigned long size,
                               unsigned xres, unsigned yres);


bool lx_emul_machine_is_compatible(char const *compat);

#ifdef __cplusplus
}
#endif

#endif /* _LX_EMUL__FB_H_ */
