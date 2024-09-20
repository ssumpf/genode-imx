/*
 * \brief  Linux emulation C helper functions
 * \author Josef Soentgen
 * \date   2021-03-22
 */

/*
 * Copyright (C) 2021 Genode Labs GmbH
 *
 * This file is distributed under the terms of the GNU General Public License
 * version 2.
 */

#include <lx_emul/task.h>
#include <lx_user/init.h>
#include <lx_user/io.h>

#include <linux/sched/task.h>

#include "emul.h"
#include "lx_emul.h"
#include "lx_drm.h"


#include <drm/drm_device.h>

extern struct drm_device *lx_drm_dev;

struct drm_device *_lx_drm_device;


#include <drm/drm_file.h>

struct lx_drm_private
{
	struct file  *file;
	struct inode *inode;
};


#include <linux/fs.h>

static struct file_operations const *_drm_fops;

int __register_chrdev(unsigned int major, unsigned int baseminor,
                      unsigned int count, char const * name,
                      struct file_operations const * fops)
{
	_drm_fops = fops;
	return 0;
}


#include <drm/drm_ioctl.h>
#include <linux/kdev_t.h>

void *lx_drm_open(void)
{
	int err;
	struct lx_drm_private *lx_drm_prv;
	if (!_drm_fops || !_drm_fops->open)
		return NULL;

	lx_drm_prv = (struct lx_drm_private*)kzalloc(sizeof (struct lx_drm_private), GFP_KERNEL);
	if (!lx_drm_prv)
		return NULL;

	lx_drm_prv->inode = alloc_anon_inode(NULL);
	if (!lx_drm_prv->inode)
		goto free_session;

	lx_drm_prv->inode->i_rdev = MKDEV(DRM_MAJOR, DRM_MINOR_PRIMARY);

	lx_drm_prv->file = (struct file*)kzalloc(sizeof (struct file), GFP_KERNEL);
	if (!lx_drm_prv->file)
		goto free_inode;

	err = _drm_fops->open(lx_drm_prv->inode, lx_drm_prv->file);
	if (err)
		goto free_file;

	return lx_drm_prv;

free_file:
	kfree(lx_drm_prv->file);
free_inode:
	kfree(lx_drm_prv->inode);
free_session:
	kfree(lx_drm_prv);
	return NULL;
}


#include <drm/drm_drv.h>
#include <../drivers/gpu/drm/drm_internal.h>

void lx_drm_close(void *p)
{
	struct lx_drm_private *lx_drm_prv;

	if (!p || !_drm_fops || !_drm_fops->release) {
		return;
	}

	lx_drm_prv = (struct lx_drm_private*)p;

	(void)_drm_fops->release(lx_drm_prv->inode, lx_drm_prv->file);

	kfree(lx_drm_prv->inode);
	kfree(lx_drm_prv->file);
	kfree(lx_drm_prv);
}


#include <drm/drm_ioctl.h>
#include <uapi/drm/drm.h>
#include <uapi/drm/etnaviv_drm.h>

unsigned lx_drm_gem_submit_bo_count(void const *p)
{
	struct drm_etnaviv_gem_submit const * const submit =
		(struct drm_etnaviv_gem_submit const*)p;
	return submit->nr_bos;
}


unsigned *lx_drm_gem_submit_bo_handle(void *p, unsigned index)
{
	struct drm_etnaviv_gem_submit * const submit =
		(struct drm_etnaviv_gem_submit*)p;

	struct drm_etnaviv_gem_submit_bo *bos =
		(struct drm_etnaviv_gem_submit_bo*)(submit->bos + (unsigned long)submit);

	struct drm_etnaviv_gem_submit_bo * const bo = &bos[index];
	return &bo->handle;
}


static void lx_drm_gem_submit_in(struct drm_etnaviv_gem_submit *submit)
{
	submit->bos    += (unsigned long)submit;
	submit->relocs += (unsigned long)submit;
	submit->pmrs   += (unsigned long)submit;
	submit->stream += (unsigned long)submit;
}


static void lx_drm_version_in(struct drm_version *version)
{
	/* set proper pointer value from offset */
	version->name += (unsigned long)version;
	version->date += (unsigned long)version;
	version->desc += (unsigned long)version;
}


static void lx_drm_version_out(struct drm_version *version)
{
	/* set proper offset value from pointer */
	version->name -= (unsigned long)version;
	version->date -= (unsigned long)version;
	version->desc -= (unsigned long)version;
}


static int lx_drm_in(unsigned int cmd, unsigned long arg)
{
	unsigned int const nr = DRM_IOCTL_NR(cmd);
	bool const is_driver_ioctl =
		nr >= DRM_COMMAND_BASE && nr < DRM_COMMAND_END;

	if (is_driver_ioctl) {
		unsigned const int dnr = nr - DRM_COMMAND_BASE;

		switch (dnr) {
		case DRM_ETNAVIV_GEM_SUBMIT:
			lx_drm_gem_submit_in((struct drm_etnaviv_gem_submit*)arg);
			break;
		default:
			break;
		}
	} else {
		switch (nr) {
		case DRM_IOCTL_NR(DRM_IOCTL_VERSION):
			lx_drm_version_in((struct drm_version*)arg);
			break;
		default:
			break;
		}
	}
	return 0;
}


static int lx_drm_out(unsigned int cmd, unsigned long arg)
{
	unsigned int const nr = DRM_IOCTL_NR(cmd);
	bool const is_driver_ioctl =
		nr >= DRM_COMMAND_BASE && nr < DRM_COMMAND_END;

	if (is_driver_ioctl) {
		unsigned const int dnr = nr - DRM_COMMAND_BASE;

		switch (dnr) {
		default:
			break;
		}
	} else {
		switch (nr) {
		case DRM_IOCTL_NR(DRM_IOCTL_VERSION):
			lx_drm_version_out((struct drm_version*)arg);
			break;
		default:
			break;
		}
	}
	return 0;
}

int lx_drm_ioctl(void *p, unsigned int cmd, unsigned long arg)
{
	struct lx_drm_private *lx_drm_prv;
	int res;

	lx_drm_prv = (struct lx_drm_private*)p;

	if (!lx_drm_prv) {
		return -1;
	}

	if (cmd & IOC_IN) {
		lx_drm_in(cmd, arg);
	}
	res = drm_ioctl(lx_drm_prv->file, cmd, arg);
	if (cmd & IOC_OUT) {
		lx_drm_out(cmd, arg);
	}
	return res;
}


int lx_drm_check_gem_new(unsigned int cmd)
{
	unsigned int const nr = DRM_IOCTL_NR(cmd);

	bool const is_driver_ioctl =
		nr >= DRM_COMMAND_BASE && nr < DRM_COMMAND_END;

	if (!is_driver_ioctl) {
		return 0;
	}

	return (nr - DRM_COMMAND_BASE) == DRM_ETNAVIV_GEM_NEW ? 1 : 0;
}


unsigned int lx_drm_get_gem_new_handle(unsigned long arg)
{
	struct drm_etnaviv_gem_new const *p =
		(struct drm_etnaviv_gem_new const *)arg;

	return p->handle;
}


int lx_drm_check_gem_close(unsigned int cmd)
{
	unsigned int const nr = DRM_IOCTL_NR(cmd);

	bool const is_driver_ioctl =
		nr >= DRM_COMMAND_BASE && nr < DRM_COMMAND_END;

	if (is_driver_ioctl) {
		return 0;
	}

	return nr == DRM_IOCTL_NR(DRM_IOCTL_GEM_CLOSE) ? 1 : 0;
}


unsigned int lx_drm_get_gem_close_handle(unsigned long arg)
{
	struct drm_gem_close const *p =
		(struct drm_gem_close const *)arg;

	return p->handle;
}


int lx_drm_close_handle(void *p, unsigned int handle)
{
	struct lx_drm_private *lx_drm_prv;

	struct drm_gem_close arg = {
		.handle = handle
	};

	lx_drm_prv = (struct lx_drm_private*)p;

	return drm_ioctl(lx_drm_prv->file, DRM_IOCTL_GEM_CLOSE, (unsigned long)&arg);
}


/*
 * The next functions are used by the Gpu lx_drm_prv to perform I/O controls.
 */

int lx_drm_ioctl_etnaviv_gem_param(void *lx_drm_prv, unsigned char param,
                                   unsigned long long *value)
{
	int err;
	struct drm_etnaviv_param req = {
		.pipe = 0,
		.param = param,
		.value = 0,
	};

	err = lx_drm_ioctl(lx_drm_prv, DRM_IOCTL_ETNAVIV_GET_PARAM, (unsigned long)&req);
	if (err) {
		return -1;
	}

	*value = req.value;
	return 0;
}


int lx_drm_ioctl_etnaviv_gem_submit(void *lx_drm_prv, unsigned long arg,
                                    unsigned int *fence)
{
	int err;
	struct drm_etnaviv_gem_submit *submit;

	err = lx_drm_ioctl(lx_drm_prv, DRM_IOCTL_ETNAVIV_GEM_SUBMIT, arg);
	if (err) {
		return err;
	}

	submit = (struct drm_etnaviv_gem_submit*)arg;

	*fence = submit->fence;
	return 0;
}


int lx_drm_ioctl_etnaviv_wait_fence(void *lx_drm_prv, unsigned int fence)
{
	int err;

	struct drm_etnaviv_wait_fence req = {
		.pipe = 0,
		.fence = fence,
		// .flags = ETNA_WAIT_NONBLOCK,
		.flags = 0,
	};

	err = lx_drm_ioctl(lx_drm_prv, DRM_IOCTL_ETNAVIV_WAIT_FENCE,
	                   (unsigned long) &req);

	return err;
}


int lx_drm_ioctl_etnaviv_gem_new(void *lx_drm_prv, unsigned long size,
                                 unsigned int *handle)
{
	int err;
	struct drm_etnaviv_gem_new req = {
		.size = size,
		.flags = ETNA_BO_WC,
		.handle = 0,
	};

	err = lx_drm_ioctl(lx_drm_prv, DRM_IOCTL_ETNAVIV_GEM_NEW, (unsigned long)&req);
	if (err) {
		return -1;
	}

	*handle = req.handle;
	return 0;
}


int lx_drm_ioctl_etnaviv_gem_info(void *lx_drm_prv, unsigned int handle,
                                  unsigned long long *offset)
{
	int err;
	struct drm_etnaviv_gem_info req = {
		.handle = handle,
	};

	err = lx_drm_ioctl(lx_drm_prv, DRM_IOCTL_ETNAVIV_GEM_INFO, (unsigned long)&req);
	if (err) {
		return -1;
	}

	*offset = req.offset;
	return 0;
}


int lx_drm_ioctl_etnaviv_cpu_prep(void *lx_drm_prv, unsigned int handle, int op)
{
	int err;

	struct drm_etnaviv_gem_cpu_prep req;
	struct timespec64 to;
	ktime_get_ts64(&to);
	// see mesa:
	//     get_abs_timeout(&req.timeout, 5000000000);
	to.tv_sec += 5;

	req.handle = handle,
	req.op     = op,
	req.timeout = (struct drm_etnaviv_timespec){ to.tv_sec, to.tv_nsec },

	err = lx_drm_ioctl(lx_drm_prv, DRM_IOCTL_ETNAVIV_GEM_CPU_PREP, (unsigned long)&req);
	if (err) {
		return -1;
	}

	return 0;
}


int lx_drm_ioctl_etnaviv_cpu_fini(void *lx_drm_prv, unsigned int handle)
{
	int err;
	struct drm_etnaviv_gem_cpu_fini req = {
		.handle = handle,
	};

	err = lx_drm_ioctl(lx_drm_prv, DRM_IOCTL_ETNAVIV_GEM_CPU_FINI, (unsigned long)&req);
	if (err) {
		return -1;
	}

	return 0;
}


int lx_drm_ioctl_gem_close(void *lx_drm_prv, unsigned int handle)
{
	int err;
	struct drm_gem_close req = {
		.handle = handle,
	};

	err = lx_drm_ioctl(lx_drm_prv, DRM_IOCTL_GEM_CLOSE, (unsigned long)&req);
	if (err) {
		return -1;
	}

	return 0;
}


#include <drm/drm_gem.h>
#include <drm/drm_vma_manager.h>

struct shmem_file_buffer
{
	struct folio *folio;
};


void *genode_lookup_mapping_from_offset(void *p,
                                        unsigned long offset,
                                        unsigned long size)
{
	struct lx_drm_private *lx_drm_prv;
	struct file *file;
	struct drm_file *drm_file;
	struct drm_device *dev;
	struct drm_vma_offset_manager *mgr;
	struct drm_vma_offset_node *node;

	lx_drm_prv = (struct lx_drm_private*)p;
	if (!lx_drm_prv)
		return NULL;

	file = lx_drm_prv->file;
	if (!file)
		return NULL;

	drm_file = file->private_data;
	if (!drm_file)
		return NULL;

	dev = drm_file->minor->dev;
	if (!dev)
		return NULL;

	mgr = dev->vma_offset_manager;

	drm_vma_offset_lock_lookup(mgr);
	node = drm_vma_offset_lookup_locked(mgr, offset / PAGE_SIZE,
	                                    size / PAGE_SIZE);
	drm_vma_offset_unlock_lookup(mgr);

	if (node) {
		struct drm_gem_object *obj =
			container_of(node, struct drm_gem_object, vma_node);
		if (obj) {
			struct file *f = obj->filp;
			if (f) {
				struct address_space *mapping = f->f_mapping;
				if (mapping) {
					struct shmem_file_buffer *private_data =
						mapping->private_data;
					if (private_data) {
						void * addr = page_address(folio_page(private_data->folio, 0));
						return addr;
					}
				}
			}
		}
	}
	return NULL;
}


#include <linux/fs.h>
#include <linux/mount.h>

int simple_pin_fs(struct file_system_type * type,struct vfsmount ** mount,int * count)
{
	*mount = kzalloc(sizeof(struct vfsmount), GFP_KERNEL);
	return 0;
}


static unsigned long _get_next_ino(void)
{
	static unsigned long count = 0;
	return ++count;
}


struct inode *alloc_anon_inode(struct super_block *s)
{
	struct inode *inode;

	inode = kzalloc(sizeof (struct inode), GFP_KERNEL);
	if (!inode) {
		return (struct inode*)ERR_PTR(-ENOMEM);
	}

	inode->i_ino = _get_next_ino();

	return inode;
}


#include <linux/dma-mapping.h>

u64 dma_get_required_mask(struct device * dev)
{
	/* XXX query HW for DMA_MASK?  */
	enum { DMA_MASK = 0xbfffffffUL, };
	return DMA_MASK;
}


#include <lx_emul/alloc.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>

void *vzalloc(unsigned long size)
{
	return lx_emul_mem_alloc_aligned(size, ARCH_KMALLOC_MINALIGN);
}


#include <linux/mount.h>
#include <linux/fs.h>
#include <linux/slab.h>

struct vfsmount * kern_mount(struct file_system_type * type)
{
	struct vfsmount *m;

	m = kzalloc(sizeof (struct vfsmount), GFP_KERNEL);
	if (!m) {
		return (struct vfsmount*)ERR_PTR(-ENOMEM);
	}

	return m;
}


#include <linux/dma-mapping.h>

int dma_map_sgtable(struct device *dev, struct sg_table *sgt,
                    enum dma_data_direction dir, unsigned long attrs)
{
	int nents = dma_map_sg_attrs(dev, sgt->sgl, sgt->orig_nents, dir, attrs);
	if (nents < 0)
		return nents;
	sgt->nents = nents;
	return 0;
}


/*
 * Very very basic folio free-up emulation
 */
#include <linux/memcontrol.h>
#include <linux/pagevec.h>


void check_move_unevictable_folios(struct folio_batch *fbatch)
{
	lx_emul_trace(__func__);
}


void free_huge_folio(struct folio *folio)
{
	lx_emul_trace_and_stop(__func__);
}


void folio_undo_large_rmappable(struct folio *folio)
{
	lx_emul_trace_and_stop(__func__);
}


void free_unref_page(struct page *page, unsigned int order)
{
	lx_emul_trace_and_stop(__func__);
}


#include <linux/mm.h>

bool folio_mark_dirty(struct folio * folio)
{
	if (!folio_test_dirty(folio))
		return !folio_test_set_dirty(folio);
	return false;
}




/*
 * see linux/src/linux/mm/page_alloc.c - mostly original code, beside __folio_put
 */
void destroy_large_folio(struct folio *folio)
{
	if (folio_test_hugetlb(folio)) {
		free_huge_folio(folio);
		return;
	}

	if (folio_test_large_rmappable(folio))
		folio_undo_large_rmappable(folio);

	mem_cgroup_uncharge(folio);

	__folio_put(folio);
}



/*
 * see linux/src/linux/mm/swap.c - this is a very shorten version of it
 */
static void __page_cache_release(struct folio *folio)
{
	if (folio_test_lru(folio)) {
		lx_emul_trace_and_stop(__func__);
	}
	/* See comment on folio_test_mlocked in release_pages() */
	if (unlikely(folio_test_mlocked(folio))) {
		lx_emul_trace_and_stop(__func__);
	}
}


/*
 * see linux/src/linux/mm/swap.c - original code
 */
static void __folio_put_large(struct folio *folio)
{
	/*
	 * __page_cache_release() is supposed to be called for thp, not for
	 * hugetlb. This is because hugetlb page does never have PageLRU set
	 * (it's never listed to any LRU lists) and no memcg routines should
	 * be called for hugetlb (it has a separate hugetlb_cgroup.)
	 */
	if (!folio_test_hugetlb(folio))
		__page_cache_release(folio);
	destroy_large_folio(folio);
}


/*
 * see linux/src/linux/mm/swap.c - this is a very shorten version of it
 */
void release_pages(release_pages_arg arg, int nr)
{
	int i;
	struct encoded_page **encoded = arg.encoded_pages;

	for (i = 0; i < nr; i++) {
		struct folio *folio;

		/* Turn any of the argument types into a folio */
		folio = page_folio(encoded_page_ptr(encoded[i]));

		if (is_huge_zero_page(&folio->page))
			continue;

		if (folio_is_zone_device(folio))
			lx_emul_trace_and_stop(__func__);

		if (!folio_put_testzero(folio))
			continue;

		if (folio_test_large(folio)) {
			lx_emul_trace(__func__);
			__folio_put_large(folio);
			continue;
		}

		if (folio_test_lru(folio))
			lx_emul_trace_and_stop(__func__);

		if (unlikely(folio_test_mlocked(folio)))
			lx_emul_trace_and_stop(__func__);
	}
}


void __folio_batch_release(struct folio_batch *fbatch)
{
	lx_emul_trace(__func__);

	release_pages(fbatch->folios, folio_batch_count(fbatch));
	folio_batch_reinit(fbatch);
}


struct file *shmem_file_setup(char const *name, loff_t size,
                              unsigned long flags)
{
	struct file *f;
	struct inode *inode;
	struct address_space *mapping;
	struct shmem_file_buffer *private_data;
	loff_t   const nrpages = DIV_ROUND_UP(size, PAGE_SIZE);
	unsigned const order   = order_base_2(nrpages);

	if (!size)
		return (struct file*)ERR_PTR(-EINVAL);

	f = kzalloc(sizeof (struct file), GFP_KERNEL);
	if (!f) {
		return (struct file*)ERR_PTR(-ENOMEM);
	}

	inode = kzalloc(sizeof (struct inode), GFP_KERNEL);
	if (!inode) {
		goto err_inode;
	}

	mapping = kzalloc(sizeof (struct address_space), GFP_KERNEL);
	if (!mapping) {
		goto err_mapping;
	}

	private_data = kzalloc(sizeof (struct shmem_file_buffer), GFP_KERNEL);
	if (!private_data) {
		goto err_private_data;
	}

	mapping->private_data = private_data;
	mapping->nrpages = nrpages;

	inode->i_mapping = mapping;

	atomic_long_set(&f->f_count, 1);
	f->f_inode    = inode;
	f->f_mapping  = mapping;
	f->f_flags    = flags;
	f->f_mode     = OPEN_FMODE(flags);
	f->f_mode    |= FMODE_OPENED;

	private_data->folio  = vma_alloc_folio(GFP_KERNEL, order, NULL, 0, true);
	return f;

err_private_data:
	kfree(mapping);
err_mapping:
	kfree(inode);
err_inode:
	kfree(f);
	return (struct file*)ERR_PTR(-ENOMEM);
}


static void _free_file(struct file *file)
{
	struct inode *inode;
	struct address_space *mapping;
	struct shmem_file_buffer *private_data;


	mapping      = file->f_mapping;
	inode        = file->f_inode;

	if (mapping) {
		private_data = mapping->private_data;

		if (private_data->folio) {
			/* freed by indirect call of __folio_batch_release in lx_emul.c */
			private_data->folio = NULL;
		}

		kfree(private_data);
		kfree(mapping);
	}

	kfree(inode);
	kfree(file->f_path.dentry);
	kfree(file);
}


void fput(struct file *file)
{
	if (!file)
		return;

	if (atomic_long_sub_and_test(1, &file->f_count)) {
		_free_file(file);
	}
}


/*
 * Identical to original from mm/page_alloc.c
 */
struct folio *__folio_alloc(gfp_t gfp, unsigned int order, int preferred_nid,
		nodemask_t *nodemask)
{
	struct page *page = __alloc_pages(gfp | __GFP_COMP, order,
			preferred_nid, nodemask);
	struct folio *folio = (struct folio *)page;

	if (folio && order > 1)
		folio_prep_large_rmappable(folio);

	return folio;
}


struct folio *shmem_read_folio_gfp(struct address_space *mapping,
                                   pgoff_t index, gfp_t gfp)
{
	struct shmem_file_buffer *private_data;

	if (index > mapping->nrpages)
		return NULL;

	private_data = mapping->private_data;

	if (index != 0) {
		printk("%s unsupported case - fail\n", __func__);
		return NULL;
	}

	if (!private_data->folio) {
		unsigned order       = order_base_2(mapping->nrpages);
		/* essence of shmem_alloc_folio function */
		private_data->folio  = vma_alloc_folio(gfp, order, NULL, 0, true);
	}

	return private_data->folio;
}
